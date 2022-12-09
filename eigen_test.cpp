#include <iostream>
#include "Eigen/Eigen"


Eigen::MatrixXf mat1(28, 28);
Eigen::MatrixXf mat2(28, 1);
Eigen::MatrixXf mat3(28, 5);



int main(int argc, char *argv[]) {
    auto res = (mat1 * mat2).transpose();// * mat6;
    auto res2 = res * mat3;
    std::cout << res2.rows() << " rows and " << res2.cols() << " cols" << std::endl;
    return 0;
}