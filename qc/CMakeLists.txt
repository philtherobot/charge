cmake_minimum_required(VERSION 3.5)
project(qc)

add_executable(qc
    src/qc.cpp)

target_link_libraries(qc
    quickc_engine)