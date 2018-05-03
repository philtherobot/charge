project(tools)

add_library(tools
    src/process_win.cpp)

target_link_libraries(tools Boost::boost)

target_include_directories(tools PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

add_subdirectory(test)
