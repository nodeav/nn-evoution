#include "mnist.h"
#include <fstream>
#include <iostream>

constexpr auto NUM_DIGITS = 10;

auto openFile(const std::string &path) {
    try {
        std::ifstream file(path, std::ios::in | std::ios::binary);
        std::ios_base::iostate exceptionMask = file.exceptions() | std::ios::failbit;
        file.exceptions(exceptionMask);
        std::cout << "opened \"" << path << "\"" << std::endl;
        return file;
    } catch (std::exception &e) {
        std::cerr << "Failed to open \"" << path << "\": " << e.what() << std::endl;
        exit(1);
    }
}

mnist::idxFile::idxFile(const std::string &path) {
    auto file = openFile(path);
    file.read((char *) &header, sizeof(header));
    header.msbToLsb();
    labels = new uint8_t[header.numLabels];
    file.read((char *) labels, header.numLabels);
    std::cout << "finished reading file " << path << std::endl;
}

mnist::idxFile::~idxFile() {
    delete[] labels;
}

std::vector<MatrixXf> mnist::idxFile::toMatrices() {
    std::vector<MatrixXf> ret;
    ret.reserve(header.numLabels);
    for (int i = 0; i < header.numLabels; i++) {
        auto asMat = MatrixXf(1, NUM_DIGITS);
        asMat.setZero();
        asMat(0, labels[i]) = 1;
        ret.emplace_back(asMat);
    }
    return ret;
}

mnist::imgFile::imgFile(const std::string &path) {
    auto file = openFile(path);
    file.read((char *) &header, sizeof(header));
    header.msbToLsb();
    pixels = new uint8_t[totalPixels()];
    file.read((char *) pixels, totalPixels());
}

mnist::imgFile::~imgFile() {
    delete[] pixels;
}

std::vector<MatrixXf> mnist::imgFile::toMatrices() {
    std::vector<MatrixXf> ret;
    ret.reserve(header.numImages);
    auto totalPx = totalPixels();
    auto pxPerImg = pixelsPerImage();
    for (int i = 0; i < totalPx; i += pxPerImg) {
        MatrixXf asMat(header.rows, header.cols);
        for (int row = 0; row < header.rows; row++) {
            for (int col = 0; col < header.cols; col++) {
                auto loc = i + row * header.cols + col;
                const auto &pixel = pixels[loc];
                asMat(row, col) = static_cast<float>(pixel);
            }
        }
        asMat.normalize();
        auto newMat = asMat.reshaped(1, header.cols * header.rows); // todo: get rid of this?
        ret.emplace_back(newMat);
    }
    return ret;
}

size_t mnist::imgFile::totalPixels() {
    return pixelsPerImage() * header.numImages;
}

size_t mnist::imgFile::pixelsPerImage() {
    return header.rows * header.cols;
}

mnist::mnist(const std::string &path) {
    auto relative = [&path](auto fileName) { return path + "/" + fileName; };
    labels = idxFile(relative("train-labels.idx1-ubyte")).toMatrices();
    images = imgFile(relative("train-images.idx3-ubyte")).toMatrices();
    testLabels = idxFile(relative("t10k-labels.idx1-ubyte")).toMatrices();
    testImages = imgFile(relative("t10k-images.idx3-ubyte")).toMatrices();
}

void flip(int32_t &msb) {
    // this implicitly converts int->uint,
    // which is implementation defined (not in the spec)
    msb = (msb & 0xff000000) >> 24 |
          (msb & 0x000000ff) << 24 |
          (msb & 0x00ff0000) >> 8 |
          (msb & 0x0000ff00) << 8;
}

void mnist::idxFile::header::msbToLsb() {
    flip(numLabels);
    flip(magic);
}

void mnist::imgFile::header::msbToLsb() {
    flip(magic);
    flip(numImages);
    flip(rows);
    flip(cols);
}
