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
#include "hal_types.h"

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

// =============================================================================
// Integer
// =============================================================================
// -----------------------------------------------------------------------------
// single-width addition
// -----------------------------------------------------------------------------
hal_status_t hal_vadd_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n);
hal_status_t hal_vadd_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n);

hal_status_t hal_vadd_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n);
hal_status_t hal_vadd_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n);

hal_status_t hal_vadd_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n);
hal_status_t hal_vadd_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n);

hal_status_t hal_vadd_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n);
hal_status_t hal_vadd_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n);

hal_status_t hal_vadd_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n);
hal_status_t hal_vadd_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n);

// -----------------------------------------------------------------------------
// widening addition
// -----------------------------------------------------------------------------
hal_status_t hal_vwadd_i8(int16_t *c, const int8_t *a, const int8_t *b, const size_t n);
hal_status_t hal_vwadd_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, const size_t n);

hal_status_t hal_vwadd_i16(int32_t *c, const int16_t *a, const int16_t *b, const size_t n);
hal_status_t hal_vwadd_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, const size_t n);

hal_status_t hal_vwadd_i32(int64_t *c, const int32_t *a, const int32_t *b, const size_t n);
hal_status_t hal_vwadd_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, const size_t n);

hal_status_t hal_vwadd_i64(int128_t *c, const int64_t *a, const int64_t *b, const size_t n);
hal_status_t hal_vwadd_u64(uint128_t *c, const uint64_t *a, const uint64_t *b, const size_t n);

hal_status_t hal_vwadd_i128(int256_t *c, const int128_t *a, const int128_t *b, const size_t n);
hal_status_t hal_vwadd_u128(uint256_t *c, const uint128_t *a, const uint128_t *b, const size_t n);

// -----------------------------------------------------------------------------
// signle-width subtraction
// -----------------------------------------------------------------------------
hal_status_t hal_vsub_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n);
hal_status_t hal_vsub_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n);

hal_status_t hal_vsub_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n);
hal_status_t hal_vsub_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n);

hal_status_t hal_vsub_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n);
hal_status_t hal_vsub_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n);

hal_status_t hal_vsub_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n);
hal_status_t hal_vsub_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n);

hal_status_t hal_vsub_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n);
hal_status_t hal_vsub_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n);

// -----------------------------------------------------------------------------
// widening multiplication
// -----------------------------------------------------------------------------
hal_status_t hal_vmul_i8(int16_t *c, const int8_t *a, const int8_t *b, const size_t n);
hal_status_t hal_vmul_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, const size_t n);

hal_status_t hal_vmul_i16(int32_t *c, const int16_t *a, const int16_t *b, const size_t n);
hal_status_t hal_vmul_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, const size_t n);

hal_status_t hal_vmul_i32(int64_t *c, const int32_t *a, const int32_t *b, const size_t n);
hal_status_t hal_vmul_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, const size_t n);

hal_status_t hal_vmul_i64(int128_t *c, const int64_t *a, const int64_t *b, const size_t n);
hal_status_t hal_vmul_u64(uint128_t *c, const uint64_t *a, const uint64_t *b, const size_t n);

hal_status_t hal_vmul_i128(int256_t *c, const int128_t *a, const int128_t *b, const size_t n);
hal_status_t hal_vmul_u128(uint256_t *c, const uint128_t *a, const uint128_t *b, const size_t n);

// -----------------------------------------------------------------------------
// widening multiplication and accumulation
// -----------------------------------------------------------------------------
hal_status_t hal_vmac_i8(int16_t *c, const int8_t *a, const int8_t *b, const size_t n);
hal_status_t hal_vmac_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, const size_t n);

hal_status_t hal_vmac_i16(int32_t *c, const int16_t *a, const int16_t *b, const size_t n);
hal_status_t hal_vmac_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, const size_t n);

hal_status_t hal_vmac_i32(int64_t *c, const int32_t *a, const int32_t *b, const size_t n);
hal_status_t hal_vmac_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, const size_t n);

hal_status_t hal_vmac_i64(int128_t *c, const int64_t *a, const int64_t *b, const size_t n);
hal_status_t hal_vmac_u64(uint128_t *c, const uint64_t *a, const uint64_t *b, const size_t n);

hal_status_t hal_vmac_i128(int256_t *c, const int128_t *a, const int128_t *b, const size_t n);
hal_status_t hal_vmac_u128(uint256_t *c, const uint128_t *a, const uint128_t *b, const size_t n);

// -----------------------------------------------------------------------------
// signle-width division
// -----------------------------------------------------------------------------
hal_status_t hal_vdiv_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n);
hal_status_t hal_vdiv_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n);

hal_status_t hal_vdiv_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n);
hal_status_t hal_vdiv_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n);

hal_status_t hal_vdiv_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n);
hal_status_t hal_vdiv_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n);

hal_status_t hal_vdiv_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n);
hal_status_t hal_vdiv_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n);

hal_status_t hal_vdiv_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n);
hal_status_t hal_vdiv_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n);

