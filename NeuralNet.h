#ifndef NN_EVOLUTION_NEURALNET_H
#define NN_EVOLUTION_NEURALNET_H

#include "Layer.h"
#include <vector>

//template<typename In=float*, typename Out=float>
class NeuralNet {
    std::vector<Layer *> layers;

public:
    void addLayer(Layer *layer);

    Eigen::MatrixXf predict(const Eigen::MatrixXf &input);

    Eigen::MatrixXf predict(const Eigen::MatrixXf &input, std::function<void(const Eigen::MatrixXf)> outputCb);

    float loss(const Eigen::MatrixXf &gt, const Eigen::MatrixXf &prediction);

    Eigen::MatrixXf lossPrime(const Eigen::MatrixXf &gt, const Eigen::MatrixXf &prediction);

    float train(std::vector<Eigen::MatrixXf> inputs, std::vector<Eigen::MatrixXf> truths, int epochs = 1000,
                float learningRate = 0.1);

    Eigen::MatrixXf backProp(const Eigen::MatrixXf &error, float learningRate);

};


#endif //NN_EVOLUTION_NEURALNET_H
