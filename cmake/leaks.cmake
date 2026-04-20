# leaks
# we need set MallocStackLogging to 1 (export MallocStackLogging=1)
# to get the stack trace of leaks, and then we can use leaks --atExit -- <executable> to check for leaks after the executable finishes.
# Note: leaks is only available on macOS, and it may not work properly with some executables that use multiple threads or have a large memory footprint. In such cases, you may need to use other tools like Valgrind or AddressSanitizer for more comprehensive memory leak detection.
# Usage:
# To enable leaks memory checking, simply include this CMake module in your main CMakeLists file:
# include(cmake/leaks.cmake)
if (APPLE)
    find_program(LEAKS_PROGRAM leaks)

    if(LEAKS_PROGRAM)
        # 'make memcheck' (or cmake --build . --target memcheck)

        message(CHECK_START "Fetching Leaks")
        set(leaks_log "leaks-log.txt")

        add_custom_target(memcheck
            COMMAND ${LEAKS_PROGRAM}
                --atExit
                -- $<TARGET_FILE:vmath_app>
                >& ${REPORT_DIR}/${leaks_log}  # output log file
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Running Leaks memory check on run_tests..."
            USES_TERMINAL
        )
        message(CHECK_PASS "fetched")
    else()
        message(WARNING "leaks not found! 'memcheck' target will not be created.")
    endif()
endif()
