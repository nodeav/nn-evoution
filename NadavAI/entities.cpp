#include "entities.h"

#include <cmath>
#include <iostream>
#include <random>
#include "FCLayer.h"
#include "FlattenLayer.h"
#include "THALayer.h"


/*** Entity ***/

int Entity::counter = 0;

Entity::Entity(loc_t x, loc_t y, speed_t speed, Radian angle, radius_t radius) :
        loc(x, y), speed_(speed), angle_(angle), radius(radius), idx(counter++) {

    FlattenLayer* flatten = new FlattenLayer({entitySize, entityNum}); // TODO: verify the order of this vs acknowledgeEntities
    FCLayer* fc1 = new FCLayer(flatten->outputSize, 64);
    FCLayer* fc2 = new FCLayer(fc1->outputSize, 32);
    FCLayer* fc3 = new FCLayer(fc2->outputSize, controlSize);
    auto tha0 = new THALayer();
    auto tha1 = new THALayer();
    auto tha2 = new THALayer();
    auto tha3 = new THALayer();
    brain.addLayer(flatten);
    brain.addLayer(tha0);
    brain.addLayer(fc1);
    brain.addLayer(tha1);
    brain.addLayer(fc2);
    brain.addLayer(tha2);
    brain.addLayer(fc3);
    brain.addLayer(tha3);
}

Entity::Entity(const Entity& other) :
    loc(other.loc), speed_(other.speed_), angle_(other.angle_), radius(other.radius),
    brain(other.brain), idx(counter++)
{ }

Entity::~Entity() {
    brain.destroyAll();
}

void Entity::mutate() {
    // TODO: only sometimes mutate?
    brain.mutate();
}

std::string Entity::toString() const {
    return std::string("{") +
            "idx: " + std::to_string(idx) +
            ", loc: " + loc.toString() +
            ", speed: " + std::to_string(speed_) +
            ", angle: " + angle_.toString() +
            ", radius: " + std::to_string(radius) +
            "}";
}

void Entity::moveInBoundries(Location boundary) {
    if (energy < speed_) {
        onEnergyDepleted();
        return;
    }

    loc_t delta_x = angle_.cosine() * speed_;
    loc_t delta_y = angle_.sine() * speed_;

    loc += {delta_x, delta_y};
    loc += boundary; // TODO: do we need this?????
    loc %= boundary;

    energy -= std::max(0.005f, speed_);
}

bool Entity::isActive() const {
    return state == State::ACTIVE;
}

bool Entity::isDead() const {
    return state == State::DEAD;
}

void Entity::die() {
    state = State::DEAD;
}

void Entity::acknowledgeEntities(std::vector<EntityDistanceResult> results) {
    if (!isActive()) {
        return;
    }
    std::sort(results.begin(), results.end(), [](auto a, auto b) {
                                                return (a.distanceToEntity < b.distanceToEntity);
                                                });
    assert(entityNum > 0);

    size_t resultsToUse = std::min(static_cast<size_t>(entityNum), results.size());

    Eigen::MatrixXf netInput(entityNum, entitySize);
    for (int i = 0; i < resultsToUse; ++i) {
        netInput.row(i) = Eigen::Map<Eigen::Vector4f>(results[i].toRow().data());
    }

    for (int i = resultsToUse; i < entityNum; ++i) {
        netInput.row(i) = Eigen::Map<Eigen::Vector4f>(EntityDistanceResult::emptyRow().data());
    }
    auto out = brain.predict(netInput);

    // order doesn't matter
    assert(out.size() == 2);

    // tanh is in range [-1, 1]
    // convert to [0, 1]
    auto normalize_tanh = [](auto n) {
        return ((n + 1) / 2);
    };

    speed_ = normalize_tanh(out(0)) / 30; // TODO: normalize prettier
    angle_ = normalize_tanh(out(1)) * 2 * M_PI;
    // std::cout << "Net's output is speed: " << speed_ << " angle: " << angle_.value() << std::endl;
}

EntityPtr Entity::maybeGiveBirth() {
    static thread_local std::random_device rd{};
    static thread_local std::mt19937 gen{rd()};
    static thread_local std::normal_distribution d{0.0, 0.01};
    if (shouldGiveBirth()) {
        EntityPtr child = clone();
        child->mutate();
        child->loc += Location(d(gen), d(gen));
        return child;
    }
    return nullptr;
}

/********** Toref *************/

Toref::Toref(const Toref& other) :
    Entity(other)
{ }

void Toref::onEnergyDepleted() {
    die();
}

EntityPtr Toref::clone() const {
    EntityPtr newToref = (std::make_shared<Toref>(*this));
    return newToref;
}

bool Toref::shouldGiveBirth() {
    if (ate > 2) {
        ate = 0;
        return true;
    }
    return false;
}

void Toref::maybeEat(std::vector<EntityDistanceResult> results) {
    if (!isActive()) { // TODO: WHY?
        return;
    }
    for (auto& result : results) {
        if (result.entity->isDead() || result.entity->getType() == EntityType::TOREF) {
            continue;
        }
        if (result.distanceToEntity < 0.0045) { // TODO: move to const
            energy = std::min(energy + 0.5, 2.); // TODO: reset energy
            result.entity->die();
            ++ate;
        }
    }
}

/********** Tarif *************/

EntityPtr Tarif::clone() const {
    EntityPtr newTarif = std::make_shared<Tarif>(*this);
    return newTarif;
}

Tarif::Tarif(const Tarif& other) :
    Entity(other)
{ }

void Tarif::onEnergyDepleted() {
    state = State::INACTIVE; // TODO: do we need this INACTIVE?
    if (restForMoveIterations == whenCanMove) {
        state = State::ACTIVE;
        energy = 2; // TODO: make better energy
        restForMoveIterations = 0;
    } else {
        ++restForMoveIterations;
    }
}

void Tarif::maybeEat(std::vector<EntityDistanceResult> entities) {
    return;
}

bool Tarif::shouldGiveBirth() {
    if (waitForBirthIterations == whenCanGiveBirth) {
        waitForBirthIterations = 0;
        return true;
    }
    ++waitForBirthIterations;
    return false;
}