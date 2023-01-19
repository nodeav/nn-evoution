#include "entities.h"

#include <cmath>


/*** Entity ***/

Entity::Entity(loc_t x, loc_t y, speed_t speed, Radian angle, radius_t radius) :
loc(x, y), speed(speed), angle_(angle), radius(radius) { }

std::string Entity::toString() const {
    return std::string("{") +
                      loc.toString() +
                      ", speed: " + std::to_string(speed) +
                      ", angle: " + angle_.toString() +
                      ", radius: " + std::to_string(radius) +
                      "}";
}

Location Entity::moveInBoundries(Location boundry) {
    loc_t delta_x = angle_.cosine() * speed;
    loc_t delta_y = angle_.sine() * speed;

    loc += {delta_x, delta_y};
    loc %= boundry;

    return loc;
}

/*** Location ***/

std::string Location::toString() const {
    return std::string("[") + std::to_string(x_) + ", " + std::to_string(y_) + "]";
}

Location::Location(loc_t x, loc_t y)
: x_(x), y_(y) { }

Location Location::operator+(const Location& other) const {
    return {x() + other.x() , y() + other.y()};
}

Location Location::operator%(const Location& other) const {
    return {std::fmod(x(), other.x()),
            std::fmod(y(), other.y())};
}

Location Location::operator%=(const Location& other) {
    return *this % other;
}

Location Location::operator+=(const Location& other) {
    return *this + other;
}
