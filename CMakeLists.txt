cmake_minimum_required(VERSION 3.10)

project(main C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_PREFIX_PATH "C:/msys64/ucrt64")

find_package(SDL2 REQUIRED)

file(GLOB SOURCES "*.c")

add_executable(main ${SOURCES})

target_link_libraries(main
    SDL2::SDL2
    SDL2::SDL2main
)