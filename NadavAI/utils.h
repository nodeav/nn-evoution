#pragma once

#include <cmath>

class Radian {
#define RADIAN "\u00b0";
private:
    float value;

public:
    Radian(float value) : value(fmod(value, 2.0 * M_PI)) {}

    std::string toString() const {
        return std::to_string(value) + RADIAN;
    }

    float toDegrees() const {
        return value * 180.0 / M_PI;
    }

    float toRadian() const {
        return value;
    }

    Radian operator+(const Radian &other) {
        return Radian{value + other.value};
    }
    Radian operator-(const Radian &other) {
        return Radian{value - other.value};
    }
    Radian operator*(float factor) {
        return Radian{value * factor};
    }
    Radian operator/(float factor) {
        return Radian{value / factor};
    }

    float cosine() const {
        return cos(value);
    }

    float sine() const {
        return sin(value);
    }
};
