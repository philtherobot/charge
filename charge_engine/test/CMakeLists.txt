project(charge_engine_test)

# Cygwin, under strict ANSI C, does not declare some
# functions needed by Boost Test.
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(tests
    test_initialization.cpp
    test_engine.cpp
	test_compiler.cpp
	test_InclusionNotesSniffer.cpp)

target_link_libraries(tests
    charge_engine
	${CONAN_LIBS})

add_test(tests tests)


# ShellProcess testing is done through a standalone
# program - not through actual unit testing.

add_executable(test_process
    test_process.cpp)

target_link_libraries(test_process
    charge_engine)


# exec testing is done through another standalone
# program.

add_executable(test_exec
	test_exec.cpp)

target_link_libraries(test_exec
	charge_engine)
