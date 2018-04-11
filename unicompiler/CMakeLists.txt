project(unicompiler)

add_library(unicompiler
    src/unicompiler.cpp
    src/process_win.cpp)

target_link_libraries(unicompiler 
    Boost::filesystem
    yaml-cpp)

target_include_directories(unicompiler PUBLIC ${YAML_CPP_INCLUDE_DIR})

target_include_directories(unicompiler PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

add_subdirectory(test)
