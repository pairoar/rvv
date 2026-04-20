#
# declare int32_t/uint32_t operation
#
.global hal_add_i32
.global hal_add_u32

.global hal_sub_i32
.global hal_sub_u32

.global hal_mul_i32
.global hal_mul_u32

.global hal_mac_i32
.global hal_mac_u32

.global hal_div_i32
.global hal_div_u32

.global hal_dot_i32
.global hal_dot_u32

.global hal_matrix_mul_i32
.global hal_matrix_mul_u32

#
# declare float 32 operation
#
.global hal_add_f32
.global hal_sub_f32
.global hal_mul_f32
.global hal_mac_f32
.global hal_div_f32
.global hal_dot_f32
.global hal_matrix_mul_f32


#
# int32_t/uint32_t
#

# void hal_add_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n)
# void hal_add_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n)
# a0 = *c, a1 = *a, a2 = *b, a3 = n
#  Vector Add: C[i] = A[i] + B[i]
hal_add_i32:
hal_add_u32:
    beqz a3, end_i32
1:  vsetvli a4, a3, e32, m1, ta, ma
    vle32.v v0, (a1)
    vle32.v v1, (a2)
    vadd.vv v2, v0, v1
    vse32.v v2, (a0)
    slli a5, a4, 2
    add a0, a0, a5
    add a1, a1, a5
    add a2, a2, a5
    sub a3, a3, a4
    bnez a3, 1b
end_i32:
    ret

# void hal_add_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n)
# void hal_add_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n)
# a0 = *c, a1 = *a, a2 = *b, a3 = n
#  Vector Add: C[i] = A[i] + B[i]
hal_sub_i32:
hal_sub_u32:
    beqz a3, end_i32
1:  vsetvli a4, a3, e32, m1, ta, ma
    vle32.v v0, (a1)
    vle32.v v1, (a2)
    vsub.vv v2, v0, v1
    vse32.v v2, (a0)
    slli a5, a4, 2
    add a0, a0, a5
    add a1, a1, a5
    add a2, a2, a5
    sub a3, a3, a4
    bnez a3, 1b
end_sub_i32:
    ret

# void hal_mul_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n);
# void hal_mul_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n);
# Vector Mul: C[i] = A[i] * B[i]
# a0 = *c, a1 = *a, a2 = *b, a3 = n
hal_mul_i32:
hal_mul_u32:
    beqz a3, end_mul_i32
1:  vsetvli a4, a3, e32, m1, ta, ma
    vle32.v v0, (a1)
    vle32.v v1, (a2)
    vmul.vv v2, v0, v1     # vmul 사용
    vse32.v v2, (a0)
    slli a5, a4, 2
    add a0, a0, a5
    add a1, a1, a5
    add a2, a2, a5
    sub a3, a3, a4
    bnez a3, 1b
end_mul_i32:
    ret

# void hal_mac_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n)
# void hal_mac_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n)
# Vector MAC: C[i] = C[i] + (A[i] * B[i])
# a0 = *c, a1 = *a, a2 = *b, a3 = n
hal_mac_i32:
hal_mac_u32:
    beqz a3, end_mac_i32
1:  vsetvli a4, a3, e32, m1, ta, ma
    vle32.v v0, (a1)        # A 로드
    vle32.v v1, (a2)        # B 로드
    vle32.v v2, (a0)        # C 로드 (누산할 기존 값)
    vmacc.vv v2, v0, v1    # v2 = v2 + (v0 * v1)
    vse32.v v2, (a0)        # 갱신된 C 저장
    slli a5, a4, 2
    add a0, a0, a5
    add a1, a1, a5
    add a2, a2, a5
    sub a3, a3, a4
    bnez a3, 1b
end_mac_i32:
    ret

# void hal_div_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n);
# void hal_div_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n);
# Vector Mul: C[i] = A[i] / B[i]
# a0 = *c, a1 = *a, a2 = *b, a3 = n
hal_div_i32:
hal_div_u32:
    beqz a3, end_div_i32
