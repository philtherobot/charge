cmake_minimum_required(VERSION 3.5)
project(charge)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS filesystem)

find_package(yaml-cpp REQUIRED)

add_subdirectory(charge)
add_subdirectory(charge_engine)
add_subdirectory(unicompiler)
add_subdirectory(tools)
