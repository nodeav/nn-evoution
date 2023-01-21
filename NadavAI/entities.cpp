#include "entities.h"

#include <cmath>


/*** Entity ***/

int Entity::counter = 0;

Entity::Entity(loc_t x, loc_t y, speed_t speed, Radian angle, radius_t radius) :
        idx(counter++), loc(x, y), speed(speed), angle_(angle), radius(radius) {}

std::string Entity::toString() const {
    return std::string("{") +
            "idx: " + std::to_string(idx) +
            ", loc: " + loc.toString() +
            ", speed: " + std::to_string(speed) +
            ", angle: " + angle_.toString() +
            ", radius: " + std::to_string(radius) +
            "}";
}

void Entity::moveInBoundries(Location boundary) {
    loc_t delta_x = angle_.cosine() * speed;
    loc_t delta_y = angle_.sine() * speed;

    loc += {delta_x, delta_y};
    loc %= boundary;
}