1:  vsetvli a4, a3, e32, m1, ta, ma
    vle32.v v0, (a1)
    vle32.v v1, (a2)
    vdiv.vv v2, v0, v1     # vdiv 사용
    vse32.v v2, (a0)
    slli a5, a4, 2
    add a0, a0, a5
    add a1, a1, a5
    add a2, a2, a5
    sub a3, a3, a4
    bnez a3, 1b
end_div_i32:
    ret


# void hal_dot_i32(int32_t *result, const int32_t *a, const int32_t *b, const size_t n)
# void hal_dot_u32(uint32_t *result, const uint32_t *a, const uint32_t *b, const size_t n)
# a0: 결과값을 저장할 스칼라(float) 포인터
# a1: 배열 A 포인터
# a2: 배열 B 포인터
# a3: 배열 길이 (n)
# a0 = *result, a1 = *a, a2 = *b, a3 = n
hal_dot_i32:
hal_dot_u32:
    # 1. 누산기로 사용할 벡터 레지스터(v8)를 0로 초기화
    vsetvli t0, zero, e32, m1, ta, ma
    vmv.v.i v8, 0
    beqz a3, 2f                      # n == 0 이면 2번 라벨로 점프
1:  # 루프 시작
    vsetvli a4, a3, e32, m1, ta, ma  # 최적 벡터 길이(vl) 설정
    
    vle32.v v0, (a1)                 # A 배열 로드
    vle32.v v1, (a2)                 # B 배열 로드
    
    # 2. 병렬 곱셈: v2 = A * B
    vmul.vv v2, v0, v1
    
    # 3. 리덕션 합(Reduction Sum)
    # v2의 모든 요소를 더한 뒤, v8의 첫 번째 요소(v8[0])에 누적시킴
    # vs접미사는 Vector-Scalar 연산을 의미하며 순서 비의존적(Unordered) 합산을 수행
    vredsum.vs v8, v2, v8
    
    # 포인터 주소 업데이트 (vl * 4바이트)
    slli a5, a4, 2
    add a1, a1, a5
    add a2, a2, a5
    
    # 남은 루프 카운트 계산 및 분기
    sub a3, a3, a4
    bnez a3, 1b

2:  # 함수 종료 및 결과 저장
    # 벡터 레지스터 v8의 0번째 요소(스칼라)를 부동소수점 레지스터 fa0로 추출
    #vfmv.f.s fa0, v8
    vmv.x.s a6, v8
    
    # 추출한 스칼라 값을 a0 포인터가 가리키는 메모리에 단일 저장
    #fsw fa0, 0(a0)
    sw a6, 0(a0)
    ret


# void hal_matrix_mul_i32(int32_t *C, const int32_t *A, const int32_t *B,
#                              int M, int N, int K)
# a0: C, a1: A, a2: B, a3: M, a4: N, a5: K
hal_matrix_mul_i32:
hal_matrix_mul_u32:
    mv t0, zero              # i = 0

row_loop_i32:
    bge t0, a3, end_mat_mul_i32  # i < M
    mv t1, zero              # k = 0

k_loop_i32:
    bge t1, a5, next_row_i32     # k < K
    
    # s = A[i][k]
    mul t2, t0, a5
    add t2, t2, t1
    slli t2, t2, 2
    add t2, a1, t2           # A 배열 포인터(a1) 적용 완료
    lw a6, 0(t2)             # 정수 로드(lw) 및 a6 레지스터 할당 완료
    
    # ptr_B = &B[k][0]
    mul t3, t1, a4
    slli t3, t3, 2
    add t3, a2, t3           # B 배열 포인터(a2) 적용 완료
    
    # ptr_C = &C[i][0]
    mul t4, t0, a4
    slli t4, t4, 2
    add t4, a0, t4           # C 배열 포인터(a0) 적용 완료
    
    mv t5, a4                # n_remaining = N

vec_n_loop_i32:
    vsetvli t6, t5, e32, m1, ta, ma
    
    vle32.v v0, (t3)
    vle32.v v1, (t4)
    
    # [수정됨] 정수형 벡터-스칼라 곱셈 누산 (Vector-X register)
    vmacc.vx v1, a6, v0
    
    vse32.v v1, (t4)
    
    # 임시 레지스터 t2 재활용 로직 (훌륭합니다!)
    slli t2, t6, 2           # t2 = vl * 4
    add t3, t3, t2
    add t4, t4, t2
    
    sub t5, t5, t6
    bnez t5, vec_n_loop_i32
    
    addi t1, t1, 1           # k++
    j k_loop_i32

