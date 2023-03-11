
#pragma once

#include <stdint.h>
#include <string>
#include <memory>
#include "utils.h"
#include "NeuralNet.h"

typedef float speed_t;
typedef float radius_t;




constexpr int entitySize = 3; // speed, angle, distance
constexpr int entityNum = 16;
constexpr int controlSize = 2;  // angle, speed

struct EntityDistanceResult {
    EntityDistanceResult() {
        speedOfEntity = 0;
        angleToEntity = 0;
        distanceToEntity = 0;
    }

    EntityDistanceResult(speed_t speed, Radian angle, distance_t dist) {
        speedOfEntity = speed;
        angleToEntity = angle;
        distanceToEntity = dist;
    }
    speed_t speedOfEntity;
    Radian angleToEntity;
    distance_t distanceToEntity;
    std::array<float, entitySize> toRow() const {
        return {speedOfEntity, angleToEntity.value(), distanceToEntity};
    }
};

class Entity {
private:
    static int counter;
    Location loc;
    speed_t speed_; // distance per frame? TBD
    Radian angle_;
    radius_t radius;
    distance_t maxSightDistance_ = 5;
    Radian fieldOfView_ = 0.5;
    NeuralNet brain;


public:
    int idx = 0;
    Entity(loc_t x, loc_t y, speed_t speed, Radian angle, radius_t size);
    Entity() : Entity(0, 0, 0, 0, 0) {}
    virtual ~Entity();
    std::string toString() const;

    void moveInBoundries(Location boundary);
    distance_t maxSightDistance() const { return maxSightDistance_; }
    speed_t speed() const { return speed_; };
    // angle of an entity is considering that 0/2pie are the upper y axis, and pie/2 is the upper x axis
    Radian angle() const { return angle_; }
    Location location() const { return loc; }
    Radian fieldOfView() const { return fieldOfView_; }
    bool operator==(const Entity& other) const { return idx == other.idx; };
    void acknowledgeEntities(std::vector<EntityDistanceResult> entities);
};

typedef std::shared_ptr<Entity> EntityPtr;

class Toref : public Entity {

};

class Tarif : public Entity {

};