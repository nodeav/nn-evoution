#include "entities.h"

#include <cmath>
#include <iostream>
#include "FCLayer.h"
#include "FlattenLayer.h"
#include "THALayer.h"


/*** Entity ***/

int Entity::counter = 0;

Entity::Entity(loc_t x, loc_t y, speed_t speed, Radian angle, radius_t radius) :
        radius(radius), idx(counter++), location(x, y), speed(speed), angle(angle) {

    FlattenLayer* flatten = new FlattenLayer({entitySize, entityNum}); // TODO: verify the order of this vs acknowledgeEntities
    FCLayer* fc1 = new FCLayer(flatten->outputSize, 4);
    FCLayer* fc2 = new FCLayer(fc1->outputSize, 4);
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
    radius(other.radius), brain(other.brain), idx(counter++),
    location(other.location), speed(other.speed), angle(other.angle)
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
            ", location: " + location.toString() +
            ", speed: " + std::to_string(speed) +
            ", angle: " + angle.toString() +
            ", radius: " + std::to_string(radius) +
            "}";
}

void Entity::moveInBoundries(Location boundary) {
    if (energy < speed) {
        onEnergyDepleted();
        return;
    }

    loc_t delta_x = angle.cosine() * speed;
    loc_t delta_y = angle.sine() * speed;

    location += {delta_x, delta_y};
    location += boundary; // TODO: do we need this?????
    location %= boundary;

    energy -= std::max(speed, min_energy_depletion_at_rest);
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
        netInput.row(i) = Eigen::Map<Eigen::Vector3f>(results[i].toRow().data());
    }

    for (int i = resultsToUse; i < entityNum; ++i) {
        netInput.row(i) = Eigen::Map<Eigen::Vector3f>(EntityDistanceResult::emptyRow().data());
    }
    auto out = brain.predict(netInput);

    // order doesn't matter
    assert(out.size() == 2);

    // tanh is in range [-1, 1]
    // convert to [0, 1]
    auto normalize_tanh = [](auto n) {
        return (n + 1) / 2;
    };

    speed = normalize_tanh(out(0)) / 50; // TODO: normalize prettier
    angle = normalize_tanh(out(1)) * 2 * M_PI;
    // std::cout << "Net's output is speed: " << speed << " angle: " << angle.value() << std::endl;
}

/********** Toref *************/

Toref::Toref(const Toref& other) :
    Entity(other)
{ }

TorefPtr Toref::maybeGiveBirth() {
    if (ate > eat_to_give_birth_threshold) {
        ate = 0;
        TorefPtr child = clone();
        child->mutate();
        return child;
    }
    return nullptr;
}

void Toref::onEnergyDepleted() {
    die();
}

TorefPtr Toref::clone() const {
    TorefPtr newToref = (std::make_shared<Toref>(*this));
    return newToref;
}


void Toref::maybeEat(std::vector<EntityDistanceResult> results) {
    if (--cooldown > 0) {
        return;
    }

    for (auto& result : results) {
        if (result.entity->isDead() || result.entity->getType() == EntityType::TOREF) {
            continue;
        }
        if (result.distanceToEntity < dist_to_entity_for_eating) {
            energy = 1; // TODO: reset energy
            result.entity->die();
            ++ate;
            cooldown = cooldown_after_eating;
        }
    }
}

/********** Tarif *************/

TarifPtr Tarif::clone() const {
    TarifPtr newTarif = std::make_shared<Tarif>(*this);
    return newTarif;
}

Tarif::Tarif(const Tarif& other) :
    Entity(other)
{ }

void Tarif::onEnergyDepleted() {
    state = State::INACTIVE; // TODO: do we need this INACTIVE?
    if (restForMoveIterations == tarif_move_cooldown) {
        state = State::ACTIVE;
        energy = 1; // TODO: make better energy
        restForMoveIterations = 0;
    } else {
        ++restForMoveIterations;
    }
}

void Tarif::maybeEat(std::vector<EntityDistanceResult> entities) {
    return;
}

TarifPtr Tarif::maybeGiveBirth() {
    if (waitForBirthIterations == tarif_birth_cooldown) {
        waitForBirthIterations = 0;
        TarifPtr child = clone();
        child->mutate();
        return child;
    }
    ++waitForBirthIterations;
    return nullptr;
}