# [수정됨] 라벨 충돌 방지를 위해 이름 변경
next_row_i32:
    addi t0, t0, 1           # i++
    j row_loop_i32

end_mat_mul_i32:
    ret

#
# float
#
# void hal_add_f32(float *c, const float *a, const float *b, const size_t n)
# a0 = *c, a1 = *a, a2 = *b, a3 = n
# Vector Add: C[i] = A[i] + B[i]
# vsetvli a4, a3, e32, m1, ta, ma
# e32 : element width
# m1 : Vector register 1 ea
# ta : tail agnostic
# ma : mask agnostic
# 
hal_add_f32:
    beqz a3, end_add
1:  vsetvli a4, a3, e32, m1, ta, ma
    vle32.v v0, (a1)
    vle32.v v1, (a2)
    vfadd.vv v2, v0, v1
    vse32.v v2, (a0)
    slli a5, a4, 2
    add a0, a0, a5
    add a1, a1, a5
    add a2, a2, a5
    sub a3, a3, a4
    bnez a3, 1b
end_add:
    ret


# void hal_sub_f32(float *c, const float *a, const float *b, const size_t n)
# Vector Sub: C[i] = A[i] - B[i]
# a0 = *c, a1 = *a, a2 = *b, a3 = n
hal_sub_f32:
    beqz a3, end_sub
1:  vsetvli a4, a3, e32, m1, ta, ma
    vle32.v v0, (a1)
    vle32.v v1, (a2)
    vfsub.vv v2, v0, v1     # vfadd 대신 vfsub 사용
    vse32.v v2, (a0)
    slli a5, a4, 2          # a5 = a4 * 4
    add a0, a0, a5          # a0(주소) = a0 + a5
    add a1, a1, a5
    add a2, a2, a5
    sub a3, a3, a4
    bnez a3, 1b
end_sub:
    ret


# void hal_mul_f32(float *c, const float *a, const float *b, const size_t n);
# Vector Mul: C[i] = A[i] * B[i]
# a0 = *c, a1 = *a, a2 = *b, a3 = n
hal_mul_f32:
    beqz a3, end_mul
1:  vsetvli a4, a3, e32, m1, ta, ma
    vle32.v v0, (a1)
    vle32.v v1, (a2)
    vfmul.vv v2, v0, v1     # vfmul 사용
    vse32.v v2, (a0)
    slli a5, a4, 2
    add a0, a0, a5
    add a1, a1, a5
    add a2, a2, a5
    sub a3, a3, a4
    bnez a3, 1b
end_mul:
    ret

# void hal_mac_f32(float *c, const float *a, const float *b, const size_t n)
# Vector MAC: C[i] = C[i] + (A[i] * B[i])
# a0 = *c, a1 = *a, a2 = *b, a3 = n
hal_mac_f32:
    beqz a3, end_mac
1:  vsetvli a4, a3, e32, m1, ta, ma
    vle32.v v0, (a1)        # A 로드
    vle32.v v1, (a2)        # B 로드
    vle32.v v2, (a0)        # C 로드 (누산할 기존 값)
    vfmacc.vv v2, v0, v1    # v2 = v2 + (v0 * v1)
    vse32.v v2, (a0)        # 갱신된 C 저장
    slli a5, a4, 2
    add a0, a0, a5
    add a1, a1, a5
    add a2, a2, a5
    sub a3, a3, a4
    bnez a3, 1b
end_mac:
    ret

# void hal_div_f32(float *c, const float *a, const float *b, const size_t n)
# Vector DIV:  C[i] = A[i] / B[i]
# a0 = *c, a1 = *a, a2 = *b, a3 = n
hal_div_f32:
    beqz a3, end_div_f32
