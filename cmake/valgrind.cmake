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