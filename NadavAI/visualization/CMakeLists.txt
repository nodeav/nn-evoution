set(VIZ_LIB "viz")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(${VIZ_LIB} main_viz.cpp)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(${VIZ_LIB} SDL2::SDL2 SDL2_image::SDL2_image)


message(STATUS SDL2_IMAGE_INCLUDE_DIRS: ${SDL2_IMAGE_INCLUDE_DIRS})
message(STATUS SDL2_IMAGE_LIBRARIES: ${SDL2_IMAGE_LIBRARIES})

configure_file(cats-fluffy.png cats-fluffy.png COPYONLY)