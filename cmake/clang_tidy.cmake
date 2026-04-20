if(USE_CLANG_TIDY)
    find_program(CLANG_TIDY_CMD NAMES clang-tidy)
    if(CLANG_TIDY_CMD)
        # When building, compile_commands.json will be generated automatically in build/ directory)
        set(CMAKE_EXPORT_COMPILE_COMMANDS ON)       # default ON

        # find source files to be checked (exclude external library)
        file(GLOB_RECURSE ALL_SOURCE_FILES
            "application/*.c" "application/*.h" "application/*.cpp" "application/*.hpp"
            "hal/*.c" "hal/*.h" "hal/*.cpp" "hal/*.hpp"
            "lib/*.c" "lib/*.h" "lib/*.cpp" "lib/*.hpp"
            "util/*.c" "util/*.h" "util/*.cpp" "util/*.hpp"
            "inc/*.h" "inc/*.hpp"
        )
        # in order to shell script error, semi-colong(;) will be replaced with empty( )
        string(REPLACE ";" " " ALL_SOURCE_FILES_SPACES "${ALL_SOURCE_FILES}")

        set(CLANG_TIDY_CHECKS "-*,cppcoreguidelines-*,google-*,readability-*")
        add_custom_target(lint
            COMMAND ${CMAKE_COMMAND} -E make_directory ${REPORT_DIR}
            COMMAND ${CLANG_TIDY_CMD} -p ${CMAKE_BINARY_DIR} -checks=${CLANG_TIDY_CHECKS} ${ALL_SOURCE_FILES} > ${REPORT_DIR}/clang_tidy_report.txt
            COMMENT "Running clang-tidy... Report saved to ${REPORT_DIR}/clang_tidy_report.txt"
        )
    endif()
endif()
