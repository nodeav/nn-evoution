#pragma once

#include <cmath>

struct Radian {
    float value;

    Radian(float value)
    : value(std::fmod(value, 2.0 * M_PI)) {}

    float toDegrees() {
        return value * 180.0 / M_PI;
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

    float cosine() {
        return cos(value);
    }

    float sine() {
        return sin(value);
    }
};
