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
# 2. DIV (Includes Divide-by-Zero Exception Handling) Macro
# -----------------------------------------------------------------------------
.macro DEF_DIV_INT NAME_I, NAME_U, INSN, INSN_U, SEW, SHIFT
.global \NAME_I
.global \NAME_U
\NAME_I:
    bnez a4, 1f          # if a4(ret) is not a NULL, jump to 1f
    j 2f
1:  sw zero, 0(a4)       # *ret = 0 (initialize)
2:  beqz a3, 9f
3:  vsetvli t0, a3, \SEW, m1, ta, ma
    vl\SEW\().v v1, (a1)
    vl\SEW\().v v2, (a2)
    \INSN v3, v1, v2     # v3 = a / b (Generates -1 on divide-by-zero)

    vmseq.vi v0, v2, 0   # v0 mask register: Set to 1 where b == 0
    vmv.v.i v4, 0        # v4 = 0
    vmerge.vvm v3, v3, v4, v0  # Overwrite with v4(0) where v0 is 1, otherwise keep v3(result)!

    vs\SEW\().v v3, (a0)

    beqz a4, 4f          # Skip if there is no need to pass an error code
    vcpop.m t2, v0       # Count the number of 1s in the v0 mask
    beqz t2, 4f          # Skip if no divide-by-zero occurred
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

# [Float 32 Division with Exception Handling]
.global hal_vdiv_f32
hal_vdiv_f32:
    bnez a4, 1f
    j 2f
1:  sw zero, 0(a4)
2:  beqz a3, 9f
    vsetvli t0, zero, e32, m1, ta, ma
    vmv.v.i v4, 0        # v4 = 0.0f
3:  vsetvli t0, a3, e32, m1, ta, ma
    vle32.v v1, (a1)     #  vle32.v
    vle32.v v2, (a2)     #  vle32.v
    vfdiv.vv v3, v1, v2
    vmfeq.vv v0, v2, v4  # Mask: b == 0.0f
    vmerge.vvm v3, v3, v4, v0
    vse32.v v3, (a0)     #  vse32.v
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
DEF_WMUL hal_vmul_i8, hal_vmul_u8, vwmul.vv, vwmulu.vv, e8, e16, 0, 1
DEF_WMUL hal_vmul_i16, hal_vmul_u16, vwmul.vv, vwmulu.vv, e16, e32, 1, 2
DEF_WMUL hal_vmul_i32, hal_vmul_u32, vwmul.vv, vwmulu.vv, e32, e64, 2, 3
DEF_WMUL hal_vmul_f32, hal_dummy_mul_f32, vfwmul.vv, vfwmul.vv, e32, e64, 2, 3

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
9:  ret



.global hal_matrix_vmul_tiled_f32
# ==============================================================================
# void hal_matrix_vmul_tiled_f32(double* c, const float* a, const float* b,
#                               int M, int N, int K, int tile_size);
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
2:  ret
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
2:  ret
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
2:  ret
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
    ret
.endm

# Macro Invocations: Generate 6 tiling functions for 8, 16, 32-bit all at once!
DEF_WMATRIX_TILED hal_matrix_vmul_tiled_i8, hal_matrix_vmul_tiled_u8, lb, lbu, vwmacc.vx, vwmaccu.vx, e8, e16, 0, 1, 1
DEF_WMATRIX_TILED hal_matrix_vmul_tiled_i16, hal_matrix_vmul_tiled_u16, lh, lhu, vwmacc.vx, vwmaccu.vx, e16, e32, 1, 2, 2
DEF_WMATRIX_TILED hal_matrix_vmul_tiled_i32, hal_matrix_vmul_tiled_u32, lw, lw, vwmacc.vx, vwmaccu.vx, e32, e64, 2, 3, 4

# ==============================================================================
# 함수명: hal_fft_init_twiddles_f32
# 설명  : FFT 회전 인자 테이블 가속 생성 (W_N^k = cos(2pi*k/N) - j*sin(2pi*k/N))
# C 형태: void hal_fft_init_twiddles_f32(dsp_complex_f32_t *w_table, int n_fft);
# a0 = w_table 주소, a1 = n_fft (포인트 수)
# ==============================================================================
.global hal_fft_init_twiddles_f32
hal_fft_init_twiddles_f32:
    li t0, 0                # k_start = 0
    srli a2, a1, 1          # loop_count = n_fft / 2 (보통 N/2개 필요)

    # 2*pi / N 상수 계산
    fcvt.s.w fa0, a1        # fa0 = (float)n_fft
    li t1, 0x40c90fdb       # t1 = 2 * pi (float bit pattern)
    fmv.w.x fa1, t1
    fdiv.s fa2, fa1, fa0    # fa2 = step = 2*pi / N

