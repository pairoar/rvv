# Assembly Support
if(TARGET_ARCH MATCHES "^riscv")
    enable_language(ASM)
endif()

# Generate HAL Target
# At the beginning, declare empty static library and then add if required
#add_library(hal STATIC hal.c)
add_library(hal STATIC)

# 아키텍처별 매크로 정의 (PRIVATE으로 설정하여 외부 유출 방지 가능)
if(TARGET_ARCH STREQUAL "host")
    target_compile_definitions(hal PRIVATE HAL_HOST)
    set(HAL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/host")
elseif(TARGET_ARCH STREQUAL "riscv32")
    target_compile_definitions(hal PRIVATE HAL_RV32)
    set(HAL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/riscv32")
elseif(TARGET_ARCH STREQUAL "riscv64")
    target_compile_definitions(hal PRIVATE HAL_RV64)
    set(HAL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/riscv64")
elseif(TARGET_ARCH STREQUAL "cust_accel")
    target_compile_definitions(hal PRIVATE HAL_CA)
    set(HAL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cust_accel")
else()
    message(FATAL_ERROR "Unsupported TARGET_ARCH: ${TARGET_ARCH}. Must be host, riscv32, riscv64, or cust_accel.")
endif()

# Dynamic allocation of source and header paths based on architecture
if(TARGET_ARCH STREQUAL "host")
    target_sources(hal PRIVATE
        host/hal_host_math.c
    )

    # hal을 링크하는 상위 타겟이 자동으로 host/ 디렉토리를 참조하도록 PUBLIC 설정
    target_include_directories(hal PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/host>
    )

elseif(TARGET_ARCH STREQUAL "riscv32")
    target_sources(hal PRIVATE
        riscv32/hal_riscv32_math.S
    )
    target_include_directories(hal PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/riscv32>
    )

elseif(TARGET_ARCH STREQUAL "riscv64")
    target_sources(hal PRIVATE
        riscv64/hal_riscv64_math.S
    )
    target_include_directories(hal PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/riscv64>
    )

elseif(TARGET_ARCH STREQUAL "cust_accel")
    target_sources(hal PRIVATE
        cust_accel/hal_ca_math.S
    )
    target_include_directories(hal PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cust_accel>
    )

else()
    message(FATAL_ERROR "Unsupported TARGET_ARCH: ${TARGET_ARCH}. Must be host, riscv32, riscv64, or cust_accel.")
endif()


# ==========================================
# Separating Test Code & HAL Standalone Test
# ==========================================
# _test.c 파일들은 라이브러리에 포함시키지 않고, 별도의 실행 파일로 묶어줍니다.
# 최상위 CMake에서 설정한 BUILD_HAL_TEST 옵션에 따라 동작합니다.
if(BUILD_HAL_TEST)
    # 공통 진입점인 hal_test.c를 기반으로 실행 파일 생성 (이 파일 내부에 main()이 존재해야 함)
    add_executable(hal_test hal_test.c)

    # 아키텍처별로 특화된 테스트 코드가 있다면 추가로 엮어줍니다.
    if(TARGET_ARCH STREQUAL "host")
        #target_sources(hal_test PRIVATE host/hal_host_test.c)
    elseif(TARGET_ARCH STREQUAL "riscv32")
        #target_sources(hal_test PRIVATE riscv32/hal_riscv32_rvv_test.c)
    elseif(TARGET_ARCH STREQUAL "riscv64")
        #target_sources(hal_test PRIVATE riscv64/hal_riscv64_rvv_test.c)
    elseif(TARGET_ARCH STREQUAL "cust_accel")
        # cust_accel용 테스트 파일이 있다면 여기에 추가
        # target_sources(hal_test PRIVATE cust_accel/hal_ca_test.c)
    endif()

    # hal_test가 hal을 링크함으로써 헤더 경로 및 의존성을 자동으로 상속받습니다.
    target_link_libraries(hal_test PRIVATE hal m)

# ---------------------------------------------------------
    # make run_hal_test 타겟 (QEMU 절대경로 탐색 기능 추가)
    # ---------------------------------------------------------
    # Makefile에 있던 기존 toolchain 경로를 HINTS로 제공합니다.
    # set(QEMU_SEARCH_PATH "$ENV{HOME}/toolchains/riscv64/bin")

    if(TARGET_ARCH STREQUAL "host")
        # 호스트 PC: 빌드된 바이너리 직접 실행
        set(RUN_CMD $<TARGET_FILE:hal_test>)

    elseif(TARGET_ARCH STREQUAL "riscv32")
        #set(QEMU_SEARCH_PATH "$ENV{HOME}/toolchains/riscv32/bin")
        set(QEMU_SEARCH_PATH "/disk/toolchains/riscv32/bin")
        # RV32: 환경 변수 및 명시적 경로에서 QEMU 탐색
        find_program(QEMU_RV32 qemu-riscv32 HINTS ${QEMU_SEARCH_PATH})
        if(NOT QEMU_RV32)
            message(FATAL_ERROR "qemu-riscv32를 찾을 수 없습니다. Toolchain 경로를 확인해주세요.")
        endif()
        set(QEMU_FLAGS "-cpu" "rv32,v=true,vlen=128")
        set(RUN_CMD ${QEMU_RV32} ${QEMU_FLAGS} $<TARGET_FILE:hal_test>)

    elseif(TARGET_ARCH STREQUAL "riscv64")
        #set(QEMU_SEARCH_PATH "$ENV{HOME}/toolchains/riscv64/bin")
        set(QEMU_SEARCH_PATH "/disk/toolchains/riscv64/bin")
        # RV64: 환경 변수 및 명시적 경로에서 QEMU 탐색
        find_program(QEMU_RV64 qemu-riscv64 HINTS ${QEMU_SEARCH_PATH})
        if(NOT QEMU_RV64)
            message(FATAL_ERROR "qemu-riscv64를 찾을 수 없습니다. Toolchain 경로를 확인해주세요.")
        endif()
        set(QEMU_FLAGS "-cpu" "rv64,v=true,zba=true,vlen=128")
        set(RUN_CMD ${QEMU_RV64} ${QEMU_FLAGS} $<TARGET_FILE:hal_test>)
    elseif(TARGET_ARCH STREQUAL "cust_accel")
        # Custom Accelerator: 시뮬레이터 실행 명령어 (예시)
        set(RUN_CMD echo "Custom Accelerator 시뮬레이터 실행 명령어를 여기에 정의하세요.")
    else()
        # 기타 아키텍처 예외 처리
        set(RUN_CMD echo "Run target is not fully defined for ${TARGET_ARCH}")
    endif()

    add_custom_target(run_hal_test
        COMMAND ${RUN_CMD}
        DEPENDS hal_test               # 실행 전 빌드 완료 보장
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running HAL Test on ${TARGET_ARCH}..."
        USES_TERMINAL                  # 터미널 실시간 출력
    )
endif()

###
target_link_libraries(hal PRIVATE project_inc ${CMAKE_PROJECT_NAME}_common_settings)

if(USE_COVERAGE)
    target_link_libraries(hal PRIVATE coverage_config)
endif()

# Attribute header paths to the target
# (distinguish between build-time and post-installation paths)
target_include_directories(hal PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Specify the public headers for this target. (list multiple headers separated by spaces.)
set_target_properties(hal PROPERTIES PUBLIC_HEADER "hal_math.h")

# add the destination for PUBLIC_HEADER to the installation rules.
install(TARGETS hal
        EXPORT HalTargets
        ARCHIVE DESTINATION lib/${TARGET_ARCH}     # .a -> lib/
        PUBLIC_HEADER DESTINATION include          # .h -> include/
)

# export configuration files so other developers can use find_package(Hal)
install(EXPORT HalTargets
        FILE HalTargets.cmake
        NAMESPACE Project::
        DESTINATION lib/${TARGET_ARCH})
