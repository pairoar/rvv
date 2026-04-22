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

cmake_minimum_required(VERSION 3.20.0)
project(RVV_APP
    VERSION 1.0.0
    LANGUAGES C CXX ASM)


# ==========================================
# 글로벌 및 인터페이스 설정
# ==========================================
add_library(${CMAKE_PROJECT_NAME}_common_settings INTERFACE)
target_compile_features(${CMAKE_PROJECT_NAME}_common_settings INTERFACE cxx_std_17)
install(TARGETS ${CMAKE_PROJECT_NAME}_common_settings EXPORT HalTargets)

# CMake 검색 경로 추가
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# 옵션 및 정의
include(options OPTIONAL)
include(cust_definition)

# 아키텍처 특화 설정
if(TARGET_ARCH STREQUAL "host")
    if(BUILD_32)
        include(build32)
    else()
        message(STATUS "Build 64bit")
        target_compile_definitions(${CMAKE_PROJECT_NAME}_common_settings INTERFACE CONFIG_SUPPORT_64)
    endif()
endif()

# 코드 품질 및 분석 도구 모음
if(USE_CHECK_COMPILER_STRICT)
    include(strict)
endif()
if (USE_EMBEDDED_C++)
    add_library(cp_cxx_options INTERFACE)
    include(embedded_c++)
endif()
if(USE_CLANG_FORMAT)
    include(clang_format)
endif()
if(USE_CLANG_TIDY)
    include(clang_tidy)
endif()
if(USE_DOXYGEN)
    include(doxygen)
endif()

# Host 전용 메모리 체크 환경변수 세팅
if(TARGET_ARCH STREQUAL "host" AND USE_MEMCHECK)
    if(NOT APPLE)
        include(valgrind)
        set(ENV{VALGRIND_OPTS} "--leak-check=full --track-origins=yes")
    else()
        include(leaks)
        set(ENV{LEAKS_OPTS} "-atExit")
        set(ENV{MallocStackLogging} 1)
    endif()
endif()

# ==========================================
# 헤더 경로 및 설치 경로 설정
# ==========================================
add_library(project_inc INTERFACE)
target_include_directories(project_inc INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/inc>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/inc>"
    "$<INSTALL_INTERFACE:inc>"
)
install(TARGETS project_inc EXPORT HalTargets)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/inc/config.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/inc/config.h" @ONLY)

# 사용자가 직접 CMAKE_INSTALL_PREFIX를 지정하지 않은 경우에만 기본값 세팅
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/build/output" CACHE PATH "Default install path" FORCE)
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/bin")

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/inc/insn_api.h DESTINATION include)


# ==========================================
# 서브 모듈 추가 (FetchContent 대신 add_subdirectory 사용)
# ==========================================
option(BUILD_HAL_TEST "Build HAL test target instead of full application" OFF)

if(BUILD_HAL_TEST)
    message(STATUS "BUILD_HAL_TEST is ON: Excluding 'app' module.")
    set(modules hal)
else()
    if(USE_GOOGLETEST)
        message(STATUS "USE_GOOGLETEST is ON: Including 'test' module.")
        set(modules hal lib util app test)
        enable_testing()
        include(gtest OPTIONAL)
    else()
        message(STATUS "USE_GOOGLETEST is OFF: Excluding 'test' module.")
        set(modules hal lib util app)
    endif()
endif()


# ==========================================
# HAL Test 빌드 시 app 모듈 제외, gtest 포함 여부에 따라 test 모듈 포함 여부 결정
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
# 로컬 모듈 빌드 트리 포함
#foreach(mod ${modules})
#    add_subdirectory(${mod})
#endforeach()

# FetchContent를 사용하여 모듈을 추가하고 빌드 시스템에 통합
foreach(mod ${modules})
    FetchContent_Declare(${mod} SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${mod})
    FetchContent_MakeAvailable(${mod})
endforeach()



# ==========================================
# 커버리지 분석
# ==========================================
if(USE_COVERAGE)
    include(coverage)
    add_coverage_target(cov "*/test/*" "*/extern/*" "/usr/*")
endif()


