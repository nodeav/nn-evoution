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

FCLayer::FCLayer(Eigen::Vector2i inputSize, int outputCols) {
    auto inputRows = inputSize.y();
    auto inputCols = inputSize.x();
    this->inputSize = {inputCols, inputRows};
    this->outputSize = {outputCols, inputRows};

    weights.setRandom(inputCols, outputCols);
    bias.setRandom(inputRows, outputCols);

    std::cout << "My input is " << this->inputSize.transpose() << ", my weights are " << weights.rows() << " "
              << weights.cols() << " and my output is " << outputSize.transpose() << "." << std::endl;
}