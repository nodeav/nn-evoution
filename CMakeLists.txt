cmake_minimum_required(VERSION 3.12)

project(nn-evolution CXX)
set(CMAKE_CXX_STANDARD 20)

# set the names of the test and library targets
set(TEST_TARGET main)
set(LIBRARY_TARGET nn)

# find the OpenBLAS and Eigen libraries
find_package(OpenBLAS REQUIRED)
find_package(Eigen3 REQUIRED)

# include the current directory in the include path
include_directories(.)

# add the library target
add_library(${LIBRARY_TARGET} SHARED FCLayer.cpp NeuralNet.cpp THALayer.cpp THALayer.h)

# link the library target to the OpenBLAS and Eigen libraries
target_link_libraries(${LIBRARY_TARGET} OpenBLAS::OpenBLAS Eigen3::Eigen)

# add the test target
add_executable(${TEST_TARGET} main.cpp)
# link the test target to the library target
target_link_libraries(${TEST_TARGET} ${LIBRARY_TARGET})

add_executable(eigen_test eigen_test.cpp)
target_link_libraries(eigen_test Eigen3::Eigen)
