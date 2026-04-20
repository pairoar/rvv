if (USE_CLANG_FORMAT)
    find_program(CLANG_FORMAT_CMD NAMES clang-format)
    if(CLANG_FORMAT_CMD)
        add_custom_target(format
            COMMAND ${CLANG_FORMAT_CMD} -i -style=file ${ALL_SOURCE_FILES}
            COMMENT "Auto-formatting all source files according to .clang-format..."
        )
    endif()
endif()