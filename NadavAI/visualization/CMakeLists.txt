set(VIZ_LIB "viz")
set(VIZ_EXE "viz_example")

set(GUI_EXEC_NAME "gui")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_library(${VIZ_LIB} visualization.cpp)
add_executable(${VIZ_EXE} viz_main.cpp)
add_executable(${GUI_EXEC_NAME} main_${GUI_EXEC_NAME}.cpp)

include_directories(. ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(${VIZ_EXE} ${VIZ_LIB} ${NAI_LIB})
target_link_libraries(${VIZ_LIB} SDL2::SDL2)
target_link_libraries(${GUI_EXEC_NAME} ${VIZ_LIB} ${NAI_LIB} Qt5::Widgets)


# TODO: make sure this is OK
if (SDL2_IMAGE_LIBRARIES)
    message(STATUS "SDL2_IMAGE library path: ${SDL2_IMAGE_LIBRARIES}")
    target_link_libraries(${VIZ_LIB} ${SDL2_IMAGE_LIBRARIES})
else()
    message(STATUS "SDL2_IMAGE not found using the SDL2_IMAGE_LIBRARIES var, trying SDL2_image::SDL2_image")
    target_link_libraries(${VIZ_LIB} SDL2_image::SDL2_image)
endif()

configure_file(cats-fluffy.png cats-fluffy.png COPYONLY)
