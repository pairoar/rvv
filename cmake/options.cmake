#
# Options
#
# for preventing CACHE issue, use option keyword
#

# ---------------------------------------------------------
# Build 32
# ---------------------------------------------------------
if(TARGET_ARCH STREQUAL "host")
    option(BUILD_32 "Build 32bit" OFF)
else()
    option(BUILD_32 "Build 32bit" OFF)
endif()

# ---------------------------------------------------------
# Verification
# ---------------------------------------------------------
option(USE_VERIFICATION "Enable Simulator" OFF)

# ---------------------------------------------------------
# Debug
# ---------------------------------------------------------
option(USE_DEBUG "Debug Feature" OFF)

# ---------------------------------------------------------
# Optimize for embedded C++
# ---------------------------------------------------------
option(USE_EMBEDDED_C++ "Embedded C++ comile flags" OFF)


# ---------------------------------------------------------
# H/W Custom Accelerator
# ---------------------------------------------------------
option(USE_ACCELERATOR "H/W Custom Accelerator" OFF)

# ---------------------------------------------------------
# Static analysis & formatting
#   -Werror -Wall -Wextra -Wswitch-default -Wswitch-enum -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wmissing-noreturn
# ---------------------------------------------------------
option(USE_CHECK_COMPILER_STRICT ON)

# ---------------------------------------------------------
# Target A: Cppcheck (memory leak, bug)
# usage: make cppcheck
# ---------------------------------------------------------
option(USE_CPPCHECK "Enable static analysis with cppcheck" OFF)

# ---------------------------------------------------------
# 타겟 B: Clang-Tidy (C++ Core Guidelines, Google Style 검사)
# 사용법: make lint
# ---------------------------------------------------------
option(USE_CLANG_TIDY "Enable linting with clang-tidy" OFF)

# ---------------------------------------------------------
# 타겟 C: Clang-Format (코드 자동 정렬)
# 사용법: make format
# ---------------------------------------------------------
option(USE_CLANG_FORMAT "Auto-formatting all source files" OFF)

# ---------------------------------------------------------
# Target : GoogleTest (googletest)
# Description : others are located in external.cmake
# ---------------------------------------------------------
option(USE_GOOGLETEST "Google Test Framework" OFF)

# ---------------------------------------------------------
# Target : Coverage Test
# Description : Enable/Disable coverage test
# ---------------------------------------------------------
option(USE_COVERAGE "Enable code coverage generation" OFF)

# ---------------------------------------------------------
# Target : memcheck
# Description : Enable/Disable valgrind memory check
# ---------------------------------------------------------
option(USE_MEMCHECK "Enable/Disable memory check" OFF)
if (USE_MEMCHECK)
    if (NOT APPLE)
        option(USE_VALGRIND "Enable/Disable valgrind memory check" ON)
    else()
        option(USE_LEAKS "Enable/Disable leaks memory check" ON)
    endif()
endif()

# ---------------------------------------------------------
# Target : Doxygen
# Description : Enable/Disable doxygen
# ---------------------------------------------------------
option(USE_DOXYGEN "Enable/Disable doxygen" OFF)


# ---------------------------------------------------------
# Prepare action for
#       cppcheck, clang_tidy, gtest, ....
# ---------------------------------------------------------
if (USE_CPPCHECK OR USE_CLANG_TIDY OR USE_GOOGLETEST OR USE_COVERAGE)
    #  make report directory (build/reports)
    set(REPORT_DIR "${CMAKE_BINARY_DIR}/reports")
    file(MAKE_DIRECTORY ${REPORT_DIR})
endif()
