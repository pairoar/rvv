# =============================================================================
# AERMOB RVV HAL LAYER (Macro-Driven Implementation - RV32 Final)
# =============================================================================

# -----------------------------------------------------------------------------
# 1. ADD / SUB 매크로 (입출력 크기 동일)
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
2:  ret

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
4:  ret
.endm

# -----------------------------------------------------------------------------
# 2. DIV (Divide by Zero 예외 처리 포함) 매크로
# -----------------------------------------------------------------------------
.macro DEF_DIV_INT NAME_I, NAME_U, INSN, INSN_U, SEW, SHIFT
.global \NAME_I
.global \NAME_U
\NAME_I:
    bnez a4, 1f          # a4(ret)가 NULL이 아니면 1f로 점프
    j 2f
1:  sw zero, 0(a4)       # *ret = 0 (초기화)
2:  beqz a3, 9f
3:  vsetvli t0, a3, \SEW, m1, ta, ma
    vl\SEW\().v v1, (a1)
    vl\SEW\().v v2, (a2)
    \INSN v3, v1, v2     # v3 = a / b (0으로 나누면 -1 발생)

    vmseq.vi v0, v2, 0   # v0 마스크 레지스터: b == 0 인 위치를 1로 세팅
    vmv.v.i v4, 0        # v4 = 0
    vmerge.vvm v3, v3, v4, v0  # v0가 1인 곳은 v4(0)으로, 아니면 v3(결과)로 덮어씀!

    vs\SEW\().v v3, (a0)

    beqz a4, 4f          # 에러 코드를 넘길 필요가 없으면 패스
    vcpop.m t2, v0       # v0 마스크에서 1인 비트의 개수를 카운트
    beqz t2, 4f          # 0으로 나눈 적이 없으면 패스
    lw t3, 0(a4)
    ori t3, t3, 1        # *ret |= HAL_MATH_ERR_DIV_BY_ZERO
    sw t3, 0(a4)
4:  slli t1, t0, \SHIFT
    add a0, a0, t1
    add a1, a1, t1
    add a2, a2, t1
    sub a3, a3, t0
    bnez a3, 3b
9:  ret

\NAME_U:
    bnez a4, 10f
    j 20f
10: sw zero, 0(a4)
20: beqz a3, 90f
30: vsetvli t0, a3, \SEW, m1, ta, ma
    vl\SEW\().v v1, (a1)
    vl\SEW\().v v2, (a2)
    \INSN_U v3, v1, v2
    vmseq.vi v0, v2, 0
    vmv.v.i v4, 0
    vmerge.vvm v3, v3, v4, v0
    vs\SEW\().v v3, (a0)
    beqz a4, 40f
    vcpop.m t2, v0
    beqz t2, 40f
    lw t3, 0(a4)
    ori t3, t3, 1
    sw t3, 0(a4)
40: slli t1, t0, \SHIFT
    add a0, a0, t1
    add a1, a1, t1
    add a2, a2, t1
    sub a3, a3, t0
    bnez a3, 30b
90: ret
.endm

# -----------------------------------------------------------------------------
# 3. MUL & MAC (Widening) 매크로
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
2:  ret
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
4:  ret
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
2:  ret
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
4:  ret
.endm

# -----------------------------------------------------------------------------
# 4. DOT Product (Widening) 매크로
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

    # [핵심] 리덕션 연산을 위해 자료형을 WSEW(확장 크기)로 강제 스위칭
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
    ret
.endm

# -----------------------------------------------------------------------------
# 5. Matrix Multiply (Widening) 매크로
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
9:  ret

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
19: ret
.endm

# =============================================================================
# 매크로 호출부
# =============================================================================

# --- 1. ADD / SUB / DIV ---
DEF_OP_SAME hal_add_i8, hal_add_u8, vadd.vv, vadd.vv, e8, 0
DEF_OP_SAME hal_add_i16, hal_add_u16, vadd.vv, vadd.vv, e16, 1
DEF_OP_SAME hal_add_i32, hal_add_u32, vadd.vv, vadd.vv, e32, 2
DEF_OP_SAME hal_add_i64, hal_add_u64, vadd.vv, vadd.vv, e64, 3

DEF_OP_SAME hal_sub_i8, hal_sub_u8, vsub.vv, vsub.vv, e8, 0
DEF_OP_SAME hal_sub_i16, hal_sub_u16, vsub.vv, vsub.vv, e16, 1
DEF_OP_SAME hal_sub_i32, hal_sub_u32, vsub.vv, vsub.vv, e32, 2
DEF_OP_SAME hal_sub_i64, hal_sub_u64, vsub.vv, vsub.vv, e64, 3

DEF_DIV_INT hal_div_i8, hal_div_u8, vdiv.vv, vdivu.vv, e8, 0
DEF_DIV_INT hal_div_i16, hal_div_u16, vdiv.vv, vdivu.vv, e16, 1
DEF_DIV_INT hal_div_i32, hal_div_u32, vdiv.vv, vdivu.vv, e32, 2
DEF_DIV_INT hal_div_i64, hal_div_u64, vdiv.vv, vdivu.vv, e64, 3

