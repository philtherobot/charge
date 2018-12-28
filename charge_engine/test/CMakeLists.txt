project(charge_engine_test)

# Cygwin, under strict ANSI C, does not declare some
# functions needed by Boost Test.
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(tests
	test_cache.cpp
	test_compiler.cpp
    test_process.cpp
	test_imports.cpp
    test_initialization.cpp
	test_InclusionNotesSniffer.cpp)

target_link_libraries(tests
    charge_engine
	${CONAN_LIBS})

add_test(tests tests)


# ShellProcess testing is done through a standalone
# program - not through actual unit testing.

add_executable(run_process
    run_process.cpp)

target_link_libraries(run_process
    charge_engine)


# exec testing is done through another standalone
# program.

add_executable(run_exec
	run_exec.cpp)

target_link_libraries(run_exec
	charge_engine)
