add_library(
    stirrup
    include/stirrup/ReadableStream.hpp
    include/stirrup/StreamCookerReader.hpp
    include/stirrup/environment_variable.hpp
    include/stirrup/error.hpp
    include/stirrup/stirrup.hpp
    include/stirrup/string.hpp
    src/StreamCookerReader.cpp
    src/environment_variable_${PLATFORM_SOURCE}.cpp
    src/error.cpp
    src/process_${PLATFORM_SOURCE}.cpp
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
