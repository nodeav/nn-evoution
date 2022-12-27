#include <iostream>
#include <algorithm>
#include <random>
#include "NeuralNet.h"
#include "FCLayer.h"
#include "THALayer.h"
#include "mnist.h"
#include "FlattenLayer.h"

#ifdef OPENCV

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#endif

NeuralNet buildNet(int inputRows, int inputCols, int outputCols) {
    NeuralNet nn;
    auto flatten = new FlattenLayer({inputCols, inputRows});
    auto fc1 = new FCLayer(flatten->outputSize, 144);
    auto fc2 = new FCLayer(fc1->outputSize, 81);
    auto fc3 = new FCLayer(fc2->outputSize, 49);
    auto fc4 = new FCLayer(fc3->outputSize, 25);
    auto fc5 = new FCLayer(fc4->outputSize, outputCols);
    auto tha1 = new THALayer();
    auto tha2 = new THALayer();
    auto tha3 = new THALayer();
    auto tha4 = new THALayer();
    auto tha5 = new THALayer();
    nn.addLayer(flatten);
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
    return nn;
}

int i = 0;
#ifdef OPENCV

void showOutputCb(const Eigen::MatrixXf &netOutput) {
    i %= 8;
    auto sideLength = (int) sqrt(netOutput.size());
    auto mat = cv::Mat(sideLength, sideLength, CV_32FC1, (float *) netOutput.data());
    cv::resize(mat, mat, {}, 32, 32);
    auto windowName = "output" + std::to_string(i);
    cv::namedWindow(windowName);
    cv::imshow(windowName, mat);
    i++;
}


void drawLines(cv::Mat &mat) {
    for (auto i = 1; i < 10; i++) {
        auto x = i * 25;
        cv::line(mat, {x, 0}, {x, 10}, {255});
    }
}

#endif

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <mnist dir>" << std::endl;
        return 1;
    }
    auto dataSet = mnist(argv[1]);
    auto nn = buildNet(28, 28, 10);
    std::cout << "Now training the nn..." << std::endl;
    auto epochs = 1000;
    auto perEpoch = 6000;
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
#ifdef OPENCV
    auto noDisplyFor = 0;
#endif
    for (int i = 0; i < nImages; i++) {
        const auto &img = dataSet.testImages[i];
        const auto &label = dataSet.testLabels[i];
#ifdef false //OPENCV
        auto prediction = nn.predict(img, showOutputCb);
//        auto predMat = cv::Mat(1, 10, CV_32FC1, (float*)prediction.data());
//        auto gtMat = cv::Mat(1, 10, CV_32FC1, (float*)label.data());
//        cv::resize(predMat, predMat, {250, 25});
//        cv::resize(gtMat, gtMat, {250, 25});
//        drawLines(predMat);
//        drawLines(gtMat);
//        cv::namedWindow("prediction");
//        cv::namedWindow("ground truth");
//        cv::imshow("prediction", predMat);
//        cv::imshow("ground truth", gtMat);
//        cv::waitKey(0);
#else
        auto prediction = nn.predict(img);
#endif
        Eigen::Index predMax, gtMax;
        prediction.row(0).maxCoeff(&predMax);
        label.row(0).maxCoeff(&gtMax);
        auto truth = predMax == gtMax;
        if (truth) {
            truths++;
        }
        auto ranks = std::vector<float>(prediction.data(), prediction.data() + prediction.size());
        std::vector<int> indices(10);
        std::iota(indices.begin(), indices.end(), 0);
        std::sort(indices.begin(), indices.end(), [&ranks](auto i1, auto i2) {
            return ranks[i1] > ranks[i2];
        });
        std::cout << "detected " << std::to_string(gtMax) << " " << (truth ? "correctly" : "incorrectly") << " as:\n";
        for (auto i = 0; i < 3; i++) {
            auto idx = indices[i];
            std::cout << " -> " << std::to_string(idx) << ": " << std::to_string(ranks[idx]) << "\n";
        }
        std::cout << std::endl;
#ifdef OPENCV
        if (noDisplyFor <= 0 && (predMax != gtMax || i % 10 == 1)) {
            std::cout << "Showing image #" << std::to_string(i) << std::endl;
            std::string label = predMax == gtMax ? "CORRECT" : "WRONG";
            auto windowName =
                    label + ": this is " + std::to_string(gtMax) + ", thought it was " + std::to_string(predMax);
            cv::namedWindow(windowName);
            auto mat = cv::Mat((int) img.rows(), (int) img.cols(), CV_32FC1, (float *) img.data());
            cv::resize(mat, mat, {560, 560});
            cv::imshow(windowName, mat);
            auto key = cv::waitKey(0) & 0xFF;
            if (key == 'q') {
                noDisplyFor = nImages;
                std::cout << "Skipping " << noDisplyFor << " images..." << std::endl;
            } else if (key == 's') {
                noDisplyFor = 100;
                std::cout << "Skipping " << noDisplyFor << " images..." << std::endl;
            } else if (key == 'S') {
                noDisplyFor = 1000;
                std::cout << "Skipping " << noDisplyFor << " images..." << std::endl;
            }
            cv::destroyWindow(windowName);
        } else {
            noDisplyFor--;
        }
#endif
    }

    auto percent = 100.f * truths / nImages;
    std::cout << "Finished training. got " << truths << " images right! that's " << percent << "% correct!"
              << std::endl;
    return 0;
}