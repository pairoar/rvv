#ifndef HAL_RVV_H
#define HAL_RVV_H

#include <stdint.h>

typedef size_t hal_size_t; 

void hal_rvv_add_i32(int32_t *c, const int32_t *a, const int32_t *b, size_t n);
void hal_rvv_add_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, size_t n);

void hal_rvv_sub_i32(int32_t *c, const int32_t *a, const int32_t *b, size_t n);
void hal_rvv_sub_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, size_t n);

void hal_rvv_mul_i32(int32_t *c, const int32_t *a, const int32_t *b, size_t n);
void hal_rvv_mul_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, size_t n);

void hal_rvv_mac_i32(int32_t *c, const int32_t *a, const int32_t *b, size_t n);
void hal_rvv_mac_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, size_t n);

void hal_rvv_dot_i32(int32_t *result, const int32_t *a, const int32_t *b, size_t n);
void hal_rvv_dot_u32(uint32_t *result, const uint32_t *a, const uint32_t *b, size_t n);

void hal_rvv_matrix_mul_i32(int32_t *C, const int32_t *A, const int32_t *B, int M, int N, int K);
void hal_rvv_matrix_mul_u32(uint32_t *C, const uint32_t *A, const uint32_t *B, int M, int N, int K);

void hal_rvv_div_i32(int32_t *c, const int32_t *a, const int32_t *b, size_t n);
void hal_rvv_div_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, size_t n);

void hal_rvv_add_f32(float *c, const float *a, const float *b, size_t n);
void hal_rvv_sub_f32(float *c, const float *a, const float *b, size_t n);
void hal_rvv_mul_f32(float *c, const float *a, const float *b, size_t n);
void hal_rvv_mac_f32(float *c, const float *a, const float *b, size_t n);
void hal_rvv_div_f32(float *c, const float *a, const float *b, size_t n);

void hal_rvv_dot_f32(float *result, const float *a, const float *b, size_t n);
void hal_rvv_matrix_mul_f32(float *C, const float *A, const float *B, int M, int N, int K);

#endif // HAL_RVV_H