DEF_OP_SAME hal_add_f32, hal_dummy_add_f32, vfadd.vv, vfadd.vv, e32, 2
DEF_OP_SAME hal_sub_f32, hal_dummy_sub_f32, vfsub.vv, vfsub.vv, e32, 2

# -----------------------------------------------------------------------------
# [수정됨] Float 32 수동 구현부 (vle / vse 명령어 스펠링 복구)
# -----------------------------------------------------------------------------

# [Float 32 예외 처리 나눗셈]
.global hal_div_f32
hal_div_f32:
    bnez a4, 1f
    j 2f
1:  sw zero, 0(a4)
2:  beqz a3, 9f
    vsetvli t0, zero, e32, m1, ta, ma
    vmv.v.i v4, 0        # v4 = 0.0f
3:  vsetvli t0, a3, e32, m1, ta, ma
    vle32.v v1, (a1)     # [수정] vle32.v
    vle32.v v2, (a2)     # [수정] vle32.v
    vfdiv.vv v3, v1, v2
    vmfeq.vv v0, v2, v4  # 마스크: b == 0.0f
    vmerge.vvm v3, v3, v4, v0
    vse32.v v3, (a0)     # [수정] vse32.v
    beqz a4, 4f
    vcpop.m t2, v0
    beqz t2, 4f
    lw t3, 0(a4)
    ori t3, t3, 1
    sw t3, 0(a4)
4:  slli t1, t0, 2
    add a0, a0, t1
    add a1, a1, t1
    add a2, a2, t1
    sub a3, a3, t0
    bnez a3, 3b
9:  ret

# --- 2. Widening MUL ---
DEF_WMUL hal_mul_i8, hal_mul_u8, vwmul.vv, vwmulu.vv, e8, e16, 0, 1
DEF_WMUL hal_mul_i16, hal_mul_u16, vwmul.vv, vwmulu.vv, e16, e32, 1, 2
DEF_WMUL hal_mul_i32, hal_mul_u32, vwmul.vv, vwmulu.vv, e32, e64, 2, 3
DEF_WMUL hal_mul_f32, hal_dummy_mul_f32, vfwmul.vv, vfwmul.vv, e32, e64, 2, 3

# --- 3. Widening MAC ---
DEF_WMAC hal_mac_i8, hal_mac_u8, vwmacc.vv, vwmaccu.vv, e8, e16, 0, 1
DEF_WMAC hal_mac_i16, hal_mac_u16, vwmacc.vv, vwmaccu.vv, e16, e32, 1, 2
DEF_WMAC hal_mac_i32, hal_mac_u32, vwmacc.vv, vwmaccu.vv, e32, e64, 2, 3
DEF_WMAC hal_mac_f32, hal_dummy_mac_f32, vfwmacc.vv, vfwmacc.vv, e32, e64, 2, 3

# --- 4. Widening DOT ---
DEF_WDOT hal_dot_i8, hal_dot_u8, vwmul.vv, vwmulu.vv, e8, e16, 0
DEF_WDOT hal_dot_i16, hal_dot_u16, vwmul.vv, vwmulu.vv, e16, e32, 1
DEF_WDOT hal_dot_i32, hal_dot_u32, vwmul.vv, vwmulu.vv, e32, e64, 2

.global hal_dot_f32
hal_dot_f32:
    vsetvli t0, zero, e64, m2, ta, ma
    vmv.v.i v8, 0
    beqz a3, 2f
1:  vsetvli t0, a3, e32, m1, ta, ma
    vle32.v v0, (a1)     # [수정] vle32.v
    vle32.v v1, (a2)     # [수정] vle32.v
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
    vse64.v v8, (a0)     # [수정] vse64.v
    ret

# --- 5. Widening MATRIX MUL ---
DEF_WMATRIX hal_matrix_mul_i8, hal_matrix_mul_u8, "lb a7, 0(t2)", "lbu a7, 0(t2)", "vwmacc.vx v2, a7, v0", "vwmaccu.vx v2, a7, v0", e8, e16, 0, 1
DEF_WMATRIX hal_matrix_mul_i16, hal_matrix_mul_u16, "lh a7, 0(t2)", "lhu a7, 0(t2)", "vwmacc.vx v2, a7, v0", "vwmaccu.vx v2, a7, v0", e16, e32, 1, 2
DEF_WMATRIX hal_matrix_mul_i32, hal_matrix_mul_u32, "lw a7, 0(t2)", "lw a7, 0(t2)", "vwmacc.vx v2, a7, v0", "vwmaccu.vx v2, a7, v0", e32, e64, 2, 3

.global hal_matrix_mul_f32
hal_matrix_mul_f32:
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
    vle32.v v0, (t3)     # [수정] vle32.v
    vle64.v v2, (t4)     # [수정] vle64.v
    vfwmacc.vf v2, fa0, v0
    vse64.v v2, (t4)     # [수정] vse64.v
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
9:  ret
