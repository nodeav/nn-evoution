#include "entities.h"

#include <cmath>


/*** Entity ***/

Entity::Entity(loc_t x, loc_t y, speed_t speed, Radian angle, Size size) :
loc(x, y), speed(speed), angle(angle), size(size) { }

std::string Entity::toString() const {
    return std::string("{") +
                      loc.toString() +
                      ", speed: " + std::to_string(speed) +
                      ", angle: " + angle.toString() +
                      ", diameter: " + size.toString() +
                      "}";
}

Location Entity::moveInBoundries(Location boundry) {
    loc_t delta_x = angle.cosine() * speed;
    loc_t delta_y = angle.sine() * speed;

    loc += {delta_x, delta_y};
    loc %= boundry;

    return loc;
}

/*** Location ***/

std::string Location::toString() const {
    return std::string("[") + std::to_string(x) + ", " + std::to_string(y) + "]";
}

Location::Location(loc_t x, loc_t y)
: x(x), y(y) { }

Location Location::operator+(const Location& other) const {
    return {x + other.x , y + other.y};
}

Location Location::operator%(const Location& other) const {
    return {std::fmod(x, other.x),
            std::fmod(y, other.y)};
}

Location Location::operator%=(const Location& other) {
    return *this % other;
}

Location Location::operator+=(const Location& other) {
    return *this + other;
}


/*** Size ***/

Size::Size(float size) : diameter(size) {}

std::string Size::toString() const {
    return std::to_string(diameter) + DIAMETER;

}