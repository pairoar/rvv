/**
 * @file   vmath.h
 * @brief  Public Library APIs
 * @author Seongjin Oh
 * @date   03-17-2026
 */

#ifndef VMATH_H
#define VMATH_H

#include "hal_math.h"
#include "vmath_types.h"
#include <stdbool.h>
#include <stdint.h>

/* ======================================================================== */
/*    Public Macros & Types                                                 */
/* ======================================================================== */

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

// --> new start

/* addition */
vmath_status_t vmath_add_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t len);
vmath_status_t vmath_add_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t len);

vmath_status_t vmath_add_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t len);
vmath_status_t vmath_add_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t len);

vmath_status_t vmath_add_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len);
vmath_status_t vmath_add_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len);

vmath_status_t vmath_add_i64(int64_t *dst, const int64_t *src_a, const int64_t *src_b,
                             const size_t len);
vmath_status_t vmath_add_u64(uint64_t *dst, const uint64_t *src_a, const uint64_t *src_b,
                             const size_t len);

                vmath_status_t vmath_add_i128(int128_t *dst, const int128_t *src_a, const int128_t *src_b,
                             const size_t len);
vmath_status_t vmath_add_u128(uint128_t *dst, const uint128_t *src_a, const uint128_t *src_b,
                             const size_t len);

/* subtraction */
vmath_status_t vmath_sub_i8(int8_t *dst, const int8_t *src_a, const int8_t *src_b,
                             const size_t len) ;
vmath_status_t vmath_sub_u8(uint8_t *dst, const uint8_t *src_a, const uint8_t *src_b,
                             const size_t len);

vmath_status_t vmath_sub_i16(int16_t *dst, const int16_t *src_a, const int16_t *src_b,
                             const size_t len);
vmath_status_t vmath_sub_u16(uint16_t *dst, const uint16_t *src_a, const uint16_t *src_b,
                             const size_t len);

vmath_status_t vmath_sub_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len);
vmath_status_t vmath_sub_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len);

vmath_status_t vmath_sub_i64(int64_t *dst, const int64_t *src_a, const int64_t *src_b,
                             const size_t len);
vmath_status_t vmath_sub_u64(uint64_t *dst, const uint64_t *src_a, const uint64_t *src_b,
                             const size_t len);

vmath_status_t vmath_sub_i128(int128_t *dst, const int128_t *src_a, const int128_t *src_b,
                             const size_t len);
vmath_status_t vmath_sub_u128(uint128_t *dst, const uint128_t *src_a, const uint128_t *src_b,
                             const size_t len);

/* multiplication */
vmath_status_t vmath_mul_i8(int16_t *dst, const int8_t *src_a, const int8_t *src_b,
                             const size_t len);
vmath_status_t vmath_mul_u8(uint16_t *dst, const uint8_t *src_a, const uint8_t *src_b,
                             const size_t len);

vmath_status_t vmath_mul_i16(int32_t *dst, const int16_t *src_a, const int16_t *src_b,
                             const size_t len);
vmath_status_t vmath_mul_u16(uint32_t *dst, const uint16_t *src_a, const uint16_t *src_b,
                             const size_t len);

vmath_status_t vmath_mul_i32(int64_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len);
vmath_status_t vmath_mul_u32(uint64_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len);

vmath_status_t vmath_mul_i64(int128_t *dst, const int64_t *src_a, const int64_t *src_b,
                             const size_t len);
vmath_status_t vmath_mul_u64(uint128_t *dst, const uint64_t *src_a, const uint64_t *src_b,
                             const size_t len);

vmath_status_t vmath_mul_i128(int256_t *dst, const int128_t *src_a, const int128_t *src_b,
                             const size_t len);
vmath_status_t vmath_mul_u128(uint256_t *dst, const uint128_t *src_a, const uint128_t *src_b,
                             const size_t len);
/* mac */
vmath_status_t vmath_mac_i8(int16_t *dst, const int8_t *src_a, const int8_t *src_b,
                             const size_t len);
vmath_status_t vmath_mac_u8(uint16_t *dst, const uint8_t *src_a, const uint8_t *src_b,
                             const size_t len);

vmath_status_t vmath_mac_i16(int32_t *dst, const int16_t *src_a, const int16_t *src_b,
                             const size_t len);
vmath_status_t vmath_mac_u16(uint32_t *dst, const uint16_t *src_a, const uint16_t *src_b,
                             const size_t len);

vmath_status_t vmath_mac_i32(int64_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len);
vmath_status_t vmath_mac_u32(uint64_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len);

vmath_status_t vmath_mac_i64(int128_t *dst, const int64_t *src_a, const int64_t *src_b,
                             const size_t len);
vmath_status_t vmath_mac_u64(uint128_t *dst, const uint64_t *src_a, const uint64_t *src_b,
                             const size_t len);

vmath_status_t vmath_mac_i128(int256_t *dst, const int128_t *src_a, const int128_t *src_b,
                             const size_t len);
vmath_status_t vmath_mac_u128(uint256_t *dst, const uint128_t *src_a, const uint128_t *src_b,
                             const size_t len);

/* div */
vmath_status_t vmath_div_i8(int8_t *dst, const int8_t *src_a, const int8_t *src_b,
                             const size_t len);
