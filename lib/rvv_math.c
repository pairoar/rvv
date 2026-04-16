#include <stdio.h>
#include "rvv_math.h"
#include "hal_rvv.h" // HAL 인터페이스 포함

/*
    int32_t/uint32_t
*/
// int32_t 용 래퍼
rvv_math_status_t rvv_math_add_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b, const size_t len) {
    if (src_a == NULL || src_b == NULL || dst == NULL) return RVV_MATH_ERR_NULL_PTR;
    if (len == 0) return RVV_MATH_ERR_INVALID_SIZE;
    
    hal_rvv_add_i32(dst, src_a, src_b, len);
    return RVV_MATH_SUCCESS;
}

// uint32_t 용 래퍼
rvv_math_status_t rvv_math_add_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b, const size_t len) {
    if (src_a == NULL || src_b == NULL || dst == NULL) return RVV_MATH_ERR_NULL_PTR;
    if (len == 0) return RVV_MATH_ERR_INVALID_SIZE;
    
    hal_rvv_add_u32(dst, src_a, src_b, len);
    return RVV_MATH_SUCCESS;
}


rvv_math_status_t rvv_math_sub_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b, const size_t len) {
    if (src_a == NULL || src_b == NULL || dst == NULL) return RVV_MATH_ERR_NULL_PTR;
    if (len == 0) return RVV_MATH_ERR_INVALID_SIZE;
    
    hal_rvv_sub_i32(dst, src_a, src_b, len);
    return RVV_MATH_SUCCESS;
}

// uint32_t 용 래퍼
rvv_math_status_t rvv_math_sub_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b, const size_t len) {
    if (src_a == NULL || src_b == NULL || dst == NULL) return RVV_MATH_ERR_NULL_PTR;
    if (len == 0) return RVV_MATH_ERR_INVALID_SIZE;
    
    hal_rvv_sub_u32(dst, src_a, src_b, len);
    return RVV_MATH_SUCCESS;
}

rvv_math_status_t rvv_math_mul_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b, const size_t len) {
    if (src_a == NULL || src_b == NULL || dst == NULL) return RVV_MATH_ERR_NULL_PTR;
    if (len == 0) return RVV_MATH_ERR_INVALID_SIZE;
    
    hal_rvv_mul_i32(dst, src_a, src_b, len);
    return RVV_MATH_SUCCESS;
}

rvv_math_status_t rvv_math_mul_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b, const size_t len) {
    if (src_a == NULL || src_b == NULL || dst == NULL) return RVV_MATH_ERR_NULL_PTR;
    if (len == 0) return RVV_MATH_ERR_INVALID_SIZE;
    
    hal_rvv_mul_u32(dst, src_a, src_b, len);
    return RVV_MATH_SUCCESS;
}

rvv_math_status_t rvv_math_mac_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b, const size_t len) {
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL) {
        return RVV_MATH_ERR_NULL_PTR;
    }
    if (len == 0) {
        return RVV_MATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_rvv_mac_i32(dst, src_a, src_b, len);

    return RVV_MATH_SUCCESS;
}

rvv_math_status_t rvv_math_mac_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b, const size_t len) {
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL) {
        return RVV_MATH_ERR_NULL_PTR;
    }
    if (len == 0) {
        return RVV_MATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_rvv_mac_u32(dst, src_a, src_b, len);

    return RVV_MATH_SUCCESS;
}

rvv_math_status_t rvv_math_div_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b, const size_t len) {
    if (src_a == NULL || src_b == NULL || dst == NULL) return RVV_MATH_ERR_NULL_PTR;
    if (len == 0) return RVV_MATH_ERR_INVALID_SIZE;
    
    hal_rvv_div_i32(dst, src_a, src_b, len);
    return RVV_MATH_SUCCESS;
}

rvv_math_status_t rvv_math_div_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b, const size_t len) {
    if (src_a == NULL || src_b == NULL || dst == NULL) return RVV_MATH_ERR_NULL_PTR;
    if (len == 0) return RVV_MATH_ERR_INVALID_SIZE;
    
    hal_rvv_div_u32(dst, src_a, src_b, len);
    return RVV_MATH_SUCCESS;
}

rvv_math_status_t rvv_math_dot_i32(int32_t *result, const int32_t *src_a, const int32_t *src_b, size_t len) {
    if (src_a == NULL || src_b == NULL || result == NULL) return RVV_MATH_ERR_NULL_PTR;
    if (len == 0) return RVV_MATH_ERR_INVALID_SIZE;
    
    // 초기 쓰레기값이 있을 수 있으므로 방어적으로 0로 초기화
    *result = 0;
    
    hal_rvv_dot_i32(result, src_a, src_b, len);
    
    return RVV_MATH_SUCCESS;
}

