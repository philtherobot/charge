cmake_minimum_required(VERSION 3.5)
project(quickc)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_subdirectory(qc)
add_subdirectory(quickc_engine)
add_subdirectory(unicompiler)
