#include "entities.h"

#include <cmath>


/*** Entity ***/

Entity::Entity(loc_t x, loc_t y, speed_t speed, Radian angle, Size size) :
loc(x, y), speed(speed), angle(angle), size(size) { }

std::string Entity::to_string() const {
    return std::string("{") +
                      loc.to_string() +
                      ", speed: " + std::to_string(speed) +
                      ", angle: " + angle.to_string() +
                      ", diameter: " + size.to_string() +
                      "}";
}

Location Entity::move() {
    loc_t delta_x = angle.cosine() * speed;
    loc_t delta_y = angle.sine() * speed;

    loc += {delta_x, delta_y};

    return loc;
}


/*** Location ***/

std::string Location::to_string() const {
    return std::string("[") + std::to_string(x) + ", " + std::to_string(y) + "]";
}


Location::Location(loc_t x, loc_t y)
: x(x), y(y) { }


/*** Size ***/

Size::Size(float size) : diameter(size) {}

std::string Size::to_string() const {
    return std::to_string(diameter) + DIAMETER;

}