// -----------------------------------------------------------------------------
// widening dot product
// -----------------------------------------------------------------------------
hal_status_t hal_vdot_i8(int16_t *result, const int8_t *a, const int8_t *b, const size_t n);
hal_status_t hal_vdot_u8(uint16_t *result, const uint8_t *a, const uint8_t *b, const size_t n);

hal_status_t hal_vdot_i16(int32_t *result, const int16_t *a, const int16_t *b, const size_t n);
hal_status_t hal_vdot_u16(uint32_t *result, const uint16_t *a, const uint16_t *b, const size_t n);

hal_status_t hal_vdot_i32(int64_t *result, const int32_t *a, const int32_t *b, const size_t n);
hal_status_t hal_vdot_u32(uint64_t *result, const uint32_t *a, const uint32_t *b, const size_t n);

hal_status_t hal_vdot_i64(int128_t *result, const int64_t *a, const int64_t *b, const size_t n);
hal_status_t hal_vdot_u64(uint128_t *result, const uint64_t *a, const uint64_t *b, const size_t n);

hal_status_t hal_vdot_i128(int256_t *result, const int128_t *a, const int128_t *b, const size_t n);
hal_status_t hal_vdot_u128(uint256_t *result, const uint128_t *a, const uint128_t *b, const size_t n);

// -----------------------------------------------------------------------------
// matrix multiplication
// -----------------------------------------------------------------------------
hal_status_t hal_matrix_vmul_i8(int16_t *c, const int8_t *a, const int8_t *b, const int M, const int N, const int K);
hal_status_t hal_matrix_vmul_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, const int M, const int N, const int K);

hal_status_t hal_matrix_vmul_i16(int32_t *c, const int16_t *a, const int16_t *b, const int M, const int N, const int K);
hal_status_t hal_matrix_vmul_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, const int M, const int N, const int K);

hal_status_t hal_matrix_vmul_i32(int64_t *c, const int32_t *a, const int32_t *b, const int M, const int N, const int K);
hal_status_t hal_matrix_vmul_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, const int M, const int N, const int K);

hal_status_t hal_matrix_vmul_i64(int128_t *c, const int64_t *a, const int64_t *b, const int M, const int N, const int K);
hal_status_t hal_matrix_vmul_u64(uint128_t *c, const uint64_t *a, const uint64_t *b, const int M, const int N, const int K);

hal_status_t hal_matrix_vmul_i128(int256_t *c, const int128_t *a, const int128_t *b, const int M, const int N, const int K);
hal_status_t hal_matrix_vmul_u128(uint256_t *c, const uint128_t *a, const uint128_t *b, const int M, const int N,
                          const int K);

// -----------------------------------------------------------------------------
// min
// -----------------------------------------------------------------------------
hal_status_t hal_vmin_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n);
hal_status_t hal_vmin_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n);
hal_status_t hal_vmin_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n);
hal_status_t hal_vmin_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n);
hal_status_t hal_vmin_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n);
hal_status_t hal_vmin_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n);
hal_status_t hal_vmin_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n);
hal_status_t hal_vmin_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n);
hal_status_t hal_vmin_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n);
hal_status_t hal_vmin_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n);

// -----------------------------------------------------------------------------
// max
// -----------------------------------------------------------------------------
hal_status_t hal_vmax_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n);
hal_status_t hal_vmax_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n);
hal_status_t hal_vmax_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n);
hal_status_t hal_vmax_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n);
hal_status_t hal_vmax_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n);
hal_status_t hal_vmax_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n);
hal_status_t hal_vmax_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n);
hal_status_t hal_vmax_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n);
hal_status_t hal_vmax_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n);
hal_status_t hal_vmax_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n);

// =============================================================================
// Floating point
// =============================================================================
hal_status_t hal_vadd_f32(float *c, const float *a, const float *b, const size_t n);
hal_status_t hal_vsub_f32(float *c, const float *a, const float *b, const size_t n);
hal_status_t hal_vmul_f32(double *c, const float *a, const float *b, const size_t n);
hal_status_t hal_vmac_f32(double *c, const float *a, const float *b, const size_t n);
hal_status_t hal_vdiv_f32(float *c, const float *a, const float *b, const size_t n);
hal_status_t hal_vdot_f32(double *result, const float *a, const float *b, const size_t n);
hal_status_t hal_matrix_vmul_f32(double *c, const float *a, const float *b, const int M, const int N, const int K);
hal_status_t hal_matrix_vmul_c_f32(double *out, const float *A, const float *B, const int M, const int N, const int K);
hal_status_t hal_vmin_f32(float *c, const float *a, const float *b, const size_t n);
hal_status_t hal_vmax_f32(float *c, const float *a, const float *b, const size_t n);
hal_status_t hal_vsqrt_f32(float *b, const float *a, const size_t n);
hal_status_t hal_vrsqrt7_f32(float *b, const float *a, const size_t n);

/* ======================================================================== */
/*    Close C++ Block                                                       */
/* ======================================================================== */

#ifdef __cplusplus
}
#endif

#endif
