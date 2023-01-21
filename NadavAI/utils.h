#pragma once

#include <cmath>
#include <string>

typedef float distance_t;

class Radian {
#define RADIAN "\u00b0";
private:
    float value;

public:
    Radian(float value) : value(fmod(value, 2.0 * M_PI)) {}
    Radian() {}

    std::string toString() const {
        return std::to_string(value) + RADIAN;
    }

    float toDegrees() const {
        return value * 180.0 / M_PI;
    }

    size_t toIntDegrees() const {
        return (value * 180 / M_PI);
    }

    float toRadian() const {
        return value;
    }

    Radian operator+(const Radian &other) const {
        return Radian{value + other.value};
    }
    Radian operator-(const Radian &other) const {
        return Radian{value - other.value};
    }
    Radian operator*(float factor) const {
        return Radian{value * factor};
    }
    Radian operator/(float factor) const {
        return Radian{value / factor};
    }

    bool operator<(const Radian& other) const {
        return value < other.value;
    }

    bool operator>(const Radian& other) const {
        return value > other.value;
    }

    bool operator<=(const Radian& other) const {
        return value<= other.value;
    }

    bool operator >=(const Radian& other) const {
        return value >= other.value;
    }

    bool operator ==(const Radian& other) const {
        return value == other.value;
    }

    float cosine() const {
        return cos(value);
    }

    float sine() const {
        return sin(value);
    }
};

// todo: improve this hash!
class RadianHasher {
public:
    size_t operator()(Radian const& radian) const {
        return (radian.toIntDegrees());
    }
};

typedef float loc_t;
class Location {
    loc_t x_;
    loc_t y_;
public:
    Location(loc_t x, loc_t y);
    std::string toString() const;
    Location operator+(const Location& other) const;
    bool operator<(const Location& other) const;
    bool operator>(const Location& other) const;
    bool operator==(const Location& other) const;
    Location operator%(const Location& other) const;
    Location operator%=(const Location& other);
    Location operator+=(const Location& other);
    loc_t x() const {return x_; }
    loc_t y() const {return y_; }
};



Radian getAngle(Location loc1, Location loc2);
distance_t getDistance(Location loc1, Location loc2);
