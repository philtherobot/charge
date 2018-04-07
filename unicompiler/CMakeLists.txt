project(unicompiler)

add_library(unicompiler
    src/unicompiler.cpp)

target_link_libraries(unicompiler 
	yaml-cpp)

target_include_directories(unicompiler PUBLIC ${YAML_CPP_INCLUDE_DIR})

target_include_directories(unicompiler PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)
