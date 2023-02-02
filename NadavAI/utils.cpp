#include "utils.h"

#include <string>

/*** Location ***/

std::string Location::toString() const {
    return std::string("[") + std::to_string(x_) + ", " + std::to_string(y_) + "]";
}

Location::Location(loc_t x, loc_t y)
        : x_(x), y_(y) {}

Location Location::operator+(const Location &other) const {
    return {x() + other.x(), y() + other.y()};
}

bool Location::operator<(const Location &other) const {
    return x_ < other.x_ && y_ < other.y_;
}

bool Location::operator>(const Location &other) const {
    return x_ > other.x_ && y_ > other.y_;
}

bool Location::operator==(const Location &other) const {
    return x_ == other.x_ && y_ == other.y_;
}

Location Location::operator%(const Location &other) const {
    return {std::fmod(x(), other.x()),
            std::fmod(y(), other.y())};
}

Location Location::operator%=(const Location &other) {
    return *this = *this % other;
}

Location Location::operator+=(const Location &other) {
    return *this = *this + other;
}


/****************/
// TODO: get a grip on ourselves about 0/180!
Radian getAngle(Location loc1, Location loc2) {
    if (loc1.x() == loc2.x()) {
        return loc1.y() >= loc2.y() ? 0 : M_PI;
    }
    return std::atan((loc2.y() - loc1.y()) / (loc2.x() - loc1.x()));
}

distance_t getDistanceSquared(Location loc1, Location loc2) {
    auto dx = loc2.x() - loc1.x();
    auto dy = loc2.y() - loc1.y();
    return dx * dx + dy * dy;
}

distance_t getDistance(Location loc1, Location loc2) {
    return std::sqrt(getDistanceSquared(loc1, loc2));
}
