cmake_minimum_required(VERSION 3.10)

project(main C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_PREFIX_PATH "C:/msys64/ucrt64")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

file(GLOB SOURCES "*.c")

add_executable(main ${SOURCES})

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

target_link_libraries(main
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    SDL2_mixer::SDL2_mixer
)