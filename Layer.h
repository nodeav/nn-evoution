#ifndef NN_EVOLUTION_LAYER_H
#define NN_EVOLUTION_LAYER_H

#include <cstddef>
#include "Eigen/Dense"

class Layer {
public:
    virtual ~Layer() = default;
    Layer(Eigen::Vector2i inputsz, Eigen::Vector2i outputsz) {
        inputSize = inputsz;
        outputSize = outputsz;
    }

    Layer(const Layer& other) {
        inputSize = other.inputSize;
        outputSize = other.outputSize;
    }

    virtual Layer *clone() = 0;

    Eigen::Vector2i inputSize;
    Eigen::Vector2i outputSize;

    Eigen::MatrixXf lastInput; // TODO: remove this!

    virtual Eigen::MatrixXf forward(Eigen::MatrixXf input) = 0;

    virtual Eigen::MatrixXf backward(Eigen::MatrixXf error, float learningRate) = 0;
};


#endif //NN_EVOLUTION_LAYER_H
