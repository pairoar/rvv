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

void hal_rvv_div_i32(int32_t *c, const int32_t *a, const int32_t *b, size_t n);
void hal_rvv_div_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, size_t n);

void hal_rvv_add_f32(float *c, const float *a, const float *b, size_t n);
void hal_rvv_sub_f32(float *c, const float *a, const float *b, size_t n);
void hal_rvv_mul_f32(float *c, const float *a, const float *b, size_t n);
void hal_rvv_mac_f32(float *c, const float *a, const float *b, size_t n);

void hal_rvv_dot_f32(float *result, const float *a, const float *b, size_t n);

#endif // HAL_RVV_H