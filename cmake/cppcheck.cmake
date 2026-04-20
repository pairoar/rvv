if(USE_CPPCHECK)
    find_program(CPPCHECK_CMD NAMES cppcheck)
    if(CPPCHECK_CMD)
        add_custom_target(cppcheck
            COMMAND ${CMAKE_COMMAND} -E make_directory ${REPORT_DIR}
            COMMAND ${CPPCHECK_CMD} --enable=warning,performance,portability --inconclusive --force --inline-suppr --output-file=${REPORT_DIR}/cppcheck_report.txt ${ALL_SOURCE_FILES}
            COMMENT "Running cppcheck... Report saved to ${REPORT_DIR}/cppcheck_report.txt"
        )
    endif()
endif()
