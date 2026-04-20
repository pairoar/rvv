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

/* int32_t / uint32_t */
vmath_status_t vmath_add_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len);
vmath_status_t vmath_add_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len);

vmath_status_t vmath_sub_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len);
vmath_status_t vmath_sub_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len);

vmath_status_t vmath_mul_i32(int64_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len);
vmath_status_t vmath_mul_u32(uint64_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len);

vmath_status_t vmath_mac_i32(int64_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len);
vmath_status_t vmath_mac_u32(uint64_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len);

vmath_status_t vmath_div_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len);
vmath_status_t vmath_div_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len);

vmath_status_t vmath_matrix_dot_i32(int64_t *result, const int32_t *src_a, const int32_t *src_b,
                                    size_t len);
vmath_status_t vmath_matrix_dot_u32(uint64_t *result, const uint32_t *src_a, const uint32_t *src_b,
                                    size_t len);

vmath_status_t vmath_matrix_mul_i32(int64_t *C, const int32_t *A, const int32_t *B, size_t M,
                                    size_t N, size_t K);
vmath_status_t vmath_matrix_mul_u32(uint64_t *C, const uint32_t *A, const uint32_t *B, size_t M,
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
