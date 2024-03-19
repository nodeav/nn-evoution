set(VIZ_LIB "viz")
set(VIZ_EXE "viz_example")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_library(${VIZ_LIB} visualization.cpp)
add_executable(${VIZ_EXE} viz_main.cpp)

include_directories(. ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(${VIZ_EXE} ${VIZ_LIB} ${NAI_LIB} ${STATS_LIB})
target_link_libraries(${VIZ_LIB} SDL2::SDL2 SDL2_ttf::SDL2_ttf)

# TODO: make sure this is OK
if (SDL2_IMAGE_LIBRARIES)
    message(STATUS "SDL2_IMAGE library path: ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF}")
    target_link_libraries(${VIZ_LIB} ${SDL2_IMAGE_LIBRARIES})
else()
    message(STATUS "SDL2_IMAGE not found using the SDL2_IMAGE_LIBRARIES var, trying SDL2_image::SDL2_image")
    target_link_libraries(${VIZ_LIB} SDL2_image::SDL2_image)
endif()

configure_file(cats-fluffy.png cats-fluffy.png COPYONLY)
configure_file(mice-fluffy.png mice-fluffy.png COPYONLY)
configure_file(arial.ttf arial.ttf COPYONLY)
