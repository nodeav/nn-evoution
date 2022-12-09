set(CMAKE_CXX_FLAGS_RELEASE "-O3 -mcpu=native -flto")
add_executable(mnist main_mnist.cpp mnist.cpp)
target_link_libraries(mnist ${LIBRARY_TARGET})

