
message("Detect support locales")

if(${PLATFORM} STREQUAL "posix")
    message("posix")
    execute_process(COMMAND locale -a OUTPUT_VARIABLE installed_locales)
    message("found ${installed_locales}")
endif()
