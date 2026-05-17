.text
.align 2
.option arch, +v    # Regardless of CMake flags, this file unconditionally enables Vector extensions

# =============================================================================
# RVV HAL LAYER (Macro-Driven Implementation - RV32 Final)
# =============================================================================

# -----------------------------------------------------------------------------
# 1. ADD / SUB Macro (Input and output sizes are identical.)
# -----------------------------------------------------------------------------
.macro DEF_OP_SAME NAME_I, NAME_U, INSN, INSN_U, SEW, SHIFT
.global \NAME_I
.global \NAME_U

\NAME_I:
    beqz a3, 2f
1:  vsetvli t0, a3, \SEW, m1, ta, ma
    vl\SEW\().v v0, (a1)
    vl\SEW\().v v1, (a2)
    \INSN v2, v0, v1
    vs\SEW\().v v2, (a0)
    slli t1, t0, \SHIFT
    add a0, a0, t1
    add a1, a1, t1
    add a2, a2, t1
    sub a3, a3, t0
    bnez a3, 1b
2:  
    li a0, 0          # HAL_OK (0) 반환
    ret

\NAME_U:
    beqz a3, 4f
3:  vsetvli t0, a3, \SEW, m1, ta, ma
    vl\SEW\().v v0, (a1)
    vl\SEW\().v v1, (a2)
    \INSN_U v2, v0, v1
    vs\SEW\().v v2, (a0)
    slli t1, t0, \SHIFT
    add a0, a0, t1
    add a1, a1, t1
    add a2, a2, t1
    sub a3, a3, t0
    bnez a3, 3b
4:  
    li a0, 0          # HAL_OK (0) 반환
    ret
.endm

# -----------------------------------------------------------------------------
# 2. DIV (Includes Divide-by-Zero Exception Handling) Macro
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
# hal_status_t hal_vdiv_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n);
# hal_status_t hal_vdiv_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n);
# -----------------------------------------------------------------------------
.macro DEF_DIV_INT NAME_I, NAME_U, INSN, INSN_U, SEW, SHIFT
.global \NAME_I
.global \NAME_U

\NAME_I:
    li t6, 0                 # t6 (err_code) = HAL_OK (0) 초기화
    beqz a3, 9f              # n == 0 이면 즉시 종료
    
3:  vsetvli t0, a3, \SEW, m1, ta, ma
    vl\SEW\().v v1, (a1)
    vl\SEW\().v v2, (a2)
    \INSN v3, v1, v2         # v3 = a / b

    vmseq.vi v0, v2, 0       # v0 마스크: b == 0 인 위치
    vmv.v.i v4, 0            # v4 = 0
    vmerge.vvm v3, v3, v4, v0 # 0으로 나눈 위치의 결과를 0으로 덮어씀

    vs\SEW\().v v3, (a0)

    vcpop.m t2, v0           # 0으로 나눈 횟수(마스크의 1 개수) 카운트
    beqz t2, 4f              # 0으로 나눈 적이 없으면 에러 세팅 건너뜀
    li t6, -2                # t6 = HAL_ERR_DIV_BY_ZERO (-2)
    
4:  slli t1, t0, \SHIFT
    add a0, a0, t1
    add a1, a1, t1
    add a2, a2, t1
    sub a3, a3, t0
    bnez a3, 3b
    
9:  
    mv a0, t6                # 누적된 에러 코드를 a0로 이동하여 반환
    ret

\NAME_U:
    li t6, 0                 # t6 (err_code) = HAL_OK (0) 초기화
    beqz a3, 90f             # n == 0 이면 즉시 종료
    
30: vsetvli t0, a3, \SEW, m1, ta, ma
    vl\SEW\().v v1, (a1)
    vl\SEW\().v v2, (a2)
    \INSN_U v3, v1, v2

    vmseq.vi v0, v2, 0
    vmv.v.i v4, 0
    vmerge.vvm v3, v3, v4, v0
    vs\SEW\().v v3, (a0)

    vcpop.m t2, v0
    beqz t2, 40f
    li t6, -2                # t6 = HAL_ERR_DIV_BY_ZERO (-2)
    
40: slli t1, t0, \SHIFT
    add a0, a0, t1
    add a1, a1, t1
    add a2, a2, t1
    sub a3, a3, t0
    bnez a3, 30b
    
90: 
    mv a0, t6                # 누적된 에러 코드를 a0로 이동하여 반환
    ret
.endm

# -----------------------------------------------------------------------------
# 3. MUL & MAC (Widening) Macro
# -----------------------------------------------------------------------------
.macro DEF_WMUL NAME_I, NAME_U, INSN, INSN_U, SEW, WSEW, SHIFT, WSHIFT
.global \NAME_I
.global \NAME_U
\NAME_I:
    beqz a3, 2f
1:  vsetvli t0, a3, \SEW, m1, ta, ma
    vl\SEW\().v v0, (a1)
    vl\SEW\().v v1, (a2)
    \INSN v2, v0, v1
    vs\WSEW\().v v2, (a0)
    slli t1, t0, \SHIFT
    add a1, a1, t1
    add a2, a2, t1
    slli t2, t0, \WSHIFT
    add a0, a0, t2
    sub a3, a3, t0
    bnez a3, 1b
2:  
    li a0, 0          # HAL_OK (0) 반환
    ret

\NAME_U:
    beqz a3, 4f
3:  vsetvli t0, a3, \SEW, m1, ta, ma
    vl\SEW\().v v0, (a1)
    vl\SEW\().v v1, (a2)
    \INSN_U v2, v0, v1
    vs\WSEW\().v v2, (a0)
    slli t1, t0, \SHIFT
    add a1, a1, t1
    add a2, a2, t1
    slli t2, t0, \WSHIFT
    add a0, a0, t2
    sub a3, a3, t0
    bnez a3, 3b
4:  
    li a0, 0          # HAL_OK (0) 반환
    ret
.endm

.macro DEF_WMAC NAME_I, NAME_U, INSN, INSN_U, SEW, WSEW, SHIFT, WSHIFT
.global \NAME_I
.global \NAME_U
\NAME_I:
    beqz a3, 2f
1:  vsetvli t0, a3, \SEW, m1, ta, ma
    vl\SEW\().v v0, (a1)
    vl\SEW\().v v1, (a2)
    vl\WSEW\().v v2, (a0)
    \INSN v2, v0, v1
    vs\WSEW\().v v2, (a0)
    slli t1, t0, \SHIFT
    add a1, a1, t1
    add a2, a2, t1
    slli t2, t0, \WSHIFT
    add a0, a0, t2
    sub a3, a3, t0
    bnez a3, 1b
2:  
    li a0, 0          # HAL_OK (0) 반환
    ret

\NAME_U:
    beqz a3, 4f
