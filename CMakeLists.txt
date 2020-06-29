cmake_minimum_required(VERSION 3.5)
project(charge)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include("${CMAKE_SOURCE_DIR}/cmake/BuildType.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/platform_source.cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_subdirectory(charge)
add_subdirectory(charge_engine)
add_subdirectory(i18n_lab)
add_subdirectory(stirrup)
