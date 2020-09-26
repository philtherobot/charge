add_library(
    stirrup
    include/stirrup/stirrup.hpp
    src/getenv_${PLATFORM_SOURCE}.cpp
    src/make_errno_message.cpp
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
