#include <iostream>
#include "NeuralNet.h"
#include "FCLayer.h"
#include "THALayer.h"
#include "Eigen/Core"

using Eigen::MatrixXf;
MatrixXf calc(float a, float b, float c, float d, float e) {
    return MatrixXf{{a*a+b-e, b*b*b+a, sqrt(sqrt(c*c*d+c)), d*(d*d*e-a-b), 10*e+2*c}};
}

int main() {
    std::cout << "yo yo" << std::endl;
    auto fc1 = FCLayer(5, 16);
    auto fc2 = FCLayer(16, 32);
    auto fc3 = FCLayer(32, 64);
    auto fc4 = FCLayer(64, 32);
    auto fc5 = FCLayer(32, 32);
    auto fc6 = FCLayer(32, 16);
    auto fc7 = FCLayer(16, 8);
    auto fc8 = FCLayer(8, 5);
    auto tanh1 = THALayer();
    auto tanh2 = THALayer();
    auto tanh3 = THALayer();
    auto tanh4 = THALayer();
    auto tanh5 = THALayer();
    auto tanh6 = THALayer();
    auto tanh7 = THALayer();
    auto tanh8 = THALayer();
    auto nn = NeuralNet();
    nn.addLayer(&fc1);
    nn.addLayer(&tanh1);
    nn.addLayer(&fc2);
    nn.addLayer(&tanh2);
    nn.addLayer(&fc3);
    nn.addLayer(&tanh3);
    nn.addLayer(&fc4);
    nn.addLayer(&tanh4);
    nn.addLayer(&fc5);
    nn.addLayer(&tanh5);
    nn.addLayer(&fc6);
    nn.addLayer(&tanh6);
    nn.addLayer(&fc7);
    nn.addLayer(&tanh7);
    nn.addLayer(&fc8);
    nn.addLayer(&tanh8);

//    std::vector<MatrixXf> inputs = {
//            MatrixXf{{1, 1}},
//            MatrixXf{{1, 0}},
//            MatrixXf{{0, 0}},
//            MatrixXf{{0, 1}}
//    };
//
//    std::vector<MatrixXf> truths = {
//            MatrixXf{{0}},
//            MatrixXf{{1}},
//            MatrixXf{{0}},
//            MatrixXf{{1}}
//    };

    auto epochs = 500;
    auto batchSize = 1000000;
    auto best = 100.f;
    for (auto i = 0; i < epochs; i++) {
        std::cout << "Starting epoch " << i << "/" << epochs << "\n";
        std::vector<MatrixXf> inputs;
        std::vector<MatrixXf> truths;
        for (auto j = 0; j < batchSize; j++) {
            auto a = static_cast<float>(rand() % 1000);
            auto b = static_cast<float>(rand() % 1000);
            auto c = static_cast<float>(rand() % 1000);
            auto d = static_cast<float>(rand() % 1000);
            auto e = static_cast<float>(rand() % 1000);
            inputs.emplace_back(MatrixXf{{a, b, c, d, e}}.normalized());
            truths.emplace_back(calc(a, b, c, d, e).normalized());
        }
        std::cout << " -> training...\n";
        auto loss = nn.train(inputs, truths, 1, 0.1);
        best = loss < best ? loss : best;
        std::cout << " -> loss: " << loss << " (best: " << best << ")\n";

    }


    auto avgDiff = 0.f;
    auto avgDiffPercent = 0.f;
    auto tests = 100;
    for (auto i = 0; i < tests; i++) {
        auto a = static_cast<float>(rand() % 1000);
        auto b = static_cast<float>(rand() % 1000);
        auto c = static_cast<float>(rand() % 1000);
        auto d = static_cast<float>(rand() % 1000);
        auto e = static_cast<float>(rand() % 1000);
        auto input = MatrixXf{{a,b,c,d,e}}.normalized();
        auto result = calc(a,b,c,d,e).normalized();
        auto pred = nn.predict(input).normalized();
        std::cout << "result: " << result << "\n";
        std::cout << "pred  : " << pred << "\n";
        avgDiff += (result - pred).array().abs().sum() / input.size();
        avgDiffPercent += avgDiff / ((result.norm()+pred.norm())/2);
    }
    avgDiff /= tests;
    avgDiffPercent /= tests;
    std::cout << "avgDiff is " << avgDiff << " and percentage is " << avgDiffPercent*100 << "%" << std::endl;

    return 0;
}