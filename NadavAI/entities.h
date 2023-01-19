
#pragma once

#include <stdint.h>
#include <string>
#include <utils.h>

typedef float loc_t;
typedef float speed_t;

class Location {
    loc_t x;
    loc_t y;
public:
    Location(loc_t x, loc_t y);
    std::string toString() const;
    Location operator+(const Location& other) const;
    Location operator%(const Location& other) const;
    Location operator%=(const Location& other);
    Location operator+=(const Location& other);
};

class Size {
#define DIAMETER "\u2300";
public:
    std::string toString() const;
    Size(float size);

private:
    typedef float diameter_t;
    diameter_t diameter;
};

class Entity {
private:
    Location loc;
    speed_t speed;
    Radian angle;
    Size size;

public:
    Entity(loc_t x, loc_t y, speed_t speed, Radian angle, Size size);
    std::string toString() const;

    Location moveInBoundries(Location boundry);



};

class Toref : public Entity {

};

class Tarif : public Entity {

};