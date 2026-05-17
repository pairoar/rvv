#include "hal_internal_math.h"
#include <stddef.h>
#include <string.h>
#include <math.h>


// -----------------------------------------------------------------------------
// General Function type
//     add, sub, multiplication, division, dot prodcut, mac, matrix multiplication
//     min, max, ...
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Type 1
//      1 input, 1 output
//      min, max, sqrt, ...
// -----------------------------------------------------------------------------

/*
    OP_N    : operation name
    OP_T    : operation type
    OP_FUNC : function name
    T_IN    : input type
    T_OUT   : output type(result)
    n       : length
    b       : output array
    a       : input array
*/
#define DEFINE_HAL_GENERAL_OPS1(OP_N, OP_T, OP_FUNC, T_IN, T_OUT)                   \
    hal_status_t hal_##OP_N##_##OP_T(T_OUT *b, const T_IN *a, const size_t n) {     \
        for (size_t i = 0; i < n; i++) {                                            \
            b[i] = OP_FUNC(a[i]);                                                   \
        }                                                                           \
        return HAL_OK;                                                              \
    }

// -----------------------------------------------------------------------------
// Type 2
//     2 input, 1 output
//     add, sub, multiplication
// -----------------------------------------------------------------------------
/*
    OP_N    : operation name
    OP_T    : operation type
    OP_FUNC : function name
    T_IN    : input type
    T_OUT   : output type(result)
    n       : length
    c       : output array
    a       : input array1
    b       : input array2
*/

#define DEFINE_HAL_GENERAL_OPS2(OP_N, OP_T, OP_FUNC, T_IN, T_OUT)                   \
    hal_status_t hal_##OP_N##_##OP_T(T_OUT *c, const T_IN *a, const T_IN *b, const size_t n) { \
        for (size_t i = 0; i < n; i++) {                                            \
            c[i] = OP_FUNC(a[i], b[i]);                                             \
        }                                                                           \
        return HAL_OK;                                                              \
    }

// -----------------------------------------------------------------------------
// Type 3
//     2 input, 1 output
//     division
// -----------------------------------------------------------------------------
/*
    OP_N    : operation name
    OP_T    : operation type
    OP_FUNC : function name
    T_IN    : input type
    T_OUT   : output type(result)
    n       : length
    c       : output array
    a       : input array1
    b       : input array2
*/

#define DEFINE_HAL_GENERAL_OPS3(OP_N, OP_T, OP_FUNC, T_IN, T_OUT)                   \
    hal_status_t hal_##OP_N##_##OP_T(T_OUT *c, const T_IN *a, const T_IN *b, const size_t n) { \
        hal_status_t ret = HAL_OK;                                                  \
        for (size_t i = 0; i < n; i++) {                                            \
            if (b[i] == 0) {                                                        \
                c[i] = 0;                                                           \
                ret |= HAL_ERR_DIV_BY_ZERO;                                         \
                continue;                                                           \
            }                                                                       \
            c[i] = OP_FUNC(a[i], b[i]);                                             \
        }                                                                           \
        return ret;                                                                 \
    }

// -----------------------------------------------------------------------------
// Type 4
//     2 input, 1 output
//     mac
//     OF_FUNC requires 3 arguments
// -----------------------------------------------------------------------------
/*
    OP_N    : operation name
    OP_T    : operation type
    OP_FUNC : function name
    T_IN    : input type
    T_OUT   : output type(result)
    n       : length
    c       : output array
    a       : input array1
    b       : input array2
*/

#define DEFINE_HAL_GENERAL_OPS4(OP_N, OP_T, OP_FUNC, T_IN, T_OUT)                   \
    hal_status_t hal_##OP_N##_##OP_T(T_OUT *c, const T_IN *a, const T_IN *b, const size_t n) { \
        for (size_t i = 0; i < n; i++) {                                            \
            OP_FUNC(c[i], a[i], b[i]);                                              \
        }                                                                           \
        return HAL_OK;                                                              \
    }

// -----------------------------------------------------------------------------
// Type 5
//     2 input, 1 output
//     dot product
//     OP_FUNC requires 3 arguments
// -----------------------------------------------------------------------------
/*
    OP_N    : operation name
    OP_T    : operation type
    OP_FUNC : function name
    T_IN    : input type
    T_OUT   : output type(result)
    n       : length
    c       : output array
    a       : input array1
    b       : input array2
*/

#define DEFINE_HAL_GENERAL_OPS5(OP_N, OP_T, OP_FUNC, T_IN, T_OUT)                   \
    hal_status_t hal_##OP_N##_##OP_T(T_OUT *c, const T_IN *a, const T_IN *b, const size_t n) { \
        for (size_t i = 0; i < n; i++) {                                            \
            *c += OP_FUNC(a[i], b[i]);                                              \
        }                                                                           \
        return HAL_OK;                                                              \
    }

