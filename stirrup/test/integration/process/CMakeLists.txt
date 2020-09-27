
add_executable(stirrup_process_invoke stirrup_process_invoke.cpp)

target_link_libraries(
    stirrup_process_invoke
    stirrup
    ${CONAN_LIBS}
)

add_executable(stirrup_child_process_actor stirrup_child_process_actor.cpp)

configure_file(test.ps1.in ${CMAKE_CURRENT_BINARY_DIR}/test.ps1)
