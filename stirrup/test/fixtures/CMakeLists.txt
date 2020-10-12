add_library(test_fixtures
    src/FakeReadableStream.cpp
)

target_include_directories(test_fixtures PUBLIC include)

target_link_libraries(test_fixtures
    stirrup
)
