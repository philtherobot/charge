
message("Detect support locales")

if(${PLATFORM} STREQUAL "posix")
    message(STATUS "Detecting available locales...")
    execute_process(COMMAND locale -a OUTPUT_VARIABLE installed_locales)

    string(FIND ${installed_locales} C.UTF-8 find_index)
    if(NOT find_index EQUAL -1)
        message(STATUS "UTF-8 locale available")
        set(INSTALLED_LOCALES "${INSTALLED_LOCALES} { \"UTF8\", \"C.UTF-8\" },")
    endif()

    string(FIND ${installed_locales} en_US.iso88591 find_index)
    if(NOT find_index EQUAL -1)
        message(STATUS "ISO-8859-1 locale available")
        set(INSTALLED_LOCALES "${INSTALLED_LOCALES} { \"Windows Western\", \"en_US.iso88591\" },")
    endif()

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/fixtures/src/installed_locales_posix.cpp.in 
        ${CMAKE_CURRENT_SOURCE_DIR}/fixtures/src/installed_locales_posix.cpp
    )

endif()
