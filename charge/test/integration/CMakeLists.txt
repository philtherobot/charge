add_executable(charge_test_integration_tests
    initialization.cpp
    test_find_script_path.cpp
)

target_link_libraries(charge_test_integration_tests
    CONAN_PKG::catch2
)

add_test(charge_test_integration_tests charge_test_integration_tests)