3:  vsetvli t0, a3, \SEW, m1, ta, ma
    vl\SEW\().v v0, (a1)
    vl\SEW\().v v1, (a2)
    vl\WSEW\().v v2, (a0)
    \INSN_U v2, v0, v1
    vs\WSEW\().v v2, (a0)
    slli t1, t0, \SHIFT
    add a1, a1, t1
    add a2, a2, t1
    slli t2, t0, \WSHIFT
    add a0, a0, t2
    sub a3, a3, t0
    bnez a3, 3b
4:  
    li a0, 0          # HAL_OK (0) 반환
    ret
.endm

# -----------------------------------------------------------------------------
# 4. DOT Product (Widening) Macro
# -----------------------------------------------------------------------------
.macro DEF_WDOT NAME_I, NAME_U, MUL_INSN, MUL_INSN_U, SEW, WSEW, SHIFT
.global \NAME_I
.global \NAME_U
\NAME_I:
    vsetvli t0, zero, \WSEW, m2, ta, ma
    vmv.v.i v8, 0
    beqz a3, 2f
1:  vsetvli t0, a3, \SEW, m1, ta, ma
    vl\SEW\().v v0, (a1)
    vl\SEW\().v v1, (a2)
    \MUL_INSN v2, v0, v1

    # [Core] Forcefully switch data type to WSEW (widened size) for reduction operation
    vsetvli zero, t0, \WSEW, m2, ta, ma
    vredsum.vs v8, v2, v8

    slli t1, t0, \SHIFT
    add a1, a1, t1
    add a2, a2, t1
    sub a3, a3, t0
    bnez a3, 1b
2:  li t0, 1
    vsetvli zero, t0, \WSEW, m1, ta, ma
    vs\WSEW\().v v8, (a0)
    li a0, 0          # HAL_OK (0) 반환
    ret

\NAME_U:
    vsetvli t0, zero, \WSEW, m2, ta, ma
    vmv.v.i v8, 0
    beqz a3, 4f
3:  vsetvli t0, a3, \SEW, m1, ta, ma
    vl\SEW\().v v0, (a1)
    vl\SEW\().v v1, (a2)
    \MUL_INSN_U v2, v0, v1

    vsetvli zero, t0, \WSEW, m2, ta, ma
    vredsum.vs v8, v2, v8

    slli t1, t0, \SHIFT
    add a1, a1, t1
    add a2, a2, t1
    sub a3, a3, t0
    bnez a3, 3b
4:  li t0, 1
    vsetvli zero, t0, \WSEW, m1, ta, ma
    vs\WSEW\().v v8, (a0)
    li a0, 0          # HAL_OK (0) 반환
    ret
.endm

# -----------------------------------------------------------------------------
# 5. Matrix Multiply (Widening) Macro
# -----------------------------------------------------------------------------
.macro DEF_WMATRIX NAME_I, NAME_U, L_SCALAR_I, L_SCALAR_U, MAC_INSN_I, MAC_INSN_U, SEW, WSEW, SHIFT, WSHIFT
.global \NAME_I
.global \NAME_U
\NAME_I:
    mv t0, zero
1:  bge t0, a3, 9f
    mv t1, zero
2:  bge t1, a5, 8f
    mul t2, t0, a5
    add t2, t2, t1
    slli t2, t2, \SHIFT
    add t2, a1, t2
    \L_SCALAR_I
    mul t3, t1, a4
    slli t3, t3, \SHIFT
    add t3, a2, t3
    mul t4, t0, a4
    slli t4, t4, \WSHIFT
    add t4, a0, t4
    mv t5, a4
3:  vsetvli a6, t5, \SEW, m1, ta, ma
    vl\SEW\().v v0, (t3)
    vl\WSEW\().v v2, (t4)
    \MAC_INSN_I
    vs\WSEW\().v v2, (t4)
    slli t2, a6, \SHIFT
    add t3, t3, t2
    slli t2, a6, \WSHIFT
    add t4, t4, t2
    sub t5, t5, a6
    bnez t5, 3b
    addi t1, t1, 1
    j 2b
8:  addi t0, t0, 1
    j 1b
9:
    li a0, 0
    ret

\NAME_U:
    mv t0, zero
10: bge t0, a3, 19f
    mv t1, zero
12: bge t1, a5, 18f
    mul t2, t0, a5
    add t2, t2, t1
    slli t2, t2, \SHIFT
    add t2, a1, t2
    \L_SCALAR_U
    mul t3, t1, a4
    slli t3, t3, \SHIFT
    add t3, a2, t3
    mul t4, t0, a4
    slli t4, t4, \WSHIFT
    add t4, a0, t4
    mv t5, a4
13: vsetvli a6, t5, \SEW, m1, ta, ma
    vl\SEW\().v v0, (t3)
    vl\WSEW\().v v2, (t4)
    \MAC_INSN_U
    vs\WSEW\().v v2, (t4)
    slli t2, a6, \SHIFT
    add t3, t3, t2
    slli t2, a6, \WSHIFT
    add t4, t4, t2
    sub t5, t5, a6
    bnez t5, 13b
    addi t1, t1, 1
    j 12b
18: addi t0, t0, 1
    j 10b
19:
    li a0, 0
    ret
.endm

# =============================================================================
# Macro Invocations
# =============================================================================

# --- 1. ADD / SUB / DIV ---
DEF_OP_SAME hal_vadd_i8, hal_vadd_u8, vadd.vv, vadd.vv, e8, 0
DEF_OP_SAME hal_vadd_i16, hal_vadd_u16, vadd.vv, vadd.vv, e16, 1
DEF_OP_SAME hal_vadd_i32, hal_vadd_u32, vadd.vv, vadd.vv, e32, 2
DEF_OP_SAME hal_vadd_i64, hal_vadd_u64, vadd.vv, vadd.vv, e64, 3

DEF_OP_SAME hal_vsub_i8, hal_vsub_u8, vsub.vv, vsub.vv, e8, 0
DEF_OP_SAME hal_vsub_i16, hal_vsub_u16, vsub.vv, vsub.vv, e16, 1
DEF_OP_SAME hal_vsub_i32, hal_vsub_u32, vsub.vv, vsub.vv, e32, 2
DEF_OP_SAME hal_vsub_i64, hal_vsub_u64, vsub.vv, vsub.vv, e64, 3

DEF_DIV_INT hal_vdiv_i8, hal_vdiv_u8, vdiv.vv, vdivu.vv, e8, 0
DEF_DIV_INT hal_vdiv_i16, hal_vdiv_u16, vdiv.vv, vdivu.vv, e16, 1
DEF_DIV_INT hal_vdiv_i32, hal_vdiv_u32, vdiv.vv, vdivu.vv, e32, 2
DEF_DIV_INT hal_vdiv_i64, hal_vdiv_u64, vdiv.vv, vdivu.vv, e64, 3

