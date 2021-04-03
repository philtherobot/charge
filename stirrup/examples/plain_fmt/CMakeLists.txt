add_executable(plain_fmt plain_fmt.cpp)

target_link_libraries(plain_fmt CONAN_PKG::fmt)
