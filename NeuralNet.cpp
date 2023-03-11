#include <iostream>
#include "NeuralNet.h"
#include <memory>

size_t NeuralNet::layersNum() const {
    return layers.size();
}

void NeuralNet::destroyAll() {
    for(int i = 0; i < layers.size(); ++i) {
        delete layers[i];
    }
}

void NeuralNet::addLayer(Layer *layer) {
//    if (!layers.empty()) {
//        assert(layer.inputSize == layers[layers.size() - 1]->outputSize);
//    }
    layers.emplace_back(layer);
}

Eigen::MatrixXf NeuralNet::predict(const Eigen::MatrixXf &input) {
    auto x = [](Eigen::MatrixXf x) {};
    return predict(input, x);
}

Eigen::MatrixXf NeuralNet::predict(const Eigen::MatrixXf &input, std::function<void(const Eigen::MatrixXf)> outputCb) {
    assert(!layers.empty());
    Eigen::MatrixXf output = input;
    outputCb(output);
    for (const auto &layer: layers) {
        outputCb(output);
        output = layer->forward(output);
    }
    return output;
}

Eigen::MatrixXf NeuralNet::backProp(const Eigen::MatrixXf &error, float learningRate) {
    auto output = error;
    for (auto layer = layers.rbegin(); layer != layers.rend(); ++layer) {
        output = (*layer)->backward(output, learningRate);
    }
    return output;
}

float NeuralNet::loss(const Eigen::MatrixXf &gt, const Eigen::MatrixXf &prediction) {
    auto diff = gt - prediction;
//    std::cout << "diff: " << diff << std::endl;
    auto ret = diff.array().pow(2).sum() / gt.size();
    return ret;
}

Eigen::MatrixXf NeuralNet::lossPrime(const Eigen::MatrixXf &gt, const Eigen::MatrixXf &prediction) {
    return (prediction - gt) * 2 / gt.size();
}

float NeuralNet::train(std::vector<Eigen::MatrixXf> inputs, std::vector<Eigen::MatrixXf> truths, int epochs,
                       float learningRate) {
    float epochErr = 0;
    for (auto epoch = 0; epoch < epochs; epoch++) {
        for (auto i = 0; i < inputs.size(); i++) {
            auto input = inputs[i];
            auto truth = truths[i];
//            assert(input.cols() == layers[0]->inputSize);
//            assert(truth.rows() == layers[layers.size()-1]->outputSize);
            auto output = predict(input);
            epochErr += loss(truth, output);
            // keep in this order, since lossPrime modifies $output
            auto error = lossPrime(truth, output);
            backProp(error, learningRate);
        }
//        std::cout << "epoch: " << epoch + 1 << "/" << epochs << ", error: " << epochErr << "\n";
    }
    epochErr /= static_cast<float>(inputs.size() * epochs);
    return epochErr;
}
