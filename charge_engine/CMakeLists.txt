project(charge_engine)

add_library(charge_engine
    src/cache.cpp
	src/compiler.cpp
    src/engine.cpp
	src/InclusionNotesSniffer.cpp
	src/exception.cpp
	src/process_win.cpp
	src/tools.cpp
	src/tools_win.cpp)

target_link_libraries(charge_engine ${CONAN_LIBS})

target_include_directories(charge_engine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

add_subdirectory(test)
