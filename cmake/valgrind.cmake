# valgrind
# This CMake module adds a custom target 'memcheck' that runs Valgrind memory checking on the 'run_tests' executable. It checks for memory leaks and other memory-related issues in the tests. The Valgrind output is saved to a log file in the specified report directory. If Valgrind is not found, a warning message is displayed and the 'memcheck' target is not created.
# Usage:
# To enable Valgrind memory checking, simply include this CMake module in your main CMakeLists file:
# include(cmake/valgrind.cmake)
# Note: Valgrind is a powerful tool for detecting memory leaks and other memory-related issues, but it may not work properly with some executables that use multiple threads or have a large memory footprint. In such cases, you may need to use other tools like AddressSanitizer for more comprehensive memory checking.
# Note: Valgrind is only available on Linux and may not be available on other platforms. Make sure to have Valgrind installed and properly configured in your development environment to use this feature.
# if(USE_VALGRIND)

# find valgrind
find_program(VALGRIND_PROGRAM valgrind)

if(VALGRIND_PROGRAM)
    # 'make memcheck' (or cmake --build . --target memcheck)
    set(valgrind_log "valgrind-log.txt")

    add_custom_target(memcheck
        COMMAND ${VALGRIND_PROGRAM}
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --log-file=${REPORT_DIR}/${valgrind_log}
            $<TARGET_FILE:run_tests>
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running Valgrind memory check on run_tests..."
        USES_TERMINAL
    )
else()
    message(WARNING "Valgrind not found! 'memcheck' target will not be created.")
endif()
