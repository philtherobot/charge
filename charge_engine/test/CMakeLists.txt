cmake_minimum_required(VERSION 3.5)
project(charge_engine_test)

# Cygwin, under strict ANSI C, does not declare some
# functions needed by Boost Test.
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(tests
    tests.cpp)

target_link_libraries(tests
    charge_engine)

add_test(tests tests)
