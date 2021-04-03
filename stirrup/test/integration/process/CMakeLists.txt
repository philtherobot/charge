
add_executable(stirrup_process_invoke stirrup_process_invoke.cpp)

target_link_libraries(
    stirrup_process_invoke
    stirrup
)

add_executable(
    stirrup_child_process_actor
    stirrup_child_process_actor.cpp
)

target_link_libraries(
    stirrup_child_process_actor
    stirrup
)

configure_file(test.in.ps1 ${CMAKE_CURRENT_BINARY_DIR}/test.ps1)
