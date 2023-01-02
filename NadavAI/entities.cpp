#include "entities.h"

#include <cmath>


Location::Location(loc_t x, loc_t y)
: x(x), y(y) { }



Entity::Entity(loc_t x, loc_t y, speed_t speed, Radian angle) : loc(x, y), speed(speed), angle(angle) {
}

std::string Location::toString() const {
    return std::string("{") + std::to_string(x) + ", " + std::to_string(y) + "}";
}

std::string Entity::toString() const {
    return loc.toString();
}

Location Entity::move() {
    loc_t delta_x = angle.cosine() * speed;
    loc_t delta_y = angle.sine() * speed;

    loc += {delta_x, delta_y};

    return loc;
}