
add_executable(console_stream_test console_stream_test.cpp)

target_link_libraries(
    console_stream_test
    stirrup
    ${CONAN_LIBS}
)
