#include "entities.h"

#include <cmath>
#include <iostream>
#include "FCLayer.h"
#include "FlattenLayer.h"
#include "THALayer.h"


/*** Entity ***/

int Entity::counter = 0;

Entity::Entity(loc_t x, loc_t y, speed_t speed, Radian angle, radius_t radius) :
        loc(x, y), speed_(speed), angle_(angle), radius(radius), idx(counter++) {

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

Entity::~Entity() {
    brain.destroyAll();
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
    loc_t delta_x = angle_.cosine() * speed_;
    loc_t delta_y = angle_.sine() * speed_;

    loc += {delta_x, delta_y};
    loc += boundary; // TODO: do we need this?????
    loc %= boundary;
}

void Entity::acknowledgeEntities(std::vector<EntityDistanceResult> entities) {
    std::sort(entities.begin(), entities.end(), [](auto a, auto b) {
                                                return (a.distanceToEntity < b.distanceToEntity);
                                                });
    entities.resize(entityNum, {});
    Eigen::MatrixXf netInput(entityNum, entitySize);
    for (int i = 0; i < entityNum; ++i) {
        netInput.row(i) = Eigen::Map<Eigen::Vector3f>(entities[i].toRow().data());
    }
    auto out = brain.predict(netInput);

    // order doesn't matter
    assert(out.size() == 2);
    speed_ = (out(0) + 1) / 400; // TODO: normalize prettier
    angle_ = out(1) * 2 * M_PI;
    // std::cout << "Net's output is speed: " << speed_ << " angle: " << angle_.value() << std::endl;
}