// -----------------------------------------------------------------------------
// Type 6
//     2 input, 1 output
//     matrix multiplication
//     OF_FUNC requires 3 arguments
// -----------------------------------------------------------------------------
/*
    OP_N    : operation name
    OP_T    : operation type
    T_IN    : input type
    T_OUT   : output type(result)
    n       : length
    c       : output array
    a       : input array1
    b       : input array2
*/

#define DEFINE_HAL_GENERAL_OPS6(OP_N, OP_T, T_IN, T_OUT)                            \
    hal_status_t hal_##OP_N##_##OP_T(T_OUT *c, const T_IN *a, const T_IN *b, const int M, const int N, const int K) { \
        T_OUT (*pc)[N] = (void *)(c);                                               \
        const T_IN (*pa)[K] = (const void *)(a);                                    \
        const T_IN (*pb)[N] = (const void *)(b);                                    \
                                                                                    \
        memset(c, 0, sizeof(T_OUT) * M * N);                                        \
        for (int i = 0; i < M; i++) {                                               \
            for (int k = 0; k < K; k++) {                                           \
                if (pa[i][k] != (T_IN)0) {                                          \
                    for (int j = 0; j < N; j++) {                                   \
                        pc[i][j] += (T_OUT)pa[i][k] * (T_OUT)pb[k][j];              \
                    }                                                               \
                }                                                                   \
            }                                                                       \
        }                                                                           \
        return HAL_OK;                                                              \
    }

// -----------------------------------------------------------------------------
// Type 7
//     2 input, 1 output
//     explicity type casting ...
// -----------------------------------------------------------------------------
/*
    OP_N    : operation name
    OP_T    : operation type
    OP_FUNC : function name
    T_IN    : input type
    T_OUT   : output type(result)
    n       : length
    c       : output array
    a       : input array1
    b       : input array2
*/
#define DEFINE_HAL_GENERAL_OPS7(OP_N, OP_T, OP_FUNC, T_IN, T_OUT)                   \
    hal_status_t hal_##OP_N##_##OP_T(T_OUT *c, const T_IN *a, const T_IN *b, const size_t n) { \
        for (size_t i = 0; i < n; i++) {                                            \
            c[i] = OP_FUNC(T_OUT, a[i], b[i]);                                      \
        }                                                                           \
        return HAL_OK;                                                              \
    }

// -----------------------------------------------------------------------------
// Integer Operation
//     i8/u8, i16/u16, i32/u32, i64/u64, i128/u128
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// signle-width addition
// -----------------------------------------------------------------------------
DEFINE_HAL_GENERAL_OPS2(vadd, i8, ADD, int8_t, int8_t)
DEFINE_HAL_GENERAL_OPS2(vadd, i16, ADD, int16_t, int16_t)
DEFINE_HAL_GENERAL_OPS2(vadd, i32, ADD, int32_t, int32_t)
DEFINE_HAL_GENERAL_OPS2(vadd, i64, ADD, int64_t, int64_t)

DEFINE_HAL_GENERAL_OPS2(vadd, u8, ADD, uint8_t, uint8_t)
DEFINE_HAL_GENERAL_OPS2(vadd, u16, ADD, uint16_t, uint16_t)
DEFINE_HAL_GENERAL_OPS2(vadd, u32, ADD, uint32_t, uint32_t)
DEFINE_HAL_GENERAL_OPS2(vadd, u64, ADD, uint64_t, uint64_t)

// -----------------------------------------------------------------------------
// widening addition
// -----------------------------------------------------------------------------
DEFINE_HAL_GENERAL_OPS7(vwadd, i8, WADD, int8_t, int16_t)
DEFINE_HAL_GENERAL_OPS7(vwadd, i16, WADD, int16_t, int32_t)
DEFINE_HAL_GENERAL_OPS7(vwadd, i32, WADD, int32_t, int64_t)
DEFINE_HAL_GENERAL_OPS2(vwadd, i64, hal_wadd_i64, int64_t, int128_t)

DEFINE_HAL_GENERAL_OPS7(vwadd, u8, WADD, uint8_t, uint16_t)
DEFINE_HAL_GENERAL_OPS7(vwadd, u16, WADD, uint16_t, uint32_t)
DEFINE_HAL_GENERAL_OPS7(vwadd, u32, WADD, uint32_t, uint64_t)
DEFINE_HAL_GENERAL_OPS2(vwadd, u64, hal_wadd_u64, uint64_t, uint128_t)

