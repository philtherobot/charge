cmake_minimum_required(VERSION 3.5)
project(charge)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/externals)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

find_package(Boost REQUIRED COMPONENTS filesystem)

find_package(YamlCpp REQUIRED)
add_library(yaml-cpp STATIC IMPORTED)

add_subdirectory(charge)
add_subdirectory(charge_engine)
