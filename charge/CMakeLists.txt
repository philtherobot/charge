project(charge)

add_executable(charge
    src/charge.cpp)

target_link_libraries(charge
    charge_engine)

add_subdirectory(test)
