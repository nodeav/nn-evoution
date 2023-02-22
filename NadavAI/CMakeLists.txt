set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto -Wall -pedantic -Wfatal-errors")
set(CMAKE_CXX_FLAGS "-Wall -pedantic")

set(EXEC_NAME "NadavAI")
set(NAI_LIB "naiLib")

option(BOARD_VIS "visualize board" ON)

add_executable(${EXEC_NAME} main_${EXEC_NAME}.cpp)

add_library(${NAI_LIB} entities.cpp
                        board.cpp
                        utils.cpp)

# include the current directory in the include path
include_directories(.)

target_link_libraries(${EXEC_NAME} ${LIBRARY_TARGET} ${NAI_LIB})

#Testing:
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    DOWNLOAD_EXTRACT_TIMESTAMP true
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests test.cpp)
target_link_libraries(tests GTest::gtest_main ${NAI_LIB})
include(GoogleTest)
gtest_discover_tests(tests)

if (${BOARD_VIS})
    # find_package(OpenCV REQUIRED)
    add_subdirectory(visualization)
endif ()