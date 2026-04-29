/**
 * @file   hal_math.h
 * @brief  Public HAL APIs.
 * @author Seongjin Oh
 * @date   03-17-2026
 */

#ifndef HAL_MATH_H
#define HAL_MATH_H

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

/*
    addition
*/
/* i8/u8 */
void hal_vadd_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n);
void hal_vadd_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n);

/* i16/u16 */
void hal_vadd_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n);
void hal_vadd_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n);

/* i32/u32 */
void hal_vadd_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n);
void hal_vadd_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n);

/* i64/u64 */
void hal_vadd_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n);
void hal_vadd_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n);

/* i128/u128 */
void hal_vadd_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n);
void hal_vadd_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n);

/*
    subtraction
*/
/* i8/u8 */
void hal_vsub_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n);
void hal_vsub_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n);

/* i16/u16 */
void hal_vsub_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n);
void hal_vsub_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n);

/* i32/u32 */
void hal_vsub_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n);
void hal_vsub_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n);

/* i64/u64 */
void hal_vsub_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n);
void hal_vsub_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n);

/* i128/u128 */
void hal_vsub_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n);
void hal_vsub_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n);

/*
    multiplication
*/
/* i8/u8 */
void hal_vmul_i8(int16_t *c, const int8_t *a, const int8_t *b, const size_t n);
void hal_vmul_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, const size_t n);

/* i16/u16 */
void hal_vmul_i16(int32_t *c, const int16_t *a, const int16_t *b, const size_t n);
void hal_vmul_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, const size_t n);

/* i32/u32 */
void hal_vmul_i32(int64_t *c, const int32_t *a, const int32_t *b, const size_t n);
void hal_vmul_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, const size_t n);

/* i64/u64 */
void hal_vmul_i64(int128_t *c, const int64_t *a, const int64_t *b, const size_t n);
void hal_vmul_u64(uint128_t *c, const uint64_t *a, const uint64_t *b, const size_t n);

/* i128/u128 */
void hal_vmul_i128(int256_t *c, const int128_t *a, const int128_t *b, const size_t n);
void hal_vmul_u128(uint256_t *c, const uint128_t *a, const uint128_t *b, const size_t n);

/*
    multiplication and accumulation
*/
/* i8/u8 */
void hal_vmac_i8(int16_t *c, const int8_t *a, const int8_t *b, const size_t n);
void hal_vmac_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, const size_t n);

/* i16/u16 */
void hal_vmac_i16(int32_t *c, const int16_t *a, const int16_t *b, const size_t n);
void hal_vmac_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, const size_t n);

/* i32/u32 */
void hal_vmac_i32(int64_t *c, const int32_t *a, const int32_t *b, const size_t n);
void hal_vmac_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, const size_t n);

/* i64/u64 */
void hal_vmac_i64(int128_t *c, const int64_t *a, const int64_t *b, const size_t n);
void hal_vmac_u64(uint128_t *c, const uint64_t *a, const uint64_t *b, const size_t n);

/* i128/u128 */
void hal_vmac_i128(int256_t *c, const int128_t *a, const int128_t *b, const size_t n);
void hal_vmac_u128(uint256_t *c, const uint128_t *a, const uint128_t *b, const size_t n);

/*
    division
*/
/* i8/u8 */
void hal_vdiv_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n, int *ret);
void hal_vdiv_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n, int *ret);

/* i16/u16 */
void hal_vdiv_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n, int *ret);
void hal_vdiv_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n, int *ret);

/* i32/u32 */
void hal_vdiv_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n, int *ret);
void hal_vdiv_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n, int *ret);

/* i64/u64 */
void hal_vdiv_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n, int *ret);
void hal_vdiv_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n, int *ret);

/* i128/u128 */
void hal_vdiv_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n, int *ret);
void hal_vdiv_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n, int *ret);

/*
    dot product
*/
/* i8/u8 */
void hal_vdot_i8(int16_t *result, const int8_t *a, const int8_t *b, const size_t n);
void hal_vdot_u8(uint16_t *result, const uint8_t *a, const uint8_t *b, const size_t n);

/* i16/u16 */
void hal_vdot_i16(int32_t *result, const int16_t *a, const int16_t *b, const size_t n);
void hal_vdot_u16(uint32_t *result, const uint16_t *a, const uint16_t *b, const size_t n);

/* i32/u32 */
void hal_vdot_i32(int64_t *result, const int32_t *a, const int32_t *b, const size_t n);
void hal_vdot_u32(uint64_t *result, const uint32_t *a, const uint32_t *b, const size_t n);

/* i64/u64 */
void hal_vdot_i64(int128_t *result, const int64_t *a, const int64_t *b, const size_t n);
void hal_vdot_u64(uint128_t *result, const uint64_t *a, const uint64_t *b, const size_t n);

void hal_vdot_i128(int256_t *result, const int128_t *a, const int128_t *b, const size_t n);
void hal_vdot_u128(uint256_t *result, const uint128_t *a, const uint128_t *b, const size_t n);
/*
    matrix multiplication
*/
/* i8/u8 */
void hal_matrix_vmul_i8(int16_t *c, const int8_t *a, const int8_t *b, int M, int N, int K);
void hal_matrix_vmul_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, int M, int N, int K);

/* i16/u16 */
void hal_matrix_vmul_i16(int32_t *c, const int16_t *a, const int16_t *b, int M, int N, int K);
void hal_matrix_vmul_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, int M, int N, int K);

/* i32/u32 */
void hal_matrix_vmul_i32(int64_t *c, const int32_t *a, const int32_t *b, int M, int N, int K);
void hal_matrix_vmul_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, int M, int N, int K);

/* i64/u64 */
void hal_matrix_vmul_i64(int128_t *c, const int64_t *a, const int64_t *b, int M, int N, int K);
void hal_matrix_vmul_u64(uint128_t *c, const uint64_t *a, const uint64_t *b, int M, int N, int K);

/* i128/u128 */
void hal_matrix_vmul_i128(int256_t *c, const int128_t *a, const int128_t *b, int M, int N, int K);
void hal_matrix_vmul_u128(uint256_t *c, const uint128_t *a, const uint128_t *b, int M, int N,
                          int K);

/*
    float 32
*/
/*
    addition
*/
void hal_vadd_f32(float *c, const float *a, const float *b, const size_t n);
/*
    subtraction
*/
void hal_vsub_f32(float *c, const float *a, const float *b, const size_t n);
/*
    multiplication
*/
void hal_vmul_f32(double *c, const float *a, const float *b, const size_t n);

/*
    multiplication and accumulation
*/
void hal_vmac_f32(double *c, const float *a, const float *b, const size_t n);

/*
    division
*/
void hal_vdiv_f32(float *c, const float *a, const float *b, const size_t n, int *ret);

/*
    dot product
*/
/* * Matrix Multiplication (Signed)
 * C (M x N) = A (M x K) * B (K x N)
 */
void hal_vdot_f32(double *result, const float *a, const float *b, const size_t n);

/*
    matrix multiplication
*/
void hal_matrix_vmul_f32(double *c, const float *a, const float *b, int M, int N, int K);

// pure C language Multiplication(Benchmark Control Group)
void hal_matrix_vmul_c_f32(double *out, const float *A, const float *B, int M, int N, int K);
// void hal_matrix_vmul_c_f32(float *out, const float *A, const float *B, int M, int N, int K);
/* ======================================================================== */
/*    Close C++ Block                                                       */
/* ======================================================================== */

#ifdef __cplusplus
}
#endif

#endif
