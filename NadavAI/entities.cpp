#include "entities.h"

#include <cmath>


/*** Entity ***/

int Entity::counter = 0;

Entity::Entity(loc_t x, loc_t y, speed_t speed, Radian angle, radius_t radius) :
        loc(x, y), speed_(speed), angle_(angle), radius(radius), idx(counter++) {}

std::string Entity::toString() const {
    return std::string("{") +
            "idx: " + std::to_string(idx) +
            ", loc: " + loc.toString() +
            ", speed: " + std::to_string(speed_) +
            ", angle: " + angle_.toString() +
            ", radius: " + std::to_string(radius) +
            "}";
}

void Entity::moveInBoundries(Location boundary) {
    loc_t delta_x = angle_.cosine() * speed_;
    loc_t delta_y = angle_.sine() * speed_;

    loc += {delta_x, delta_y};
    loc %= boundary;
}
