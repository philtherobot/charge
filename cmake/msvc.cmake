if (WIN32)
    # yaml-cpp 0.6.3 causes these warnings.  We choose to silence this across the project.
    add_definitions(-D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)

    # This is a cross-platform project.  The inability to use a classic (yet unsafe function) is an hindrance.
    # Safety is assumed to be maintained through unit testing.  We realize this is not perfect but that
    # it is an acceptable tradeoff.
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()


