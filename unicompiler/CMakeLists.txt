cmake_minimum_required(VERSION 3.5)
project(unicompiler)

add_library(unicompiler
    src/Compiler.cpp)

target_include_directories(unicompiler PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)
