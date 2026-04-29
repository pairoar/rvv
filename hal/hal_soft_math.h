#ifndef HAL_SOFT_MATH_H
#define HAL_SOFT_MATH_H

#include "hal_math.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Tiled Matrix Multiplication (Signed) */
void hal_matrix_vmul_tiled_i8(int16_t *c, const int8_t *a, const int8_t *b, int M, int N, int K,
                             int tile_size);
void hal_matrix_vmul_tiled_i16(int32_t *c, const int16_t *a, const int16_t *b, int M, int N, int K,
                              int tile_size);
void hal_matrix_vmul_tiled_i32(int64_t *c, const int32_t *a, const int32_t *b, int M, int N, int K,
                              int tile_size);
void hal_matrix_vmul_tiled_i64(int128_t *c, const int64_t *a, const int64_t *b, int M, int N, int
K,
                              int tile_size);
void hal_matrix_vmul_tiled_i128(int256_t *c, const int128_t *a, const int128_t *b, int M, int N,
                               int K, int tile_size);

/* Tiled Matrix Multiplication (Unsigned) */
void hal_matrix_vmul_tiled_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, int M, int N, int K,
                             int tile_size);
void hal_matrix_vmul_tiled_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, int M, int N,
                              int K, int tile_size);
void hal_matrix_vmul_tiled_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, int M, int N,
                              int K, int tile_size);
void hal_matrix_vmul_tiled_u64(uint128_t *c, const uint64_t *a, const uint64_t *b, int M, int N,
                              int K, int tile_size);
void hal_matrix_vmul_tiled_u128(uint256_t *c, const uint128_t *a, const uint128_t *b, int M, int
N,
                               int K, int tile_size);

/* Tiled Matrix Multiplication (Float) */
void hal_matrix_vmul_tiled_f32(double *c, const float *a, const float *b, int M, int N, int K,
                              int tile_size);
// void hal_matrix_vmul_tiled_f32(float *c, const float *a, const float *b, int M, int N, int K,
//                               int tile_size);

#ifdef __cplusplus
}
#endif

#endif // HAL_SOFT_MATH_H
