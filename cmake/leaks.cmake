# leaks
# we need set MallocStackLogging to 1 (export MallocStackLogging=1)

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
