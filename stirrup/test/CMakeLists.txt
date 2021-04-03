
include(detect_supported_locales.cmake)

add_subdirectory(fixtures)
add_subdirectory(integration)
add_subdirectory(support)

set(SOURCES 
#    environment_variable_test.cpp
#    error_test.cpp
#    file_test.cpp
#    format_test.cpp
#    locale_test.cpp
    initialization.cpp
#    stream_test.cpp
    string_test.cpp
)

if(${PLATFORM} STREQUAL "windows")
    set(SOURCES ${SOURCES} string_windows_test.cpp)
endif()


add_executable(stirrup_tests ${SOURCES})

target_link_libraries(
    stirrup_tests
    stirrup
    test_fixtures
    test_support
    CONAN_PKG::catch2
)

add_test(stirrup_tests stirrup_tests)
