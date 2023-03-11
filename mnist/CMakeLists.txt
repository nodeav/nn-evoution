set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto")

add_executable(mnist main_mnist.cpp mnist.cpp)

target_link_libraries(mnist ${NeuralNetLib})

option(OPENCV "OpenCV support" ON)

message(STATUS "Using OpenCV? " ${OPENCV})

if (${OPENCV})
    find_package(OpenCV REQUIRED)
    target_compile_definitions(mnist PRIVATE OPENCV)
    target_link_libraries(mnist ${OpenCV_LIBRARIES})
endif ()

