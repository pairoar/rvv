cmake_minimum_required(VERSION 3.20.0)

# [수정] Zephyr 패키지 로드 전에 QEMU 벡터 하드웨어 플래그를 주입합니다!
list(APPEND QEMU_EXTRA_FLAGS "-cpu" "rv32,v=true,vlen=128,vext_spec=v1.0")
#list(APPEND QEMU_EXTRA_FLAGS "-cpu" "rv64,v=true,vlen=128,vext_spec=v1.0")

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(vmath_zephyr)

# 기존 소스코드 경로를 변수로 지정
set(HAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../hal)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib)   # <--- [추가] lib 폴더 경로
set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../app)

# 헤더 경로 추가
zephyr_include_directories(${HAL_DIR})
zephyr_include_directories(${LIB_DIR})            # <--- [추가] lib 헤더 경로 포함
zephyr_include_directories(${APP_DIR})
zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# 소스 코드 컴파일 목록 추가
zephyr_library_sources(
    src/main.c
    lib/vmath_driver.c

    # HAL 계층
    ${HAL_DIR}/riscv32/hal_riscv32_math.S
    #${HAL_DIR}/riscv64/hal_riscv64_math.S
    ${HAL_DIR}/hal_soft_math.c

    # Middleware (lib) 계층 소스 추가 <--- [추가]
    ${LIB_DIR}/nn_layers.c
    ${LIB_DIR}/dsp_fir.c
    ${LIB_DIR}/dsp_iir.c
    ${LIB_DIR}/image_filter.c

    # Application 계층
    ${APP_DIR}/test_nn_dense.c
    ${APP_DIR}/test_mlp_xor.c
    ${APP_DIR}/test_fir.c
    ${APP_DIR}/test_iir.c
    ${APP_DIR}/test_conv2d.c
)

# RISC-V Vector 컴파일러 플래그 강제 주입
#zephyr_cc_option(-march=rv32gcv)
#zephyr_cc_option(-mabi=ilp32d)

# Zephyr 앱 타겟에만(PRIVATE) 벡터 컴파일 옵션을 격리해서 적용합니다.
target_compile_options(app PRIVATE -march=rv32gcv -mabi=ilp32d)
#target_compile_options(app PRIVATE -march=rv64gcv -mabi=lp64d)