DEF_OP_SAME hal_vadd_f32, hal_dummy_add_f32, vfadd.vv, vfadd.vv, e32, 2
DEF_OP_SAME hal_vsub_f32, hal_dummy_sub_f32, vfsub.vv, vfsub.vv, e32, 2

# -----------------------------------------------------------------------------
#  Float 32 Manual Implementation (Restored vle / vse instruction spelling)
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Float 32 Division with Exception Handling (Divide-by-zero to 0.0f)
# hal_status_t hal_vdiv_f32(float *dest(a0), const float *srcA(a1), const float *srcB(a2), size_t n(a3))
# -----------------------------------------------------------------------------
.global hal_vdiv_f32
.type hal_vdiv_f32, @function

hal_vdiv_f32:

    # 처리할 요소(n)가 0이면 루프 없이 종료
    beqz a3, 9f             

    # 2. 루프 외부 초기화 (단 한 번만 실행)
    vsetvli t0, zero, e32, m1, ta, ma
    vmv.v.i v4, 0           # v4 = 0.0f (Integer 0 == Float 0.0f 비트 패턴)
    li t4, 0                # t4 = 예외 발생 여부 누적용 레지스터 (루프 내 메모리 I/O 방지)

2:  # === VECTOR LOOP ===
    vsetvli t0, a3, e32, m1, ta, ma
    
    # 데이터 로드
    vle32.v v1, (a1)        # v1 = srcA
    vle32.v v2, (a2)        # v2 = srcB
    
    # 핵심 연산: 나눗셈 및 분모 0.0f 검사
    vfdiv.vv v3, v1, v2
    vmfeq.vv v0, v2, v4     # Mask (v0): srcB가 0.0f 인 위치
    
    # 예외 처리: 0으로 나눈 결과(Inf/NaN)를 v4(0.0f)로 덮어쓰기
    vmerge.vvm v3, v3, v4, v0
    
    # 메모리에 결과 저장
    vse32.v v3, (a0)        # dest = v3
    
    # 예외 플래그 로컬 누적 (메모리 접근 없이 t4 레지스터에만 OR 연산)
    vcpop.m t2, v0          # 마스크 v0에서 1로 세팅된 비트의 개수를 t2에 카운트
    or t4, t4, t2           # 카운트된 값을 t4에 누적
    
    # 포인터 및 카운터 업데이트
    slli t1, t0, 2          # float 4바이트만큼 오프셋 계산 (t1 = t0 * 4)
    add a0, a0, t1
    add a1, a1, t1
    add a2, a2, t1
    sub a3, a3, t0
    
    # 남은 데이터가 있으면 반복
    bnez a3, 2b             

    # === LOOP END ===
    
    # 3. 누적된 예외 결과를 메모리에 단 한 번만 업데이트
    beqz a4, 9f             # a4 포인터가 NULL이면 업데이트 스킵
    beqz t4, 9f             # 루프 동안 예외가 한 번도 없었으면 스킵
    
    lw t3, 0(a4)            # 기존 상태 로드 (위에서 0으로 초기화했으므로 사실 li t3, 1로 덮어써도 무방함)
    ori t3, t3, 1           # 예외 비트 (1) Set
    sw t3, 0(a4)            # 메모리에 최종 저장

9:  
    li a0, 0                # HAL_OK (0) 반환 
    ret                     # (이때 루프에서 갱신된 a0는 반환값으로 덮어써지며 C ABI를 완벽히 준수함)


# --- 2. Widening MUL ---
DEF_WMUL hal_vmul_i8, hal_vmul_u8, vwmul.vv, vwmulu.vv, e8, e16, 0, 1
DEF_WMUL hal_vmul_i16, hal_vmul_u16, vwmul.vv, vwmulu.vv, e16, e32, 1, 2
DEF_WMUL hal_vmul_i32, hal_vmul_u32, vwmul.vv, vwmulu.vv, e32, e64, 2, 3
DEF_WMUL hal_vmul_f32, hal_dummy_mul_f32, vfwmul.vv, vfwmul.vv, e32, e64, 2, 3
# check : vwmul.vv

# --- 3. Widening MAC ---
DEF_WMAC hal_vmac_i8, hal_vmac_u8, vwmacc.vv, vwmaccu.vv, e8, e16, 0, 1
DEF_WMAC hal_vmac_i16, hal_vmac_u16, vwmacc.vv, vwmaccu.vv, e16, e32, 1, 2
DEF_WMAC hal_vmac_i32, hal_vmac_u32, vwmacc.vv, vwmaccu.vv, e32, e64, 2, 3
DEF_WMAC hal_vmac_f32, hal_dummy_mac_f32, vfwmacc.vv, vfwmacc.vv, e32, e64, 2, 3

# --- 4. Widening DOT ---
DEF_WDOT hal_vdot_i8, hal_vdot_u8, vwmul.vv, vwmulu.vv, e8, e16, 0
DEF_WDOT hal_vdot_i16, hal_vdot_u16, vwmul.vv, vwmulu.vv, e16, e32, 1
DEF_WDOT hal_vdot_i32, hal_vdot_u32, vwmul.vv, vwmulu.vv, e32, e64, 2

.global hal_vdot_f32
hal_vdot_f32:
    vsetvli t0, zero, e64, m2, ta, ma
    vmv.v.i v8, 0
    beqz a3, 2f
1:  vsetvli t0, a3, e32, m1, ta, ma
    vle32.v v0, (a1)     #  vle32.v
    vle32.v v1, (a2)     #  vle32.v
    vfwmul.vv v2, v0, v1
    vsetvli zero, t0, e64, m2, ta, ma
    vfredusum.vs v8, v2, v8
    slli t1, t0, 2
    add a1, a1, t1
    add a2, a2, t1
    sub a3, a3, t0
    bnez a3, 1b
2:  li t0, 1
    vsetvli zero, t0, e64, m1, ta, ma
    vse64.v v8, (a0)     #  vse64.v
    li a0, 0          # HAL_OK (0) 반환
    ret

# --- 5. Widening MATRIX MUL ---
DEF_WMATRIX hal_matrix_vmul_i8, hal_matrix_vmul_u8, "lb a7, 0(t2)", "lbu a7, 0(t2)", "vwmacc.vx v2, a7, v0", "vwmaccu.vx v2, a7, v0", e8, e16, 0, 1
DEF_WMATRIX hal_matrix_vmul_i16, hal_matrix_vmul_u16, "lh a7, 0(t2)", "lhu a7, 0(t2)", "vwmacc.vx v2, a7, v0", "vwmaccu.vx v2, a7, v0", e16, e32, 1, 2
DEF_WMATRIX hal_matrix_vmul_i32, hal_matrix_vmul_u32, "lw a7, 0(t2)", "lw a7, 0(t2)", "vwmacc.vx v2, a7, v0", "vwmaccu.vx v2, a7, v0", e32, e64, 2, 3

