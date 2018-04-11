project(charge_engine_test)

# Cygwin, under strict ANSI C, does not declare some
# functions needed by Boost Test.
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(tests_unicompiler
    tests.cpp)

target_link_libraries(tests_unicompiler
    unicompiler)

add_test(tests_unicompiler tests_unicompiler)


add_executable(process_test
    process_test.cpp)

target_include_directories(process_test
    PRIVATE ../src)

target_link_libraries(process_test
    unicompiler)