// -----------------------------------------------------------------------------
// subtraction
// -----------------------------------------------------------------------------
DEFINE_HAL_GENERAL_OPS2(vsub, i8, SUB, int8_t, int8_t)
DEFINE_HAL_GENERAL_OPS2(vsub, i16, SUB, int16_t, int16_t)
DEFINE_HAL_GENERAL_OPS2(vsub, i32, SUB, int32_t, int32_t)
DEFINE_HAL_GENERAL_OPS2(vsub, i64, SUB, int64_t, int64_t)

DEFINE_HAL_GENERAL_OPS2(vsub, u8, SUB, uint8_t, uint8_t)
DEFINE_HAL_GENERAL_OPS2(vsub, u16, SUB, uint16_t, uint16_t)
DEFINE_HAL_GENERAL_OPS2(vsub, u32, SUB, uint32_t, uint32_t)
DEFINE_HAL_GENERAL_OPS2(vsub, u64, SUB, uint64_t, uint64_t)

// -----------------------------------------------------------------------------
// widening subtraction
// -----------------------------------------------------------------------------
DEFINE_HAL_GENERAL_OPS7(vwsub, i8, WSUB, int8_t, int16_t)
DEFINE_HAL_GENERAL_OPS7(vwsub, i16, WSUB, int16_t, int32_t)
DEFINE_HAL_GENERAL_OPS7(vwsub, i32, WSUB, int32_t, int64_t)
DEFINE_HAL_GENERAL_OPS2(vwsub, i64, hal_wsub_i64, int64_t, int128_t)

DEFINE_HAL_GENERAL_OPS7(vwsub, u8, WSUB, uint8_t, uint16_t)
DEFINE_HAL_GENERAL_OPS7(vwsub, u16, WSUB, uint16_t, uint32_t)
DEFINE_HAL_GENERAL_OPS7(vwsub, u32, WSUB, uint32_t, uint64_t)
DEFINE_HAL_GENERAL_OPS2(vwsub, u64, hal_wsub_u64, uint64_t, uint128_t)

// -----------------------------------------------------------------------------
// multiplicaton
// -----------------------------------------------------------------------------
DEFINE_HAL_GENERAL_OPS2(vmul, i8, MUL, int8_t, int16_t)
DEFINE_HAL_GENERAL_OPS2(vmul, u8, MUL, uint8_t, uint16_t)
DEFINE_HAL_GENERAL_OPS2(vmul, i16, MUL, int16_t, int32_t)
DEFINE_HAL_GENERAL_OPS2(vmul, u16, MUL, uint16_t, uint32_t)
DEFINE_HAL_GENERAL_OPS2(vmul, i32, MUL, int32_t, int64_t)
DEFINE_HAL_GENERAL_OPS2(vmul, u32, MUL, uint32_t, uint64_t)
DEFINE_HAL_GENERAL_OPS2(vmul, i64, hal_mul_i64, int64_t, int128_t)
DEFINE_HAL_GENERAL_OPS2(vmul, u64, hal_mul_u64, uint64_t, uint128_t)

// -----------------------------------------------------------------------------
// multiplication and accumulation
// -----------------------------------------------------------------------------
DEFINE_HAL_GENERAL_OPS4(vmac, i8, MAC, int8_t, int16_t)
DEFINE_HAL_GENERAL_OPS4(vmac, u8, MAC, uint8_t, uint16_t)
DEFINE_HAL_GENERAL_OPS4(vmac, i16, MAC, int16_t, int32_t)
DEFINE_HAL_GENERAL_OPS4(vmac, u16, MAC, uint16_t, uint32_t)
DEFINE_HAL_GENERAL_OPS4(vmac, i32, MAC, int32_t, int64_t)
DEFINE_HAL_GENERAL_OPS4(vmac, u32, MAC, uint32_t, uint64_t)

// -----------------------------------------------------------------------------
// division
// -----------------------------------------------------------------------------
DEFINE_HAL_GENERAL_OPS3(vdiv, i8, DIV, int8_t, int8_t)
DEFINE_HAL_GENERAL_OPS3(vdiv, u8, DIV, uint8_t, uint8_t)
DEFINE_HAL_GENERAL_OPS3(vdiv, i16, DIV, int16_t, int16_t)
DEFINE_HAL_GENERAL_OPS3(vdiv, u16, DIV, uint16_t, uint16_t)
DEFINE_HAL_GENERAL_OPS3(vdiv, i32, DIV, int32_t, int32_t)
DEFINE_HAL_GENERAL_OPS3(vdiv, u32, DIV, uint32_t, uint32_t)
DEFINE_HAL_GENERAL_OPS3(vdiv, i64, DIV, int64_t, int64_t)
DEFINE_HAL_GENERAL_OPS3(vdiv, u64, DIV, uint64_t, uint64_t)

