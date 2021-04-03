project(charge_engine_test)

# Cygwin, under strict ANSI C, does not declare some
# functions needed by Boost Test.
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(
    charge_engine_tests
    initialization.cpp
    test_cache.cpp
    test_command_line.cpp
    test_compiler.cpp
    test_process.cpp
    test_imports.cpp
    test_InclusionNotesPredicate.cpp
    helpers_${PLATFORM_SOURCE}.cpp
)

target_link_libraries(
    charge_engine_tests
    charge_engine
)

add_test(
    charge_engine_tests 
    charge_engine_tests
)


# ShellProcess testing is done through a standalone
# program - not through actual unit testing.

add_executable(
    run_process
    run_process.cpp
)

target_link_libraries(
    run_process
    charge_engine
)


# exec testing is done through another standalone
# program.

add_executable(
    run_exec
    run_exec.cpp
)

target_link_libraries(
    run_exec
    charge_engine
)
