#ifndef RVV_MATH_H
#define RVV_MATH_H

#include <stddef.h>
#include <stdint.h>


// 라이브러리 동작 상태 (에러 코드) 정의
typedef enum {
    RVV_MATH_SUCCESS = 0,
    RVV_MATH_ERR_NULL_PTR = -1,
    RVV_MATH_ERR_INVALID_SIZE = -2
} rvv_math_status_t;


// API 선언

/* int32_t / uint32_t */
rvv_math_status_t rvv_math_add_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b, const size_t len);
rvv_math_status_t rvv_math_add_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b, const size_t len);

rvv_math_status_t rvv_math_sub_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b, const size_t len);
rvv_math_status_t rvv_math_sub_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b, const size_t len);

rvv_math_status_t rvv_math_mul_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b, const size_t len);
rvv_math_status_t rvv_math_mul_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b, const size_t len);

rvv_math_status_t rvv_math_mac_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b, const size_t len);
rvv_math_status_t rvv_math_mac_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b, const size_t len);

rvv_math_status_t rvv_math_div_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b, const size_t len);
rvv_math_status_t rvv_math_div_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b, const size_t len);

rvv_math_status_t rvv_math_dot_i32(int32_t *result, const int32_t *src_a, const int32_t *src_b, size_t len);
rvv_math_status_t rvv_math_dot_u32(uint32_t *result, const uint32_t *src_a, const uint32_t *src_b, size_t len);

rvv_math_status_t rvv_matrix_mul_i32(int32_t *C, const int32_t *A, const int32_t *B, size_t M, size_t N, size_t K);
rvv_math_status_t rvv_matrix_mul_u32(uint32_t *C, const uint32_t *A, const uint32_t *B, size_t M, size_t N, size_t K);

/* float 32 */
rvv_math_status_t rvv_math_add_f32(float *dst, const float *src_a, const float *src_b, const size_t len);
rvv_math_status_t rvv_math_sub_f32(float *dst, const float *src_a, const float *src_b, const size_t len);
rvv_math_status_t rvv_math_mul_f32(float *dst, const float *src_a, const float *src_b, const size_t len);
rvv_math_status_t rvv_math_mac_f32(float *dst, const float *src_a, const float *src_b, const size_t len);
rvv_math_status_t rvv_math_div_f32(float *dst, const float *src_a, const float *src_b, const size_t len);

rvv_math_status_t rvv_math_dot_f32(float *result, const float *src_a, const float *src_b, size_t len);

rvv_math_status_t rvv_matrix_mul_f32(float *C, const float *A, const float *B, size_t M, size_t N, size_t K);

#endif // RVV_MATH_H
