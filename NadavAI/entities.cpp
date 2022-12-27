#include "entities.h"


Location::Location(uint64_t x, uint64_t y) : x(x), y(y)
{ }



Entity::Entity(uint64_t x, uint64_t y) : loc(x, y) {
}

std::string Location::toString() const {
    return std::string("{") + std::to_string(x) + ", " + std::to_string(y) + "}";
}

std::string Entity::toString() const {
    return loc.toString();
}
