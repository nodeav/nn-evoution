
#pragma once

#include <stdint.h>
#include <string>

typedef float loc_t;

class Location {
    loc_t x;
    loc_t y;
public:
    Location(loc_t x, loc_t y);
    std::string toString() const;
};

class Entity {
public:
    Entity(loc_t x, loc_t y);
    std::string toString() const;
    float speed;
    float angle;

private:
    Location loc;

};

class Toref : public Entity {

};

class Tarif : public Entity {

};