.global hal_matrix_vmul_f32
hal_matrix_vmul_f32:
    mv t0, zero
1:  bge t0, a3, 9f
    mv t1, zero
2:  bge t1, a5, 8f
    mul t2, t0, a5
    add t2, t2, t1
    slli t2, t2, 2
    add t2, a1, t2
    flw fa0, 0(t2)
    mul t3, t1, a4
    slli t3, t3, 2
    add t3, a2, t3
    mul t4, t0, a4
    slli t4, t4, 3
    add t4, a0, t4
    mv t5, a4
3:  vsetvli a6, t5, e32, m1, ta, ma
    vle32.v v0, (t3)     #  vle32.v
    vle64.v v2, (t4)     #  vle64.v
    vfwmacc.vf v2, fa0, v0
    vse64.v v2, (t4)     #  vse64.v
    slli t2, a6, 2
    add t3, t3, t2
    slli t2, a6, 3
    add t4, t4, t2
    sub t5, t5, a6
    bnez t5, 3b
    addi t1, t1, 1
    j 2b
8:  addi t0, t0, 1
    j 1b
9:
    li a0, 0          # HAL_OK (0) 반환
    ret



.global hal_matrix_vmul_tiled_f32
# ==============================================================================
# void hal_matrix_vmul_tiled_f32(double* c, const float* a, const float* b,
#                               const int M, const int N, const int K, const int tile_size);
# ==============================================================================
hal_matrix_vmul_tiled_f32:
    beqz a3, .L_end_f32
    beqz a4, .L_end_f32
    beqz a5, .L_end_f32

    slli a7, a4, 2           # a7 = B matrix Stride (N * 4 bytes)
    li t0, 0                 # i = 0
.L_loop_M_f32:
    bge t0, a3, .L_end_f32
    li t1, 0                 # j = 0
.L_loop_N_f32:
    bge t1, a4, .L_next_M_f32

    sub t2, a4, t1
    bltu t2, a6, .L_set_vl_f32
    mv t2, a6
.L_set_vl_f32:
    vsetvli t3, t2, e32, m1, ta, ma

    # Calculate and load C buffer (accumulator) pointer
    mul t4, t0, a4
    add t4, t4, t1
    slli t4, t4, 3
    add t4, a0, t4

    vsetvli zero, t3, e64, m2, ta, ma
    vle64.v v16, (t4)

    vsetvli zero, t3, e32, m1, ta, ma

    # Initialize A, B pointers
    mul t6, t0, a5
    slli t6, t6, 2
    add t6, a1, t6
    slli t5, t1, 2
    add t5, a2, t5

    li t2, 0                 # k = 0
.L_loop_K_f32:
    bge t2, a5, .L_next_N_f32

    flw fa0, 0(t6)           # Scalar A[i][k]
    vle32.v v8, (t5)         # Vector B[k][j : j+vl]

    # Widening MAC
    vfwmacc.vf v16, fa0, v8

    # Pointer Bumping
    addi t6, t6, 4           # A_ptr++
    add t5, t5, a7           # B_ptr += B_stride

    addi t2, t2, 1
    j .L_loop_K_f32

.L_next_N_f32:
    vsetvli zero, t3, e64, m2, ta, ma
    vse64.v v16, (t4)
    add t1, t1, t3
    j .L_loop_N_f32

.L_next_M_f32:
    addi t0, t0, 1
    j .L_loop_M_f32

.L_end_f32:
    li a0, 0          # HAL_OK (0) 반환
    ret

# -----------------------------------------------------------------------------
#  128-bit ADD / SUB (Utilizing Carry & Borrow Masks)
# -----------------------------------------------------------------------------
.macro DEF_ADD_128 NAME
.global \NAME
\NAME:
    li t1, 8                 # Lower 64-bit (l) offset within the struct
    li t6, 16                # Total struct size (Stride = 16 bytes)
    beqz a3, 2f
1:  vsetvli t0, a3, e64, m1, ta, ma
    add t4, a1, t1           # Address of a.l
    add t5, a2, t1           # Address of b.l
    vlse64.v v2, (t4), t6    # v2 = a.l (strided load)
    vlse64.v v4, (t5), t6    # v4 = b.l

    vadd.vv v6, v2, v4       # v6 = a.l + b.l
    vmsltu.vv v0, v6, v2     # Check for Carry generation -> store in v0 mask

    vlse64.v v8, (a1), t6    # v8 = a.u
    vlse64.v v10, (a2), t6   # v10 = b.u
    vadc.vvm v12, v8, v10, v0 # v12 = a.u + b.u + carry

    add t4, a0, t1
    vsse64.v v6, (t4), t6    # Store c.l
    vsse64.v v12, (a0), t6   # Store c.u

    slli t2, t0, 4           # Pointer advance (elements processed * 16 bytes)
    add a0, a0, t2
    add a1, a1, t2
    add a2, a2, t2
    sub a3, a3, t0
    bnez a3, 1b
2:
    li a0, 0          # HAL_OK (0) 반환
    ret
.endm

.macro DEF_SUB_128 NAME
.global \NAME
\NAME:
    li t1, 8
    li t6, 16
    beqz a3, 2f
1:  vsetvli t0, a3, e64, m1, ta, ma
    add t4, a1, t1
    add t5, a2, t1
    vlse64.v v2, (t4), t6    # a.l
    vlse64.v v4, (t5), t6    # b.l

    vsub.vv v6, v2, v4       # v6 = a.l - b.l
    vmsltu.vv v0, v2, v4     # Check for Borrow generation (a.l < b.l) -> v0 mask

    vlse64.v v8, (a1), t6    # a.u
    vlse64.v v10, (a2), t6   # b.u
    vsbc.vvm v12, v8, v10, v0 # v12 = a.u - b.u - borrow

    add t4, a0, t1
    vsse64.v v6, (t4), t6
    vsse64.v v12, (a0), t6

    slli t2, t0, 4
    add a0, a0, t2
    add a1, a1, t2
    add a2, a2, t2
    sub a3, a3, t0
    bnez a3, 1b
2:
    li a0, 0          # HAL_OK (0) 반환
    ret
.endm

DEF_ADD_128 hal_vadd_i128
DEF_ADD_128 hal_vadd_u128
DEF_SUB_128 hal_vsub_i128
DEF_SUB_128 hal_vsub_u128


# -----------------------------------------------------------------------------
#  64-bit MUL -> 128-bit Result (Utilizing vmulh and struct mapping)
# Since int128_t is ordered as {int64_t u; uint64_t l;}, vsseg2e64 is used.
# -----------------------------------------------------------------------------
.macro DEF_MUL64 NAME, MULH_INSN
.global \NAME
\NAME:
    beqz a3, 2f
