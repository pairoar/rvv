.global hal_rvv_add_i32
.global hal_rvv_add_u32

.global hal_rvv_sub_i32
.global hal_rvv_sub_u32

.global hal_rvv_mul_i32
.global hal_rvv_mul_u32

.global hal_rvv_div_i32
.global hal_rvv_div_u32

.global hal_rvv_add_f32
.global hal_rvv_sub_f32
.global hal_rvv_mul_f32
.global hal_rvv_mac_f32

.global hal_rvv_dot_f32

#
# int32_t/uint32_t
#

# void hal_rvv_add_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n)
# void hal_rvv_add_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n)
# a0 = *c, a1 = *a, a2 = *b, a3 = n
#  Vector Add: C[i] = A[i] + B[i]
hal_rvv_add_i32:
hal_rvv_add_u32:
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


# void hal_rvv_add_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n)
# void hal_rvv_add_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n)
# a0 = *c, a1 = *a, a2 = *b, a3 = n
#  Vector Add: C[i] = A[i] + B[i]
hal_rvv_sub_i32:
hal_rvv_sub_u32:
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

# void hal_rvv_mul_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n);
# void hal_rvv_mul_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n);
# Vector Mul: C[i] = A[i] * B[i]
# a0 = *c, a1 = *a, a2 = *b, a3 = n
hal_rvv_mul_i32:
hal_rvv_mul_u32:
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

# void hal_rvv_div_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n);
# void hal_rvv_div_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n);
# Vector Mul: C[i] = A[i] / B[i]
# a0 = *c, a1 = *a, a2 = *b, a3 = n
hal_rvv_div_i32:
hal_rvv_div_u32:
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


#
# float
#
# void hal_rvv_add_f32(float *c, const float *a, const float *b, const size_t n)
# a0 = *c, a1 = *a, a2 = *b, a3 = n
# Vector Add: C[i] = A[i] + B[i]
# vsetvli a4, a3, e32, m1, ta, ma
# e32 : element width
# m1 : Vector register 1 ea
# ta : tail agnostic
# ma : mask agnostic
# 
hal_rvv_add_f32:
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


# void hal_rvv_sub_f32(float *c, const float *a, const float *b, const size_t n)
# Vector Sub: C[i] = A[i] - B[i]
# a0 = *c, a1 = *a, a2 = *b, a3 = n
hal_rvv_sub_f32:
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


# void hal_rvv_mul_f32(float *c, const float *a, const float *b, const size_t n);
# Vector Mul: C[i] = A[i] * B[i]
# a0 = *c, a1 = *a, a2 = *b, a3 = n
hal_rvv_mul_f32:
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

# void hal_rvv_mac_f32(float *c, const float *a, const float *b, const size_t n)
# Vector MAC: C[i] = C[i] + (A[i] * B[i])
# a0 = *c, a1 = *a, a2 = *b, a3 = n
hal_rvv_mac_f32:
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

# void hal_rvv_dot_f32(float *result, const float *a, const float *b, size_t n)
# a0: 결과값을 저장할 스칼라(float) 포인터
# a1: 배열 A 포인터
# a2: 배열 B 포인터
# a3: 배열 길이 (n)
# a0 = *result, a1 = *a, a2 = *b, a3 = n
hal_rvv_dot_f32:
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