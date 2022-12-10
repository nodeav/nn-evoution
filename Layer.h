#ifndef NN_EVOLUTION_LAYER_H
#define NN_EVOLUTION_LAYER_H

#include <cstddef>
#include <Eigen/Dense>

class Layer {
public:
    virtual ~Layer() = default;

    Eigen::Vector2i inputSize;
    Eigen::Vector2i outputSize;

    Eigen::MatrixXf lastInput;

    virtual Eigen::MatrixXf forward(Eigen::MatrixXf input) = 0;

    virtual Eigen::MatrixXf backward(Eigen::MatrixXf error, float learningRate) = 0;
};


#endif //NN_EVOLUTION_LAYER_H
