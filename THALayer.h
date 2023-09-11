#ifndef NN_EVOLUTION_THALAYER_H
#define NN_EVOLUTION_THALAYER_H


#include "Layer.h"

class THALayer : public Layer {
public:
    ~THALayer() override = default;
    THALayer(const THALayer& other) :
        Layer(other)
        { }

    Layer* clone() override;

    Eigen::MatrixXf lastInput;

    Eigen::MatrixXf forward(Eigen::MatrixXf input) override;

    Eigen::MatrixXf backward(Eigen::MatrixXf error, float learningRate) override;

public:
    THALayer() :
        Layer({-1, -1}, {-1, -1})
        { }

};


#endif //NN_EVOLUTION_THALAYER_H
