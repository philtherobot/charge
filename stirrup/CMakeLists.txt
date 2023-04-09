add_library(
    stirrup
    include/stirrup/environment_variable.hpp
    include/stirrup/error.hpp
    include/stirrup/file.hpp
    include/stirrup/format.hpp
    include/stirrup/locale.hpp
    include/stirrup/readable_stream.hpp
    include/stirrup/standard_streams.hpp
    include/stirrup/stirrup.hpp
    include/stirrup/stream.hpp
    include/stirrup/stream_cooker_reader.hpp
    include/stirrup/string.hpp
    include/stirrup/string_windows.hpp
    src/environment_variable_${PLATFORM_SOURCE}.cpp
    src/error.cpp
    src/file.cpp
    src/file_windows.cpp  #// todo-php:  use ${PLATFORM_SOURCE}
    src/format.cpp
    src/locale.cpp
    src/process_${PLATFORM_SOURCE}.cpp
    src/standard_streams.cpp
    src/stream.cpp
    src/stream_cooker_reader.cpp
    src/string.cpp
    src/string_windows.cpp #// todo-php:  use ${PLATFORM_SOURCE}
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

add_subdirectory(examples)
add_subdirectory(proof_of_concept)
add_subdirectory(test)
