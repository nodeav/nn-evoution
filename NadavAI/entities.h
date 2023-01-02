
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
    Location operator+(const Location& other) {
        x += other.x;
        y += other.y;

        return *this;
    }

    Location operator+=(const Location& other) {
        return *this + other;
    }
};

class Entity {
private:
    Location loc;

public:
    Entity(loc_t x, loc_t y, speed_t speed, Radian angle);
    std::string toString() const;
    speed_t speed;
    Radian angle;

    Location move();



};

class Toref : public Entity {

};

class Tarif : public Entity {

};