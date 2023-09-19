#include "FCLayer.h"
#include <iostream>
#include <random>
#include <functional>

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

float sample (float source) {
    static thread_local std::random_device rd{};
    static thread_local std::mt19937 gen{rd()};
    static thread_local std::normal_distribution d{0.0, 0.1};
    auto random = d(gen);
    return (source + random);
}

void FCLayer::mutate() {
    bias = bias.unaryExpr(&sample);
    weights = weights.unaryExpr(&sample);
}