rvv_math_status_t rvv_math_dot_u32(uint32_t *result, const uint32_t *src_a, const uint32_t *src_b, size_t len) {
    if (src_a == NULL || src_b == NULL || result == NULL) return RVV_MATH_ERR_NULL_PTR;
    if (len == 0) return RVV_MATH_ERR_INVALID_SIZE;
    
    // 초기 쓰레기값이 있을 수 있으므로 방어적으로 0로 초기화
    *result = 0;
    
    hal_rvv_dot_u32(result, src_a, src_b, len);
    
    return RVV_MATH_SUCCESS;
}

rvv_math_status_t rvv_matrix_mul_i32(int32_t *C, const int32_t *A, const int32_t *B,
                                       size_t M, size_t N, size_t K) {
    if (A == NULL || B == NULL || C == NULL) return RVV_MATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0) return RVV_MATH_ERR_INVALID_SIZE;

    // 결과 행렬 C 초기화 (누산을 위해 필요)
    for (size_t i = 0; i < M * N; i++) {
        C[i] = 0;
    }

    hal_rvv_matrix_mul_i32(C, A, B, (int)M, (int)N, (int)K);
    
    return RVV_MATH_SUCCESS;
}

rvv_math_status_t rvv_matrix_mul_u32(uint32_t *C, const uint32_t *A, const uint32_t *B,
                                       size_t M, size_t N, size_t K) {
    if (A == NULL || B == NULL || C == NULL) return RVV_MATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0) return RVV_MATH_ERR_INVALID_SIZE;

    // 결과 행렬 C 초기화 (누산을 위해 필요)
    for (size_t i = 0; i < M * N; i++) {
        C[i] = 0;
    }

    hal_rvv_matrix_mul_u32(C, A, B, (int)M, (int)N, (int)K);
    
    return RVV_MATH_SUCCESS;
}

/*
    float
*/
rvv_math_status_t rvv_math_add_f32(float *dst, const float *src_a, const float *src_b, const size_t len) {
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL) {
        return RVV_MATH_ERR_NULL_PTR;
    }
    if (len == 0) {
        return RVV_MATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_rvv_add_f32(dst, src_a, src_b, len);

    return RVV_MATH_SUCCESS;
}

rvv_math_status_t rvv_math_sub_f32(float *dst, const float *src_a, const float *src_b, const size_t len) {
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL) {
        return RVV_MATH_ERR_NULL_PTR;
    }
    if (len == 0) {
        return RVV_MATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_rvv_sub_f32(dst, src_a, src_b, len);

    return RVV_MATH_SUCCESS;
}


rvv_math_status_t rvv_math_mul_f32(float *dst, const float *src_a, const float *src_b, const size_t len) {
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL) {
        return RVV_MATH_ERR_NULL_PTR;
    }
    if (len == 0) {
        return RVV_MATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_rvv_mul_f32(dst, src_a, src_b, len);

    return RVV_MATH_SUCCESS;
}

rvv_math_status_t rvv_math_mac_f32(float *dst, const float *src_a, const float *src_b, const size_t len) {
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL) {
        return RVV_MATH_ERR_NULL_PTR;
    }
    if (len == 0) {
        return RVV_MATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_rvv_mac_f32(dst, src_a, src_b, len);

    return RVV_MATH_SUCCESS;
}

rvv_math_status_t rvv_math_div_f32(float *dst, const float *src_a, const float *src_b, const size_t len) {
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL) {
        return RVV_MATH_ERR_NULL_PTR;
    }
    if (len == 0) {
        return RVV_MATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_rvv_div_f32(dst, src_a, src_b, len);

    return RVV_MATH_SUCCESS;
}

rvv_math_status_t rvv_math_dot_f32(float *result, const float *src_a, const float *src_b, size_t len) {
    if (src_a == NULL || src_b == NULL || result == NULL) return RVV_MATH_ERR_NULL_PTR;
    if (len == 0) return RVV_MATH_ERR_INVALID_SIZE;
    
    // 초기 쓰레기값이 있을 수 있으므로 방어적으로 0.0f로 초기화
    *result = 0.0f;
    
    hal_rvv_dot_f32(result, src_a, src_b, len);
    
    return RVV_MATH_SUCCESS;
}

// rvv_math_status_t rvv_matrix_mul_f32(float *C, const float *A, const float *B, int M, int N, int K);
rvv_math_status_t rvv_matrix_mul_f32(float *C, const float *A, const float *B,
                                       size_t M, size_t N, size_t K) {
    if (A == NULL || B == NULL || C == NULL) return RVV_MATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0) return RVV_MATH_ERR_INVALID_SIZE;

    // 결과 행렬 C 초기화 (누산을 위해 필요)
    for (size_t i = 0; i < M * N; i++) {
        C[i] = 0.0f;
    }

    hal_rvv_matrix_mul_f32(C, A, B, (int)M, (int)N, (int)K);
    
    return RVV_MATH_SUCCESS;
}
