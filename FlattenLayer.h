#ifndef NN_EVOLUTION_FLATTENLAYER_H
#define NN_EVOLUTION_FLATTENLAYER_H

#include "Layer.h"

class FlattenLayer : public Layer {

    Eigen::MatrixXf forward(Eigen::MatrixXf input) override;

    Eigen::MatrixXf backward(const Eigen::MatrixXf outputError, float learningRate) override;

public:
    FlattenLayer(Eigen::Vector2i inputSize);

    Layer* clone() override;
};


#endif //NN_EVOLUTION_FLATTENLAYER_H
