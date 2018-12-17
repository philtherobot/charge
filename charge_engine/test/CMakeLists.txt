project(charge_engine_test)

# Cygwin, under strict ANSI C, does not declare some
# functions needed by Boost Test.
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(tests
    initialization.cpp
    engine.cpp
	compiler.cpp
	InclusionNotesSniffer.cpp)

target_link_libraries(tests
    charge_engine
	${CONAN_LIBS})

add_test(tests tests)


# ShellProcess testing is done through a standalone
# program - not through actual unit testing.

add_executable(process_test
    process_test.cpp)

target_link_libraries(process_test
    charge_engine)


# exec testing is done through another standalone
# program.

add_executable(exec_test
	exec_test.cpp)

target_link_libraries(exec_test
	charge_engine)