1:  vsetvli t0, a3, e64, m1, ta, ma
    vle64.v v8, (a1)
    vle64.v v10, (a2)

    \MULH_INSN v14, v8, v10  # Upper 64 bits (u) -> v14
    vmul.vv v15, v8, v10     # Lower 64 bits (l) -> v15

    # vsseg2e64.v: Interleaved store for v14 and v15.
    # v14 perfectly fits into offset 0 (u), and v15 into offset 8 (l)!
    vsseg2e64.v v14, (a0)

    slli t1, t0, 4           # c += 16
    add a0, a0, t1
    slli t1, t0, 3           # a, b += 8
    add a1, a1, t1
    add a2, a2, t1
    sub a3, a3, t0
    bnez a3, 1b
2:
    li a0, 0          # HAL_OK (0) 반환
    ret
.endm

DEF_MUL64 hal_vmul_i64, vmulh.vv
DEF_MUL64 hal_vmul_u64, vmulhu.vv

# -----------------------------------------------------------------------------
#  8/16/32-bit Tiled Matrix Multiply Macro (Utilizing s0 register and stack)
# -----------------------------------------------------------------------------
.macro DEF_WMATRIX_TILED NAME_I, NAME_U, L_SCALAR_I, L_SCALAR_U, MAC_INSN_I, MAC_INSN_U, SEW, WSEW, SHIFT, WSHIFT, BYTES
.global \NAME_I
.global \NAME_U

\NAME_I:
    # Backup s0 to stack due to temporary register exhaustion
    addi sp, sp, -16
    sw s0, 0(sp)

    beqz a3, 9f
    beqz a4, 9f
    beqz a5, 9f
    slli a7, a4, \SHIFT      # a7 = B matrix Stride
    li t0, 0                 # i = 0
1:  bge t0, a3, 9f
    li t1, 0                 # j = 0
2:  bge t1, a4, 8f
    sub t2, a4, t1
    bltu t2, a6, 3f
    mv t2, a6
3:  vsetvli t3, t2, \SEW, m1, ta, ma
    # Calculate c_ptr
    mul t4, t0, a4
    add t4, t4, t1
    slli t4, t4, \WSHIFT
    add t4, a0, t4
    # Load C buffer
    vsetvli zero, t3, \WSEW, m2, ta, ma
    vl\WSEW\().v v16, (t4)
    vsetvli zero, t3, \SEW, m1, ta, ma
    # Calculate A_ptr, B_ptr
    mul t6, t0, a5
    slli t6, t6, \SHIFT
    add t6, a1, t6           # t6 = &A[i][0]
    slli t5, t1, \SHIFT
    add t5, a2, t5           # t5 = &B[0][j]
    li t2, 0                 # k = 0
4:  bge t2, a5, 7f
    \L_SCALAR_I s0, 0(t6)    #  t7 -> s0
    vl\SEW\().v v8, (t5)     # Load vector B
    \MAC_INSN_I v16, s0, v8  #  t7 -> s0
    addi t6, t6, \BYTES      # A_ptr++
    add t5, t5, a7           # B_ptr += B_Stride
    addi t2, t2, 1
    j 4b
7:  vsetvli zero, t3, \WSEW, m2, ta, ma
    vs\WSEW\().v v16, (t4)   # Store to C buffer
    add t1, t1, t3
    j 2b
8:  addi t0, t0, 1
    j 1b
9:
    # Perfectly restore s0 before returning
    lw s0, 0(sp)
    addi sp, sp, 16
    li a0, 0          # HAL_OK (0) 반환
    ret

\NAME_U:
    # Backup s0 to stack due to temporary register exhaustion
    addi sp, sp, -16
    sw s0, 0(sp)

    # Unsigned version logic (Same structure as Signed, only instructions changed)
    beqz a3, 19f
    beqz a4, 19f
    beqz a5, 19f
    slli a7, a4, \SHIFT
    li t0, 0
11: bge t0, a3, 19f
    li t1, 0
12: bge t1, a4, 18f
    sub t2, a4, t1
    bltu t2, a6, 13f
    mv t2, a6
13: vsetvli t3, t2, \SEW, m1, ta, ma
    mul t4, t0, a4
    add t4, t4, t1
    slli t4, t4, \WSHIFT
    add t4, a0, t4
    vsetvli zero, t3, \WSEW, m2, ta, ma
    vl\WSEW\().v v16, (t4)
    vsetvli zero, t3, \SEW, m1, ta, ma
    mul t6, t0, a5
    slli t6, t6, \SHIFT
    add t6, a1, t6
    slli t5, t1, \SHIFT
    add t5, a2, t5
    li t2, 0
14: bge t2, a5, 17f
    \L_SCALAR_U s0, 0(t6)    #  t7 -> s0
    vl\SEW\().v v8, (t5)
    \MAC_INSN_U v16, s0, v8  #  t7 -> s0
    addi t6, t6, \BYTES
    add t5, t5, a7
    addi t2, t2, 1
    j 14b
17: vsetvli zero, t3, \WSEW, m2, ta, ma
    vs\WSEW\().v v16, (t4)
    add t1, t1, t3
    j 12b
18: addi t0, t0, 1
    j 11b
19:
    # Perfectly restore s0 before returning
    lw s0, 0(sp)
    addi sp, sp, 16
    li a0, 0          # HAL_OK (0) 반환
    ret
.endm

# Macro Invocations: Generate 6 tiling functions for 8, 16, 32-bit all at once!
DEF_WMATRIX_TILED hal_matrix_vmul_tiled_i8, hal_matrix_vmul_tiled_u8, lb, lbu, vwmacc.vx, vwmaccu.vx, e8, e16, 0, 1, 1
DEF_WMATRIX_TILED hal_matrix_vmul_tiled_i16, hal_matrix_vmul_tiled_u16, lh, lhu, vwmacc.vx, vwmaccu.vx, e16, e32, 1, 2, 2
DEF_WMATRIX_TILED hal_matrix_vmul_tiled_i32, hal_matrix_vmul_tiled_u32, lw, lw, vwmacc.vx, vwmaccu.vx, e32, e64, 2, 3, 4


# --->
# -----------------------------------------------------------------------------
# MIN / MAX : vminu.vv / 
# -----------------------------------------------------------------------------
# hal_status_t hal_min_i8(int8_t *dest (a0), const int8_t *src1 (a1), const int8_t *src2 (a2), size_t n (a3))
# hal_status_t hal_min_u8(uint8_t *dest (a0), const uint8_t *src1 (a1), const uint8_t *src2 (a2), size_t n (a3))

.macro DEF_MINMAX_INT NAME_I, NAME_U, INSN, INSN_U, SEW, SHIFT
.global \NAME_I
.global \NAME_U