vmath_status_t vmath_div_u8(uint8_t *dst, const uint8_t *src_a, const uint8_t *src_b,
                             const size_t len);

vmath_status_t vmath_div_i16(int16_t *dst, const int16_t *src_a, const int16_t *src_b,
                             const size_t len);
vmath_status_t vmath_div_u16(uint16_t *dst, const uint16_t *src_a, const uint16_t *src_b,
                             const size_t len);

vmath_status_t vmath_div_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len);
vmath_status_t vmath_div_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len);

vmath_status_t vmath_div_i64(int64_t *dst, const int64_t *src_a, const int64_t *src_b,
                             const size_t len);
vmath_status_t vmath_div_u64(uint64_t *dst, const uint64_t *src_a, const uint64_t *src_b,
                             const size_t len);

vmath_status_t vmath_div_i128(int128_t *dst, const int128_t *src_a, const int128_t *src_b,
                             const size_t len);
vmath_status_t vmath_div_u128(uint128_t *dst, const uint128_t *src_a, const uint128_t *src_b,
                             const size_t len);

/* dot product */
vmath_status_t vmath_matrix_dot_i8(int16_t *result, const int8_t *src_a, const int8_t *src_b,
                                    size_t len);
vmath_status_t vmath_matrix_dot_u8(uint16_t *result, const uint8_t *src_a, const uint8_t *src_b,
                                    size_t len);

vmath_status_t vmath_matrix_dot_i16(int32_t *result, const int16_t *src_a, const int16_t *src_b,
                                    size_t len);
vmath_status_t vmath_matrix_dot_u16(uint32_t *result, const uint16_t *src_a, const uint16_t *src_b,
                                    size_t len);

vmath_status_t vmath_matrix_dot_i32(int64_t *result, const int32_t *src_a, const int32_t *src_b,
                                    size_t len);
vmath_status_t vmath_matrix_dot_u32(uint64_t *result, const uint32_t *src_a, const uint32_t *src_b,
                                    size_t len);

vmath_status_t vmath_matrix_dot_i64(int128_t *result, const int64_t *src_a, const int64_t *src_b,
                                    size_t len);
vmath_status_t vmath_matrix_dot_u64(uint128_t *result, const uint64_t *src_a, const uint64_t *src_b,
                                    size_t len);

vmath_status_t vmath_matrix_dot_i128(int256_t *result, const int128_t *src_a, const int128_t *src_b,
                                    size_t len);
vmath_status_t vmath_matrix_dot_u128(uint256_t *result, const uint128_t *src_a, const uint128_t *src_b,
                                    size_t len);

/* matrix multiplication */
vmath_status_t vmath_matrix_mul_i8(int16_t *C, const int8_t *A, const int8_t *B, size_t M,
                                    size_t N, size_t K);
vmath_status_t vmath_matrix_mul_u8(uint16_t *C, const uint8_t *A, const uint8_t *B, size_t M,
                                    size_t N, size_t K);

vmath_status_t vmath_matrix_mul_i16(int32_t *C, const int16_t *A, const int16_t *B, size_t M,
                                    size_t N, size_t K);
vmath_status_t vmath_matrix_mul_u16(uint32_t *C, const uint16_t *A, const uint16_t *B, size_t M,
                                    size_t N, size_t K);

vmath_status_t vmath_matrix_mul_i32(int64_t *C, const int32_t *A, const int32_t *B, size_t M,
                                    size_t N, size_t K);
vmath_status_t vmath_matrix_mul_u32(uint64_t *C, const uint32_t *A, const uint32_t *B, size_t M,
                                    size_t N, size_t K);

vmath_status_t vmath_matrix_mul_i64(int128_t *C, const int64_t *A, const int64_t *B, size_t M,
                                    size_t N, size_t K);
vmath_status_t vmath_matrix_mul_u64(uint128_t *C, const uint64_t *A, const uint64_t *B, size_t M,
                                    size_t N, size_t K);

vmath_status_t vmath_matrix_mul_i128(int256_t *C, const int128_t *A, const int128_t *B, size_t M,
                                    size_t N, size_t K);
vmath_status_t vmath_matrix_mul_u128(uint256_t *C, const uint128_t *A, const uint128_t *B, size_t M,
                                    size_t N, size_t K);
/* float 32 */
vmath_status_t vmath_add_f32(float *dst, const float *src_a, const float *src_b, const size_t len);
vmath_status_t vmath_sub_f32(float *dst, const float *src_a, const float *src_b, const size_t len);
vmath_status_t vmath_mul_f32(double *dst, const float *src_a, const float *src_b, const size_t len);
vmath_status_t vmath_mac_f32(double *dst, const float *src_a, const float *src_b, const size_t len);
vmath_status_t vmath_div_f32(float *dst, const float *src_a, const float *src_b, const size_t len);

vmath_status_t vmath_matrix_dot_f32(double *result, const float *src_a, const float *src_b,
                                    size_t len);

vmath_status_t vmath_matrix_mul_f32(double *C, const float *A, const float *B, size_t M, size_t N,
                                    size_t K);

// <-- new end

/* ======================================================================== */
/*    Close C++ Block                                                       */
/* ======================================================================== */
#ifdef __cplusplus
}
#endif

#endif
