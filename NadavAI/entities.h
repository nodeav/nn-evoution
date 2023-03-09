
#pragma once

#include <stdint.h>
#include <string>
#include "utils.h"

typedef float speed_t;
typedef float radius_t;



class Entity {
private:
    static int counter;
    Location loc;
    speed_t speed_; // distance per frame? TBD
    Radian angle_;
    radius_t radius;
    distance_t maxSightDistance_ = 5;
    Radian fieldOfView_ = 0.5;

public:
    int idx = 0;
    Entity(loc_t x, loc_t y, speed_t speed, Radian angle, radius_t size);
    std::string toString() const;

    void moveInBoundries(Location boundary);
    distance_t maxSightDistance() const { return maxSightDistance_; }
    speed_t speed() const { return speed_; };
    // angle of an entity is considering that 0/2pie are the upper y axis, and pie/2 is the upper x axis
    Radian angle() const { return angle_; }
    Location location() const { return loc; }
    Radian fieldOfView() const { return fieldOfView_; }
    bool operator==(const Entity& other) const { return idx == other.idx; };
};

class Toref : public Entity {

};

class Tarif : public Entity {

};