add_library(test_fixtures
    src/fake_readable_stream.cpp
    src/installed_locales_${PLATFORM_SOURCE}.cpp
)

target_include_directories(test_fixtures PUBLIC include)

target_link_libraries(test_fixtures
    stirrup
)
