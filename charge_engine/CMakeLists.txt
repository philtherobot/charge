project(charge_engine)

add_library(charge_engine
    src/engine.cpp)

target_link_libraries(charge_engine 
	Boost::boost
	Boost::filesystem
	yaml-cpp)

target_include_directories(charge_engine PUBLIC ${YAML_CPP_INCLUDE_DIR})

target_include_directories(charge_engine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

add_subdirectory(test)