// -----------------------------------------------------------------------------
// dot product
// -----------------------------------------------------------------------------
DEFINE_HAL_GENERAL_OPS5(vdot, i8, MUL, int8_t, int8_t)
DEFINE_HAL_GENERAL_OPS5(vdot, u8, MUL, uint8_t, uint8_t)
DEFINE_HAL_GENERAL_OPS5(vdot, i16, MUL, int16_t, int16_t)
DEFINE_HAL_GENERAL_OPS5(vdot, u16, MUL, uint16_t, uint16_t)
DEFINE_HAL_GENERAL_OPS5(vdot, i32, MUL, int32_t, int32_t)
DEFINE_HAL_GENERAL_OPS5(vdot, u32, MUL, uint32_t, uint32_t)

// -----------------------------------------------------------------------------
// min
// -----------------------------------------------------------------------------
DEFINE_HAL_GENERAL_OPS2(vmin, u8, MIN, uint8_t, uint8_t)
DEFINE_HAL_GENERAL_OPS2(vmin, i8, MIN, int8_t, int8_t)
DEFINE_HAL_GENERAL_OPS2(vmin, u16, MIN, uint16_t, uint16_t)
DEFINE_HAL_GENERAL_OPS2(vmin, i16, MIN, int16_t, int16_t)
DEFINE_HAL_GENERAL_OPS2(vmin, u32, MIN, uint32_t, uint32_t)
DEFINE_HAL_GENERAL_OPS2(vmin, i32, MIN, int32_t, int32_t)
DEFINE_HAL_GENERAL_OPS2(vmin, u64, MIN, uint64_t, uint64_t)
DEFINE_HAL_GENERAL_OPS2(vmin, i64, MIN, int64_t, int64_t)

// -----------------------------------------------------------------------------
// max
// -----------------------------------------------------------------------------
DEFINE_HAL_GENERAL_OPS2(vmax, u8, MAX, uint8_t, uint8_t)
DEFINE_HAL_GENERAL_OPS2(vmax, i8, MAX, int8_t, int8_t)
DEFINE_HAL_GENERAL_OPS2(vmax, u16, MAX, uint16_t, uint16_t)
DEFINE_HAL_GENERAL_OPS2(vmax, i16, MAX, int16_t, int16_t)
DEFINE_HAL_GENERAL_OPS2(vmax, u32, MAX, uint32_t, uint32_t)
DEFINE_HAL_GENERAL_OPS2(vmax, i32, MAX, int32_t, int32_t)
DEFINE_HAL_GENERAL_OPS2(vmax, u64, MAX, uint64_t, uint64_t)
DEFINE_HAL_GENERAL_OPS2(vmax, i64, MAX, int64_t, int64_t)

// -----------------------------------------------------------------------------
// matrix multiplication
// -----------------------------------------------------------------------------
DEFINE_HAL_GENERAL_OPS6(matrix_vmul, i8, int8_t, int16_t)
DEFINE_HAL_GENERAL_OPS6(matrix_vmul, u8, uint8_t, uint16_t)
DEFINE_HAL_GENERAL_OPS6(matrix_vmul, i16, int16_t, int32_t)
DEFINE_HAL_GENERAL_OPS6(matrix_vmul, u16, uint16_t, uint32_t)
DEFINE_HAL_GENERAL_OPS6(matrix_vmul, i32, int32_t, int64_t)
DEFINE_HAL_GENERAL_OPS6(matrix_vmul, u32, uint32_t, uint64_t)

// =============================================================================
// float/double Operations
//     float/double
// =============================================================================
DEFINE_HAL_GENERAL_OPS2(vadd, f32, ADD, float, float)
DEFINE_HAL_GENERAL_OPS2(vsub, f32, SUB, float, float)
DEFINE_HAL_GENERAL_OPS2(vmul, f32, MUL, float, double)
DEFINE_HAL_GENERAL_OPS4(vmac, f32, MAC, float, double)
DEFINE_HAL_GENERAL_OPS3(vdiv, f32, DIV, float, float)
DEFINE_HAL_GENERAL_OPS5(vdot, f32, MUL, float, double)
DEFINE_HAL_GENERAL_OPS2(vmin, f32, MIN, float, float)
DEFINE_HAL_GENERAL_OPS2(vmax, f32, MAX, float, float)
DEFINE_HAL_GENERAL_OPS1(vsqrt, f32, sqrtf, float, float)
DEFINE_HAL_GENERAL_OPS1(vrsqrt7, f32, hal_rsqrt7_f32, float, float)
DEFINE_HAL_GENERAL_OPS6(matrix_vmul, f32, float, double)
