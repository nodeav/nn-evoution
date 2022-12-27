set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto")

set(EXEC_NAME "NadavAI")
set(LIB_NAME "naiLib")

add_executable(${EXEC_NAME} main_${EXEC_NAME}.cpp)

add_library(${LIB_NAME} entities.cpp
                        board.cpp)

# include the current directory in the include path
include_directories(.)

target_link_libraries(${EXEC_NAME} ${LIBRARY_TARGET} ${LIB_NAME})