\NAME_I:
    # a3(n)이 0이면 바로 종료 (예외 처리)
    beqz a3, 2f
1:
    # 1. 벡터 설정 (vsetvli)
    # 현재 처리할 요소 수(a3)를 요청하고, 실제 처리 가능한 요소 수를 t0에 반환받음
    # e32: 32비트 데이터 (uint32_t)
    # m1: LMUL=1 (벡터 레지스터 1개 사용)
    # ta, ma: Tail Agnostic, Mask Agnostic (성능 최적화를 위해 남은 부분은 신경 쓰지 않음)
    # vsetvli t0, a3, e32, m1, ta, ma
    vsetvli t0, a3, \SEW, m1, ta, ma

    # 2. 메모리에서 벡터 레지스터로 로드 (Unit-stride Load)
    #vle32.v v8, (a1)        # v8 = src1[i ... i+t0-1]
    #vle32.v v9, (a2)        # v9 = src2[i ... i+t0-1]
    vl\SEW\().v v8, (a1)        # v8 = src1[i ... i+t0-1]
    vl\SEW\().v v9, (a2)        # v9 = src2[i ... i+t0-1]

    # 3. 핵심 연산: Unsigned Minimum
    # v10 = min_unsigned(v8, v9)
    \INSN v10, v8, v9

    # 4. 벡터 레지스터에서 메모리로 저장 (Unit-stride Store)
    # vse32.v v10, (a0)       # dest[i ... i+t0-1] = v10
    vs\SEW\().v v10, (a0)       # dest[i ... i+t0-1] = v10

    # 5. 포인터 및 카운터 업데이트
    # slli t1, t0, 2          # t1 = t0 * 4 (32비트는 4바이트이므로 포인터 이동 거리 계산)
    slli t1, t0, \SHIFT          # t1 = t0 * 4 (32비트는 4바이트이므로 포인터 이동 거리 계산)
    
    add a1, a1, t1          # src1 포인터 증가
    add a2, a2, t1          # src2 포인터 증가
    add a0, a0, t1          # dest 포인터 증가
    
    sub a3, a3, t0          # 남은 요소 수(n) = n - t0

    # 6. 남은 요소가 있으면 루프 반복
    bnez a3, 1b

2:
    li a0, 0                # HAL_OK(0)
    ret                     # 함수 반환

\NAME_U:
    beqz a3, 4f
3:
    # 1. 벡터 설정 (vsetvli)
    # 현재 처리할 요소 수(a3)를 요청하고, 실제 처리 가능한 요소 수를 t0에 반환받음
    # e32: 32비트 데이터 (uint32_t)
    # m1: LMUL=1 (벡터 레지스터 1개 사용)
    # ta, ma: Tail Agnostic, Mask Agnostic (성능 최적화를 위해 남은 부분은 신경 쓰지 않음)
    # vsetvli t0, a3, e32, m1, ta, ma
    vsetvli t0, a3, \SEW, m1, ta, ma

    # 2. 메모리에서 벡터 레지스터로 로드 (Unit-stride Load)
    #vle32.v v8, (a1)        # v8 = src1[i ... i+t0-1]
    #vle32.v v9, (a2)        # v9 = src2[i ... i+t0-1]
    vl\SEW\().v v8, (a1)        # v8 = src1[i ... i+t0-1]
    vl\SEW\().v v9, (a2)        # v9 = src2[i ... i+t0-1]

    # 3. 핵심 연산: Unsigned Minimum
    # v10 = min_unsigned(v8, v9)
    \INSN v10, v8, v9

    # 4. 벡터 레지스터에서 메모리로 저장 (Unit-stride Store)
    # vse32.v v10, (a0)       # dest[i ... i+t0-1] = v10
    vs\SEW\().v v10, (a0)       # dest[i ... i+t0-1] = v10

    # 5. 포인터 및 카운터 업데이트
    # slli t1, t0, 2          # t1 = t0 * 4 (32비트는 4바이트이므로 포인터 이동 거리 계산)
    slli t1, t0, \SHIFT          # t1 = t0 * 4 (32비트는 4바이트이므로 포인터 이동 거리 계산)
    
    add a1, a1, t1          # src1 포인터 증가
    add a2, a2, t1          # src2 포인터 증가
    add a0, a0, t1          # dest 포인터 증가
    
    sub a3, a3, t0          # 남은 요소 수(n) = n - t0

    # 6. 남은 요소가 있으면 루프 반복
    bnez a3, 3b

4:
    li a0, 0                # HAL_OK(0)
    ret                     # 함수 반환

.endm

# =============================================================================
# Macro Invocations
# =============================================================================
# --- Min
DEF_MINMAX_INT hal_vmin_i8, hal_vmin_u8, vmin.vv, vminu.vv, e8, 0
DEF_MINMAX_INT hal_vmin_i16, hal_vmin_u16, vmin.vv, vminu.vv, e16, 1
DEF_MINMAX_INT hal_vmin_i32, hal_vmin_u32, vmin.vv, vminu.vv, e32, 2
DEF_MINMAX_INT hal_vmin_i64, hal_vmin_u64, vmin.vv, vminu.vv, e64, 3

# --- Max
DEF_MINMAX_INT hal_vmax_i8, hal_vmax_u8, vmax.vv, vmaxu.vv, e8, 0
DEF_MINMAX_INT hal_vmax_i16, hal_vmax_u16, vmax.vv, vmaxu.vv, e16, 1
DEF_MINMAX_INT hal_vmax_i32, hal_vmax_u32, vmax.vv, vmaxu.vv, e32, 2
DEF_MINMAX_INT hal_vmax_i64, hal_vmax_u64, vmax.vv, vmaxu.vv, e64, 3


# -----------------------------------------------------------------------------
#  128-bit Min / Max
# -----------------------------------------------------------------------------
.macro DEF_MIN_128 NAME
.global \NAME
\NAME:
    li t1, 8                 # Lower 64-bit (l) offset within the struct
    li t6, 16                # Total struct size (Stride = 16 bytes)
    beqz a3, 2f
1:  vsetvli t0, a3, e64, m1, ta, ma
    add t4, a1, t1           # Address of a.l
    add t5, a2, t1           # Address of b.l
    vlse64.v v2, (t4), t6    # v2 = a.l (strided load)
    vlse64.v v4, (t5), t6    # v4 = b.l

    # 

    vadd.vv v6, v2, v4       # v6 = a.l + b.l
    vmsltu.vv v0, v6, v2     # Check for Carry generation -> store in v0 mask

    vlse64.v v8, (a1), t6    # v8 = a.u
    vlse64.v v10, (a2), t6   # v10 = b.u
    vadc.vvm v12, v8, v10, v0 # v12 = a.u + b.u + carry

    add t4, a0, t1
    vsse64.v v6, (t4), t6    # Store c.l
    vsse64.v v12, (a0), t6   # Store c.u

    slli t2, t0, 4           # Pointer advance (elements processed * 16 bytes)
    add a0, a0, t2
    add a1, a1, t2
    add a2, a2, t2
    sub a3, a3, t0
    bnez a3, 1b
