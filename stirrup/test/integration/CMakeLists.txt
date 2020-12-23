add_subdirectory(console_stream)
add_subdirectory(process)

configure_file(test.ps1 ${CMAKE_CURRENT_BINARY_DIR}/test.ps1)
