cmake_minimum_required(VERSION 3.10)

project(MySDLGame C)

set(SDL2_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SDL2_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

include_directories(${SDL2_INCLUDE_DIR})

link_directories(${SDL2_LIB_DIR})

file(GLOB SOURCES "*.c")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_executable(MySDLGame ${SOURCES})

target_link_libraries(MySDLGame 
    mingw32 
    SDL2main 
    SDL2 
    SDL2_image 
    SDL2_ttf 
    SDL2_mixer
)