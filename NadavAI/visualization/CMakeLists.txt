set(VIZ_LIB "viz")
set(VIZ_EXE "viz_example")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_library(${VIZ_LIB} visualization.cpp)
add_executable(${VIZ_EXE} viz_main.cpp)

include_directories(. ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(${VIZ_LIB} SDL2::SDL2 SDL2_image::SDL2_image)
target_link_libraries(${VIZ_EXE} ${VIZ_LIB} ${NAI_LIB})

message(STATUS SDL2_IMAGE_INCLUDE_DIRS: ${SDL2_IMAGE_INCLUDE_DIRS})
message(STATUS SDL2_IMAGE_LIBRARIES: ${SDL2_IMAGE_LIBRARIES})

configure_file(cats-fluffy.png cats-fluffy.png COPYONLY)