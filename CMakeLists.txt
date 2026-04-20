# 툴체인 파일은 반드시 project() 호출 전에 정의되어야 합니다.
if(NOT CMAKE_TOOLCHAIN_FILE)
    if(TARGET_ARCH STREQUAL "arm")
        message(STATUS "Target architecture: ARM")
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain-arm.cmake")
    elseif(TARGET_ARCH STREQUAL "riscv32")
        message(STATUS "Target architecture: RISC-V 32-bit")
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain-riscv32.cmake")
    elseif(TARGET_ARCH STREQUAL "riscv64")
        message(STATUS "Target architecture: RISC-V 64-bit")
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain-riscv64.cmake")
    elseif(TARGET_ARCH STREQUAL "cust_accel")
        message(STATUS "Target architecture: Custom Accelerator")
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain-ca.cmake")
    else()
        message(STATUS "No architecture specified, building for Host.")
        set(TARGET_ARCH "host")
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain-host.cmake")
    endif()
endif()

cmake_minimum_required(VERSION 3.12)
project(RVV_APP
    VERSION 1.0.0
    LANGUAGES C CXX ASM)


# Target : {CMAKE_PROJECT_NAME}_common_settings
# Description : C++ Standard
add_library(${CMAKE_PROJECT_NAME}_common_settings INTERFACE)
#target_compile_features(${CMAKE_PROJECT_NAME}_common_settings INTERFACE c_std_17 cxx_std_17)
target_compile_features(${CMAKE_PROJECT_NAME}_common_settings INTERFACE cxx_std_17)
install(TARGETS ${CMAKE_PROJECT_NAME}_common_settings EXPORT HalTargets)

# add cmake search path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# options
include(options OPTIONAL)
include(cust_definition)

# architecture specific settings
if(TARGET_ARCH STREQUAL "host")
    if(BUILD_32)
        include(build32)
    else()
        message(STATUS "Build 64bit")
        target_compile_definitions(${CMAKE_PROJECT_NAME}_common_settings INTERFACE
                CONFIG_SUPPORT_64              # #define CONFIG_SUPPORT_64
            )
    endif()
endif()

# strict compiler settings
if(USE_CHECK_COMPILER_STRICT)
    include(strict)
endif()

# embedded C++
if (USE_EMBEDDED_C++)
    # create INTERFACE Target for compile options
    add_library(cp_cxx_options INTERFACE)
    include(embedded_c++)
endif()

# code analysis tools
if(USE_CLANG_FORMAT)
    include (clang_format)
endif()

# clang-tidy
if(USE_CLANG_TIDY)
    include (clang_tidy)
endif()

# memory check tools
if(TARGET_ARCH STREQUAL "host")
    if (USE_MEMCHECK)
        if (NOT APPLE)
            include (valgrind)
        else()
            include (leaks)
        endif()
    endif()
endif()

# doxygen
if(USE_DOXYGEN)
    include(doxygen)
endif()

# Target : project_inc
# Description : top level header directory
add_library(project_inc INTERFACE)
target_include_directories(project_inc INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/inc>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/inc>"
    "$<INSTALL_INTERFACE:inc>"
)
install(TARGETS project_inc EXPORT HalTargets)

# config.h
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/inc/config.h.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/inc/config.h"
    @ONLY
)

# Set the path for build artifacts (output directory in the project root)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/build/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/output/bin)

# copy the specified inc/insn_api.h file to inc/insn_api.h
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/inc/insn_api.h
        DESTINATION include)

# set env if required
if(TARGET_ARCH STREQUAL "host")
    if (USE_MEMCHECK)
        if (NOT APPLE)
            set(ENV{VALGRIND_OPTS} "--leak-check=full --track-origins=yes")
        else()
            set(ENV{LEAKS_OPTS} "-atExit")
        endif()
    endif()
    if (APPLE AND USE_MEMCHECK)
        set(ENV{MallocStackLogging} 1)
    endif()
endif()

# ==========================================
# [추가/수정된 부분] HAL Test 빌드 옵션 추가
# ==========================================
option(BUILD_HAL_TEST "Build HAL test target instead of full application" OFF)

# modules
include(FetchContent)

# dependency order: lower -> high layer
if(BUILD_HAL_TEST)
    message(STATUS "BUILD_HAL_TEST is ON: Excluding 'app' module.")
    # hal_test 빌드 시 app 모듈 제외
    set(modules hal)
else()
    # 일반 빌드 시 app 포함
    if(USE_GOOGLETEST)
        message(STATUS "USE_GOOGLETEST is ON: Including 'test' module.")
        set(modules hal lib util app test)
    else()
        message(STATUS "USE_GOOGLETEST is OFF: Excluding 'test' module.")
        set(modules hal lib util app)
    endif()
endif()
# ==========================================


# modules
include(FetchContent)

# gtest
if(USE_GOOGLETEST)
    enable_testing()
endif()

# FetchContent를 사용하여 모듈을 추가하고 빌드 시스템에 통합
foreach(mod ${modules})
    FetchContent_Declare(${mod} SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${mod})
    FetchContent_MakeAvailable(${mod})
endforeach()

# gtest
if(USE_GOOGLETEST)
    include(gtest OPTIONAL)
endif()

# coverage
if(USE_COVERAGE)
    include(coverage)
    # 전체 커버리지 리포트 타겟 생성
    add_coverage_target(cov "*/test/*" "*/extern/*" "/usr/*")
    # add_coverage_target(cov  "*/app/*" "/usr/*")
endif()

