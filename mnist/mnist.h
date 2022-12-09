#ifndef NN_EVOLUTION_MNIST_H
#define NN_EVOLUTION_MNIST_H

#include <vector>
#include "Eigen/Dense"

using Eigen::MatrixXf;

class mnist {
private:
    struct idxFile {
        struct header {
            int32_t magic;
            int32_t numLabels;
            void msbToLsb();
        } header;
        uint8_t* labels;
        idxFile(const std::string& path);
        std::vector<MatrixXf> toMatrices();
        ~idxFile();
    };

    struct imgFile {
        struct header {
            int32_t magic;
            int32_t numImages;
            int32_t rows;
            int32_t cols;
            void msbToLsb();
        } header;
        uint8_t* pixels;
        imgFile(const std::string& path);
        size_t pixelsPerImage();
        size_t totalPixels();
        std::vector<MatrixXf> toMatrices();
        ~imgFile();
    };

public:
    mnist(const std::string& mnistDataDir);

    std::vector<MatrixXf> images;
    std::vector<MatrixXf> labels;
    std::vector<MatrixXf> testImages;
    std::vector<MatrixXf> testLabels;

};


#endif //NN_EVOLUTION_MNIST_H
