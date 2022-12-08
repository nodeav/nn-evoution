#include "FCLayer.h"

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

FCLayer::FCLayer(std::size_t inputSize, std::size_t outputSize) {
    this->inputSize = inputSize;
    this->outputSize = outputSize;
    weights.setRandom(inputSize, outputSize);
    bias.setRandom(1, outputSize);
}