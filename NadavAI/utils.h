#pragma once

#include <cmath>
#include <string>

typedef float distance_t;

class Radian {
private:
    float value_;
    static constexpr float epsilon = 1e-4;

    struct RadianHasher {
      size_t operator()(auto radian) const {
        return std::hash<float>()(radian.value());
      }
    };

public:
    Radian(float value) : value_(fmod(value+2.0*M_PI, 2.0 * M_PI)) {}
    Radian() {}

    using Hasher = decltype(RadianHasher());

    // TODO: unit-test this
    bool between(Radian start, Radian end) const {
        // assumes all angles are positive
        if (end > start) {
            return start <= *this && *this <= end;
        }
        return start <= *this || *this <= end;
    }

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
        return value_ < other.value_ || value_ == other.value_;
    }

    bool operator >=(const Radian& other) const {
        return value_ > other.value_ || value_ == other.value_;
    }

    bool operator ==(const Radian& other) const {
        return abs(value_ - other.value_) <= epsilon;
    }

    float cosine() const {
        return cos(value_);
    }

    float sine() const {
        return sin(value_);
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
distance_t getDistanceSquared(Location loc1, Location loc2);
distance_t getDistance(Location loc1, Location loc2);
