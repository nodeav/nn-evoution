#include "entities.h"


Location::Location(loc_t x, loc_t y) : x(x), y(y)
{ }



Entity::Entity(loc_t x, loc_t y) : loc(x, y) {
}

std::string Location::toString() const {
    return std::string("{") + std::to_string(x) + ", " + std::to_string(y) + "}";
}

std::string Entity::toString() const {
    return loc.toString();
}