1:  vsetvli a4, a3, e32, m1, ta, ma
    vle32.v v0, (a1)
    vle32.v v1, (a2)
    vfdiv.vv v2, v0, v1     # vfdiv 사용
    vse32.v v2, (a0)
    slli a5, a4, 2
    add a0, a0, a5
    add a1, a1, a5
    add a2, a2, a5
    sub a3, a3, a4
    bnez a3, 1b
end_div_f32:
    ret


# void hal_dot_f32(float *result, const float *a, const float *b, const size_t n)
# a0: 결과값을 저장할 스칼라(float) 포인터
# a1: 배열 A 포인터
# a2: 배열 B 포인터
# a3: 배열 길이 (n)
# a0 = *result, a1 = *a, a2 = *b, a3 = n
hal_dot_f32:
    # 1. 누산기로 사용할 벡터 레지스터(v8)를 0.0f로 초기화
    # float 0.0f는 정수 0과 비트 패턴이 동일하므로 vmv.v.i (정수 초기화) 사용 가능
    vsetvli t0, zero, e32, m1, ta, ma
    vmv.v.i v8, 0

    beqz a3, 2f                      # n == 0 이면 2번 라벨로 점프

1:  # 루프 시작
    vsetvli a4, a3, e32, m1, ta, ma  # 최적 벡터 길이(vl) 설정
    
    vle32.v v0, (a1)                 # A 배열 로드
    vle32.v v1, (a2)                 # B 배열 로드
    
    # 2. 병렬 곱셈: v2 = A * B
    vfmul.vv v2, v0, v1
    
    # 3. 리덕션 합(Reduction Sum)
    # v2의 모든 요소를 더한 뒤, v8의 첫 번째 요소(v8[0])에 누적시킴
    # vs접미사는 Vector-Scalar 연산을 의미하며 순서 비의존적(Unordered) 합산을 수행
    vfredusum.vs v8, v2, v8
    
    # 포인터 주소 업데이트 (vl * 4바이트)
    slli a5, a4, 2
    add a1, a1, a5
    add a2, a2, a5
    
    # 남은 루프 카운트 계산 및 분기
    sub a3, a3, a4
    bnez a3, 1b

2:  # 함수 종료 및 결과 저장
    # 벡터 레지스터 v8의 0번째 요소(스칼라)를 부동소수점 레지스터 fa0로 추출
    vfmv.f.s fa0, v8
    
    # 추출한 스칼라 값을 a0 포인터가 가리키는 메모리에 단일 저장
    fsw fa0, 0(a0)
    ret

# hal_matrix_mul_f32


# void hal_matrix_mul_f32(float *C, const float *A, const float *B,
#                              int M, int N, int K)
# before - a0: A, a1: B, a2: C, a3: M, a4: N, a5: K
# after - a0: C, a1: A, a2: B, a3: M, a4: N, a5: K
hal_matrix_mul_f32:
    mv t0, zero              # i = 0

row_loop_f32:
    bge t0, a3, end_mat_mul_f32  # i < M
    mv t1, zero              # k = 0

k_loop_f32:
    bge t1, a5, next_row_f32     # k < K
    
    # s = A[i][k]
    mul t2, t0, a5
    add t2, t2, t1
    slli t2, t2, 2
    add t2, a1, t2
    flw fa0, 0(t2)
    
    # ptr_B = &B[k][0]
    mul t3, t1, a4
    slli t3, t3, 2
    add t3, a2, t3
    
    # ptr_C = &C[i][0]
    mul t4, t0, a4
    slli t4, t4, 2
    add t4, a0, t4
    
    mv t5, a4                # n_remaining = N

vec_n_loop_f32:
    vsetvli t6, t5, e32, m1, ta, ma
    
    vle32.v v0, (t3)
    vle32.v v1, (t4)
    vfmacc.vf v1, fa0, v0
    vse32.v v1, (t4)
    
    slli t2, t6, 2           # t2 = vl * 4
    add t3, t3, t2
    add t4, t4, t2
    
    sub t5, t5, t6
    bnez t5, vec_n_loop_f32
    
    addi t1, t1, 1           # k++
    j k_loop_f32

next_row_f32:
    addi t0, t0, 1           # i++
    j row_loop_f32

end_mat_mul_f32:
    ret
