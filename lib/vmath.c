/**
 * @file   aermob_lib_hw.c
 * @brief  Library
 * @author Seongjin Oh
 * @date   03-17-2026
 */

/* ======================================================================== */
/* Standard Library Headers                                                 */
/* ======================================================================== */
#include <stdio.h>

/* ======================================================================== */
/* User-Defined Headers                                                     */
/* ======================================================================== */
#include "hal_math.h"
#include "vmath_types.h"

/* ======================================================================== */
/*  Macros (#define)                                                        */
/* ======================================================================== */

/* ======================================================================== */
/* Local Types (enum, struct, typedef)                                      */
/* ======================================================================== */

/* ======================================================================== */
/* Global Variables (Minimize usage recommended)                            */
/* ======================================================================== */

/* ======================================================================== */
/* Static Variables (File-scope global variables)                           */
/* ======================================================================== */

/* ======================================================================== */
/* Static Function Prototypes (Forward declarations)                        */
/* ======================================================================== */

/* ======================================================================== */
/* Public Functions (API Implementations)                                   */
/* ======================================================================== */

// --> new start

/*
    int32_t/uint32_t
*/
// int32_t 용 래퍼
vmath_status_t vmath_add_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len) {
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_add_i32(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

// uint32_t 용 래퍼
vmath_status_t vmath_add_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len) {
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_add_u32(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_sub_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len) {
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_sub_i32(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

// uint32_t 용 래퍼
vmath_status_t vmath_sub_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len) {
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_sub_u32(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_mul_i32(int64_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len) {
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_mul_i32(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_mul_u32(uint64_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len) {
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_mul_u32(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_mac_i32(int64_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len) {
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL) {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0) {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_mac_i32(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_mac_u32(uint64_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len) {
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL) {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0) {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_mac_u32(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_div_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len) {
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;
    int ret_code = 0;
    hal_div_i32(dst, src_a, src_b, len, &ret_code);
    if (ret_code != 0) {
        // 하드웨어에서 나누기 연산 중 오류 발생 (예: 0으로 나누기)
        return VMATH_ERR_HW_FAULT;
    }
    return VMATH_SUCCESS;
}

vmath_status_t vmath_div_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len) {
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;
    int ret_code = 0;
    hal_div_u32(dst, src_a, src_b, len, &ret_code);
    if (ret_code != 0) {
        // 하드웨어에서 나누기 연산 중 오류 발생 (예: 0으로 나누기)
        return VMATH_ERR_HW_FAULT;
    }
    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_dot_i32(int64_t *result, const int32_t *src_a, const int32_t *src_b,
                                    size_t len) {
    if (src_a == NULL || src_b == NULL || result == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 초기 쓰레기값이 있을 수 있으므로 방어적으로 0로 초기화
    *result = 0;

    hal_dot_i32(result, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_dot_u32(uint64_t *result, const uint32_t *src_a, const uint32_t *src_b,
                                    size_t len) {
    if (src_a == NULL || src_b == NULL || result == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 초기 쓰레기값이 있을 수 있으므로 방어적으로 0로 초기화
    *result = 0;

    hal_dot_u32(result, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_i32(int64_t *C, const int32_t *A, const int32_t *B, size_t M,
                                    size_t N, size_t K) {
    if (A == NULL || B == NULL || C == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 C 초기화 (누산을 위해 필요)
    for (size_t i = 0; i < M * N; i++) {
        C[i] = 0;
    }

    hal_matrix_mul_i32(C, A, B, (int)M, (int)N, (int)K);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_u32(uint64_t *C, const uint32_t *A, const uint32_t *B, size_t M,
                                    size_t N, size_t K) {
    if (A == NULL || B == NULL || C == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 C 초기화 (누산을 위해 필요)
    for (size_t i = 0; i < M * N; i++) {
        C[i] = 0;
    }

    hal_matrix_mul_u32(C, A, B, (int)M, (int)N, (int)K);

    return VMATH_SUCCESS;
}

/*
    float
*/
vmath_status_t vmath_add_f32(float *dst, const float *src_a, const float *src_b, const size_t len) {
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL) {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0) {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_add_f32(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_sub_f32(float *dst, const float *src_a, const float *src_b, const size_t len) {
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL) {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0) {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_sub_f32(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_mul_f32(double *dst, const float *src_a, const float *src_b,
                             const size_t len) {
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL) {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0) {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_mul_f32(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_mac_f32(double *dst, const float *src_a, const float *src_b,
                             const size_t len) {
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL) {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0) {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_mac_f32(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_div_f32(float *dst, const float *src_a, const float *src_b, const size_t len) {
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL) {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0) {
        return VMATH_ERR_INVALID_SIZE;
    }
    int ret_code = 0;
    hal_div_f32(dst, src_a, src_b, len, &ret_code);
    if (ret_code != 0) {
        // 하드웨어에서 나누기 연산 중 오류 발생 (예: 0으로 나누기)
        return VMATH_ERR_HW_FAULT;
    }

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_dot_f32(double *result, const float *src_a, const float *src_b,
                                    size_t len) {
    if (src_a == NULL || src_b == NULL || result == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 초기 쓰레기값이 있을 수 있으므로 방어적으로 0.0f로 초기화
    *result = 0.0f;

    hal_dot_f32(result, src_a, src_b, len);

    return VMATH_SUCCESS;
}

// vmath_status_t vmath_matrix_mul_f32(float *C, const float *A, const float *B, int M, int N, int
// K);
vmath_status_t vmath_matrix_mul_f32(double *C, const float *A, const float *B, size_t M, size_t N,
                                    size_t K) {
    if (A == NULL || B == NULL || C == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 C 초기화 (누산을 위해 필요)
    for (size_t i = 0; i < M * N; i++) {
        C[i] = 0.0f;
    }

    hal_matrix_mul_f32(C, A, B, (int)M, (int)N, (int)K);

    return VMATH_SUCCESS;
}

// --> new end

/* ======================================================================== */
/* Static Functions (Internal implementations)                              */
/* ======================================================================== */
