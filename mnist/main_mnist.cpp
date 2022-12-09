#include <iostream>
#include <algorithm>
#include <random>
#include "NeuralNet.h"
#include "FCLayer.h"
#include "THALayer.h"
#include "mnist.h"

NeuralNet buildNet(int inputRows, int inputCols, int outputRows, int outputCols) {
    NeuralNet nn;
    auto inputSz = inputRows * inputCols;
    auto outputSz = outputRows * outputCols;
    auto fc1 = new FCLayer(inputSz, 128);
    auto fc2 = new FCLayer(128, 96);
    auto fc3 = new FCLayer(96, 80);
    auto fc4 = new FCLayer(80, 64);
    auto fc5 = new FCLayer(64, 32);
    auto fc6 = new FCLayer(32, outputRows, true);
    auto fc7 = new FCLayer(1, outputCols, true);
    auto tha1 = new THALayer();
    auto tha2 = new THALayer();
    auto tha3 = new THALayer();
    auto tha4 = new THALayer();
    auto tha5 = new THALayer();
    auto tha6 = new THALayer();
    auto tha7 = new THALayer();
    nn.addLayer(fc1);
    nn.addLayer(tha1);
    nn.addLayer(fc2);
    nn.addLayer(tha2);
    nn.addLayer(fc3);
    nn.addLayer(tha3);
    nn.addLayer(fc4);
    nn.addLayer(tha4);
    nn.addLayer(fc5);
    nn.addLayer(tha5);
    nn.addLayer(fc6);
    nn.addLayer(tha6);
    nn.addLayer(fc7);
    nn.addLayer(tha7);
    return nn;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <mnist dir>" << std::endl;
        return 1;
    }
    auto dataSet = mnist(argv[1]);
    auto nn = buildNet(28, 28, 1, 10);
    std::cout << "Now training the nn..." << std::endl;
    auto epochs = 500;
    auto perEpoch = 7500;
    auto bestLoss = 100.f;

    uint seed = rand();
    auto imgRng = std::mt19937{seed};
    auto labelRng = std::mt19937{seed};

    // just making sure they're really the same
    for (auto i = 0; i < 2000; i++) {
        assert(labelRng() == imgRng());
    }

    for (auto i = 0; i < epochs; i++) {
        std::cout << "Starting epoch " << i << "/" << epochs << std::endl;
        decltype(dataSet.images) images;
        images.reserve(dataSet.images.size());
        decltype(dataSet.labels) labels;
        labels.reserve(dataSet.labels.size());
        std::sample(dataSet.images.begin(), dataSet.images.end(), std::back_inserter(images), perEpoch, imgRng);
        std::sample(dataSet.labels.begin(), dataSet.labels.end(), std::back_inserter(labels), perEpoch, labelRng);
        auto learningRate = 0.15 - (i * 0.00006);
        std::cout << " -> training using lr " << learningRate << "...\n";
        auto loss = nn.train(images, labels, 1, learningRate);
        bestLoss = std::min(bestLoss, loss);
        std::cout << " -> loss: " << loss << " (best: " << bestLoss << ")\n\n";
    }
    std::cout << "\nFinished training, now testing..." << std::endl;

    auto truths = 0;
    auto nImages = dataSet.testImages.size();
    for (int i = 0; i < nImages; i++) {
        const auto &img = dataSet.testImages[i];
        const auto &label = dataSet.testLabels[i];
        auto prediction = nn.predict(img);
        Eigen::Index predMax, gtMax;
        prediction.row(0).maxCoeff(&predMax);
        label.row(0).maxCoeff(&gtMax);
        if (predMax == gtMax) {
            truths++;
        }
    }

    auto percent = 100.f * truths / nImages;
    std::cout << "Finished training. got " << truths << " images right! that's " << percent << "% correct!"
              << std::endl;
    return 0;
}