
add_subdirectory(integration)
add_subdirectory(support)

add_executable(stirrup_tests
    getenv_test.cpp
    initialization.cpp
    make_errno_message_test.cpp
    quote_if_needed_test.cpp
)

target_link_libraries(
    stirrup_tests
    stirrup
    ${CONAN_LIBS}
)

add_test(stirrup_tests stirrup_tests)
