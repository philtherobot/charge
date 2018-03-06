cmake_minimum_required(VERSION 3.5)
project(quickc_engine)

add_library(quickc_engine
    src/engine.cpp)

target_link_libraries(quickc_engine 
	boost_filesystem
	boost_system
	yaml-cpp)

target_include_directories(quickc_engine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

add_executable(tests
    test/tests.cpp)

target_link_libraries(tests
    quickc_engine)

add_test(tests tests)