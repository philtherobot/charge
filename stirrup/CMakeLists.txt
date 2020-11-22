add_library(
    stirrup
    include/stirrup/environment_variable.hpp
    include/stirrup/error.hpp
    include/stirrup/readable_stream.hpp
    include/stirrup/stirrup.hpp
    include/stirrup/stream_cooker_reader.hpp
    include/stirrup/string.hpp
    src/environment_variable_${PLATFORM_SOURCE}.cpp
    src/error.cpp
    src/process_${PLATFORM_SOURCE}.cpp
    src/stream_cooker_reader.cpp
    src/string.cpp
)

target_link_libraries(
    stirrup
    ${CONAN_LIBS}
)

target_include_directories(
    stirrup
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src
)

add_subdirectory(test)
