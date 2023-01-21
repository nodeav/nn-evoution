
#pragma once

#include <stdint.h>
#include <string>
#include <utils.h>


typedef float loc_t;
typedef float speed_t;
typedef float radius_t;
typedef float distance_t;

class Location {
    loc_t x_;
    loc_t y_;
public:
    Location(loc_t x, loc_t y);
    std::string toString() const;
    Location operator+(const Location& other) const;
    bool operator<(const Location& other) const;
    bool operator>(const Location& other) const;
    bool operator==(const Location& other) const;
    Location operator%(const Location& other) const;
    Location operator%=(const Location& other);
    Location operator+=(const Location& other);
    loc_t x() const {return x_; }
    loc_t y() const {return y_; }
};

class Entity {
private:
    Location loc;
    speed_t speed; // distance per frame? TBD
    Radian angle_;
    radius_t radius;
    distance_t distance_ = 5;
    Radian fieldOfView_;

public:
    Entity(loc_t x, loc_t y, speed_t speed, Radian angle, radius_t radius);
    std::string toString() const;

    Location moveInBoundries(Location boundry);
    distance_t distance() const { return distance_; }
    Radian angle() const { return angle_; }
    Location location() const { return loc; }
    Radian fieldOfView() const { return fieldOfView(); }

};

class Toref : public Entity {

};

class Tarif : public Entity {

};