
add_subdirectory(fixtures)
add_subdirectory(integration)
add_subdirectory(support)

add_executable(stirrup_tests
    environment_variable_test.cpp
    initialization.cpp
    error_test.cpp
    string_test.cpp
    stream_cooker_reader_test.cpp
)

target_link_libraries(
    stirrup_tests
    stirrup
    test_fixtures
    ${CONAN_LIBS}
)

add_test(stirrup_tests stirrup_tests)
