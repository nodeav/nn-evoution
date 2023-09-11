#include "FCLayer.h"
#include <iostream>

Eigen::MatrixXf FCLayer::forward(Eigen::MatrixXf input) {
    lastInput = input;
    return input * weights + bias;
}

Eigen::MatrixXf FCLayer::backward(const Eigen::MatrixXf outputError, float learningRate) {
    auto inputError = outputError * weights.transpose();
    auto weightsError = lastInput.transpose() * outputError;
    weights -= learningRate * weightsError;
    bias -= learningRate * outputError;
    return inputError;
}

FCLayer::FCLayer(std::size_t inputCols, std::size_t outputCols) :
        FCLayer({1, inputCols}, outputCols) {
}

FCLayer::FCLayer(Eigen::Vector2i inputSize, int outputCols)
: Layer({inputSize.x(), inputSize.y()},
        {outputCols, inputSize.y()}) {
    weights.setRandom(inputSize.x(), outputCols);
    bias.setRandom(inputSize.y(), outputCols);
}

FCLayer::FCLayer(const FCLayer& other)
: Layer(other) {
    weights = other.weights;
    bias = other.bias;
}

Layer* FCLayer::clone() {
    return new FCLayer(*this);
}