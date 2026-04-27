/**
 * @file   hal_math.h
 * @brief  Public HAL APIs.
 * @author Seongjin Oh
 * @date   03-17-2026
 */

#ifndef HAL_MATH_H
#define HAL_MATH_H

// #include "insn_api.h"
#include <stddef.h>
#include <stdint.h>

/* ======================================================================== */
/*    Public Macros & Types                                                 */
/* ======================================================================== */

// 128-bit signed integer type definition
typedef struct {
    int64_t u;
    uint64_t l;
} int128_t;

// 256-bit signed integer type definition
typedef struct {
    uint64_t d[4];
} int256_t;

// 128-bit signed integer type definition
typedef struct {
    uint64_t u;
    uint64_t l;
} uint128_t;

// 256-bit unsigned integer type definition
typedef struct {
    uint64_t d[4]; // d[0]: LSB, d[3]: MSB
} uint256_t;

#define HAL_OP_SUCCESS 0
#define HAL_OP_ERR_DIV_BY_ZERO 1

typedef enum {
    HAL_MATH_SUCCESS = 0,
    HAL_MATH_ERR_DIV_BY_ZERO = (1 << HAL_OP_ERR_DIV_BY_ZERO),
} hal_math_status_t;

/* ======================================================================== */
/*    Inline Functions                                                      */
/* ======================================================================== */

/* ======================================================================== */
/*    C++ Name Mangling Prevention (Crucial for C/C++ interop)              */
/* ======================================================================== */
// Tells the C++ compiler to treat these functions as C-style functions,
// preventing it from altering their names (mangling) during compilation.

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/*    Public Function Prototypes (Exposed APIs)                             */
/* ======================================================================== */

extern void hal_wait_until_done(void);

/*
    addition
*/
/* i8/u8 */
void hal_add_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n);
void hal_add_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n);

/* i16/u16 */
void hal_add_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n);
void hal_add_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n);

/* i32/u32 */
void hal_add_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n);
void hal_add_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n);

/* i64/u64 */
void hal_add_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n);
void hal_add_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n);

/* i128/u128 */
void hal_add_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n);
void hal_add_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n);

/*
    subtraction
*/
/* i8/u8 */
void hal_sub_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n);
void hal_sub_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n);

/* i16/u16 */
void hal_sub_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n);
void hal_sub_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n);

/* i32/u32 */
void hal_sub_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n);
void hal_sub_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n);

/* i64/u64 */
void hal_sub_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n);
void hal_sub_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n);

/* i128/u128 */
void hal_sub_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n);
void hal_sub_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n);

/*
    multiplication
*/
/* i8/u8 */
void hal_mul_i8(int16_t *c, const int8_t *a, const int8_t *b, const size_t n);
void hal_mul_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, const size_t n);

/* i16/u16 */
void hal_mul_i16(int32_t *c, const int16_t *a, const int16_t *b, const size_t n);
void hal_mul_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, const size_t n);

/* i32/u32 */
void hal_mul_i32(int64_t *c, const int32_t *a, const int32_t *b, const size_t n);
void hal_mul_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, const size_t n);

/* i64/u64 */
void hal_mul_i64(int128_t *c, const int64_t *a, const int64_t *b, const size_t n);
void hal_mul_u64(uint128_t *c, const uint64_t *a, const uint64_t *b, const size_t n);

/* i128/u128 */
void hal_mul_i128(int256_t *c, const int128_t *a, const int128_t *b, const size_t n);
void hal_mul_u128(uint256_t *c, const uint128_t *a, const uint128_t *b, const size_t n);

/*
    multiplication and accumulation
*/
/* i8/u8 */
void hal_mac_i8(int16_t *c, const int8_t *a, const int8_t *b, const size_t n);
void hal_mac_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, const size_t n);

/* i16/u16 */
void hal_mac_i16(int32_t *c, const int16_t *a, const int16_t *b, const size_t n);
void hal_mac_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, const size_t n);

/* i32/u32 */
void hal_mac_i32(int64_t *c, const int32_t *a, const int32_t *b, const size_t n);
void hal_mac_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, const size_t n);

/* i64/u64 */
void hal_mac_i64(int128_t *c, const int64_t *a, const int64_t *b, const size_t n);
void hal_mac_u64(uint128_t *c, const uint64_t *a, const uint64_t *b, const size_t n);

