.option arch, +v
# ==============================================================================
# Array Reduction 가속 함수들 (Max Pooling & Softmax 용)
# ==============================================================================
.global hal_max_array_f32
.type hal_max_array_f32, @function
hal_max_array_f32:
    flw fa0, 0(a0)
    vsetvli t0, zero, e32, m1, ta, ma
    vfmv.s.f v0, fa0
1:
    vsetvli t0, a1, e32, m1, ta, ma
    vle32.v v1, (a0)
    vfredmax.vs v0, v1, v0    # [핵심] Float 전용 최댓값 리덕션!

    slli t1, t0, 2
    add a0, a0, t1
    sub a1, a1, t0
    bnez a1, 1b

    vfmv.f.s fa0, v0
    ret

.global hal_sum_array_f32
.type hal_sum_array_f32, @function
hal_sum_array_f32:
    vsetvli t0, zero, e32, m1, ta, ma
    vmv.v.i v0, 0         # v0[0] = 0.0f
1:
    vsetvli t0, a1, e32, m1, ta, ma
    vle32.v v1, (a0)
    vfredusum.vs v0, v1, v0   # [핵심] Float 전용 누적합 리덕션!

    slli t1, t0, 2
    add a0, a0, t1
    sub a1, a1, t0
    bnez a1, 1b

    vfmv.f.s fa0, v0
    ret
