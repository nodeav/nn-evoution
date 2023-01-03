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

Location Entity::move() {
    loc_t delta_x = angle.cosine() * speed;
    loc_t delta_y = angle.sine() * speed;

    loc += {delta_x, delta_y};

    return loc;
}


/*** Location ***/

std::string Location::toString() const {
    return std::string("[") + std::to_string(x) + ", " + std::to_string(y) + "]";
}


Location::Location(loc_t x, loc_t y)
: x(x), y(y) { }


/*** Size ***/

Size::Size(float size) : diameter(size) {}

std::string Size::toString() const {
    return std::to_string(diameter) + DIAMETER;

}