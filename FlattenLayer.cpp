#include "FlattenLayer.h"
#include <iostream>

FlattenLayer::FlattenLayer(Eigen::Vector2i inputSize) {
    this->inputSize = inputSize;
    this->outputSize = {inputSize.x() * inputSize.y(), 1};
}

Eigen::MatrixXf FlattenLayer::forward(Eigen::MatrixXf input) {
    return input.reshaped(outputSize.y(), outputSize.x());
}

Eigen::MatrixXf FlattenLayer::backward(const Eigen::MatrixXf outputError, float learningRate) {
    return outputError.reshaped(inputSize.y(), inputSize.x());
}
