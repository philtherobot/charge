cmake_minimum_required(VERSION 3.5)
project(charge)

add_executable(charge
    src/charge.cpp)

target_link_libraries(charge
    charge_engine)
