# strict
# This CMake module adds strict compiler warnings and sanitizers to the project when the USE_CHECK_COMPILER_STRICT option is enabled. It configures the target_compile_options for the common settings target to include various warning flags and sanitizers that help catch potential issues in the code during compilation. This can help improve code quality and catch bugs early in the development process.
# Usage:
# To enable strict compiler warnings and sanitizers, set the USE_CHECK_COMPILER_STRICT variable to ON when configuring the project:
# cmake -DUSE_CHECK_COMPILER_STRICT=ON ..
# Note: Enabling strict compiler warnings and sanitizers may increase compilation time and may require fixing existing warnings in the codebase. It is recommended to enable this option during development and testing to catch potential issues, but it can be disabled for production builds if necessary.
# if(USE_CHECK_COMPILER_STRICT)

#message(STATUS "Add compile option -Wall -Wextra -Wpedantic -Wshadow -Wconversion")
#target_compile_options(${CMAKE_PROJECT_NAME}_common_settings INTERFACE
#    -Wall -Wextra -Wpedantic -Wshadow -Wconversion
#)
target_compile_options(${CMAKE_PROJECT_NAME}_common_settings INTERFACE
    -Werror -Wall -Wextra -Wswitch-default -Wswitch-enum -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wmissing-noreturn -fsanitize=address -fno-omit-frame-pointer -Wsign-compare
)

