add_library(
    stirrup
    include/stirrup/ReadableStream.hpp
    include/stirrup/StreamCookerReader.hpp
    include/stirrup/stirrup.hpp
    src/StreamCookerReader.cpp
    src/getenv_${PLATFORM_SOURCE}.cpp
    src/make_errno_message.cpp
    src/process_${PLATFORM_SOURCE}.cpp
    src/quote_if_needed.cpp
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
