project(tools_test)

# Cygwin, under strict ANSI C, does not declare some
# functions needed by Boost Test.
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(process_test
    process_test.cpp)

target_link_libraries(process_test
    tools)