/* i128/u128 */
void hal_mac_i128(int256_t *c, const int128_t *a, const int128_t *b, const size_t n);
void hal_mac_u128(uint256_t *c, const uint128_t *a, const uint128_t *b, const size_t n);

/*
    division
*/
/* i8/u8 */
void hal_div_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n, int *ret);
void hal_div_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n, int *ret);

/* i16/u16 */
void hal_div_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n, int *ret);
void hal_div_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n, int *ret);

/* i32/u32 */
void hal_div_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n, int *ret);
void hal_div_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n, int *ret);

/* i64/u64 */
void hal_div_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n, int *ret);
void hal_div_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n, int *ret);

/* i128/u128 */
void hal_div_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n, int *ret);
void hal_div_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n, int *ret);

/*
    dot product
*/
/* i8/u8 */
void hal_dot_i8(int16_t *result, const int8_t *a, const int8_t *b, const size_t n);
void hal_dot_u8(uint16_t *result, const uint8_t *a, const uint8_t *b, const size_t n);

/* i16/u16 */
void hal_dot_i16(int32_t *result, const int16_t *a, const int16_t *b, const size_t n);
void hal_dot_u16(uint32_t *result, const uint16_t *a, const uint16_t *b, const size_t n);

/* i32/u32 */
void hal_dot_i32(int64_t *result, const int32_t *a, const int32_t *b, const size_t n);
void hal_dot_u32(uint64_t *result, const uint32_t *a, const uint32_t *b, const size_t n);

/* i64/u64 */
void hal_dot_i64(int128_t *result, const int64_t *a, const int64_t *b, const size_t n);
void hal_dot_u64(uint128_t *result, const uint64_t *a, const uint64_t *b, const size_t n);

void hal_dot_i128(int256_t *result, const int128_t *a, const int128_t *b, const size_t n);
void hal_dot_u128(uint256_t *result, const uint128_t *a, const uint128_t *b, const size_t n);
/*
    matrix multiplication
*/
/* i8/u8 */
void hal_matrix_mul_i8(int16_t *c, const int8_t *a, const int8_t *b, int M, int N, int K);
void hal_matrix_mul_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, int M, int N, int K);

/* i16/u16 */
void hal_matrix_mul_i16(int32_t *c, const int16_t *a, const int16_t *b, int M, int N, int K);
void hal_matrix_mul_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, int M, int N, int K);

/* i32/u32 */
void hal_matrix_mul_i32(int64_t *c, const int32_t *a, const int32_t *b, int M, int N, int K);
void hal_matrix_mul_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, int M, int N, int K);

/* i64/u64 */
void hal_matrix_mul_i64(int128_t *c, const int64_t *a, const int64_t *b, int M, int N, int K);
void hal_matrix_mul_u64(uint128_t *c, const uint64_t *a, const uint64_t *b, int M, int N, int K);

/* i128/u128 */
void hal_matrix_mul_i128(int256_t *c, const int128_t *a, const int128_t *b, int M, int N, int K);
void hal_matrix_mul_u128(uint256_t *c, const uint128_t *a, const uint128_t *b, int M, int N, int K);

/*
    float 32
*/
/*
    addition
*/
void hal_add_f32(float *c, const float *a, const float *b, const size_t n);
/*
    subtraction
*/
void hal_sub_f32(float *c, const float *a, const float *b, const size_t n);
/*
    multiplication
*/
void hal_mul_f32(double *c, const float *a, const float *b, const size_t n);

/*
    multiplication and accumulation
*/
void hal_mac_f32(double *c, const float *a, const float *b, const size_t n);

/*
    division
*/
void hal_div_f32(float *c, const float *a, const float *b, const size_t n, int *ret);

/*
    dot product
*/
/* * 행렬 곱셈 (Signed)
 * C (M x N) = A (M x K) * B (K x N)
 */
void hal_dot_f32(double *result, const float *a, const float *b, const size_t n);

/*
    matrix multiplication
*/
void hal_matrix_mul_f32(double *c, const float *a, const float *b, int M, int N, int K);

// 순수 C언어 행렬 곱셈 (벤치마크 대조군)
// void hal_matrix_mul_c_f32(double *out, const float *A, const float *B, int M, int N, int K);
void hal_matrix_mul_c_f32(float *out, const float *A, const float *B, int M, int N, int K);
/* ======================================================================== */
/*    Close C++ Block                                                       */
/* ======================================================================== */

#ifdef __cplusplus
}
#endif

#endif // AERMOB_HAL_H
