set(VIZ_LIB "viz")

find_package(SDL2 REQUIRED)
find_package(SDL_image REQUIRED)

add_executable(${VIZ_LIB} main.cpp)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL_IMAGE_INCLUDE_DIRS})

target_link_libraries(${VIZ_LIB} ${SDL2_LIBRARIES} ${SDL_IMAGE_LIBRARIES})

configure_file(cats-fluffy.png cats-fluffy.png COPYONLY)