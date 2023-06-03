project(charge_engine_test)

# Cygwin, under strict ANSI C, does not declare some
# functions needed by Boost Test.
set(CMAKE_CXX_EXTENSIONS ON)


set(platform_sources)

if(WIN32)

    list(APPEND platform_sources
        helpers_win.cpp
    )

else()

    list(APPEND platform_sources
        helpers_posix.cpp
    )

endif()

add_executable(charge_engine_tests
    initialization.cpp
    test_cache.cpp
    test_command_line.cpp
    test_compiler.cpp
    test_process.cpp
    test_imports.cpp
    test_InclusionNotesPredicate.cpp

    ${platform_sources}
)

target_link_libraries(charge_engine_tests
    charge_engine
    ${CONAN_LIBS})

add_test(charge_engine_tests charge_engine_tests)


# ShellProcess testing is done through a standalone
# program - not through actual unit testing.

add_executable(run_process
    run_process.cpp)

target_link_libraries(run_process
    charge_engine
    stirrup)


# exec testing is done through another standalone
# program.

add_executable(run_exec
    run_exec.cpp)

target_link_libraries(run_exec
    charge_engine
    stirrup)
