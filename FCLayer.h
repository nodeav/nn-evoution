#ifndef NN_EVOLUTION_FCLAYER_H
#define NN_EVOLUTION_FCLAYER_H

#include <vector>
#include "Layer.h"

class FCLayer : public Layer {
    bool transpose;
public:
    Eigen::MatrixXf weights;
    Eigen::MatrixXf bias;

    FCLayer(std::size_t inputSize, std::size_t outputSize, bool transpose = false);

    ~FCLayer() override = default;

    // Move assignment operator
    FCLayer &operator=(FCLayer &&other) noexcept = default;

    Eigen::MatrixXf forward(Eigen::MatrixXf input) override;

    Eigen::MatrixXf backward(const Eigen::MatrixXf outputError, float learningRate) override;
};

#endif //NN_EVOLUTION_FCLAYER_H
