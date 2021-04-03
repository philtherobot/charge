project(charge_engine)

add_library(charge_engine
    src/cache.cpp
    src/charge.cpp
    src/compiler.cpp
    src/command_line.cpp
    src/config.cpp
    src/InclusionNotesPredicate.cpp
    src/exception.cpp
    src/header_dependencies.cpp
    src/imports.cpp
    src/tools.cpp
    src/types.cpp
    src/platform_${PLATFORM_SOURCE}.cpp
    src/process_${PLATFORM_SOURCE}.cpp
    src/tools_${PLATFORM_SOURCE}.cpp
)

target_link_libraries(charge_engine 
    CONAN_PKG::boost
    CONAN_PKG::yaml-cpp 
)

target_include_directories(charge_engine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

add_subdirectory(test)