2:
    li a0, 0          # HAL_OK (0) 반환
    ret
.endm

.macro DEF_MAX_128 NAME
.global \NAME
\NAME:
    li t1, 8
    li t6, 16
    beqz a3, 2f
1:  vsetvli t0, a3, e64, m1, ta, ma
    add t4, a1, t1
    add t5, a2, t1
    vlse64.v v2, (t4), t6    # a.l
    vlse64.v v4, (t5), t6    # b.l

    vsub.vv v6, v2, v4       # v6 = a.l - b.l
    vmsltu.vv v0, v2, v4     # Check for Borrow generation (a.l < b.l) -> v0 mask

    vlse64.v v8, (a1), t6    # a.u
    vlse64.v v10, (a2), t6   # b.u
    vsbc.vvm v12, v8, v10, v0 # v12 = a.u - b.u - borrow

    add t4, a0, t1
    vsse64.v v6, (t4), t6
    vsse64.v v12, (a0), t6

    slli t2, t0, 4
    add a0, a0, t2
    add a1, a1, t2
    add a2, a2, t2
    sub a3, a3, t0
    bnez a3, 1b
2:
    li a0, 0          # HAL_OK (0) 반환
    ret
.endm


####
.macro DEF_MINMAX_128 NAME_I, NAME_U, INSN, INSN_U, INSN_C

.global \NAME_I
.global \NAME_U

.type \NAME_I, @function
.type \NAME_U, @function

# void \NAME_I(int128_t *dest (a0), const int128_t *srcA (a1), const int128_t *srcB (a2), size_t n (a3))
\NAME_I:
    beqz a3, 2f             # 처리할 요소가 없으면 종료

1:
    vsetvli t0, a3, e64, m1, ta, ma

    # Segmented Load (데이터 레지스터: v2~v5 사용)
    vlseg2e64.v v2, (a1)    # v2 = srcA.u, v3 = srcA.l
    vlseg2e64.v v4, (a2)    # v4 = srcB.u, v5 = srcB.l

    # 마스크 레지스터로 v1, v8, v9 사용 (데이터 레지스터 v2, v3와 충돌 방지)
    # 조건 1: 상위 비트 비교
    \INSN v1, v2, v4        # v1 = (srcA.u < srcB.u) 또는 (srcA.u > srcB.u)

    # 조건 2-1: 상위 비트 같음 확인
    vmseq.vv v8, v2, v4     # v8 = (srcA.u == srcB.u)

    # 조건 2-2: 하위 비트 비교 (항상 Unsigned)
    \INSN_C v9, v3, v5      # v9 = (srcA.l < srcB.l) 또는 (srcA.l > srcB.l)

    # 조건 2 결합: (A.u == B.u) && (A.l < B.l)
    vmand.mm v8, v8, v9     # v8 = v8 AND v9

    # 최종 마스크 생성 (v0는 vmerge의 전용 마스크 레지스터)
    vmor.mm v0, v1, v8      # v0 = v1 OR v8

    # 더 작은/큰 값을 선택하여 v6, v7에 저장
    vmerge.vvm v6, v4, v2, v0   # v6 = (v0) ? srcA.u : srcB.u
    vmerge.vvm v7, v5, v3, v0   # v7 = (v0) ? srcA.l : srcB.l

    # Segmented Store
    vsseg2e64.v v6, (a0)    # dest.u = v6, dest.l = v7

    # 포인터 및 카운터 업데이트
    slli t1, t0, 4          # t1 = t0 * 16 (128-bit = 16 bytes)
    add a1, a1, t1
    add a2, a2, t1
    add a0, a0, t1
    sub a3, a3, t0

    bnez a3, 1b             # 남은 데이터가 있으면 반복
2:
    li a0, 0                # HAL_OK (0) 반환
    ret

# void \NAME_U(uint128_t *dest (a0), const uint128_t *srcA (a1), const uint128_t *srcB (a2), size_t n (a3))
\NAME_U:
    beqz a3, 4f

3:
    vsetvli t0, a3, e64, m1, ta, ma

    vlseg2e64.v v2, (a1)
    vlseg2e64.v v4, (a2)

    # Unsigned 전용 명령어(\INSN_U) 사용
    \INSN_U v1, v2, v4

    vmseq.vv v8, v2, v4
    \INSN_C v9, v3, v5

    vmand.mm v8, v8, v9
    vmor.mm v0, v1, v8

    vmerge.vvm v6, v4, v2, v0
    vmerge.vvm v7, v5, v3, v0

    vsseg2e64.v v6, (a0)

    slli t1, t0, 4
    add a1, a1, t1
    add a2, a2, t1
    add a0, a0, t1
    sub a3, a3, t0

    bnez a3, 3b
4:
    li a0, 0
    ret

.endm

# -----------------------------------------------------------------------------
# Macro Instantiations
# -----------------------------------------------------------------------------

# MIN 연산: 상위가 '작다(lt)'일 때 A 선택
DEF_MINMAX_128 hal_vmin_i128 hal_vmin_u128 vmslt.vv vmsltu.vv vmsltu.vv

# MAX 연산: 상위가 '크다(gt)'일 때 A 선택 
# [중요] vmsge(>=)가 아니라 vmsgt(>)를 사용하여 하위 비트 비교 로직이 정상 작동하도록 수정
DEF_MINMAX_128 hal_vmax_i128 hal_vmax_u128 vmsgt.vv vmsgtu.vv vmsgtu.vv


# -----------------------------------------------------------------------------
# MIN / MAX : float
# -----------------------------------------------------------------------------
# hal_status_t hal_min_f32(float *dest (a0), const float *src1 (a1), const float *src2 (a2), size_t n (a3))
# hal_status_t hal_max_f32(float *dest (a0), const float *src1 (a1), const float *src2 (a2), size_t n (a3))

.macro DEF_MINMAX_F32 NAME_FMIN, NAME_FMAX, INSN_MIN, INSN_MAX
.global \NAME_FMIN
.global \NAME_FMAX

\NAME_FMIN:
    # a3(n)이 0이면 바로 종료 (예외 처리)
    beqz a3, 2f
