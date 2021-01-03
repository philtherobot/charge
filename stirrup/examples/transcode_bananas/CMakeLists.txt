add_executable(transcode_bananas transcode_bananas.cpp)

target_link_libraries(transcode_bananas stirrup ${CONAN_LIBS})
