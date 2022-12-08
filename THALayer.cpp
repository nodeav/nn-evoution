#include <iostream>
#include "THALayer.h"

Eigen::MatrixXf THALayer::forward(Eigen::MatrixXf input) {
    lastInput = input;
    return input.array().tanh();
}

Eigen::MatrixXf THALayer::backward(Eigen::MatrixXf error, float learningRate) {
    auto tanhPrime = 1 - lastInput.array().tanh().pow(2);
    return error.array() * tanhPrime;
}

THALayer::THALayer() {
    this->inputSize = -1;
    this->outputSize = -1;
}
