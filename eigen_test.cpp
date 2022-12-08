#include <iostream>
#include "Eigen/Eigen"


Eigen::MatrixXf mat1({{1, 2, 3}});
Eigen::MatrixXf mat2({{10, 100, 1000}});

int main(int argc, char *argv[]) {
    std::cout << mat1 * mat2.transpose() << std::endl;
    return 0;
}