1:
    # 1. 벡터 설정 (vsetvli)
    # 현재 처리할 요소 수(a3)를 요청하고, 실제 처리 가능한 요소 수를 t0에 반환받음
    # e32: 32비트 데이터 (uint32_t)
    # m1: LMUL=1 (벡터 레지스터 1개 사용)
    # ta, ma: Tail Agnostic, Mask Agnostic (성능 최적화를 위해 남은 부분은 신경 쓰지 않음)
    vsetvli t0, a3, e32, m1, ta, ma

    # 2. 메모리에서 벡터 레지스터로 로드 (Unit-stride Load)
    vle32.v v8, (a1)        # v8 = src1[i ... i+t0-1]
    vle32.v v9, (a2)        # v9 = src2[i ... i+t0-1]

    # 3. 핵심 연산: Unsigned Minimum
    # v10 = min_unsigned(v8, v9)
    \INSN_MIN v10, v8, v9

    # 4. 벡터 레지스터에서 메모리로 저장 (Unit-stride Store)
    vse32.v v10, (a0)       # dest[i ... i+t0-1] = v10

    # 5. 포인터 및 카운터 업데이트
    slli t1, t0, 2          # t1 = t0 * 4 (32비트는 4바이트이므로 포인터 이동 거리 계산)
    
    add a1, a1, t1          # src1 포인터 증가
    add a2, a2, t1          # src2 포인터 증가
    add a0, a0, t1          # dest 포인터 증가
    
    sub a3, a3, t0          # 남은 요소 수(n) = n - t0

    # 6. 남은 요소가 있으면 루프 반복
    bnez a3, 1b

2:
    li a0, 0                # HAL_OK(0)
    ret                     # 함수 반환

\NAME_FMAX:
    beqz a3, 4f
3:
    # 1. 벡터 설정 (vsetvli)
    # 현재 처리할 요소 수(a3)를 요청하고, 실제 처리 가능한 요소 수를 t0에 반환받음
    # e32: 32비트 데이터 (uint32_t)
    # m1: LMUL=1 (벡터 레지스터 1개 사용)
    # ta, ma: Tail Agnostic, Mask Agnostic (성능 최적화를 위해 남은 부분은 신경 쓰지 않음)
    vsetvli t0, a3, e32, m1, ta, ma
    
    # 2. 메모리에서 벡터 레지스터로 로드 (Unit-stride Load)
    vle32.v v8, (a1)        # v8 = src1[i ... i+t0-1]
    vle32.v v9, (a2)        # v9 = src2[i ... i+t0-1]

    # 3. 핵심 연산: Unsigned Maximum
    # v10 = max_unsigned(v8, v9)
    \INSN_MAX v10, v8, v9

    # 4. 벡터 레지스터에서 메모리로 저장 (Unit-stride Store)
    vse32.v v10, (a0)       # dest[i ... i+t0-1] = v10

    # 5. 포인터 및 카운터 업데이트
    slli t1, t0, 2          # t1 = t0 * 4 (32비트는 4바이트이므로 포인터 이동 거리 계산)
    
    add a1, a1, t1          # src1 포인터 증가
    add a2, a2, t1          # src2 포인터 증가
    add a0, a0, t1          # dest 포인터 증가
    
    sub a3, a3, t0          # 남은 요소 수(n) = n - t0

    # 6. 남은 요소가 있으면 루프 반복
    bnez a3, 3b

4:
    li a0, 0                # HAL_OK(0)
    ret                     # 함수 반환

.endm

# =============================================================================
# Macro Invocations
# =============================================================================
# .macro DEF_MINMAX_F32 NAME_FMIN, NAME_FMAX, INSN_MIN, INSN_MAX
# --- Min
DEF_MINMAX_F32 hal_vmin_f32, hal_vmax_f32, vfmin.vv, vfmax.vv


# -----------------------------------------------------------------------------
# SQRT / SQRT7 (Floating-point square-root / Reciprocal square-root estimate)
# -----------------------------------------------------------------------------
# hal_status_t hal_vsqrt_f32(float *dest (a0), const float *src1 (a1), size_t n (a2))
# hal_status_t hal_vsqrt7_f32(float *dest (a0), const float *src1 (a1), size_t n (a2))

.macro DEF_SQRT_F32 NAME_SQRT, NAME_SQRT7, INSN_SQRT, INSN_SQRT7
.global \NAME_SQRT
.global \NAME_SQRT7

.type \NAME_SQRT, @function
.type \NAME_SQRT7, @function

\NAME_SQRT:
    # a2(n)이 0이면 바로 종료
    beqz a2, 2f
1:
    # 1. 벡터 설정 (e32: float32_t)
    # n(a2)을 이용해 vsetvli 호출
    vsetvli t0, a2, e32, m1, ta, ma

    # 2. 메모리에서 벡터 레지스터로 로드
    vle32.v v8, (a1)        # v8 = src1[i ... i+t0-1]

    # 3. 핵심 연산: Floating-point Square Root
    # 꼬리표(m1, ma 등) 제거하고 순수 명령어만 사용
    \INSN_SQRT v9, v8       # v9 = sqrt(v8)

    # 4. 벡터 레지스터에서 메모리로 저장
    vse32.v v9, (a0)        # dest[i ... i+t0-1] = v9

    # 5. 포인터 및 카운터 업데이트
    slli t1, t0, 2          # t1 = t0 * 4 (float는 4바이트)
    
    add a1, a1, t1          # src1 포인터 증가
    add a0, a0, t1          # dest 포인터 증가
    
    sub a2, a2, t0          # 남은 요소 수(n) = n - t0

    # 6. 남은 요소가 있으면 루프 반복
    bnez a2, 1b

2:
    li a0, 0                # HAL_OK(0)
    ret

\NAME_SQRT7:
    # a2(n)이 0이면 바로 종료
    beqz a2, 4f
3:
    # 1. 벡터 설정
    vsetvli t0, a2, e32, m1, ta, ma
    
    # 2. 벡터 레지스터 로드
    vle32.v v8, (a1)        # v8 = src1[i ... i+t0-1]

    # 3. 핵심 연산: Reciprocal Square-Root Estimate (1/sqrt(x))
    \INSN_SQRT7 v9, v8      # v9 = 1/sqrt(v8) (approx 7-bit)

    # 4. 메모리로 저장 (v10 오타를 v9로 수정)
    vse32.v v9, (a0)        # dest[i ... i+t0-1] = v9

    # 5. 포인터 및 카운터 업데이트
    slli t1, t0, 2
    
    add a1, a1, t1
    add a0, a0, t1
    
    sub a2, a2, t0

    # 6. 남은 요소가 있으면 루프 반복 (a3 오타를 a2로 수정)
    bnez a2, 3b

4:
    li a0, 0                # HAL_OK(0)
    ret

.endm

# =============================================================================
# Macro Invocations
# =============================================================================
# SQRT 매크로 호출 (vfsqrt.v: 제곱근, vfrsqrt7.v: 역제곱근 근사치)
DEF_SQRT_F32 hal_vsqrt_f32, hal_vrsqrt7_f32, vfsqrt.v, vfrsqrt7.v



# <---
