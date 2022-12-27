
#pragma once

#include <stdint.h>
#include <string>


class Location {
    uint64_t x;
    uint64_t y;
public:
    Location(uint64_t x, uint64_t y);
    std::string toString() const;
};

class Entity {
public:
    Entity(uint64_t x, uint64_t y);
    std::string toString() const;


private:
    Location loc;

};

class Toref : public Entity {

};

class Tarif : public Entity {

};