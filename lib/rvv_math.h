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
rvv_math_status_t rvv_math_add_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b, const size_t len);
rvv_math_status_t rvv_math_add_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b, const size_t len);

rvv_math_status_t rvv_math_sub_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b, const size_t len);
rvv_math_status_t rvv_math_sub_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b, const size_t len);

rvv_math_status_t rvv_math_mul_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b, const size_t len);
rvv_math_status_t rvv_math_mul_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b, const size_t len);

rvv_math_status_t rvv_math_div_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b, const size_t len);
rvv_math_status_t rvv_math_div_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b, const size_t len);

rvv_math_status_t rvv_math_add_f32(float *dst, const float *src_a, const float *src_b, const size_t len);
rvv_math_status_t rvv_math_sub_f32(float *dst, const float *src_a, const float *src_b, const size_t len);
rvv_math_status_t rvv_math_mul_f32(float *dst, const float *src_a, const float *src_b, const size_t len);
rvv_math_status_t rvv_math_mac_f32(float *dst, const float *src_a, const float *src_b, const size_t len);

// 결과가 배열이 아닌 단일 스칼라 값이므로 float *dst 대신 float *result 를 사용합니다.
rvv_math_status_t rvv_math_dot_f32(float *result, const float *src_a, const float *src_b, size_t len);


#endif // RVV_MATH_H