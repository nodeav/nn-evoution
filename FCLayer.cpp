#include "FCLayer.h"

Eigen::MatrixXf FCLayer::forward(Eigen::MatrixXf input) {
    lastInput = input;
    auto ret = lastInput * weights + bias;
    return transpose ? ret.transpose().nestedExpression() : ret;
}

Eigen::MatrixXf FCLayer::backward(const Eigen::MatrixXf outputError, float learningRate) {
    auto inputError = outputError * weights.transpose();
    auto weightsError = lastInput.transpose() * outputError;
    weights -= learningRate * weightsError;
    bias -= learningRate * outputError;
    return transpose ? inputError.transpose().nestedExpression() : inputError;
}

FCLayer::FCLayer(std::size_t inputSize, std::size_t outputSize, bool transpose) {
    this->inputSize = inputSize;
    this->outputSize = outputSize;
    this->transpose = transpose;
    weights.setRandom(inputSize, outputSize);
    bias.setRandom(1, outputSize);
}