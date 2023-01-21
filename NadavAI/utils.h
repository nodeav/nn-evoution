#pragma once

#include <cmath>
#include <string>

typedef float distance_t;

class Radian {
private:
    float value_;

public:
    Radian(float value) : value_(fmod(value, 2.0 * M_PI)) {}
    Radian() {}

    std::string toString() const {
        return std::to_string(value_) + " rad";
    }

    float toDegrees() const {
        return value_ * 180.0 / M_PI;
    }

    size_t toIntDegrees() const {
        return (value_ * 180 / M_PI);
    }

    float value() const {
        return value_;
    }

    Radian operator+(const Radian &other) const {
        return Radian{value_ + other.value_};
    }
    Radian operator-(const Radian &other) const {
        return Radian{value_ - other.value_};
    }
    Radian operator*(float factor) const {
        return Radian{value_ * factor};
    }
    Radian operator/(float factor) const {
        return Radian{value_ / factor};
    }

    bool operator<(const Radian& other) const {
        return value_ < other.value_;
    }

    bool operator>(const Radian& other) const {
        return value_ > other.value_;
    }

    bool operator<=(const Radian& other) const {
        return value_ <= other.value_;
    }

    bool operator >=(const Radian& other) const {
        return value_ >= other.value_;
    }

    bool operator ==(const Radian& other) const {
        return value_ == other.value_;
    }

    float cosine() const {
        return cos(value_);
    }

    float sine() const {
        return sin(value_);
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
