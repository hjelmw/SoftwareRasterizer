cmake_minimum_required(VERSION 3.10)

project(Rasterizer)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories("${CMAKE_SOURCE_DIR}/src/")

file(GLOB SOURCES src/*)
add_executable(Rasterizer ${SOURCES})
target_link_libraries(Rasterizer ${SDL2_LIBRARIES})
