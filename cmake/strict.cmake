if(USE_CHECK_COMPILER_STRICT)
    #message(STATUS "Add compile option -Wall -Wextra -Wpedantic -Wshadow -Wconversion")
    #target_compile_options(${CMAKE_PROJECT_NAME}_common_settings INTERFACE 
    #    -Wall -Wextra -Wpedantic -Wshadow -Wconversion
    #)
    target_compile_options(${CMAKE_PROJECT_NAME}_common_settings INTERFACE 
        -Werror -Wall -Wextra -Wswitch-default -Wswitch-enum -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wmissing-noreturn -fsanitize=address -fno-omit-frame-pointer -Wsign-compare
    )
endif()
