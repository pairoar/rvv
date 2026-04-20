
# generate executable file
add_executable(vmath_app
    main.c
    #test_nn_dense.c
    #test_dsp_fir.c
    #test_dsp_iir.c
    #test_conv2d.c
    test_mlp_xor.c
)

# set to use the C++ linker for the final linking stage
set_target_properties(vmath_app PROPERTIES LINKER_LANGUAGE CXX)

# Link the libaermob.a, libhal.a, util library to the generated executable
target_link_libraries(vmath_app
    PRIVATE
        vmath hal util ${CMAKE_PROJECT_NAME}_common_settings
)

if(USE_GOOGLETEST)
    target_link_libraries(vmath PRIVATE GTest::gtest)
endif()

# coverage test?
if(USE_COVERAGE)
    target_link_libraries(vmath_app PRIVATE coverage_config)
endif()

# copy application to bin directory
install(TARGETS vmath_app DESTINATION bin)

# add 'make run'
if(TARGET_ARCH STREQUAL "host")
    add_custom_target(run
        COMMAND ./vmath_app
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        DEPENDS vmath_app
        COMMENT "Running vmath_app..."
    )
elseif(TARGET_ARCH STREQUAL "cust_accel")
    add_custom_target(run
        COMMAND echo "Custom Accelerator 시뮬레이터 실행 명령어를 여기에 정의하세요."
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        DEPENDS vmath_app
        COMMENT "Running vmath_app on Custom Accelerator..."
    )
elseif(TARGET_ARCH STREQUAL "riscv32" OR TARGET_ARCH STREQUAL "riscv64")
    # 1. QEMU 실행 파일 탐색
    #set(QEMU_SEARCH_PATH "/disk/toolchains/riscv64/bin")

    if(TARGET_ARCH STREQUAL "riscv32")
        set(QEMU_SEARCH_PATH "/disk/toolchains/riscv32/bin")
        find_program(QEMU_EXEC qemu-riscv32 HINTS ${QEMU_SEARCH_PATH})
        set(QEMU_FLAGS "-cpu" "rv32,v=true,vlen=128")
    else()
        set(QEMU_SEARCH_PATH "/disk/toolchains/riscv64/bin")
        find_program(QEMU_EXEC qemu-riscv64 HINTS ${QEMU_SEARCH_PATH})
        set(QEMU_FLAGS "-cpu" "rv64,v=true,zba=true,vlen=128")
    endif()

    if(NOT QEMU_EXEC)
        message(FATAL_ERROR "${TARGET_ARCH}용 QEMU를 찾을 수 없습니다.")
    endif()

    # 2. vmath_app을 위한 run 타겟 추가
    add_custom_target(run
        COMMAND ${QEMU_EXEC} ${QEMU_FLAGS} $<TARGET_FILE:vmath_app>
        DEPENDS vmath_app               # 실행 전 빌드 완료 보장
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running vmath_app on ${TARGET_ARCH} using QEMU..."
        USES_TERMINAL                   # 터미널 실시간 출력
    )
else()
    add_custom_target(run
        COMMAND echo "Run target is not fully defined for ${TARGET_ARCH}"
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        DEPENDS vmath_app
        COMMENT "Running vmath_app on ${TARGET_ARCH}..."
    )
endif()
