# coverage.cmake

# common coverage target & flags
if(NOT TARGET coverage_config)
    add_library(coverage_config INTERFACE)
    target_compile_options(coverage_config INTERFACE --coverage -O0 -g -fprofile-arcs -ftest-coverage)
    target_link_options(coverage_config INTERFACE --coverage -fprofile-arcs -ftest-coverage)
    install(TARGETS coverage_config EXPORT HalTargets)
endif()

# Custom Target function
function(add_coverage_target target_name)
    find_program(GCOV gcov)
    find_program(LCOV lcov)
    find_program(GENHTML genhtml)

    if (NOT GCOV OR NOT LCOV OR NOT GENHTML)
        message(WARNING "Coverage tools not found. Coverage target '${target_name}' will be skipped.")
        return()
    endif()

    set(covname "cov.info")
    set(covdir "coverage_html")
    set(exclude_patterns ${ARGN})

    add_custom_target(${target_name}
        COMMAND ${LCOV} --zerocounters --directory ${CMAKE_BINARY_DIR}
        COMMAND ${LCOV} --capture --initial --directory ${CMAKE_BINARY_DIR} --base-directory ${CMAKE_SOURCE_DIR} --output-file ${REPORT_DIR}/${covname} --gcov-tool ${GCOV}  --ignore-errors mismatch
        #COMMAND ${LCOV} --no-checksum --directory ${CMAKE_BINARY_DIR} --base-directory ${CMAKE_SOURCE_DIR} --capture --output-file ${REPORT_DIR}/${covname} --gcov-tool ${GCOV} --ignore-errors mismatch
        COMMAND ${LCOV} --remove ${REPORT_DIR}/${covname} ${exclude_patterns} --output-file ${REPORT_DIR}/${covname} --ignore-errors mismatch,unused
        COMMAND ${GENHTML} ${REPORT_DIR}/${covname} --output-directory ${REPORT_DIR}/${covdir} --ignore-errors mismatch
        COMMAND ${LCOV} --list ${REPORT_DIR}/${covname}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating HTML coverage report..."
    )

    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${covname}" "${covdir}")
endfunction()