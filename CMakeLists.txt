cmake_minimum_required(VERSION 3.5)
project(charge)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_subdirectory(charge)
add_subdirectory(charge_engine)
add_subdirectory(unicompiler)
