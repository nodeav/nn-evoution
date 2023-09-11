#ifndef NN_EVOLUTION_FCLAYER_H
#define NN_EVOLUTION_FCLAYER_H

#include <vector>
#include "Layer.h"

class FCLayer : public Layer {
public:
    Eigen::MatrixXf weights;
    Eigen::MatrixXf bias;

    FCLayer(std::size_t inputCols, std::size_t outputCols);

    FCLayer(Eigen::Vector2i inputSize, int outputCols);

    FCLayer(const FCLayer& other);

    Layer* clone() override;

    ~FCLayer() override = default;

    // Move assignment operator
    FCLayer &operator=(FCLayer &&other) noexcept = default;

    Eigen::MatrixXf forward(Eigen::MatrixXf input) override;

    Eigen::MatrixXf backward(const Eigen::MatrixXf outputError, float learningRate) override;
};

#endif //NN_EVOLUTION_FCLAYER_H
