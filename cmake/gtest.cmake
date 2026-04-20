# ##############################################################################
# This CMake module uses FetchContent to download and integrate GoogleTest into the project. It checks if the target architecture is a bare-metal environment (e.g., RISC-V) and disables pthreads if necessary. The fetched GoogleTest library will be available for use in the project, allowing you to write and run unit tests using GoogleTest.
# Usage:
# To fetch and integrate GoogleTest, simply include this CMake module in your main CMakeLists file:
# include(cmake/gtest.cmake)
# ===========================================================================
# Check if GoogleTest is already fetched
# The FetchContent module is used to download and integrate GoogleTest into the project. It checks if the target architecture is a bare-metal environment (e.g., RISC-V) and disables pthreads if necessary. The fetched GoogleTest library will be available for use in the project, allowing you to write and run unit tests using GoogleTest.
# ===========================================================================

# if(USE_GOOGLETEST)

message(CHECK_START "Fetching GoogleTest")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# CPU 아키텍처 이름 대신, 베어메탈(Generic) 환경인지 확인하여 적용하거나
# 기존처럼 아키텍처를 쓰시려면 정규식(MATCHES)으로 여러 네이밍을 커버합니다.
if(CMAKE_SYSTEM_NAME STREQUAL "Generic" OR TARGET_ARCH MATCHES "^(rv32|rv64|riscv32|riscv64)$")
    set(gtest_disable_pthreads ON CACHE BOOL "Disable pthreads for bare-metal" FORCE)
endif()

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
    # PATCH_COMMAND git apply --ignore-whitespace "${CMAKE_CURRENT_LIST_DIR}/../../patches/abc.patch"
)
FetchContent_MakeAvailable(googletest)

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "fetched")
