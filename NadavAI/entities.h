
#pragma once

#include <stdint.h>
#include <string>
#include <memory>
#include "utils.h"
#include "NeuralNet.h"

typedef float speed_t;
typedef float radius_t;
typedef float energy_t;

constexpr int entitySize = 3; // speed, angle, distance
constexpr int entityNum = 16;
constexpr int controlSize = 2;  // angle, speed

enum class State {
    ACTIVE = 0,
    INACTIVE = 1,
    DEAD = 2,
};

enum class EntityType {
    TOREF = 0,
    TARIF = 1,
};

struct EntityDistanceResult;
class Entity;
typedef std::shared_ptr<Entity> EntityPtr;


class Entity {
private:
    static int counter;
    Location loc;
    speed_t speed_; // distance per frame? TBD
    Radian angle_;
    radius_t radius;
    distance_t maxSightDistance_ = 5;
    Radian fieldOfView_ = 0.5;
    virtual EntityPtr clone() const = 0;

protected:
    NeuralNet brain;
    energy_t energy = 1;
    State state = State::ACTIVE;

    bool isActive() const;
public:
    int idx = 0;
    Entity(loc_t x, loc_t y, speed_t speed, Radian angle, radius_t size);
    Entity() : Entity(0, 0, 0, 0, 0) {}
    Entity(const Entity& other);
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
    virtual void onEnergyDepleted() = 0;
    virtual void maybeEat(std::vector<EntityDistanceResult> results) = 0;
    virtual EntityType getType() const = 0;
    void die();
    bool isDead() const;
    EntityPtr maybeGiveBirth();
    virtual bool shouldGiveBirth() = 0;
};

struct EntityDistanceResult {
    EntityDistanceResult() {
        entity = nullptr;
        angleToEntity = 0;
        distanceToEntity = 0;
    }

    EntityDistanceResult(EntityPtr entity_, Radian angle, distance_t dist) {
        entity = entity_;
        angleToEntity = angle;
        distanceToEntity = dist;
    }
    EntityPtr entity;
    Radian angleToEntity;
    distance_t distanceToEntity;
    std::array<float, entitySize> toRow() const { // TODO: add entity type
        return {entity->speed(), angleToEntity.value(), distanceToEntity};
    }
    static std::array<float, entitySize> emptyRow() { // TODO: add entity type
        return {0, 0, 0};
    }
};


class Toref : public Entity {
public:
    Toref(loc_t x, loc_t y, speed_t speed, Radian angle, radius_t size) :
         Entity(x, y, speed, angle, size) {}
    Toref() : Entity() {}
    Toref(const Toref& other);
    void onEnergyDepleted() override;
    void maybeEat(std::vector<EntityDistanceResult> results) override;
    EntityType getType() const override { return EntityType::TOREF; }

private:
    bool shouldGiveBirth() override;
    EntityPtr clone() const override;
    uint8_t ate = 0;
};

class Tarif : public Entity {
public:
    Tarif(loc_t x, loc_t y, speed_t speed, Radian angle, radius_t size) :
         Entity(x, y, speed, angle, size) {}
    Tarif() : Entity() {}
    Tarif(const Tarif& other);
    void onEnergyDepleted() override;
    void maybeEat(std::vector<EntityDistanceResult> results) override;
    EntityType getType() const override { return EntityType::TARIF; }

private:
    uint32_t restForMoveIterations = 0;
    uint32_t waitForBirthIterations = 0;
    static const uint32_t whenCanMove = 60;
    static const uint32_t whenCanGiveBirth = 90;
    bool shouldGiveBirth() override;
    EntityPtr clone() const override;
};