# toolchain-riscv64.cmake

#set(ARCH_FLAGS "-march=rv64gcv")

# -g: include debug symbols
# -Og: optimize for debugging

#set(CMAKE_CXX_FLAGS "${ARCH_FLAGS} -g -Og -Wall -Wextra -fno-exceptions -fno-rtti" CACHE STRING "" FORCE)
#set(CMAKE_C_FLAGS   "${ARCH_FLAGS} -g -Og -Wall -Wextra " CACHE STRING "" FORCE)


# 1. 타겟 시스템 설정
# 시뮬레이터나 베어메탈 환경인 경우 "Generic", 리눅스 환경이면 "Linux"를 사용합니다.
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR riscv64)

# 2. 툴체인 경로 설정 (핵심)
# Ubuntu 환경의 기본 패키지 경로를 기본값으로 하되, 외부에서 인자로 덮어쓸 수 있도록 CACHE 변수로 지정합니다.
#set(RISCV_TOOLCHAIN_DIR "/usr" CACHE PATH "Path to the RISC-V toolchain root directory")
set(RISCV_TOOLCHAIN_DIR "/disk/toolchains/riscv64" CACHE PATH "Path to the RISC-V toolchain root directory")

# 베어메탈 환경(newlib)이라면 "riscv64-unknown-elf-"를 사용하세요.
#set(TOOLCHAIN_PREFIX "riscv64-linux-gnu-")
set(TOOLCHAIN_PREFIX "riscv64-unknown-elf-")

# 3. 컴파일러 및 유틸리티 경로 명시
set(CMAKE_C_COMPILER       "${RISCV_TOOLCHAIN_DIR}/bin/${TOOLCHAIN_PREFIX}gcc")
set(CMAKE_CXX_COMPILER     "${RISCV_TOOLCHAIN_DIR}/bin/${TOOLCHAIN_PREFIX}g++")

# 커스텀 어셈블리 명령어나 링커 스크립트를 다룰 때 필요한 도구들
set(CMAKE_ASM_COMPILER     "${RISCV_TOOLCHAIN_DIR}/bin/${TOOLCHAIN_PREFIX}gcc")
set(CMAKE_OBJCOPY          "${RISCV_TOOLCHAIN_DIR}/bin/${TOOLCHAIN_PREFIX}objcopy")
set(CMAKE_OBJDUMP          "${RISCV_TOOLCHAIN_DIR}/bin/${TOOLCHAIN_PREFIX}objdump")
set(CMAKE_AR               "${RISCV_TOOLCHAIN_DIR}/bin/${TOOLCHAIN_PREFIX}ar")
set(CMAKE_RANLIB           "${RISCV_TOOLCHAIN_DIR}/bin/${TOOLCHAIN_PREFIX}ranlib")

# 4. 아키텍처 및 ABI 플래그 설정
# 행렬 곱셈 등 데이터 병렬 처리를 위해 Vector 확장이 포함된 rv64gcv를 기본으로 적용합니다.
set(RISCV_ARCH_FLAGS "-march=rv64gcv -mabi=lp64d")

# 최적화 플래그를 컴파일러 캐시에 강제로 병합
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${RISCV_ARCH_FLAGS}" CACHE STRING "C flags" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RISCV_ARCH_FLAGS}" CACHE STRING "C++ flags" FORCE)
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${RISCV_ARCH_FLAGS}" CACHE STRING "ASM flags" FORCE)

# 5. 크로스 컴파일 탐색 동작 제어 (Cross-compilation Rules)
# 만약 커스텀 툴체인을 사용한다면 Sysroot를 명시해야 할 수 있습니다.
# set(CMAKE_SYSROOT "${RISCV_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}/sysroot")

# 호스트(Ubuntu)의 라이브러리와 타겟(RISC-V)의 라이브러리가 섞이지 않도록 격리
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER) # 실행 파일은 호스트에서 찾음 (예: cmake, make)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)  # 라이브러리는 타겟 환경(툴체인)에서만 찾음
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)  # 헤더 파일은 타겟 환경에서만 찾음
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)  # 패키지도 타겟 환경에서만 찾음
