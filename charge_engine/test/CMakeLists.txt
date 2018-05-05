project(charge_engine_test)

# Cygwin, under strict ANSI C, does not declare some
# functions needed by Boost Test.
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(tests_engine
    tests.cpp)

target_link_libraries(tests_engine
    charge_engine)

add_test(tests_engine tests_engine)


add_executable(process_test
    process_test.cpp)

target_link_libraries(process_test
    charge_engine)
