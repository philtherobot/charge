project(charge_engine)


set(platform_sources)

if(WIN32)

    list(APPEND platform_sources
        src/platform_win.cpp
        src/process_win.cpp
        src/tools_win.cpp
    )

else()

    list(APPEND platform_sources
        src/platform_posix.cpp
        src/process_posix.cpp
        src/tools_posix.cpp
    )

endif()


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

    ${platform_sources}
)

target_link_libraries(charge_engine ${CONAN_LIBS})

target_include_directories(charge_engine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

add_subdirectory(test)