1:
    vsetvli t1, a2, e32, m1, ta, ma

    # 1. 인덱스 벡터 k 생성: [0, 1, 2, ...]
    vid.v v0
    vfmv.v.f v1, fa2        # step 벡터 복사

    # 현재 시작 인덱스 t0를 더해줌
    fcvt.s.w fa3, t0
    vfadd.vf v0, v0, fa3    # v0 = [k, k+1, ...]

    # 2. 각도 theta = k * step 계산
    vfmul.vv v1, v0, v1     # v1 = theta 벡터

    # 3. cos(theta) 및 sin(theta) 계산 (단순화를 위해 Taylor 근사 3차항 사용)
    # sin(x) ~ x - x^3/6
    # cos(x) ~ 1 - x^2/2
    vfmul.vv v2, v1, v1     # v2 = x^2

    # =================================================================
    # [수정된 부분] 상수를 float 레지스터(fa4, fa5)를 거쳐서 벡터로 로드
    # =================================================================
    # cos 계산: v3 = 1.0 - x^2 * 0.5
    li t3, 0x3f000000       # 0.5f
    fmv.w.x fa4, t3
    vfmv.v.f v3, fa4        # v3 = 0.5f

    li t4, 0x3f800000       # 1.0f
    fmv.w.x fa5, t4
    vfmv.v.f v4, fa5        # v4 = 1.0f

    vfnmsac.vv v4, v2, v3   # v4 = 1.0 - (x^2 * 0.5) -> Real (cos)

    # sin 계산: v5 = x - (x^2 * x * 0.1666...)
    vfmul.vv v3, v2, v1     # v3 = x^3
    li t2, 0x3e2aaaab       # 1/6 (0.1666...)
    fmv.w.x fa4, t2

    vmv.v.v v5, v1          # [수정] 벡터 간 복사는 vmv.v.v 사용 (v5 = x)
    vfnmsac.vf v5, fa4, v3  # v5 = x - (x^3 * 1/6) -> Imag (sin)
    # =================================================================

    # FFT Twiddle은 exp(-j*theta) 이므로 허수부에 -1을 곱함
    vfsgnjn.vv v5, v5, v5

    # 4. 인터리브 저장 (실수, 허수 쌍으로 저장)
    # vsseg2e32.v: v4(Real), v5(Imag)를 {Re, Im, Re, Im...} 순서로 저장
    vsseg2e32.v v4, (a0)

    # 포인터 및 인덱스 갱신
    slli t2, t1, 3          # 처리한 개수 * 8바이트 (float 2개)
    add a0, a0, t2
    add t0, t0, t1
    sub a2, a2, t1
    bnez a2, 1b

    ret


# ==============================================================================
# 함수명: hal_fft_butterfly_f32
# 설명  : RVV 가속 복소수 버터플라이 연산 코어
# C 형태: void hal_fft_butterfly_f32(float *top, float *bot, const float *w_table, int count, int w_stride_bytes);
# a0 = top_ptr, a1 = bot_ptr, a2 = twiddle_ptr, a3 = count, a4 = w_stride_bytes
# ==============================================================================
.global hal_fft_butterfly_f32
.type hal_fft_butterfly_f32, @function
hal_fft_butterfly_f32:
    beqz a3, 2f
1:
    vsetvli t0, a3, e32, m1, ta, ma

    # 1. Segmented Load: [Re, Im] 교차 배열을 두 개의 독립된 벡터로 분리 로드!
    vlseg2e32.v v0, (a0)      # v0 = A_real, v1 = A_imag
    vlseg2e32.v v2, (a1)      # v2 = B_real, v3 = B_imag

    # 2. Strided Segmented Load: Twiddle Factor를 stride만큼 건너뛰며 분리 로드!
    vlsseg2e32.v v4, (a2), a4 # v4 = W_real, v5 = W_imag

    # 3. 복소수 곱셈: Temp = W * B
    # temp_real = W_real * B_real - W_imag * B_imag
    vfmul.vv v6, v4, v2       # v6 = W_re * B_re
    vfnmsac.vv v6, v5, v3     # v6 = v6 - (W_im * B_im)

    # temp_imag = W_real * B_imag + W_imag * B_real
    vfmul.vv v7, v4, v3       # v7 = W_re * B_im
    vfmacc.vv v7, v5, v2      # v7 = v7 + (W_im * B_re)

    # 4. 버터플라이 덧셈/뺄셈 (X 교차 연산)
    # X_bot = A - Temp
    vfsub.vv v8, v0, v6       # bot_real = A_re - temp_re
    vfsub.vv v9, v1, v7       # bot_imag = A_im - temp_im

    # X_top = A + Temp
    vfadd.vv v0, v0, v6       # top_real = A_re + temp_re
    vfadd.vv v1, v1, v7       # top_imag = A_im + temp_im

    # 5. Segmented Store: 다시 [Re, Im] 교차 배열로 묶어서 메모리에 저장!
    vsseg2e32.v v0, (a0)
    vsseg2e32.v v8, (a1)

    # 6. 포인터 전진
    slli t1, t0, 3            # 처리 개수 * 8 바이트 (float 2개)
    add a0, a0, t1            # top_ptr 이동
    add a1, a1, t1            # bot_ptr 이동

    mul t2, t0, a4            # Twiddle은 stride 크기만큼 이동
    add a2, a2, t2

    sub a3, a3, t0            # 남은 개수 감소
    bnez a3, 1b
2:  ret
