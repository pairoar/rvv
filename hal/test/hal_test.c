#include "hal_internal_math.h"
#include "hal_soft_math.h"
#include "hal_test_mat_data.h"
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    https://blog.regehr.org/archives/213
*/

#ifdef CONFIG_CUSTOM_ACCELERATOR
#include "cust_accel.h"
extern void ca_hw_init(void);
#endif

#define SAFE_FREE(ptr)                                                                             \
    do {                                                                                           \
        if (ptr != NULL) {                                                                         \
            free(ptr);                                                                             \
            ptr = NULL;                                                                            \
        }                                                                                          \
    } while (0)

typedef enum {
    HAL_TEST_INIT,

    // single-width add
    HAL_TEST_VADD_I8,
    HAL_TEST_VADD_I16,
    HAL_TEST_VADD_I32,
    HAL_TEST_VADD_I64,
    HAL_TEST_VADD_I128,

    HAL_TEST_VADD_U8,
    HAL_TEST_VADD_U16,
    HAL_TEST_VADD_U32,
    HAL_TEST_VADD_U64,
    HAL_TEST_VADD_U128,

    // widening add
    HAL_TEST_VWADD_I8,
    HAL_TEST_VWADD_I16,
    HAL_TEST_VWADD_I32,
    HAL_TEST_VWADD_I64,
    HAL_TEST_VWADD_I128,

    HAL_TEST_VWADD_U8,
    HAL_TEST_VWADD_U16,
    HAL_TEST_VWADD_U32,
    HAL_TEST_VWADD_U64,
    HAL_TEST_VWADD_U128,

    // sub
    HAL_TEST_VSUB_I8,
    HAL_TEST_VSUB_I16,
    HAL_TEST_VSUB_I32,
    HAL_TEST_VSUB_I64,
    HAL_TEST_VSUB_I128,

    HAL_TEST_VSUB_U8,
    HAL_TEST_VSUB_U16,
    HAL_TEST_VSUB_U32,
    HAL_TEST_VSUB_U64,
    HAL_TEST_VSUB_U128,

    HAL_TEST_VWSUB_I8,
    HAL_TEST_VWSUB_I16,
    HAL_TEST_VWSUB_I32,
    HAL_TEST_VWSUB_I64,
    HAL_TEST_VWSUB_I128,

    HAL_TEST_VWSUB_U8,
    HAL_TEST_VWSUB_U16,
    HAL_TEST_VWSUB_U32,
    HAL_TEST_VWSUB_U64,
    HAL_TEST_VWSUB_U128,

    // mul
    HAL_TEST_VMUL_I8,
    HAL_TEST_VMUL_I16,
    HAL_TEST_VMUL_I32,
    HAL_TEST_VMUL_I64,
    HAL_TEST_VMUL_I128,

    HAL_TEST_VMUL_U8,
    HAL_TEST_VMUL_U16,
    HAL_TEST_VMUL_U32,
    HAL_TEST_VMUL_U64,
    HAL_TEST_VMUL_U128,

    HAL_TEST_VWMUL_I8,
    HAL_TEST_VWMUL_I16,
    HAL_TEST_VWMUL_I32,
    HAL_TEST_VWMUL_I64,
    HAL_TEST_VWMUL_I128,

    HAL_TEST_VWMUL_U8,
    HAL_TEST_VWMUL_U16,
    HAL_TEST_VWMUL_U32,
    HAL_TEST_VWMUL_U64,
    HAL_TEST_VWMUL_U128,

    // mac
    HAL_TEST_VMAC_I8,
    HAL_TEST_VMAC_I16,
    HAL_TEST_VMAC_I32,
    HAL_TEST_VMAC_I64,
    HAL_TEST_VMAC_I128,

    HAL_TEST_VMAC_U8,
    HAL_TEST_VMAC_U16,
    HAL_TEST_VMAC_U32,
    HAL_TEST_VMAC_U64,
    HAL_TEST_VMAC_U128,

    HAL_TEST_VWMAC_I8,
    HAL_TEST_VWMAC_I16,
    HAL_TEST_VWMAC_I32,
    HAL_TEST_VWMAC_I64,
    HAL_TEST_VWMAC_I128,

    HAL_TEST_VWMAC_U8,
    HAL_TEST_VWMAC_U16,
    HAL_TEST_VWMAC_U32,
    HAL_TEST_VWMAC_U64,
    HAL_TEST_VWMAC_U128,

    // div
    HAL_TEST_VDIV_I8,
    HAL_TEST_VDIV_I16,
    HAL_TEST_VDIV_I32,
    HAL_TEST_VDIV_I64,
    HAL_TEST_VDIV_I128,

    HAL_TEST_VDIV_U8,
    HAL_TEST_VDIV_U16,
    HAL_TEST_VDIV_U32,
    HAL_TEST_VDIV_U64,
    HAL_TEST_VDIV_U128,

    // dot product
    HAL_TEST_VDOT_I8,
    HAL_TEST_VDOT_I16,
    HAL_TEST_VDOT_I32,
    HAL_TEST_VDOT_I64,
    HAL_TEST_VDOT_I128,

    HAL_TEST_VDOT_U8,
    HAL_TEST_VDOT_U16,
    HAL_TEST_VDOT_U32,
    HAL_TEST_VDOT_U64,
    HAL_TEST_VDOT_U128,

    HAL_TEST_VWDOT_I8,
    HAL_TEST_VWDOT_I16,
    HAL_TEST_VWDOT_I32,
    HAL_TEST_VWDOT_I64,
    HAL_TEST_VWDOT_I128,

    HAL_TEST_VWDOT_U8,
    HAL_TEST_VWDOT_U16,
    HAL_TEST_VWDOT_U32,
    HAL_TEST_VWDOT_U64,
    HAL_TEST_VWDOT_U128,

    // matrix
    HAL_TEST_MTRX_MUL_I8,
    HAL_TEST_MTRX_MUL_I16,
    HAL_TEST_MTRX_MUL_I32,
    HAL_TEST_MTRX_MUL_I64,
    HAL_TEST_MTRX_MUL_I128,

    HAL_TEST_MTRX_MUL_U8,
    HAL_TEST_MTRX_MUL_U16,
    HAL_TEST_MTRX_MUL_U32,
    HAL_TEST_MTRX_MUL_U64,
    HAL_TEST_MTRX_MUL_U128,

    HAL_TEST_MTRX_WMUL_I8,
    HAL_TEST_MTRX_WMUL_I16,
    HAL_TEST_MTRX_WMUL_I32,
    HAL_TEST_MTRX_WMUL_I64,
    HAL_TEST_MTRX_WMUL_I128,

    HAL_TEST_MTRX_WMUL_U8,
    HAL_TEST_MTRX_WMUL_U16,
    HAL_TEST_MTRX_WMUL_U32,
    HAL_TEST_MTRX_WMUL_U64,
    HAL_TEST_MTRX_WMUL_U128,

    // matrix tiled
    HAL_TEST_MTRX_MUL_TILED_I8,
    HAL_TEST_MTRX_MUL_TILED_I16,
    HAL_TEST_MTRX_MUL_TILED_I32,
    HAL_TEST_MTRX_MUL_TILED_I64,
    HAL_TEST_MTRX_MUL_TILED_I128,

    HAL_TEST_MTRX_MUL_TILED_U8,
    HAL_TEST_MTRX_MUL_TILED_U16,
    HAL_TEST_MTRX_MUL_TILED_U32,
    HAL_TEST_MTRX_MUL_TILED_U64,
    HAL_TEST_MTRX_MUL_TILED_U128,

    HAL_TEST_MTRX_WMUL_TILED_I8,
    HAL_TEST_MTRX_WMUL_TILED_I16,
    HAL_TEST_MTRX_WMUL_TILED_I32,
    HAL_TEST_MTRX_WMUL_TILED_I64,
    HAL_TEST_MTRX_WMUL_TILED_I128,

    HAL_TEST_MTRX_WMUL_TILED_U8,
    HAL_TEST_MTRX_WMUL_TILED_U16,
    HAL_TEST_MTRX_WMUL_TILED_U32,
    HAL_TEST_MTRX_WMUL_TILED_U64,
    HAL_TEST_MTRX_WMUL_TILED_U128,

    // min
    HAL_TEST_VMIN_I8,
    HAL_TEST_VMIN_I16,
    HAL_TEST_VMIN_I32,
    HAL_TEST_VMIN_I64,
    HAL_TEST_VMIN_I128,

    HAL_TEST_VMIN_U8,
    HAL_TEST_VMIN_U16,
    HAL_TEST_VMIN_U32,
    HAL_TEST_VMIN_U64,
    HAL_TEST_VMIN_U128,

    // max
    HAL_TEST_VMAX_I8,
    HAL_TEST_VMAX_I16,
    HAL_TEST_VMAX_I32,
    HAL_TEST_VMAX_I64,
    HAL_TEST_VMAX_I128,

    HAL_TEST_VMAX_U8,
    HAL_TEST_VMAX_U16,
    HAL_TEST_VMAX_U32,
    HAL_TEST_VMAX_U64,
    HAL_TEST_VMAX_U128,

    //
    // floating piont
    //
    HAL_TEST_ADD_F32,
    HAL_TEST_SUB_F32,
    HAL_TEST_MUL_F32,
    HAL_TEST_MAC_F32,
    HAL_TEST_DIV_F32,
    HAL_TEST_DOT_F32,
    HAL_TEST_MTRX_MUL_F32,
    HAL_TEST_MTRX_MUL_TILED_F32,
    HAL_TEST_VMIN_F32,
    HAL_TEST_VMAX_F32,
    HAL_TEST_VSQRT_F32,
    HAL_TEST_VRSQRT7_F32,

    HAL_TEST_WADD_F32,
    HAL_TEST_WSUB_F32,
    HAL_TEST_WMUL_F32,
    HAL_TEST_WMAC_F32,
    HAL_TEST_WDOT_F32,
    HAL_TEST_MTRX_WMUL_F32,
    HAL_TEST_MTRX_WMUL_TILED_F32,    

    //
    // Bitwise
    //
    // and(&)
    HAL_TEST_VAND_I8,
    HAL_TEST_VAND_I16,
    HAL_TEST_VAND_I32,
    HAL_TEST_VAND_I64,
    HAL_TEST_VAND_I128,

    HAL_TEST_VAND_U8,
    HAL_TEST_VAND_U16,
    HAL_TEST_VAND_U32,
    HAL_TEST_VAND_U64,
    HAL_TEST_VAND_U128,

    // or(|)
    HAL_TEST_VOR_I8,
    HAL_TEST_VOR_I16,
    HAL_TEST_VOR_I32,
    HAL_TEST_VOR_I64,
    HAL_TEST_VOR_I128,

    HAL_TEST_VOR_U8,
    HAL_TEST_VOR_U16,
    HAL_TEST_VOR_U32,
    HAL_TEST_VOR_U64,
    HAL_TEST_VOR_U128,

    // xor(^)
    HAL_TEST_VXOR_I8,
    HAL_TEST_VXOR_I16,
    HAL_TEST_VXOR_I32,
    HAL_TEST_VXOR_I64,
    HAL_TEST_VXOR_I128,

    HAL_TEST_VXOR_U8,
    HAL_TEST_VXOR_U16,
    HAL_TEST_VXOR_U32,
    HAL_TEST_VXOR_U64,
    HAL_TEST_VXOR_U128,

    // not(~)
    HAL_TEST_VNOT_I8,
    HAL_TEST_VNOT_I16,
    HAL_TEST_VNOT_I32,
    HAL_TEST_VNOT_I64,
    HAL_TEST_VNOT_I128,

    HAL_TEST_VNOT_U8,
    HAL_TEST_VNOT_U16,
    HAL_TEST_VNOT_U32,
    HAL_TEST_VNOT_U64,
    HAL_TEST_VNOT_U128,

    HAL_TEST_MAX,
} HAL_TEST_LIST_E;

typedef struct {
    HAL_TEST_LIST_E idx;
    int (*func)(void);
    const char *name; // Added: Function (test) name
    const char *description;
} HAL_TEST_FUNCTION_DEF;

#define EPSILON 0.0001f

// -----------------------------------------------------------------------------
//  Read Hardware Cycle Counter (Performance Profiling)
// -----------------------------------------------------------------------------
static inline uint64_t get_mcycle(void) {
#if defined(__riscv)
#if __riscv_xlen == 32
    uint32_t lo, hi, hi2;
    // Prevent the upper 32 bits from overflowing while reading the lower 32 bits
    do {
        // Changed mcycleh -> cycleh, mcycle -> cycle
        __asm__ volatile("csrr %0, cycleh" : "=r"(hi)::"memory");
        __asm__ volatile("csrr %0, cycle" : "=r"(lo));
        __asm__ volatile("csrr %0, cycleh" : "=r"(hi2));
    } while (hi != hi2);
    return (((uint64_t)hi) << 32) | lo;
#else
    uint64_t cycles;
    // Changed mcycle -> cycle
    __asm__ volatile("csrr %0, cycle" : "=r"(cycles));
    return cycles;
#endif
#else
    // Return dummy value for Host PC (e.g., x86) environments
    return 0;
#endif
}

// -----------------------------------------------------------------------------
// helper functions
// -----------------------------------------------------------------------------
static void hal_print_test_result(const char *test_name, int result) {
    if (result == 0) {
        printf("[PASS] %s\n", test_name);
    } else {
        printf("[FAIL] %s\n", test_name);
    }
}

static int hal_verify_result_i8(const int8_t a, const int8_t b) { return a == b; }
static int hal_verify_result_i16(const int16_t a, const int16_t b) { return a == b; }
static int hal_verify_result_i32(const int32_t a, const int32_t b) { return a == b; }
static int hal_verify_result_i64(const int64_t a, const int64_t b) { return a == b; }
static int hal_verify_result_i128(const int128_t a, const int128_t b) {
    return ((a.u == b.u) && (a.l == b.l));
}

static int hal_verify_result_i256(const int256_t a, const int256_t b) {
    return ((a.d[3] == b.d[3]) && (a.d[2] == b.d[2]) && (a.d[1] == b.d[1]) && (a.d[0] == b.d[0]));
}

static int hal_verify_result_u8(const uint8_t a, const uint8_t b) { return a == b; }
static int hal_verify_result_u16(const uint16_t a, const uint16_t b) { return a == b; }
static int hal_verify_result_u32(const uint32_t a, const uint32_t b) { return a == b; }
static int hal_verify_result_u64(const uint64_t a, const uint64_t b) { return a == b; }
static int hal_verify_result_u128(const uint128_t a, const uint128_t b) {
    return ((a.u == b.u) && (a.l == b.l));
}
static int hal_verify_result_u256(const uint256_t a, const uint256_t b) {
    return ((a.d[3] == b.d[3]) && (a.d[2] == b.d[2]) && (a.d[1] == b.d[1]) && (a.d[0] == b.d[0]));
}

static int hal_verify_result_f32(const float a, const float b) { return (fabsf(a - b) < EPSILON); }
static int hal_verify_result_f64(const double a, const double b) { return (fabs(a - b) < EPSILON); }

static int hal_verify_array_i8(const int8_t *a, const int8_t *b, const size_t n) {
    int result = 0;

    if (a == NULL || b == NULL)
        goto RET;

    if (n == 0) {
        result = 1;
        goto RET;
    }

    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            goto RET;
        }
    }
    result = 1;

RET:
    return result;
}

static int hal_verify_array_i16(const int16_t *a, const int16_t *b, const size_t n) {
    int result = 0;

    if (a == NULL || b == NULL)
        goto RET;

    if (n == 0) {
        result = 1;
        goto RET;
    }

    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            goto RET;
        }
    }
    result = 1;

RET:
    return result;
}

static int hal_verify_array_i32(const int32_t *a, const int32_t *b, const size_t n) {
    int result = 0;

    if (a == NULL || b == NULL)
        goto RET;

    if (n == 0) {
        result = 1;
        goto RET;
    }

    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            goto RET;
        }
    }
    result = 1;

RET:
    return result;
}

static int hal_verify_array_i64(const int64_t *a, const int64_t *b, const size_t n) {
    int result = 0;

    if (a == NULL || b == NULL)
        goto RET;

    if (n == 0) {
        result = 1;
        goto RET;
    }

    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            goto RET;
        }
    }
    result = 1;

RET:
    return result;
}

static int hal_verify_array_i128(const int128_t *a, const int128_t *b, const size_t n) {
    int result = 0;

    if (a == NULL || b == NULL)
        goto RET;

    if (n == 0) {
        result = 1;
        goto RET;
    }

    for (size_t i = 0; i < n; i++) {
        if ((a[i].u != b[i].u) || (a[i].l != b[i].l)) {
            goto RET;
        }
    }
    result = 1;

RET:
    return result;
}

static int hal_verify_array_i256(const int256_t *a, const int256_t *b, const size_t n) {
    int result = 0;

    if (a == NULL || b == NULL)
        goto RET;

    if (n == 0) {
        result = 1;
        goto RET;
    }

    for (size_t i = 0; i < n; i++) {
        if ((a[i].d[3] != b[i].d[3]) || (a[i].d[2] != b[i].d[2]) || (a[i].d[1] != b[i].d[1]) ||
            (a[i].d[0] != b[i].d[0])) {
            goto RET;
        }
    }
    result = 1;

RET:
    return result;
}

static int hal_verify_array_u8(const uint8_t *a, const uint8_t *b, const size_t n) {
    int result = 0;

    if (a == NULL || b == NULL)
        goto RET;

    if (n == 0) {
        result = 1;
        goto RET;
    }

    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            printf("    [Mismatch] idx %zu: SW=%u, HW=%u\n", i, a[i],
                   b[i]); // Print mismatch index for debugging
            goto RET;
        }
    }
    result = 1;

RET:
    return result;
}

static int hal_verify_array_u16(const uint16_t *a, const uint16_t *b, const size_t n) {
    int result = 0;

    if (a == NULL || b == NULL)
        goto RET;

    if (n == 0) {
        result = 1;
        goto RET;
    }

    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            printf("    [Mismatch] idx %zu: SW=%u, HW=%u\n", i, a[i],
                   b[i]); // Print mismatch index for debugging
            goto RET;
        }
    }
    result = 1;

RET:
    return result;
}

static int hal_verify_array_u32(const uint32_t *a, const uint32_t *b, const size_t n) {
    int result = 0;

    if (a == NULL || b == NULL)
        goto RET;

    if (n == 0) {
        result = 1;
        goto RET;
    }

    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            goto RET;
        }
    }
    result = 1;

RET:
    return result;
}

static int hal_verify_array_u64(const uint64_t *a, const uint64_t *b, const size_t n) {
    int result = 0;

    if (a == NULL || b == NULL)
        goto RET;

    if (n == 0) {
        result = 1;
        goto RET;
    }

    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            goto RET;
        }
    }
    result = 1;

RET:
    return result;
}

static int hal_verify_array_u128(const uint128_t *a, const uint128_t *b, const size_t n) {
    int result = 0;

    if (a == NULL || b == NULL)
        goto RET;

    if (n == 0) {
        result = 1;
        goto RET;
    }

    for (size_t i = 0; i < n; i++) {
        if ((a[i].u != b[i].u) || (a[i].l != b[i].l)) {
            goto RET;
        }
    }
    result = 1;

RET:

    return result;
}

static int hal_verify_array_u256(const uint256_t *a, const uint256_t *b, const size_t n) {
    int result = 0;

    if (a == NULL || b == NULL)
        goto RET;

    if (n == 0) {
        result = 1;
        goto RET;
    }

    for (size_t i = 0; i < n; i++) {
        if ((a[i].d[3] != b[i].d[3]) || (a[i].d[2] != b[i].d[2]) || (a[i].d[1] != b[i].d[1]) ||
            (a[i].d[0] != b[i].d[0])) {
            goto RET;
        }
    }
    result = 1;

RET:
    return result;
}

static int hal_verify_array_f32(const float *a, const float *b, const size_t n) {
    if (a == NULL || b == NULL)
        return 0;
    if (n == 0)
        return 1;

    for (size_t i = 0; i < n; i++) {
        // 1. Pass if both are NaN (Handle exception since NaN == NaN is false)
        if (isnan(a[i]) && isnan(b[i]))
            continue;

        // 2. Pass if both are Infinity (Inf) with the same sign
        if (isinf(a[i]) && isinf(b[i]) && (a[i] == b[i]))
            continue;

        // 3. Comparison within typical error margin
        if (fabsf(a[i] - b[i]) > EPSILON) {
            // Print mismatch index for debugging on failure (Optional)
            printf("    [Mismatch] idx %zu: SW=%f, HW=%f\n", i, a[i], b[i]);
            return 0;
        }
    }
    return 1;
}

static int hal_verify_array_f64(const double *a, const double *b, const size_t n) {
    if (a == NULL || b == NULL)
        return 0;
    if (n == 0)
        return 1;

    for (size_t i = 0; i < n; i++) {
        // 1. Pass if both are NaN (Handle exception since NaN == NaN is false)
        if (isnan(a[i]) && isnan(b[i]))
            continue;

        // 2. Pass if both are Infinity (Inf) with the same sign
        if (isinf(a[i]) && isinf(b[i]) && (a[i] == b[i]))
            continue;

        // 3. Comparison within typical error margin
        if (fabs(a[i] - b[i]) > EPSILON) {
            // Print mismatch index for debugging on failure (Optional)
            printf("    [Mismatch] idx %zu: SW=%f, HW=%f\n", i, a[i], b[i]);
            return 0;
        }
    }
    return 1;
}

static void hal_init() {
#ifdef CONFIG_CUSTOM_ACCELERATOR
    ca_hw_init();
#endif
}

// -----------------------------------------------------------------------------
// [Magic Test Generation Macro V3 - Seperate single-width and widening
// [Magic Test Generation Macro V2 - Added DIV error check]
// -----------------------------------------------------------------------------
#define TEST_LEN 1024

// Math Same Width
// vadd, vsub, vdiv, vmin, vmax
#define DEFINE_HAL_TESTS_MATH_SAME_WIDTH(T_IN, S_IN)                                               \
    /* 1. ADD Test */                                                                              \
    static int test_hal_vadd_##S_IN(void) {                                                        \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},                                         \
             b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};                                              \
        T_IN c_hw[10] = {0}, c_sw[10] = {0};                                                       \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] = a[i] + b[i];                                                                 \
                                                                                                   \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_vadd_##S_IN(c_hw, a, b, 10);                                                     \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
                                                                                                   \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 10)) {                                             \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }                                                                                              \
    /* 2. SUB Test */                                                                              \
    static int test_hal_vsub_##S_IN(void) {                                                        \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[10] = {5, 6, 15, 12, 25, 18, 35, 24, 45, 30},                                       \
             b[10] = {3, 10, 9, 20, 15, 30, 21, 40, 27, 50};                                       \
        T_IN c_hw[10] = {0}, c_sw[10] = {0};                                                       \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] = a[i] - b[i];                                                                 \
                                                                                                   \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_vsub_##S_IN(c_hw, a, b, 10);                                                     \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
                                                                                                   \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 10)) {                                             \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }                                                                                              \
    /* 3. MUL Test */                                                                              \
    static int test_hal_vmul_##S_IN(void) {                                                        \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};    \
        T_IN c_hw[10] = {0}, c_sw[10] = {0};                                                       \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] = a[i] * b[i];                                                                 \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_vmul_##S_IN(c_hw, a, b, 10);                                                     \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
                                                                                                   \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 10)) {                                             \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }                                                                                              \
    /* 4. MAC Test */                                                                              \
    static int test_hal_vmac_##S_IN(void) {                                                        \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};    \
        T_IN c_hw[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};                                  \
        T_IN c_sw[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};                                  \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] += a[i] * b[i];                                                                \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_vmac_##S_IN(c_hw, a, b, 10);                                                     \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 10)) {                                             \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }                                                                                              \
    /* 5. DIV Test (Enhanced exception handling and ret output) */                                 \
    static int test_hal_vdiv_##S_IN(void) {                                                        \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[10] = {20, 20, 30, 40, 50, 60, 70, 80, 90, 100};                                    \
        /* Intentionally insert 0 at indices 2 and 7 to test exception handling. */                \
        T_IN b[10] = {2, 2, 0, 2, 2, 2, 2, 0, 2, 2};                                               \
        T_IN c_hw[10] = {0}, c_sw[10] = {0};                                                       \
        /* Generate SW reference answer (Prevent PC crash from divide-by-zero) */                  \
        for (int i = 0; i < 10; i++) {                                                             \
            if (b[i] == 0)                                                                         \
                c_sw[i] = 0;                                                                       \
            else                                                                                   \
                c_sw[i] = a[i] / b[i];                                                             \
        }                                                                                          \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_vdiv_##S_IN(c_hw, a, b, 10);                                                     \
        uint64_t end = get_mcycle();                                                               \
        /* Print log if HAL function detects an error */                                           \
                                                                                                   \
        if (ret != 0) {                                                                            \
            if (ret == 1 || ret == HAL_ERR_DIV_BY_ZERO) {                                          \
                printf("    -> [INFO] Caught expected DIV_BY_ZERO error in %s\n", __func__);       \
            } else {                                                                               \
                printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);          \
            }                                                                                      \
        }                                                                                          \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 10)) {                                             \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }                                                                                              \
    /* 6. DOT Test */                                                                              \
    static int test_hal_vdot_##S_IN(void) {                                                        \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};      \
        T_IN c_hw = 0, c_sw = 0;                                                                   \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw += a[i] * b[i];                                                                   \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_vdot_##S_IN(&c_hw, a, b, 10);                                                    \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_IN(&c_sw, &c_hw, 1)) {                                            \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }                                                                                              \
    /* 7. MTRX MUL Test */                                                                         \
    static int test_hal_mtrx_vmul_##S_IN(void) {                                                   \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[6] = {1, 2, 3, 4, 5, 6}, b[6] = {1, 2, 3, 4, 5, 6};                                 \
        T_IN c_hw[4] = {0}, c_sw[4] = {0};                                                         \
        for (int i = 0; i < 2; i++) {                                                              \
            for (int j = 0; j < 2; j++) {                                                          \
                T_IN sum = 0;                                                                      \
                for (int k = 0; k < 3; k++)                                                        \
                    sum += a[i * 3 + k] * b[k * 2 + j];                                            \
                c_sw[i * 2 + j] = sum;                                                             \
            }                                                                                      \
        }                                                                                          \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_matrix_vmul_##S_IN(c_hw, a, b, 2, 2, 3);                                         \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
                                                                                                   \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 4)) {                                              \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }                                                                                              \
    /* 8. MTRX MUL Tiled Test */                                                                   \
    static int test_hal_mtrx_vmul_tiled_##S_IN(void) {                                             \
        hal_status_t ret = HAL_OK;                                                                 \
        int result = -1;                                                                           \
        int M = 131, N = 71, K = 79;                                                               \
        int tile_size = 64;                                                                        \
        T_IN *A = (T_IN *)malloc(sizeof(T_IN) * M * K);                                            \
        T_IN *B = (T_IN *)malloc(sizeof(T_IN) * K * N);                                            \
        T_IN *CS = (T_IN *)malloc(sizeof(T_IN) * M * N);                                           \
        T_IN *CH = (T_IN *)malloc(sizeof(T_IN) * M * N);                                           \
        T_IN(*pa)[K] = (T_IN(*)[K])A;                                                              \
        T_IN(*pb)[N] = (T_IN(*)[N])B;                                                              \
        T_IN(*ps)[N] = (T_IN(*)[N])CS;                                                             \
        for (int _idx = 0; _idx < M * K; _idx++)                                                   \
            A[_idx] = (T_IN)1;                                                                     \
        for (int _idx = 0; _idx < K * N; _idx++)                                                   \
            B[_idx] = (T_IN)2;                                                                     \
        memset(CS, 0, sizeof(T_IN) * M * N);                                                       \
        memset(CH, 0, sizeof(T_IN) * M * N);                                                       \
        /* 3. i-k-j order operation (Cache optimization and Sparse matrix handling) */             \
        for (int i = 0; i < M; i++) {                                                              \
            for (int k = 0; k < K; k++) {                                                          \
                if (pa[i][k] != 0) {                                                               \
                    for (int j = 0; j < N; j++) {                                                  \
                        ps[i][j] += pa[i][k] * pb[k][j];                                           \
                    }                                                                              \
                }                                                                                  \
            }                                                                                      \
        }                                                                                          \
        /* --- Profiling Start --- */                                                              \
        uint64_t start_hal = get_mcycle();                                                         \
        ret = hal_matrix_vmul_tiled_##S_IN(CH, A, B, M, N, K, tile_size);                          \
        uint64_t end_hal = get_mcycle();                                                           \
        /* --- Profiling End --- */                                                                \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_IN(CS, CH, M * N)) {                                              \
            result = (int)(end_hal - start_hal); /* Return elapsed clock cycles on success */      \
        }                                                                                          \
        SAFE_FREE(A);                                                                              \
        SAFE_FREE(B);                                                                              \
        SAFE_FREE(CS);                                                                             \
        SAFE_FREE(CH);                                                                             \
        return result;                                                                             \
    }                                                                                              \
    /* 9. min Test */                                                                              \
    static int test_hal_vmin_##S_IN(void) {                                                        \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},                                         \
             b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};                                              \
        T_IN c_hw[10] = {0}, c_sw[10] = {0};                                                       \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] = MIN(a[i], b[i]);                                                             \
                                                                                                   \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_vmin_##S_IN(c_hw, a, b, 10);                                                     \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
                                                                                                   \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 10)) {                                             \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }                                                                                              \
    /* 10. max Test */                                                                             \
    static int test_hal_vmax_##S_IN(void) {                                                        \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},                                         \
             b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};                                              \
        T_IN c_hw[10] = {0}, c_sw[10] = {0};                                                       \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] = MAX(a[i], b[i]);                                                             \
                                                                                                   \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_vmax_##S_IN(c_hw, a, b, 10);                                                     \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
                                                                                                   \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 10)) {                                             \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }


// Math Widening
// vwmul, vwmac, vwdot, hal_matrix_vmul_tiled
#define DEFINE_HAL_TESTS_MATH_WIDENING(T_IN, T_OUT, S_IN, S_OUT)                                   \
    /* 1. ADD Test */                                                                              \
    static int test_hal_vwadd_##S_IN(void) {                                                       \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},                                         \
             b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};                                              \
        T_OUT c_hw[10] = {0}, c_sw[10] = {0};                                                      \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] = a[i] + b[i];                                                                 \
                                                                                                   \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_vwadd_##S_IN(c_hw, a, b, 10);                                                    \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
                                                                                                   \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_OUT(c_sw, c_hw, 10)) {                                            \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }                                                                                              \
    /* 2. SUB Test */                                                                              \
    static int test_hal_vwsub_##S_IN(void) {                                                       \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[10] = {5, 6, 15, 12, 25, 18, 35, 24, 45, 30},                                       \
             b[10] = {3, 10, 9, 20, 15, 30, 21, 40, 27, 50};                                       \
        T_OUT c_hw[10] = {0}, c_sw[10] = {0};                                                      \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] = (T_OUT)a[i] - (T_OUT)b[i];                                                   \
                                                                                                   \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_vwsub_##S_IN(c_hw, a, b, 10);                                                    \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
                                                                                                   \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_OUT(c_sw, c_hw, 10)) {                                            \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }                                                                                              \
    /* 3. MUL Test */                                                                              \
    static int test_hal_vwmul_##S_IN(void) {                                                       \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};    \
        T_OUT c_hw[10] = {0}, c_sw[10] = {0};                                                      \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] = (T_OUT)a[i] * (T_OUT)b[i];                                                   \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_vwmul_##S_IN(c_hw, a, b, 10);                                                    \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
                                                                                                   \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_OUT(c_sw, c_hw, 10)) {                                            \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }                                                                                              \
    /* 4. MAC Test */                                                                              \
    static int test_hal_vwmac_##S_IN(void) {                                                       \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};    \
        T_OUT c_hw[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};                                 \
        T_OUT c_sw[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};                                 \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] += (T_OUT)a[i] * (T_OUT)b[i];                                                  \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_vwmac_##S_IN(c_hw, a, b, 10);                                                    \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_OUT(c_sw, c_hw, 10)) {                                            \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }                                                                                              \
    /* 6. DOT Test */                                                                              \
    static int test_hal_vwdot_##S_IN(void) {                                                       \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};      \
        T_OUT c_hw = 0, c_sw = 0;                                                                  \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw += (T_OUT)a[i] * (T_OUT)b[i];                                                     \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_vwdot_##S_IN(&c_hw, a, b, 10);                                                   \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_OUT(&c_sw, &c_hw, 1)) {                                           \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }                                                                                              \
    /* 7. MTRX MUL Test */                                                                         \
    static int test_hal_mtrx_vwmul_##S_IN(void) {                                                  \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[6] = {1, 2, 3, 4, 5, 6}, b[6] = {1, 2, 3, 4, 5, 6};                                 \
        T_OUT c_hw[4] = {0}, c_sw[4] = {0};                                                        \
        for (int i = 0; i < 2; i++) {                                                              \
            for (int j = 0; j < 2; j++) {                                                          \
                T_OUT sum = 0;                                                                     \
                for (int k = 0; k < 3; k++)                                                        \
                    sum += (T_OUT)a[i * 3 + k] * (T_OUT)b[k * 2 + j];                              \
                c_sw[i * 2 + j] = sum;                                                             \
            }                                                                                      \
        }                                                                                          \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_matrix_vwmul_##S_IN(c_hw, a, b, 2, 2, 3);                                        \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
                                                                                                   \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_OUT(c_sw, c_hw, 4)) {                                             \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }                                                                                              \
    /* 8. MTRX MUL Tiled Test */                                                                   \
    static int test_hal_mtrx_vwmul_tiled_##S_IN(void) {                                            \
        hal_status_t ret = HAL_OK;                                                                 \
        int result = -1;                                                                           \
        int M = 131, N = 71, K = 79;                                                               \
        int tile_size = 64;                                                                        \
        T_IN *A = (T_IN *)malloc(sizeof(T_IN) * M * K);                                            \
        T_IN *B = (T_IN *)malloc(sizeof(T_IN) * K * N);                                            \
        T_OUT *CS = (T_OUT *)malloc(sizeof(T_OUT) * M * N);                                        \
        T_OUT *CH = (T_OUT *)malloc(sizeof(T_OUT) * M * N);                                        \
        T_IN(*pa)[K] = (T_IN(*)[K])A;                                                              \
        T_IN(*pb)[N] = (T_IN(*)[N])B;                                                              \
        T_OUT(*ps)[N] = (T_OUT(*)[N])CS;                                                           \
        for (int _idx = 0; _idx < M * K; _idx++)                                                   \
            A[_idx] = (T_IN)1;                                                                     \
        for (int _idx = 0; _idx < K * N; _idx++)                                                   \
            B[_idx] = (T_IN)2;                                                                     \
        memset(CS, 0, sizeof(T_OUT) * M * N);                                                      \
        memset(CH, 0, sizeof(T_OUT) * M * N);                                                      \
        /* 3. i-k-j order operation (Cache optimization and Sparse matrix handling) */             \
        for (int i = 0; i < M; i++) {                                                              \
            for (int k = 0; k < K; k++) {                                                          \
                if (pa[i][k] != 0) {                                                               \
                    for (int j = 0; j < N; j++) {                                                  \
                        ps[i][j] += (T_OUT)pa[i][k] * pb[k][j];                                    \
                    }                                                                              \
                }                                                                                  \
            }                                                                                      \
        }                                                                                          \
        /* --- Profiling Start --- */                                                              \
        uint64_t start_hal = get_mcycle();                                                         \
        ret = hal_matrix_vwmul_tiled_##S_IN(CH, A, B, M, N, K, tile_size);                         \
        uint64_t end_hal = get_mcycle();                                                           \
        /* --- Profiling End --- */                                                                \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_OUT(CS, CH, M * N)) {                                             \
            result = (int)(end_hal - start_hal); /* Return elapsed clock cycles on success */      \
        }                                                                                          \
        SAFE_FREE(A);                                                                              \
        SAFE_FREE(B);                                                                              \
        SAFE_FREE(CS);                                                                             \
        SAFE_FREE(CH);                                                                             \
        return result;                                                                             \
    }


// Logical Operations    
// vand, vor, vxor
#define DEFINE_HAL_TESTS_LOGICAL(T_IN, S_IN)                                                       \
    /* 1. and */                                                                                  \
    static int test_hal_vand_##S_IN(void) {                                                        \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},                                         \
             b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};                                              \
        T_IN c_hw[10] = {0}, c_sw[10] = {0};                                                       \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] = a[i] & b[i];                                                                 \
                                                                                                   \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_vand_##S_IN(c_hw, a, b, 10);                                                     \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
                                                                                                   \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 10)) {                                             \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }                                                                                              \
    /* 2. or */                                                                                   \
    static int test_hal_vor_##S_IN(void) {                                                         \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},                                         \
             b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};                                              \
        T_IN c_hw[10] = {0}, c_sw[10] = {0};                                                       \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] = a[i] | b[i];                                                                 \
                                                                                                   \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_vor_##S_IN(c_hw, a, b, 10);                                                      \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
                                                                                                   \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 10)) {                                             \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }                                                                                              \
    /* 3. xor */                                                                                  \
    static int test_hal_vxor_##S_IN(void) {                                                        \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},                                         \
             b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};                                              \
        T_IN c_hw[10] = {0}, c_sw[10] = {0};                                                       \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] = a[i] ^ b[i];                                                                 \
                                                                                                   \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_vxor_##S_IN(c_hw, a, b, 10);                                                     \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
                                                                                                   \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 10)) {                                             \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }                                                                                              \
    /* 4. not */                                                                                   \
    static int test_hal_vnot_##S_IN(void) {                                                        \
        hal_status_t ret = HAL_OK;                                                                 \
        T_IN a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};                                         \
        T_IN c_hw[10] = {0}, c_sw[10] = {0};                                                       \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] = ~(a[i]);                                                                     \
                                                                                                   \
        /* --- Profiling Start --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        ret = hal_vnot_##S_IN(c_hw, a, 10);                                                        \
        uint64_t end = get_mcycle();                                                               \
        /* --- Profiling End --- */                                                                \
                                                                                                   \
        if (ret != HAL_OK) {                                                                       \
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);              \
        }                                                                                          \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 10)) {                                             \
            return (int)(end - start); /* Return elapsed clock cycles on success */                \
        } else {                                                                                   \
            return -1; /* Return -1 on failure */                                                  \
        }                                                                                          \
    }


// 1. Integer Type Master Macros
#define DEFINE_HAL_TESTS_INTEGER(T_IN, T_OUT, S_IN, S_OUT)                                         \
    DEFINE_HAL_TESTS_MATH_SAME_WIDTH(T_IN, S_IN)                                                   \
    DEFINE_HAL_TESTS_MATH_WIDENING(T_IN, T_OUT, S_IN, S_OUT)                                       \
    DEFINE_HAL_TESTS_LOGICAL(T_IN, S_IN)

// 2. Real Type Master Macros
#define DEFINE_HAL_TESTS_FLOAT(T_IN, T_OUT, S_IN, S_OUT)                                           \
    DEFINE_HAL_TESTS_MATH_SAME_WIDTH(T_IN, S_IN)                                                   \
    DEFINE_HAL_TESTS_MATH_WIDENING(T_IN, T_OUT, S_IN, S_OUT)


// -----------------------------------------------------------------------------
// Generate integer test functions at once using macros!
// -----------------------------------------------------------------------------
// 정수형: 8비트 입력 -> 16비트 확장 / 8비트 유지 테스트 모두 자동 생성
DEFINE_HAL_TESTS_INTEGER(int8_t, int16_t, i8, i16)
DEFINE_HAL_TESTS_INTEGER(int16_t, int32_t, i16, i32)
DEFINE_HAL_TESTS_INTEGER(int32_t, int64_t, i32, i64)

DEFINE_HAL_TESTS_INTEGER(uint8_t, uint16_t, u8, u16)
DEFINE_HAL_TESTS_INTEGER(uint16_t, uint32_t, u16, u32)
DEFINE_HAL_TESTS_INTEGER(uint32_t, uint64_t, u32, u64)

// -----------------------------------------------------------------------------
// Generate real test functions at once using macros!
// -----------------------------------------------------------------------------
DEFINE_HAL_TESTS_FLOAT(float, double, f32, f64)


// =============================================================================
//  special operations for non-primitive type
// =============================================================================
// -----------------------------------------------------------------------------
//  add
// -----------------------------------------------------------------------------
static int test_hal_vadd_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20}, b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = a[i] + b[i];

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vadd_i64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vadd_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20}, b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = a[i] + b[i];

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vadd_u64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vadd_i128(void) {
    hal_status_t ret = HAL_OK;
    int128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                      {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
             b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                      {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    int128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_add_i128(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vadd_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vadd_u128(void) {
    hal_status_t ret = HAL_OK;
    uint128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                       {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
              b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                       {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    uint128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_add_u128(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vadd_u128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vwadd_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20}, b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_wadd_i64(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwadd_i64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vwadd_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20}, b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_wadd_u64(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwadd_u64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vwadd_i128(void) {
    hal_status_t ret = HAL_OK;
    int128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                      {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
             b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                      {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    int256_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_wadd_i128(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwadd_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i256(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vwadd_u128(void) {
    hal_status_t ret = HAL_OK;
    uint128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                       {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
              b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                       {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    uint256_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_wadd_u128(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwadd_u128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u256(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// -----------------------------------------------------------------------------
//  sub
// -----------------------------------------------------------------------------
static int test_hal_vsub_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20}, b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = a[i] - b[i];

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vsub_i64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vsub_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20}, b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = a[i] - b[i];

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vsub_u64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vsub_i128(void) {
    hal_status_t ret = HAL_OK;
    int128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                      {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
             b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                      {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    int128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_sub_i128(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vsub_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vsub_u128(void) {
    hal_status_t ret = HAL_OK;
    uint128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                       {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
              b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                       {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    uint128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_sub_u128(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vsub_u128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vwsub_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20}, b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_wsub_i64(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwsub_i64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vwsub_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20}, b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_wsub_u64(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwsub_u64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vwsub_i128(void) {
    hal_status_t ret = HAL_OK;
    int128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                      {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
             b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                      {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    int256_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_wsub_i128(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwsub_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i256(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vwsub_u128(void) {
    hal_status_t ret = HAL_OK;
    uint128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                       {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
              b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                       {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    uint256_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_wsub_u128(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwsub_u128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u256(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// -----------------------------------------------------------------------------
//  mul
// -----------------------------------------------------------------------------
static int test_hal_vmul_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_mul_i64(a[i], b[i]);
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vmul_i64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// mul_u64
static int test_hal_vmul_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    uint64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_mul_u64(a[i], b[i]);
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vmul_u64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vmul_i128(void) {
    hal_status_t ret = HAL_OK;
    int128_t a[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                      {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}},
             b[10] = {{0, 2}, {0, 3}, {0, 4}, {0, 5},  {0, 6},
                      {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}};
    int128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_mul_i128(a[i], b[i]);
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vmul_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// mul_u128
static int test_hal_vmul_u128(void) {
    hal_status_t ret = HAL_OK;
    uint128_t a[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                       {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}},
              b[10] = {{0, 2}, {0, 3}, {0, 4}, {0, 5},  {0, 6},
                       {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}};
    uint128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_mul_u128(a[i], b[i]);
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vmul_u128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}


static int test_hal_vwmul_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_wmul_i64(a[i], b[i]);
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwmul_i64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// mul_u64
static int test_hal_vwmul_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    uint128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_wmul_u64(a[i], b[i]);
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwmul_u64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vwmul_i128(void) {
    hal_status_t ret = HAL_OK;
    int128_t a[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                      {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}},
             b[10] = {{0, 2}, {0, 3}, {0, 4}, {0, 5},  {0, 6},
                      {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}};
    int256_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_wmul_i128(a[i], b[i]);
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwmul_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i256(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// mul_u128
static int test_hal_vwmul_u128(void) {
    hal_status_t ret = HAL_OK;
    uint128_t a[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                       {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}},
              b[10] = {{0, 2}, {0, 3}, {0, 4}, {0, 5},  {0, 6},
                       {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}};
    uint256_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_wmul_u128(a[i], b[i]);
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwmul_u128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u256(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// -----------------------------------------------------------------------------
//  mac
// -----------------------------------------------------------------------------
static int test_hal_vmac_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int64_t c_hw[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
    int64_t c_sw[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};

    for (int i = 0; i < 10; i++) {
        c_sw[i] += a[i] * b[i];
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vmac_i64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// mac_u64
static int test_hal_vmac_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    uint64_t c_hw[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
    uint64_t c_sw[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};

    for (int i = 0; i < 10; i++) {
        c_sw[i] += a[i] * b[i];
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vmac_u64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vmac_i128(void) {
    hal_status_t ret = HAL_OK;
    int128_t a[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                      {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}},
             b[10] = {{0, 2}, {0, 3}, {0, 4}, {0, 5},  {0, 6},
                      {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}};
    int128_t c_hw[10] = {{0, 10}, {0, 10}, {0, 10}, {0, 10},
                         {0, 10}, {0, 10}, {0, 10}, {0, 10},
                         {0, 10}, {0, 10}};
    int128_t c_sw[10] = {{0, 10}, {0, 10}, {0, 10}, {0, 10},
                         {0, 10}, {0, 10}, {0, 10}, {0, 10},
                         {0, 10}, {0, 10}};

    for (int i = 0; i < 10; i++) {
        int128_t prod = hal_mul_i128(a[i], b[i]);
        c_sw[i] = hal_add_i128(c_sw[i], prod);
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vmac_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vmac_u128(void) {
    hal_status_t ret = HAL_OK;
    uint128_t a[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                       {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}},
              b[10] = {{0, 2}, {0, 3}, {0, 4}, {0, 5},  {0, 6},
                       {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}};
    uint128_t c_hw[10] = {{0, 10}, {0, 10}, {0, 10}, {0, 10},
                         {0, 10}, {0, 10}, {0, 10}, {0, 10},
                         {0, 10}, {0, 10}};
    uint128_t c_sw[10] = {{0, 10}, {0, 10}, {0, 10}, {0, 10},
                         {0, 10}, {0, 10}, {0, 10}, {0, 10},
                         {0, 10}, {0, 10}};

    for (int i = 0; i < 10; i++) {
        uint128_t prod = hal_mul_u128(a[i], b[i]);
        c_sw[i] = hal_add_u128(c_sw[i], prod);
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vmac_u128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}


static int test_hal_vwmac_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int128_t c_hw[10] = {{0, 10}, {0, 10}, {0, 10}, {0, 10}, {0, 10},
                         {0, 10}, {0, 10}, {0, 10}, {0, 10}, {0, 10}};
    int128_t c_sw[10] = {{0, 10}, {0, 10}, {0, 10}, {0, 10}, {0, 10},
                         {0, 10}, {0, 10}, {0, 10}, {0, 10}, {0, 10}};

    for (int i = 0; i < 10; i++) {
        int128_t prod = hal_wmul_i64(a[i], b[i]);
        c_sw[i] = hal_add_i128(c_sw[i], prod);
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwmac_i64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// mac_u64
static int test_hal_vwmac_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    uint128_t c_hw[10] = {{0, 10}, {0, 10}, {0, 10}, {0, 10}, {0, 10},
                          {0, 10}, {0, 10}, {0, 10}, {0, 10}, {0, 10}};
    uint128_t c_sw[10] = {{0, 10}, {0, 10}, {0, 10}, {0, 10}, {0, 10},
                          {0, 10}, {0, 10}, {0, 10}, {0, 10}, {0, 10}};

    for (int i = 0; i < 10; i++) {
        uint128_t prod = hal_wmul_u64(a[i], b[i]);
        c_sw[i] = hal_add_u128(c_sw[i], prod);
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwmac_u64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vwmac_i128(void) {
    hal_status_t ret = HAL_OK;
    int128_t a[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                      {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}},
             b[10] = {{0, 2}, {0, 3}, {0, 4}, {0, 5},  {0, 6},
                      {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}};
    int256_t c_hw[10] = {{{0, 0, 0, 10}}, {{0, 0, 0, 10}}, {{0, 0, 0, 10}}, {{0, 0, 0, 10}},
                         {{0, 0, 0, 10}}, {{0, 0, 0, 10}}, {{0, 0, 0, 10}}, {{0, 0, 0, 10}},
                         {{0, 0, 0, 10}}, {{0, 0, 0, 10}}};
    int256_t c_sw[10] = {{{0, 0, 0, 10}}, {{0, 0, 0, 10}}, {{0, 0, 0, 10}}, {{0, 0, 0, 10}},
                         {{0, 0, 0, 10}}, {{0, 0, 0, 10}}, {{0, 0, 0, 10}}, {{0, 0, 0, 10}},
                         {{0, 0, 0, 10}}, {{0, 0, 0, 10}}};

    for (int i = 0; i < 10; i++) {
        int256_t prod = hal_wmul_i128(a[i], b[i]);
        c_sw[i] = hal_add_i256(c_sw[i], prod);
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwmac_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i256(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vwmac_u128(void) {
    hal_status_t ret = HAL_OK;
    uint128_t a[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                       {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}},
              b[10] = {{0, 2}, {0, 3}, {0, 4}, {0, 5},  {0, 6},
                       {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}};
    uint256_t c_hw[10] = {{{0, 0, 0, 10}}, {{0, 0, 0, 10}}, {{0, 0, 0, 10}}, {{0, 0, 0, 10}},
                          {{0, 0, 0, 10}}, {{0, 0, 0, 10}}, {{0, 0, 0, 10}}, {{0, 0, 0, 10}},
                          {{0, 0, 0, 10}}, {{0, 0, 0, 10}}};
    uint256_t c_sw[10] = {{{0, 0, 0, 10}}, {{0, 0, 0, 10}}, {{0, 0, 0, 10}}, {{0, 0, 0, 10}},
                          {{0, 0, 0, 10}}, {{0, 0, 0, 10}}, {{0, 0, 0, 10}}, {{0, 0, 0, 10}},
                          {{0, 0, 0, 10}}, {{0, 0, 0, 10}}};

    for (int i = 0; i < 10; i++) {
        uint256_t prod = hal_wmul_u128(a[i], b[i]);
        c_sw[i] = hal_add_u256(c_sw[i], prod);
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwmac_u128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u256(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// -----------------------------------------------------------------------------
//  div
// -----------------------------------------------------------------------------
static int test_hal_vdiv_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    /* Intentionally insert 0 at indices 2 and 7 to test exception handling. */
    int64_t b[10] = {2, 2, 0, 2, 2, 2, 2, 0, 2, 2};
    int64_t c_hw[10] = {0}, c_sw[10] = {0};
    /* Generate SW reference answer (Prevent PC crash from divide-by-zero) */
    for (int i = 0; i < 10; i++) {
        if (b[i] == 0)
            c_sw[i] = 0;
        else
            c_sw[i] = a[i] / b[i];
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vdiv_i64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* Print log if HAL function detects an error */
    if (ret != 0) {
        if (ret == 1 || ret == HAL_ERR_DIV_BY_ZERO) {
            printf("    -> [INFO] Caught expected DIV_BY_ZERO error in %s\n", __func__);
        } else {
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
        }
    }
    if (hal_verify_array_i64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vdiv_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    /* Intentionally insert 0 at indices 2 and 7 to test exception handling. */
    uint64_t b[10] = {2, 2, 0, 2, 2, 2, 2, 0, 2, 2};
    uint64_t c_hw[10] = {0}, c_sw[10] = {0};
    /* Generate SW reference answer (Prevent PC crash from divide-by-zero) */
    for (int i = 0; i < 10; i++) {
        if (b[i] == 0)
            c_sw[i] = 0;
        else
            c_sw[i] = a[i] / b[i];
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vdiv_u64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* Print log if HAL function detects an error */
    if (ret != 0) {
        if (ret == 1 || ret == HAL_ERR_DIV_BY_ZERO) {
            printf("    -> [INFO] Caught expected DIV_BY_ZERO error in %s\n", __func__);
        } else {
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
        }
    }
    if (hal_verify_array_u64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vdiv_i128(void) {
    hal_status_t ret = HAL_OK;
    int128_t a[10] = {{0, 10}, {0, 20}, {0, 30}, {0, 40}, {0, 50},
                      {0, 60}, {0, 70}, {0, 80}, {0, 90}, {0, 100}};
    /* Intentionally insert 0 at indices 2 and 7 to test exception handling. */
    int128_t b[10] = {{0, 2}, {0, 2}, {0}, {0, 2}, {0, 2}, {0, 2}, {0, 2}, {0}, {0, 2}, {0, 2}};
    int128_t c_hw[10] = {0}, c_sw[10] = {0};
    /* Generate SW reference answer (Prevent PC crash from divide-by-zero) */
    for (int i = 0; i < 10; i++) {
        if ((b[i].u == 0) && (b[i].l == 0)) {
            c_sw[i].u = 0;
            c_sw[i].l = 0;
        } else {
            c_sw[i] = hal_div_i128(a[i], b[i]);
        }
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vdiv_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* Print log if HAL function detects an error */
    if (ret != 0) {
        if (ret == 1 || ret == HAL_ERR_DIV_BY_ZERO) {
            printf("    -> [INFO] Caught expected DIV_BY_ZERO error in %s\n", __func__);
        } else {
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
        }
    }
    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vdiv_u128(void) {
    hal_status_t ret = HAL_OK;
    uint128_t a[10] = {{0, 10}, {0, 20}, {0, 30}, {0, 40}, {0, 50},
                       {0, 60}, {0, 70}, {0, 80}, {0, 90}, {0, 100}};
    /* Intentionally insert 0 at indices 2 and 7 to test exception handling. */
    uint128_t b[10] = {{0, 2}, {0, 2}, {0}, {0, 2}, {0, 2}, {0, 2}, {0, 2}, {0}, {0, 2}, {0, 2}};
    uint128_t c_hw[10] = {0}, c_sw[10] = {0};
    /* Generate SW reference answer (Prevent PC crash from divide-by-zero) */
    for (int i = 0; i < 10; i++) {
        if ((b[i].u == 0) && (b[i].l == 0)) {
            c_sw[i].u = 0;
            c_sw[i].l = 0;
        } else {
            c_sw[i] = hal_div_u128(a[i], b[i]);
        }
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vdiv_u128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* Print log if HAL function detects an error */
    if (ret != 0) {
        if (ret == 1 || ret == HAL_ERR_DIV_BY_ZERO) {
            printf("    -> [INFO] Caught expected DIV_BY_ZERO error in %s\n", __func__);
        } else {
            printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
        }
    }
    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// -----------------------------------------------------------------------------
//  dot
// -----------------------------------------------------------------------------
static int test_hal_vdot_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int64_t c_hw = {0}, c_sw = {0};
    for (int i = 0; i < 10; i++) {
        c_sw += a[i] * b[i];
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vdot_i64(&c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i64(&c_sw, &c_hw, 1)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vdot_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    uint64_t c_hw = {0}, c_sw = {0};
    for (int i = 0; i < 10; i++) {
        c_sw += a[i] * b[i];
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vdot_u64(&c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u64(&c_sw, &c_hw, 1)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vdot_i128(void) {
    hal_status_t ret = HAL_OK;
    int128_t a[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                      {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}},
             b[10] = {{0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1},
                      {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}};
    int128_t c_hw = {0}, c_sw = {0};
    for (int i = 0; i < 10; i++) {
        int128_t prod = hal_mul_i128(a[i], b[i]);
        c_sw = hal_add_i128(c_sw, prod);
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vdot_i128(&c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_result_i128(c_sw, c_hw)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// dot_u128
static int test_hal_vdot_u128(void) {
    hal_status_t ret = HAL_OK;
    uint128_t a[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                       {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}},
              b[10] = {{0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1},
                       {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}};
    uint128_t c_hw = {0}, c_sw = {0};
    for (int i = 0; i < 10; i++) {
        uint128_t prod = hal_mul_u128(a[i], b[i]);
        c_sw = hal_add_u128(c_sw, prod);
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vdot_u128(&c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_result_u128(c_sw, c_hw)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vwdot_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int128_t c_hw = {0}, c_sw = {0};
    for (int i = 0; i < 10; i++) {
        int128_t prod = hal_wmul_i64(a[i], b[i]);
        c_sw = hal_add_i128(c_sw, prod);
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwdot_i64(&c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(&c_sw, &c_hw, 1)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vwdot_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    uint128_t c_hw = {0}, c_sw = {0};
    for (int i = 0; i < 10; i++) {
        uint128_t prod = hal_wmul_u64(a[i], b[i]);
        c_sw = hal_add_u128(c_sw, prod);
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwdot_u64(&c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(&c_sw, &c_hw, 1)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vwdot_i128(void) {
    hal_status_t ret = HAL_OK;
    int128_t a[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                      {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}},
             b[10] = {{0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1},
                      {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}};
    int256_t c_hw = {0}, c_sw = {0};
    for (int i = 0; i < 10; i++) {
        int256_t prod = hal_wmul_i128(a[i], b[i]);
        c_sw = hal_add_i256(c_sw, prod);
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwdot_i128(&c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_result_i256(c_sw, c_hw)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// dot_u128
static int test_hal_vwdot_u128(void) {
    hal_status_t ret = HAL_OK;
    uint128_t a[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                       {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}},
              b[10] = {{0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1},
                       {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}};
    uint256_t c_hw = {0}, c_sw = {0};
    for (int i = 0; i < 10; i++) {
        uint256_t prod = hal_wmul_u128(a[i], b[i]);
        c_sw = hal_add_u256(c_sw, prod);
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwdot_u128(&c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_result_u256(c_sw, c_hw)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// -----------------------------------------------------------------------------
// matrix mul
// -----------------------------------------------------------------------------
static int test_hal_mtrx_vmul_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[6] = {1, 2, 3, 4, 5, 6}, b[6] = {1, 2, 3, 4, 5, 6};
    int64_t c_hw[4] = {0}, c_sw[4] = {0};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int64_t sum = {0};
            for (int k = 0; k < 3; k++) {
                sum += a[i * 3 + k] * b[k * 2 + j];
            }
            c_sw[i * 2 + j] = sum;
        }
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    hal_matrix_vmul_i64(c_hw, a, b, 2, 2, 3);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i64(c_sw, c_hw, 4)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// matrix_mul_u64
static int test_hal_mtrx_vmul_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[6] = {1, 2, 3, 4, 5, 6}, b[6] = {1, 2, 3, 4, 5, 6};
    uint64_t c_hw[4] = {0}, c_sw[4] = {0};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            uint64_t sum = {0};
            for (int k = 0; k < 3; k++) {
                sum += a[i * 3 + k] * b[k * 2 + j];
            }
            c_sw[i * 2 + j] = sum;
        }
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    hal_matrix_vmul_u64(c_hw, a, b, 2, 2, 3);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u64(c_sw, c_hw, 4)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_mtrx_vmul_i128(void) {
    hal_status_t ret = HAL_OK;
    int128_t a[6] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}},
             b[6] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}};
    int128_t c_hw[4] = {0}, c_sw[4] = {0};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int128_t sum = {0};
            for (int k = 0; k < 3; k++) {
                sum = hal_add_i128(hal_mul_i128(a[i * 3 + k], b[k * 2 + j]), sum);
            }
            c_sw[i * 2 + j] = sum;
        }
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    hal_matrix_vmul_i128(c_hw, a, b, 2, 2, 3);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(c_sw, c_hw, 4)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// matrix_mul_u128
static int test_hal_mtrx_vmul_u128(void) {
    hal_status_t ret = HAL_OK;
    uint128_t a[6] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}},
              b[6] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}};
    uint128_t c_hw[4] = {0}, c_sw[4] = {0};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            uint128_t sum = {0};
            for (int k = 0; k < 3; k++) {
                sum = hal_add_u128(hal_mul_u128(a[i * 3 + k], b[k * 2 + j]), sum);
            }
            c_sw[i * 2 + j] = sum;
        }
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    hal_matrix_vmul_u128(c_hw, a, b, 2, 2, 3);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(c_sw, c_hw, 4)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_mtrx_vwmul_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[6] = {1, 2, 3, 4, 5, 6}, b[6] = {1, 2, 3, 4, 5, 6};
    int128_t c_hw[4] = {0}, c_sw[4] = {0};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int128_t sum = {0};
            for (int k = 0; k < 3; k++) {
                sum = hal_add_i128(hal_wmul_i64(a[i * 3 + k], b[k * 2 + j]), sum);
            }
            c_sw[i * 2 + j] = sum;
        }
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    hal_matrix_vwmul_i64(c_hw, a, b, 2, 2, 3);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(c_sw, c_hw, 4)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// matrix_mul_u64
static int test_hal_mtrx_vwmul_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[6] = {1, 2, 3, 4, 5, 6}, b[6] = {1, 2, 3, 4, 5, 6};
    uint128_t c_hw[4] = {0}, c_sw[4] = {0};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            uint128_t sum = {0};
            for (int k = 0; k < 3; k++) {
                sum = hal_add_u128(hal_wmul_u64(a[i * 3 + k], b[k * 2 + j]), sum);
            }
            c_sw[i * 2 + j] = sum;
        }
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    hal_matrix_vwmul_u64(c_hw, a, b, 2, 2, 3);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(c_sw, c_hw, 4)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_mtrx_vwmul_i128(void) {
    hal_status_t ret = HAL_OK;
    int128_t a[6] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}},
             b[6] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}};
    int256_t c_hw[4] = {0}, c_sw[4] = {0};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int256_t sum = {0};
            for (int k = 0; k < 3; k++) {
                sum = hal_add_i256(hal_wmul_i128(a[i * 3 + k], b[k * 2 + j]), sum);
            }
            c_sw[i * 2 + j] = sum;
        }
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    hal_matrix_vwmul_i128(c_hw, a, b, 2, 2, 3);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i256(c_sw, c_hw, 4)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// matrix_mul_u128
static int test_hal_mtrx_vwmul_u128(void) {
    hal_status_t ret = HAL_OK;
    uint128_t a[6] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}},
              b[6] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}};
    uint256_t c_hw[4] = {0}, c_sw[4] = {0};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            uint256_t sum = {0};
            for (int k = 0; k < 3; k++) {
                sum = hal_add_u256(hal_wmul_u128(a[i * 3 + k], b[k * 2 + j]), sum);
            }
            c_sw[i * 2 + j] = sum;
        }
    }
    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    hal_matrix_vwmul_u128(c_hw, a, b, 2, 2, 3);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u256(c_sw, c_hw, 4)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// -----------------------------------------------------------------------------
// matrix mul tiled
// -----------------------------------------------------------------------------
static int test_hal_mtrx_vmul_tiled_i64(void) {
    hal_status_t ret = HAL_OK;
    int result = -1;
    int M = 131, N = 71, K = 79;
    int tile_size = 64;
    int64_t *A = (int64_t *)malloc(sizeof(int64_t) * M * K);
    int64_t *B = (int64_t *)malloc(sizeof(int64_t) * K * N);
    int64_t *CS = (int64_t *)malloc(sizeof(int64_t) * M * N);
    int64_t *CH = (int64_t *)malloc(sizeof(int64_t) * M * N);
    int64_t (*pa)[K] = (int64_t (*)[K])A;
    int64_t (*pb)[N] = (int64_t (*)[N])B;
    int64_t(*ps)[N] = (int64_t(*)[N])CS;

    memset(A, 0, sizeof(int64_t) * M * K);
    memset(B, 0, sizeof(int64_t) * K * N);
    memset(CS, 0, sizeof(int64_t) * M * N);
    memset(CH, 0, sizeof(int64_t) * M * N);

    const int8_t (*ta)[K] = (const int8_t (*)[K])test_mat_a;
    const int8_t (*tb)[N] = (const int8_t (*)[N])test_mat_b;

    // fill matrix
    int i, j;
    // a[]
    for (i = 0; i < M; i++) {
        for (j = 0; j < K; j++) {
            pa[i][j] = (int64_t)ta[i][j];
        }
    }
    // b[]
    for (i = 0; i < K; i++) {
        for (j = 0; j < N; j++) {
            pb[i][j] = (int64_t)tb[i][j];
        }
    }
    // c[]
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    ps[i][j] += pa[i][k] * pb[k][j];
                }
            }
        }
    }
    /* --- Profiling Start --- */
    uint64_t start_hal = get_mcycle();
    hal_matrix_vmul_tiled_i64(CH, A, B, M, N, K, tile_size);
    uint64_t end_hal = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i64(CS, CH, M * N)) {
        result = (int)(end_hal - start_hal); /* Return elapsed clock cycles on success */
    }
    SAFE_FREE(A);
    SAFE_FREE(B);
    SAFE_FREE(CS);
    SAFE_FREE(CH);
    return result;
}

// mtrx_mul_tile_u64
static int test_hal_mtrx_vmul_tiled_u64(void) {
    hal_status_t ret = HAL_OK;
    int result = -1;
    int M = 131, N = 71, K = 79;
    int tile_size = 64;
    uint64_t *A = (uint64_t *)malloc(sizeof(uint64_t) * M * K);
    uint64_t *B = (uint64_t *)malloc(sizeof(uint64_t) * K * N);
    uint64_t *CS = (uint64_t *)malloc(sizeof(uint64_t) * M * N);
    uint64_t *CH = (uint64_t *)malloc(sizeof(uint64_t) * M * N);
    uint64_t (*pa)[K] = (uint64_t (*)[K])A;
    uint64_t (*pb)[N] = (uint64_t (*)[N])B;
    uint64_t(*ps)[N] = (uint64_t(*)[N])CS;

    memset(A, 0, sizeof(uint64_t) * M * K);
    memset(B, 0, sizeof(uint64_t) * K * N);
    memset(CS, 0, sizeof(uint64_t) * M * N);
    memset(CH, 0, sizeof(uint64_t) * M * N);

    const int8_t (*ta)[K] = (const int8_t (*)[K])test_mat_a;
    const int8_t (*tb)[K] = (const int8_t (*)[N])test_mat_b;

    // fill matrix
    int i, j;
    // a[]
    for (i = 0; i < M; i++) {
        for (j = 0; j < K; j++) {
            pa[i][j] = (uint64_t)ta[i][j];
        }
    }
    // b[]
    for (i = 0; i < K; i++) {
        for (j = 0; j < N; j++) {
            pb[i][j] = (uint64_t)tb[i][j];
        }
    }
    // c[]
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    ps[i][j] += pa[i][k] * pb[k][j];
                }
            }
        }
    }
    /* --- Profiling Start --- */
    uint64_t start_hal = get_mcycle();
    hal_matrix_vmul_tiled_u64(CH, A, B, M, N, K, tile_size);
    uint64_t end_hal = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u64(CS, CH, M * N)) {
        result = (int)(end_hal - start_hal); /* Return elapsed clock cycles on success */
    }

    SAFE_FREE(A);
    SAFE_FREE(B);
    SAFE_FREE(CS);
    SAFE_FREE(CH);

    return result;
}

static int test_hal_mtrx_vmul_tiled_i128(void) {
    hal_status_t ret = HAL_OK;
    int result = -1;
    int M = 131, N = 71, K = 79;
    int tile_size = 64;
    int128_t *A = (int128_t *)malloc(sizeof(int128_t) * M * K);
    int128_t *B = (int128_t *)malloc(sizeof(int128_t) * K * N);
    int128_t *CS = (int128_t *)malloc(sizeof(int128_t) * M * N);
    int128_t *CH = (int128_t *)malloc(sizeof(int128_t) * M * N);
    int128_t(*pa)[K] = (int128_t(*)[K])A;
    int128_t(*pb)[N] = (int128_t(*)[N])B;
    int128_t(*ps)[N] = (int128_t(*)[N])CS;

    memset(A, 0, sizeof(int128_t) * M * K);
    memset(B, 0, sizeof(int128_t) * K * N);
    memset(CS, 0, sizeof(int128_t) * M * N);
    memset(CH, 0, sizeof(int128_t) * M * N);

    const int8_t (*ta)[K] = (const int8_t (*)[K])test_mat_a;
    const int8_t (*tb)[K] = (const int8_t (*)[N])test_mat_b;

    // fill matrix
    int i, j;
    // a[]
    for (i = 0; i < M; i++) {
        for (j = 0; j < K; j++) {
            pa[i][j].l = (int64_t)ta[i][j];
            pa[i][j].u = (ta[i][j] < 0) ? -1 : 0;
        }
    }
    // b[]
    for (i = 0; i < K; i++) {
        for (j = 0; j < N; j++) {
            pb[i][j].l = (int64_t)tb[i][j];
            pb[i][j].u = (tb[i][j] < 0) ? -1 : 0;
        }
    }
    // c[]
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k].l != 0 || pa[i][k].u != 0) {
                for (int j = 0; j < N; j++) {
                    int128_t prod = hal_mul_i128(pa[i][k], pb[k][j]);
                    ps[i][j] = hal_add_i128(ps[i][j], prod);
                }
            }
        }
    }
    /* --- Profiling Start --- */
    uint64_t start_hal = get_mcycle();
    hal_matrix_vmul_tiled_i128(CH, A, B, M, N, K, tile_size);
    uint64_t end_hal = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(CS, CH, M * N)) {
        result = (int)(end_hal - start_hal); /* Return elapsed clock cycles on success */
    }

    SAFE_FREE(A);
    SAFE_FREE(B);
    SAFE_FREE(CS);
    SAFE_FREE(CH);

    return result;
}

// mtrx_mul_tile_u128
static int test_hal_mtrx_vmul_tiled_u128(void) {
    hal_status_t ret = HAL_OK;
    int result = -1;
    int M = 131, N = 71, K = 79;
    int tile_size = 64;
    uint128_t *A = (uint128_t *)malloc(sizeof(uint128_t) * M * K);
    uint128_t *B = (uint128_t *)malloc(sizeof(uint128_t) * K * N);
    uint128_t *CS = (uint128_t *)malloc(sizeof(uint128_t) * M * N);
    uint128_t *CH = (uint128_t *)malloc(sizeof(uint128_t) * M * N);
    uint128_t(*pa)[K] = (uint128_t(*)[K])A;
    uint128_t(*pb)[N] = (uint128_t(*)[N])B;
    uint128_t(*ps)[N] = (uint128_t(*)[N])CS;

    memset(A, 0, sizeof(uint128_t) * M * K);
    memset(B, 0, sizeof(uint128_t) * K * N);
    memset(CS, 0, sizeof(uint128_t) * M * N);
    memset(CH, 0, sizeof(uint128_t) * M * N);

    const int8_t (*ta)[K] = (const int8_t (*)[K])test_mat_a;
    const int8_t (*tb)[K] = (const int8_t (*)[N])test_mat_b;

    // fill matrix
    int i, j;
    // a[]
    for (i = 0; i < M; i++) {
        for (j = 0; j < K; j++) {
            pa[i][j].u = 0;
            pa[i][j].l = (uint64_t)ta[i][j];
        }
    }
    // b[]
    for (i = 0; i < K; i++) {
        for (j = 0; j < N; j++) {
            pb[i][j].l = (uint64_t)tb[i][j];
        }
    }

    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k].l != 0 || pa[i][k].u != 0) {
                for (int j = 0; j < N; j++) {
                    uint128_t prod = hal_mul_u128(pa[i][k], pb[k][j]);
                    ps[i][j] = hal_add_u128(ps[i][j], prod);
                }
            }
        }
    }

    /* --- Profiling Start --- */
    uint64_t start_hal = get_mcycle();
    hal_matrix_vmul_tiled_u128(CH, A, B, M, N, K, tile_size);
    uint64_t end_hal = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(CS, CH, M * N)) {
        result = (int)(end_hal - start_hal); /* Return elapsed clock cycles on success */
    }

    SAFE_FREE(A);
    SAFE_FREE(B);
    SAFE_FREE(CS);
    SAFE_FREE(CH);

    return result;
}

static int test_hal_mtrx_vwmul_tiled_i64(void) {
    hal_status_t ret = HAL_OK;
    int result = -1;
    int M = 131, N = 71, K = 79;
    int tile_size = 64;
    int64_t *A = (int64_t *)malloc(sizeof(int64_t) * M * K);
    int64_t *B = (int64_t *)malloc(sizeof(int64_t) * K * N);
    int128_t *CS = (int128_t *)malloc(sizeof(int128_t) * M * N);
    int128_t *CH = (int128_t *)malloc(sizeof(int128_t) * M * N);
    int64_t (*pa)[K] = (int64_t (*)[K])A;
    int64_t (*pb)[N] = (int64_t (*)[N])B;
    int128_t(*ps)[N] = (int128_t(*)[N])CS;

    memset(A, 0, sizeof(int64_t) * M * K);
    memset(B, 0, sizeof(int64_t) * K * N);
    memset(CS, 0, sizeof(int128_t) * M * N);
    memset(CH, 0, sizeof(int128_t) * M * N);

    const int8_t (*ta)[K] = (const int8_t (*)[K])test_mat_a;
    const int8_t (*tb)[N] = (const int8_t (*)[N])test_mat_b;

    // fill matrix
    int i, j;
    // a[]
    for (i = 0; i < M; i++) {
        for (j = 0; j < K; j++) {
            pa[i][j] = (int64_t)ta[i][j];
        }
    }
    // b[]
    for (i = 0; i < K; i++) {
        for (j = 0; j < N; j++) {
            pb[i][j] = (int64_t)tb[i][j];
        }
    }
    // c[]
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    int128_t prod = hal_wmul_i64(pa[i][k], pb[k][j]);
                    ps[i][j] = hal_add_i128(ps[i][j], prod);
                }
            }
        }
    }
    /* --- Profiling Start --- */
    uint64_t start_hal = get_mcycle();
    hal_matrix_vwmul_tiled_i64(CH, A, B, M, N, K, tile_size);
    uint64_t end_hal = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(CS, CH, M * N)) {
        result = (int)(end_hal - start_hal); /* Return elapsed clock cycles on success */
    }
    SAFE_FREE(A);
    SAFE_FREE(B);
    SAFE_FREE(CS);
    SAFE_FREE(CH);
    return result;
}

// mtrx_mul_tile_u64
static int test_hal_mtrx_vwmul_tiled_u64(void) {
    hal_status_t ret = HAL_OK;
    int result = -1;
    int M = 131, N = 71, K = 79;
    int tile_size = 64;
    uint64_t *A = (uint64_t *)malloc(sizeof(uint64_t) * M * K);
    uint64_t *B = (uint64_t *)malloc(sizeof(uint64_t) * K * N);
    uint128_t *CS = (uint128_t *)malloc(sizeof(uint128_t) * M * N);
    uint128_t *CH = (uint128_t *)malloc(sizeof(uint128_t) * M * N);
    uint64_t (*pa)[K] = (uint64_t (*)[K])A;
    uint64_t (*pb)[N] = (uint64_t (*)[N])B;
    uint128_t(*ps)[N] = (uint128_t(*)[N])CS;

    memset(A, 0, sizeof(uint64_t) * M * K);
    memset(B, 0, sizeof(uint64_t) * K * N);
    memset(CS, 0, sizeof(uint128_t) * M * N);
    memset(CH, 0, sizeof(uint128_t) * M * N);

    const int8_t (*ta)[K] = (const int8_t (*)[K])test_mat_a;
    const int8_t (*tb)[K] = (const int8_t (*)[N])test_mat_b;

    // fill matrix
    int i, j;
    // a[]
    for (i = 0; i < M; i++) {
        for (j = 0; j < K; j++) {
            pa[i][j] = (uint64_t)ta[i][j];
        }
    }
    // b[]
    for (i = 0; i < K; i++) {
        for (j = 0; j < N; j++) {
            pb[i][j] = (uint64_t)tb[i][j];
        }
    }
    // c[]
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    uint128_t prod = hal_wmul_u64(pa[i][k], pb[k][j]);
                    ps[i][j] = hal_add_u128(ps[i][j], prod);
                }
            }
        }
    }
    /* --- Profiling Start --- */
    uint64_t start_hal = get_mcycle();
    hal_matrix_vwmul_tiled_u64(CH, A, B, M, N, K, tile_size);
    uint64_t end_hal = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(CS, CH, M * N)) {
        result = (int)(end_hal - start_hal); /* Return elapsed clock cycles on success */
    }

    SAFE_FREE(A);
    SAFE_FREE(B);
    SAFE_FREE(CS);
    SAFE_FREE(CH);

    return result;
}

#if 1
static int test_hal_mtrx_vwmul_tiled_i128(void) {
    hal_status_t ret = HAL_OK;
    int result = -1;
    int M = 131, N = 71, K = 79;
    int tile_size = 64;
    int128_t *A = (int128_t *)malloc(sizeof(int128_t) * M * K);
    int128_t *B = (int128_t *)malloc(sizeof(int128_t) * K * N);
    int256_t *CS = (int256_t *)malloc(sizeof(int256_t) * M * N);
    int256_t *CH = (int256_t *)malloc(sizeof(int256_t) * M * N);
    int128_t(*pa)[K] = (int128_t(*)[K])A;
    int128_t(*pb)[N] = (int128_t(*)[N])B;
    int256_t(*ps)[N] = (int256_t(*)[N])CS;

    memset(A, 0, sizeof(int128_t) * M * K);
    memset(B, 0, sizeof(int128_t) * K * N);
    memset(CS, 0, sizeof(int256_t) * M * N);
    memset(CH, 0, sizeof(int256_t) * M * N);

    const int8_t (*ta)[K] = (const int8_t (*)[K])test_mat_a;
    const int8_t (*tb)[K] = (const int8_t (*)[N])test_mat_b;

    // fill matrix
    int i, j;
    // a[]
    for (i = 0; i < M; i++) {
        for (j = 0; j < K; j++) {
            pa[i][j].l = (int64_t)ta[i][j];
            pa[i][j].u = (ta[i][j] < 0) ? -1 : 0;
        }
    }
    // b[]
    for (i = 0; i < K; i++) {
        for (j = 0; j < N; j++) {
            pb[i][j].l = (int64_t)tb[i][j];
            pb[i][j].u = (tb[i][j] < 0) ? -1 : 0;
        }
    }
    // c[]
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k].l != 0 || pa[i][k].u != 0) {
                for (int j = 0; j < N; j++) {
                    int256_t prod = hal_wmul_i128(pa[i][k], pb[k][j]);
                    ps[i][j] = hal_add_i256(ps[i][j], prod);
                }
            }
        }
    }
    /* --- Profiling Start --- */
    uint64_t start_hal = get_mcycle();
    hal_matrix_vwmul_tiled_i128(CH, A, B, M, N, K, tile_size);
    uint64_t end_hal = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i256(CS, CH, M * N)) {
        result = (int)(end_hal - start_hal); /* Return elapsed clock cycles on success */
    }

    SAFE_FREE(A);
    SAFE_FREE(B);
    SAFE_FREE(CS);
    SAFE_FREE(CH);

    return result;
}


// mtrx_mul_tile_u128
static int test_hal_mtrx_vwmul_tiled_u128(void) {
    hal_status_t ret = HAL_OK;
    int result = -1;
    int M = 131, N = 71, K = 79;
    int tile_size = 64;
    uint128_t *A = (uint128_t *)malloc(sizeof(uint128_t) * M * K);
    uint128_t *B = (uint128_t *)malloc(sizeof(uint128_t) * K * N);
    uint256_t *CS = (uint256_t *)malloc(sizeof(uint256_t) * M * N);
    uint256_t *CH = (uint256_t *)malloc(sizeof(uint256_t) * M * N);
    uint128_t(*pa)[K] = (uint128_t(*)[K])A;
    uint128_t(*pb)[N] = (uint128_t(*)[N])B;
    uint256_t(*ps)[N] = (uint256_t(*)[N])CS;

    memset(A, 0, sizeof(uint128_t) * M * K);
    memset(B, 0, sizeof(uint128_t) * K * N);
    memset(CS, 0, sizeof(uint256_t) * M * N);
    memset(CH, 0, sizeof(uint256_t) * M * N);

    const int8_t (*ta)[K] = (const int8_t (*)[K])test_mat_a;
    const int8_t (*tb)[K] = (const int8_t (*)[N])test_mat_b;

    // fill matrix
    int i, j;
    // a[]
    for (i = 0; i < M; i++) {
        for (j = 0; j < K; j++) {
            pa[i][j].u = 0;
            pa[i][j].l = (uint64_t)ta[i][j];
        }
    }
    // b[]
    for (i = 0; i < K; i++) {
        for (j = 0; j < N; j++) {
            pb[i][j].l = (uint64_t)tb[i][j];
        }
    }

    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k].l != 0 || pa[i][k].u != 0) {
                for (int j = 0; j < N; j++) {
                    uint256_t prod = hal_wmul_u128(pa[i][k], pb[k][j]);
                    ps[i][j] = hal_add_u256(ps[i][j], prod);
                }
            }
        }
    }

    /* --- Profiling Start --- */
    uint64_t start_hal = get_mcycle();
    hal_matrix_vwmul_tiled_u128(CH, A, B, M, N, K, tile_size);
    uint64_t end_hal = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u256(CS, CH, M * N)) {
        result = (int)(end_hal - start_hal); /* Return elapsed clock cycles on success */
    }

    SAFE_FREE(A);
    SAFE_FREE(B);
    SAFE_FREE(CS);
    SAFE_FREE(CH);

    return result;
}
#endif

// -----------------------------------------------------------------------------
// vmin
// -----------------------------------------------------------------------------
// vmin_u64
static int test_hal_vmin_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20}, b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = MIN(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vmin_u64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}


// vmin_i64
static int test_hal_vmin_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20}, b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = MIN(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vmin_i64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}


static int test_hal_vmin_u128(void) {
    hal_status_t ret = HAL_OK;
    uint128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                       {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
              b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                       {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    uint128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_min_u128(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vmin_u128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// vmin_i128
static int test_hal_vmin_i128(void) {
    hal_status_t ret = HAL_OK;
    int128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                      {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
             b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                      {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    int128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_min_i128(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vmin_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// -----------------------------------------------------------------------------
// vmax
// -----------------------------------------------------------------------------

static int test_hal_vmax_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20}, b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = MAX(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vmax_u64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}


// vmax_i64
static int test_hal_vmax_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20}, b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = MAX(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vmax_i64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}


static int test_hal_vmax_u128(void) {
    hal_status_t ret = HAL_OK;
    uint128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                       {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
              b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                       {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    uint128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_max_u128(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vmax_u128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// -----------------------------------------------------------------------------
// max
// -----------------------------------------------------------------------------

static int test_hal_vmax_i128(void) {
    hal_status_t ret = HAL_OK;
    const int128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                            {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
                   b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                            {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    int128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = hal_max_i128(a[i], b[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vmax_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// -----------------------------------------------------------------------------
// sqrt
// -----------------------------------------------------------------------------

static int test_hal_vsqrt_f32(void) {
    hal_status_t ret = HAL_OK;
    float a[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    float c_hw[10] = {0.0f}, c_sw[10] = {0.0f};
    for (int i = 0; i < 10; i++)
        c_sw[i] = sqrtf(a[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vsqrt_f32(c_hw, a, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_f32(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vrsqrt7_f32(void) {
    hal_status_t ret = HAL_OK;
    float a[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    float c_hw[10] = {0.0f}, c_sw[10] = {0.0f};
    for (int i = 0; i < 10; i++)
        c_sw[i] = sqrtf(a[i]);

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vsqrt_f32(c_hw, a, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_f32(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}


// -----------------------------------------------------------------------------
// Bitwise and(&)
// -----------------------------------------------------------------------------
static int test_hal_vand_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},
            b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = a[i] & b[i];

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vand_u64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */

    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vand_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},
            b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = a[i] & b[i];

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vand_i64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */

    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vand_u128(void) {
    hal_status_t ret = HAL_OK;
    const uint128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                        {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
                b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                        {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    uint128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++) {
        c_sw[i].l = a[i].l & b[i].l;
        c_sw[i].u = a[i].u & b[i].u;
    }

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vand_u128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */

    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vand_i128(void) {
    hal_status_t ret = HAL_OK;
    const int128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                            {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
                   b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                            {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    int128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++) {
        c_sw[i].l = a[i].l & b[i].l;
        c_sw[i].u = a[i].u & b[i].u;
    }

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vand_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */

    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}


// -----------------------------------------------------------------------------
// Bitwise or(|)
// -----------------------------------------------------------------------------
static int test_hal_vor_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},
            b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = a[i] | b[i];

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vor_u64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */

    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vor_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},
            b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = a[i] | b[i];

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vor_i64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */

    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vor_u128(void) {
    hal_status_t ret = HAL_OK;
    const uint128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                        {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
                b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                        {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    uint128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++) {
        c_sw[i].l = a[i].l | b[i].l;
        c_sw[i].u = a[i].u | b[i].u;
    }

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vor_u128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */

    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vor_i128(void) {
    hal_status_t ret = HAL_OK;
    const int128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                            {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
                   b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                            {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    int128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++) {
        c_sw[i].l = a[i].l | b[i].l;
        c_sw[i].u = a[i].u | b[i].u;
    }

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vor_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */

    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// -----------------------------------------------------------------------------
// Bitwise xor(^)
// -----------------------------------------------------------------------------
static int test_hal_vxor_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},
            b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = a[i] ^ b[i];

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vxor_u64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */

    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected errxor code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vxor_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},
            b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = a[i] ^ b[i];

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vxor_i64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */

    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected errxor code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vxor_u128(void) {
    hal_status_t ret = HAL_OK;
    const uint128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                        {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
                b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                        {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    uint128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++) {
        c_sw[i].l = a[i].l ^ b[i].l;
        c_sw[i].u = a[i].u ^ b[i].u;
    }

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vxor_u128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */

    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected errxor code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vxor_i128(void) {
    hal_status_t ret = HAL_OK;
    const int128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                            {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}},
                   b[10] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
                            {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}};
    int128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++) {
        c_sw[i].l = a[i].l ^ b[i].l;
        c_sw[i].u = a[i].u ^ b[i].u;
    }

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vxor_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */

    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected errxor code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}


// -----------------------------------------------------------------------------
// Bitwise not(^)
// -----------------------------------------------------------------------------
static int test_hal_vnot_u64(void) {
    hal_status_t ret = HAL_OK;
    uint64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    uint64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = ~a[i];

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vnot_u64(c_hw, a, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */

    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected errnot code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vnot_i64(void) {
    hal_status_t ret = HAL_OK;
    int64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    int64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = ~a[i];

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vnot_i64(c_hw, a, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */

    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected errnot code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vnot_u128(void) {
    hal_status_t ret = HAL_OK;
    const uint128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                        {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}};
    uint128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++) {
        c_sw[i].l = ~a[i].l;
        c_sw[i].u = ~a[i].u;
    }

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vnot_u128(c_hw, a, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */

    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected errnot code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

static int test_hal_vnot_i128(void) {
    hal_status_t ret = HAL_OK;
    const int128_t a[10] = {{0, 2},  {0, 4},  {0, 6},  {0, 8},  {0, 10},
                            {0, 12}, {0, 14}, {0, 16}, {0, 18}, {0, 20}};
    int128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++) {
        c_sw[i].l = ~a[i].l;
        c_sw[i].u = ~a[i].u;
    }

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vnot_i128(c_hw, a, 10);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */

    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected errnot code %d in %s\n", ret, __func__);
    }
    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* Return elapsed clock cycles on success */
    } else {
        return -1; /* Return -1 on failure */
    }
}

// =============================================================================
// [Edge Case Test] Integer overflow/underflow verification
// =============================================================================

// -----------------------------------------------------------------------------
// [Edge Case Test 1] Integer overflow/underflow verification (i32)
// -----------------------------------------------------------------------------
static int test_hal_edge_overflow_i32(void) {
    hal_status_t ret = HAL_OK;
    const size_t LEN = 4;
    // Boundary values: [Max, Min, Normal, -1]
    int32_t a[4] = {INT32_MAX, INT32_MIN, 100, -1};
    int32_t b[4] = {1, -1, 200, 1};
    int32_t c_hw[4] = {0};
    int32_t c_sw[4] = {0};

    // SW Calculation: Cast to Unsigned to avoid Undefined Behavior (UB) in C
    for (size_t i = 0; i < LEN; i++) {
        c_sw[i] = (int32_t)((uint32_t)a[i] + (uint32_t)b[i]);
    }

    ret = hal_vadd_i32(c_hw, a, b, LEN);
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    // Verify if INT32_MAX + 1 correctly rolls over to INT32_MIN
    return hal_verify_array_i32(c_sw, c_hw, LEN) ? 0 : -1;
}

// -----------------------------------------------------------------------------
// [Edge Case Test 2] widening overflow/undefined verification (i32)
// -----------------------------------------------------------------------------
static int test_hal_widening_i32(void) {
    hal_status_t ret = HAL_OK;
    const size_t LEN = 7;
    // Boundary values: [Max, Min, Normal, -1, ...]
    int32_t a[7] = {INT32_MAX, INT32_MIN, 100, -1, -2000000000, INT32_MAX, INT32_MIN};
    int32_t b[7] = {1, -1, 200, 1, 1000000000, INT32_MAX, INT32_MIN};
    int64_t c_hw[7] = {0};
    int64_t c_sw[7] = {0};

    // SW Calculation: Cast to Unsigned to avoid Undefined Behavior (UB) in C
    for (size_t i = 0; i < LEN; i++) {
        c_sw[i] = (int64_t)((int64_t)a[i] + (int64_t)b[i]);
    }

    ret = hal_vwadd_i32(c_hw, a, b, LEN);
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    // Verify if INT32_MAX + 1 correctly rolls over to INT32_MIN
    return hal_verify_array_i64(c_sw, c_hw, LEN) ? 0 : -1;
}

// -----------------------------------------------------------------------------
// [Edge Case Test 3] widening overflow/undefined verification (u32)
// -----------------------------------------------------------------------------
static int test_hal_widening_u32(void) {
    hal_status_t ret = HAL_OK;
    const size_t LEN = 5;

    // Boundary values: [Max+Max, Max+1, MSB+MSB, Zero, Normal]
    uint32_t a[5] = {UINT32_MAX, UINT32_MAX, 0x80000000, 0, 100};
    uint32_t b[5] = {UINT32_MAX, 1, 0x80000000, 0, 200};

    // Widening 결과는 64-bit 공간에 저장됨
    uint64_t c_hw[5] = {0};
    uint64_t c_sw[5] = {0};

    // SW Calculation: 32비트에서 캐리가 유실(Wrap-around)되는 것을 막기 위해
    // 연산 전에 반드시 64비트로 캐스팅(Zero Extension) 후 더합니다.
    for (size_t i = 0; i < LEN; i++) {
        c_sw[i] = (uint64_t)a[i] + (uint64_t)b[i];
    }


    // Unsigned Widening Add 함수 호출 (이름이 vwaddu 임에 주의)
    ret = hal_vwadd_u32(c_hw, a, b, LEN);

    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }

    // Verify: 64-bit 부호 없는 정수 전용 검증 함수 사용
    return hal_verify_array_u64(c_sw, c_hw, LEN) ? 0 : -1;
}

// -----------------------------------------------------------------------------
// [Edge Case Test 4] Floating-point extreme values verification (f32)
// -----------------------------------------------------------------------------
static int test_hal_edge_float_limits(void) {
    const size_t LEN = 5;
    hal_status_t ret = HAL_OK;
    // 1. FLT_MAX (Max value) + FLT_MAX = +Infinity
    // 2. -FLT_MAX (Min value) - FLT_MAX = -Infinity
    // 3. +Infinity * 0.0f = NaN (Not a Number)
    // 4. Very small number (Subnormal) addition
    // 5. 0.0 / 0.0 = NaN
    float a[5] = {FLT_MAX, -FLT_MAX, INFINITY, 1e-40f, 0.0f};
    float b[5] = {FLT_MAX, -FLT_MAX, 0.0f, 1e-40f, 0.0f};
    float c_hw[5] = {0};
    float c_sw[5] = {0};

    // SW calculation (for addition)
    for (size_t i = 0; i < LEN; i++)
        c_sw[i] = a[i] + b[i];


    ret = hal_vadd_f32(c_hw, a, b, LEN);
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    return hal_verify_array_f32(c_sw, c_hw, LEN) ? 0 : -1;
}

// -----------------------------------------------------------------------------
// [Edge Case Test 5] Unaligned Memory Access verification
// -----------------------------------------------------------------------------
static int test_hal_edge_unaligned_access(void) {
    const size_t LEN = 5;
    hal_status_t ret = HAL_OK;
    // Allocate a sufficiently large buffer.
    uint8_t buffer_a[100] = {0};
    uint8_t buffer_b[100] = {0};
    uint8_t buffer_c[100] = {0};

    // Force pointer casting to misalign the array start address by 1 or 3 bytes. (Break alignment)
    float *unaligned_a = (float *)(&buffer_a[1]); // 1-byte misalignment
    float *unaligned_b = (float *)(&buffer_b[3]); // 3-byte misalignment
    float *unaligned_c = (float *)(&buffer_c[2]); // 2-byte misalignment

    // Set initial values
    for (size_t i = 0; i < LEN; i++) {
        unaligned_a[i] = (float)i;
        unaligned_b[i] = (float)(i * 2);
    }


    // If QEMU generates a Segmentation Fault while running this function,
    // the current RISC-V core or kernel does not support Unaligned Access.
    // If it passes safely, it is supported.
    ret = hal_vadd_f32(unaligned_c, unaligned_a, unaligned_b, LEN);
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    return 0; // PASS if it survives up to here
}

// -----------------------------------------------------------------------------
// [Note] 64-bit and 128-bit test stubs
// In C, standard operators like +, * cannot be used directly on structs (int128_t),
// so they cannot be generated with the macros above.
// -----------------------------------------------------------------------------
#define DUMMY_TEST(NAME)                                                                           \
    static int NAME(void) { return 0; }

// DUMMY_TEST(test_hal_vsub_i64)
// DUMMY_TEST(test_hal_vsub_u64)
// DUMMY_TEST(test_hal_vsub_i64)
// DUMMY_TEST(test_hal_vsub_u64)
// DUMMY_TEST(test_hal_vmul_i64)
// DUMMY_TEST(test_hal_vmul_u64)
// DUMMY_TEST(test_hal_vmac_i64)
// DUMMY_TEST(test_hal_vmac_u64)
// DUMMY_TEST(test_hal_vdiv_i64)
// DUMMY_TEST(test_hal_vdiv_u64)
// DUMMY_TEST(test_hal_vdot_i64)
// DUMMY_TEST(test_hal_vdot_u64)
// DUMMY_TEST(test_hal_mtrx_vmul_i64)
// DUMMY_TEST(test_hal_mtrx_vmul_u64)
// DUMMY_TEST(test_hal_vadd_i128)
// DUMMY_TEST(test_hal_vadd_u128)
// DUMMY_TEST(test_hal_vsub_i128)
// DUMMY_TEST(test_hal_vsub_u128)
// DUMMY_TEST(test_hal_vmul_i128)
// DUMMY_TEST(test_hal_vmul_u128)
// DUMMY_TEST(test_hal_vmac_i128)
// DUMMY_TEST(test_hal_vmac_u128)
// DUMMY_TEST(test_hal_vdiv_i128)
// DUMMY_TEST(test_hal_vdiv_u128)
// DUMMY_TEST(test_hal_vdot_i128)
// DUMMY_TEST(test_hal_vdot_u128)
// DUMMY_TEST(test_hal_mtrx_vmul_i128)
// DUMMY_TEST(test_hal_mtrx_vmul_u128)

// DUMMY_TEST(test_hal_mtrx_vmul_tiled_i64)
// DUMMY_TEST(test_hal_mtrx_vmul_tiled_u64)
// DUMMY_TEST(test_hal_mtrx_vmul_tiled_i128)
// DUMMY_TEST(test_hal_mtrx_vmul_tiled_u128)


// DUMMY_TEST(test_hal_vand_i128)
// DUMMY_TEST(test_hal_vand_u128)
// DUMMY_TEST(test_hal_vand_f32)

// <---

// Use C99 Designated Initializers
// Safely map Enum values and array indices even if the order is mixed

static const HAL_TEST_FUNCTION_DEF hal_test_lists[] = {
    // single-width add
    [HAL_TEST_VADD_I8] = {HAL_TEST_VADD_I8, test_hal_vadd_i8, "VADD_I8", "Vector add (int8_t)"},
    [HAL_TEST_VADD_I16] = {HAL_TEST_VADD_I16, test_hal_vadd_i16, "VADD_I16",
                           "Vector add (int16_t)"},
    [HAL_TEST_VADD_I32] = {HAL_TEST_VADD_I32, test_hal_vadd_i32, "VADD_I32",
                           "Vector add (int32_t)"},
    [HAL_TEST_VADD_I64] = {HAL_TEST_VADD_I64, test_hal_vadd_i64, "VADD_I64",
                           "Vector add (int64_t)"},
    [HAL_TEST_VADD_I128] = {HAL_TEST_VADD_I128, test_hal_vadd_i128, "VADD_I128",
                            "Vector add (int128_t)"},

    [HAL_TEST_VADD_U8] = {HAL_TEST_VADD_U8, test_hal_vadd_u8, "VADD_U8", "Vector add (uint8_t)"},
    [HAL_TEST_VADD_U16] = {HAL_TEST_VADD_U16, test_hal_vadd_u16, "VADD_U16",
                           "Vector add (uint16_t)"},
    [HAL_TEST_VADD_U32] = {HAL_TEST_VADD_U32, test_hal_vadd_u32, "VADD_U32",
                           "Vector add (uint32_t)"},
    [HAL_TEST_VADD_U64] = {HAL_TEST_VADD_U64, test_hal_vadd_u64, "VADD_U64",
                           "Vector add (uint64_t)"},
    [HAL_TEST_VADD_U128] = {HAL_TEST_VADD_U128, test_hal_vadd_u128, "VADD_U128",
                            "Vector add (uint128_t)"},

    // widening add
    [HAL_TEST_VWADD_I8] = {HAL_TEST_VWADD_I8, test_hal_vwadd_i8, "VWADD_I8",
                           "Vector widening add (int8_t)"},
    [HAL_TEST_VWADD_I16] = {HAL_TEST_VWADD_I16, test_hal_vwadd_i16, "VWADD_I16",
                            "Vector widening add (int16_t)"},
    [HAL_TEST_VWADD_I32] = {HAL_TEST_VWADD_I32, test_hal_vwadd_i32, "VWADD_I32",
                            "Vector widening add (int32_t)"},
    [HAL_TEST_VWADD_I64] = {HAL_TEST_VWADD_I64, test_hal_vwadd_i64, "VWADD_I64",
                            "Vector widening add (int64_t)"},
    [HAL_TEST_VWADD_I128] = {HAL_TEST_VWADD_I128, test_hal_vwadd_i128, "VWADD_I128",
                             "Vector widening add (int128_t)"},

    [HAL_TEST_VWADD_U8] = {HAL_TEST_VWADD_U8, test_hal_vwadd_u8, "VWADD_U8",
                           "Vector widening add (uint8_t)"},
    [HAL_TEST_VWADD_U16] = {HAL_TEST_VWADD_U16, test_hal_vwadd_u16, "VWADD_U16",
                            "Vector widening add (uint16_t)"},
    [HAL_TEST_VWADD_U32] = {HAL_TEST_VWADD_U32, test_hal_vwadd_u32, "VWADD_U32",
                            "Vector widening add (uint32_t)"},
    [HAL_TEST_VWADD_U64] = {HAL_TEST_VWADD_U64, test_hal_vwadd_u64, "VWADD_U64",
                            "Vector widening add (uint64_t)"},
    [HAL_TEST_VWADD_U128] = {HAL_TEST_VWADD_U128, test_hal_vwadd_u128, "VWADD_U128",
                             "Vector widening add (uint128_t)"},

    // single-width sub
    [HAL_TEST_VSUB_I8] = {HAL_TEST_VSUB_I8, test_hal_vsub_i8, "VSUB_I8", "Vector sub (int8_t)"},
    [HAL_TEST_VSUB_I16] = {HAL_TEST_VSUB_I16, test_hal_vsub_i16, "VSUB_I16",
                           "Vector sub (int16_t)"},
    [HAL_TEST_VSUB_I32] = {HAL_TEST_VSUB_I32, test_hal_vsub_i32, "VSUB_I32",
                           "Vector sub (int32_t)"},
    [HAL_TEST_VSUB_I64] = {HAL_TEST_VSUB_I64, test_hal_vsub_i64, "VSUB_I64",
                           "Vector sub (int64_t)"},
    [HAL_TEST_VSUB_I128] = {HAL_TEST_VSUB_I128, test_hal_vsub_i128, "VSUB_I128",
                            "Vector sub (int128_t)"},

    [HAL_TEST_VSUB_U8] = {HAL_TEST_VSUB_U8, test_hal_vsub_u8, "VSUB_U8", "Vector sub (uint8_t)"},
    [HAL_TEST_VSUB_U16] = {HAL_TEST_VSUB_U16, test_hal_vsub_u16, "VSUB_U16",
                           "Vector sub (uint16_t)"},
    [HAL_TEST_VSUB_U32] = {HAL_TEST_VSUB_U32, test_hal_vsub_u32, "VSUB_U32",
                           "Vector sub (uint32_t)"},
    [HAL_TEST_VSUB_U64] = {HAL_TEST_VSUB_U64, test_hal_vsub_u64, "VSUB_U64",
                           "Vector sub (uint64_t)"},
    [HAL_TEST_VSUB_U128] = {HAL_TEST_VSUB_U128, test_hal_vsub_u128, "VSUB_U128",
                            "Vector sub (uint128_t)"},

    // widening sub
    [HAL_TEST_VWSUB_I8] = {HAL_TEST_VWSUB_I8, test_hal_vwsub_i8, "VWSUB_I8",
                           "Vector widening sub (int8_t)"},
    [HAL_TEST_VWSUB_I16] = {HAL_TEST_VWSUB_I16, test_hal_vwsub_i16, "VWSUB_I16",
                            "Vector widening sub (int16_t)"},
    [HAL_TEST_VWSUB_I32] = {HAL_TEST_VWSUB_I32, test_hal_vwsub_i32, "VWSUB_I32",
                            "Vector widening sub (int32_t)"},
    [HAL_TEST_VWSUB_I64] = {HAL_TEST_VWSUB_I64, test_hal_vwsub_i64, "VWSUB_I64",
                            "Vector widening sub (int64_t)"},
    [HAL_TEST_VWSUB_I128] = {HAL_TEST_VWSUB_I128, test_hal_vwsub_i128, "VWSUB_I128",
                             "Vector widening sub (int128_t)"},

    [HAL_TEST_VWSUB_U8] = {HAL_TEST_VWSUB_U8, test_hal_vwsub_u8, "VWSUB_U8",
                           "Vector widening sub (uint8_t)"},
    [HAL_TEST_VWSUB_U16] = {HAL_TEST_VWSUB_U16, test_hal_vwsub_u16, "VWSUB_U16",
                            "Vector widening sub (uint16_t)"},
    [HAL_TEST_VWSUB_U32] = {HAL_TEST_VWSUB_U32, test_hal_vwsub_u32, "VWSUB_U32",
                            "Vector widening sub (uint32_t)"},
    [HAL_TEST_VWSUB_U64] = {HAL_TEST_VWSUB_U64, test_hal_vwsub_u64, "VWSUB_U64",
                            "Vector widening sub (uint64_t)"},
    [HAL_TEST_VWSUB_U128] = {HAL_TEST_VWSUB_U128, test_hal_vwsub_u128, "VWSUB_U128",
                             "Vector widening sub (uint128_t)"},

    [HAL_TEST_VMUL_I8] = {HAL_TEST_VMUL_I8, test_hal_vmul_i8, "VMUL_I8", "Vector mul (int8_t)"},
    [HAL_TEST_VMUL_I16] = {HAL_TEST_VMUL_I16, test_hal_vmul_i16, "VMUL_I16",
                           "Vector mul (int16_t)"},
    [HAL_TEST_VMUL_I32] = {HAL_TEST_VMUL_I32, test_hal_vmul_i32, "VMUL_I32",
                           "Vector mul (int32_t)"},
    [HAL_TEST_VMUL_I64] = {HAL_TEST_VMUL_I64, test_hal_vmul_i64, "VMUL_I64",
                           "Vector mul (int64_t)"},
    [HAL_TEST_VMUL_I128] = {HAL_TEST_VMUL_I128, test_hal_vmul_i128, "VMUL_I128",
                            "Vector mul (int128_t)"},

    [HAL_TEST_VMUL_U8] = {HAL_TEST_VMUL_U8, test_hal_vmul_u8, "VMUL_U8", "Vector mul (uint8_t)"},
    [HAL_TEST_VMUL_U16] = {HAL_TEST_VMUL_U16, test_hal_vmul_u16, "VMUL_U16",
                           "Vector mul (uint16_t)"},
    [HAL_TEST_VMUL_U32] = {HAL_TEST_VMUL_U32, test_hal_vmul_u32, "VMUL_U32",
                           "Vector mul (uint32_t)"},
    [HAL_TEST_VMUL_U64] = {HAL_TEST_VMUL_U64, test_hal_vmul_u64, "VMUL_U64",
                           "Vector mul (uint64_t)"},
    [HAL_TEST_VMUL_U128] = {HAL_TEST_VMUL_U128, test_hal_vmul_u128, "VMUL_U128",
                            "Vector mul (uint128_t)"},

    [HAL_TEST_VWMUL_I8] = {HAL_TEST_VWMUL_I8, test_hal_vwmul_i8, "VWMUL_I8", "Vector mul (int8_t)"},
    [HAL_TEST_VWMUL_I16] = {HAL_TEST_VWMUL_I16, test_hal_vwmul_i16, "VWMUL_I16",
                           "Vector mul (int16_t)"},
    [HAL_TEST_VWMUL_I32] = {HAL_TEST_VWMUL_I32, test_hal_vwmul_i32, "VWMUL_I32",
                           "Vector mul (int32_t)"},
    [HAL_TEST_VWMUL_I64] = {HAL_TEST_VWMUL_I64, test_hal_vwmul_i64, "VWMUL_I64",
                           "Vector mul (int64_t)"},
    [HAL_TEST_VWMUL_I128] = {HAL_TEST_VWMUL_I128, test_hal_vwmul_i128, "VWMUL_I128",
                            "Vector mul (int128_t)"},

    [HAL_TEST_VWMUL_U8] = {HAL_TEST_VWMUL_U8, test_hal_vwmul_u8, "VWMUL_U8", "Vector mul (uint8_t)"},
    [HAL_TEST_VWMUL_U16] = {HAL_TEST_VWMUL_U16, test_hal_vwmul_u16, "VWMUL_U16",
                           "Vector mul (uint16_t)"},
    [HAL_TEST_VWMUL_U32] = {HAL_TEST_VWMUL_U32, test_hal_vwmul_u32, "VWMUL_U32",
                           "Vector mul (uint32_t)"},
    [HAL_TEST_VWMUL_U64] = {HAL_TEST_VWMUL_U64, test_hal_vwmul_u64, "VWMUL_U64",
                           "Vector mul (uint64_t)"},
    [HAL_TEST_VWMUL_U128] = {HAL_TEST_VWMUL_U128, test_hal_vwmul_u128, "VWMUL_U128",
                            "Vector mul (uint128_t)"},
    
    [HAL_TEST_VMAC_I8] = {HAL_TEST_VMAC_I8, test_hal_vmac_i8, "VMAC_I8", "Vector mac (int8_t)"},
    [HAL_TEST_VMAC_I16] = {HAL_TEST_VMAC_I16, test_hal_vmac_i16, "VMAC_I16",
                           "Vector mac (int16_t)"},
    [HAL_TEST_VMAC_I32] = {HAL_TEST_VMAC_I32, test_hal_vmac_i32, "VMAC_I32",
                           "Vector mac (int32_t)"},
    [HAL_TEST_VMAC_I64] = {HAL_TEST_VMAC_I64, test_hal_vmac_i64, "VMAC_I64",
                           "Vector mac (int64_t)"},
    [HAL_TEST_VMAC_I128] = {HAL_TEST_VMAC_I128, test_hal_vmac_i128, "VMAC_I128",
                            "Vector mac (int128_t)"},

    [HAL_TEST_VMAC_U8] = {HAL_TEST_VMAC_U8, test_hal_vmac_u8, "VMAC_U8", "Vector mac (uint8_t)"},
    [HAL_TEST_VMAC_U16] = {HAL_TEST_VMAC_U16, test_hal_vmac_u16, "VMAC_U16",
                           "Vector mac (uint16_t)"},
    [HAL_TEST_VMAC_U32] = {HAL_TEST_VMAC_U32, test_hal_vmac_u32, "VMAC_U32",
                           "Vector mac (uint32_t)"},
    [HAL_TEST_VMAC_U64] = {HAL_TEST_VMAC_U64, test_hal_vmac_u64, "VMAC_U64",
                           "Vector mac (uint64_t)"},
    [HAL_TEST_VMAC_U128] = {HAL_TEST_VMAC_U128, test_hal_vmac_u128, "VMAC_U128",
                            "Vector mac (uint128_t)"},

    [HAL_TEST_VWMAC_I8] = {HAL_TEST_VWMAC_I8, test_hal_vwmac_i8, "VWMAC_I8", "Vector wmac (int8_t)"},
    [HAL_TEST_VWMAC_I16] = {HAL_TEST_VWMAC_I16, test_hal_vwmac_i16, "VWMAC_I16",
                           "Vector wmac (int16_t)"},
    [HAL_TEST_VWMAC_I32] = {HAL_TEST_VWMAC_I32, test_hal_vwmac_i32, "VWMAC_I32",
                           "Vector wmac (int32_t)"},
    [HAL_TEST_VWMAC_I64] = {HAL_TEST_VWMAC_I64, test_hal_vwmac_i64, "VWMAC_I64",
                           "Vector wmac (int64_t)"},
    [HAL_TEST_VWMAC_I128] = {HAL_TEST_VWMAC_I128, test_hal_vwmac_i128, "VWMAC_I128",
                            "Vector wmac (int128_t)"},

    [HAL_TEST_VWMAC_U8] = {HAL_TEST_VWMAC_U8, test_hal_vwmac_u8, "VWMAC_U8", "Vector wmac (uint8_t)"},
    [HAL_TEST_VWMAC_U16] = {HAL_TEST_VWMAC_U16, test_hal_vwmac_u16, "VWMAC_U16",
                           "Vector wmac (uint16_t)"},
    [HAL_TEST_VWMAC_U32] = {HAL_TEST_VWMAC_U32, test_hal_vwmac_u32, "VWMAC_U32",
                           "Vector wmac (uint32_t)"},
    [HAL_TEST_VWMAC_U64] = {HAL_TEST_VWMAC_U64, test_hal_vwmac_u64, "VWMAC_U64",
                           "Vector wmac (uint64_t)"},
    [HAL_TEST_VWMAC_U128] = {HAL_TEST_VWMAC_U128, test_hal_vwmac_u128, "VWMAC_U128",
                            "Vector wmac (uint128_t)"},

    [HAL_TEST_VDIV_I8] = {HAL_TEST_VDIV_I8, test_hal_vdiv_i8, "VDIV_I8", "Vector div (int8_t)"},
    [HAL_TEST_VDIV_I16] = {HAL_TEST_VDIV_I16, test_hal_vdiv_i16, "VDIV_I16",
                           "Vector div (int16_t)"},
    [HAL_TEST_VDIV_I32] = {HAL_TEST_VDIV_I32, test_hal_vdiv_i32, "VDIV_I32",
                           "Vector div (int32_t)"},
    [HAL_TEST_VDIV_I64] = {HAL_TEST_VDIV_I64, test_hal_vdiv_i64, "VDIV_I64",
                           "Vector div (int64_t)"},
    [HAL_TEST_VDIV_I128] = {HAL_TEST_VDIV_I128, test_hal_vdiv_i128, "VDIV_I128",
                            "Vector div (int128_t)"},

    [HAL_TEST_VDIV_U8] = {HAL_TEST_VDIV_U8, test_hal_vdiv_u8, "VDIV_U8", "Vector div (uint8_t)"},
    [HAL_TEST_VDIV_U16] = {HAL_TEST_VDIV_U16, test_hal_vdiv_u16, "VDIV_U16",
                           "Vector div (uint16_t)"},
    [HAL_TEST_VDIV_U32] = {HAL_TEST_VDIV_U32, test_hal_vdiv_u32, "VDIV_U32",
                           "Vector div (uint32_t)"},
    [HAL_TEST_VDIV_U64] = {HAL_TEST_VDIV_U64, test_hal_vdiv_u64, "VDIV_U64",
                           "Vector div (uint64_t)"},
    [HAL_TEST_VDIV_U128] = {HAL_TEST_VDIV_U128, test_hal_vdiv_u128, "VDIV_U128",
                            "Vector div (uint128_t)"},

    [HAL_TEST_VDOT_I8] = {HAL_TEST_VDOT_I8, test_hal_vdot_i8, "VDOT_I8",
                          "Vector dot product (int8_t)"},
    [HAL_TEST_VDOT_I16] = {HAL_TEST_VDOT_I16, test_hal_vdot_i16, "VDOT_I16",
                           "Vector dot product (int16_t)"},
    [HAL_TEST_VDOT_I32] = {HAL_TEST_VDOT_I32, test_hal_vdot_i32, "VDOT_I32",
                           "Vector dot product (int32_t)"},
    [HAL_TEST_VDOT_I64] = {HAL_TEST_VDOT_I64, test_hal_vdot_i64, "VDOT_I64",
                           "Vector dot product (int64_t)"},
    [HAL_TEST_VDOT_I128] = {HAL_TEST_VDOT_I128, test_hal_vdot_i128, "VDOT_I128",
                            "Vector dot product (int128_t)"},

    [HAL_TEST_VDOT_U8] = {HAL_TEST_VDOT_U8, test_hal_vdot_u8, "VDOT_U8",
                          "Vector dot product (uint8_t)"},
    [HAL_TEST_VDOT_U16] = {HAL_TEST_VDOT_U16, test_hal_vdot_u16, "VDOT_U16",
                           "Vector dot product (uint16_t)"},
    [HAL_TEST_VDOT_U32] = {HAL_TEST_VDOT_U32, test_hal_vdot_u32, "VDOT_U32",
                           "Vector dot product (uint32_t)"},
    [HAL_TEST_VDOT_U64] = {HAL_TEST_VDOT_U64, test_hal_vdot_u64, "VDOT_U64",
                           "Vector dot product (uint64_t)"},
    [HAL_TEST_VDOT_U128] = {HAL_TEST_VDOT_U128, test_hal_vdot_u128, "VDOT_U128",
                            "Vector dot product (uint128_t)"},

    [HAL_TEST_VWDOT_I8] = {HAL_TEST_VWDOT_I8, test_hal_vwdot_i8, "VWDOT_I8",
                          "Vector wdot product (int8_t)"},
    [HAL_TEST_VWDOT_I16] = {HAL_TEST_VWDOT_I16, test_hal_vwdot_i16, "VWDOT_I16",
                           "Vector wdot product (int16_t)"},
    [HAL_TEST_VWDOT_I32] = {HAL_TEST_VWDOT_I32, test_hal_vwdot_i32, "VWDOT_I32",
                           "Vector wdot product (int32_t)"},
    [HAL_TEST_VWDOT_I64] = {HAL_TEST_VWDOT_I64, test_hal_vwdot_i64, "VWDOT_I64",
                           "Vector wdot product (int64_t)"},
    [HAL_TEST_VWDOT_I128] = {HAL_TEST_VWDOT_I128, test_hal_vwdot_i128, "VWDOT_I128",
                            "Vector wdot product (int128_t)"},

    [HAL_TEST_VWDOT_U8] = {HAL_TEST_VWDOT_U8, test_hal_vwdot_u8, "VWDOT_U8",
                          "Vector wdot product (uint8_t)"},
    [HAL_TEST_VWDOT_U16] = {HAL_TEST_VWDOT_U16, test_hal_vwdot_u16, "VWDOT_U16",
                           "Vector wdot product (uint16_t)"},
    [HAL_TEST_VWDOT_U32] = {HAL_TEST_VWDOT_U32, test_hal_vwdot_u32, "VWDOT_U32",
                           "Vector wdot product (uint32_t)"},
    [HAL_TEST_VWDOT_U64] = {HAL_TEST_VWDOT_U64, test_hal_vwdot_u64, "VWDOT_U64",
                           "Vector wdot product (uint64_t)"},
    [HAL_TEST_VWDOT_U128] = {HAL_TEST_VWDOT_U128, test_hal_vwdot_u128, "VWDOT_U128",
                            "Vector wdot product (uint128_t)"},

    [HAL_TEST_MTRX_MUL_I8] = {HAL_TEST_MTRX_MUL_I8, test_hal_mtrx_vmul_i8, "MTRX_MUL_I8",
                              "Matrix Mul (int8_t)"},
    [HAL_TEST_MTRX_MUL_I16] = {HAL_TEST_MTRX_MUL_I16, test_hal_mtrx_vmul_i16, "MTRX_MUL_I16",
                               "Matrix Mul (int16_t)"},
    [HAL_TEST_MTRX_MUL_I32] = {HAL_TEST_MTRX_MUL_I32, test_hal_mtrx_vmul_i32, "MTRX_MUL_I32",
                               "Matrix Mul (int32_t)"},
    [HAL_TEST_MTRX_MUL_I64] = {HAL_TEST_MTRX_MUL_I64, test_hal_mtrx_vmul_i64, "MTRX_MUL_I64",
                               "Matrix Mul (int64_t)"},
    [HAL_TEST_MTRX_MUL_I128] = {HAL_TEST_MTRX_MUL_I128, test_hal_mtrx_vmul_i128, "MTRX_MUL_I128",
                                "Matrix Mul (int128_t)"},

    [HAL_TEST_MTRX_MUL_U8] = {HAL_TEST_MTRX_MUL_U8, test_hal_mtrx_vmul_u8, "MTRX_MUL_U8",
                              "Matrix Mul (uint8_t)"},
    [HAL_TEST_MTRX_MUL_U16] = {HAL_TEST_MTRX_MUL_U16, test_hal_mtrx_vmul_u16, "MTRX_MUL_U16",
                               "Matrix Mul (uint16_t)"},
    [HAL_TEST_MTRX_MUL_U32] = {HAL_TEST_MTRX_MUL_U32, test_hal_mtrx_vmul_u32, "MTRX_MUL_U32",
                               "Matrix Mul (uint32_t)"},
    [HAL_TEST_MTRX_MUL_U64] = {HAL_TEST_MTRX_MUL_U64, test_hal_mtrx_vmul_u64, "MTRX_MUL_U64",
                               "Matrix Mul (uint64_t)"},
    [HAL_TEST_MTRX_MUL_U128] = {HAL_TEST_MTRX_MUL_U128, test_hal_mtrx_vmul_u128, "MTRX_MUL_U128",
                                "Matrix Mul (uint128_t)"},

    [HAL_TEST_MTRX_WMUL_I8] = {HAL_TEST_MTRX_WMUL_I8, test_hal_mtrx_vwmul_i8, "MTRX_WMUL_I8",
                              "Matrix Mul (int8_t)"},
    [HAL_TEST_MTRX_WMUL_I16] = {HAL_TEST_MTRX_WMUL_I16, test_hal_mtrx_vwmul_i16, "MTRX_WMUL_I16",
                               "Matrix Mul (int16_t)"},
    [HAL_TEST_MTRX_WMUL_I32] = {HAL_TEST_MTRX_WMUL_I32, test_hal_mtrx_vwmul_i32, "MTRX_WMUL_I32",
                               "Matrix Mul (int32_t)"},
    [HAL_TEST_MTRX_WMUL_I64] = {HAL_TEST_MTRX_WMUL_I64, test_hal_mtrx_vwmul_i64, "MTRX_WMUL_I64",
                               "Matrix Mul (int64_t)"},
    [HAL_TEST_MTRX_WMUL_I128] = {HAL_TEST_MTRX_WMUL_I128, test_hal_mtrx_vwmul_i128, "MTRX_WMUL_I128",
                                "Matrix Mul (int128_t)"},

    [HAL_TEST_MTRX_WMUL_U8] = {HAL_TEST_MTRX_WMUL_U8, test_hal_mtrx_vwmul_u8, "MTRX_WMUL_U8",
                              "Matrix Mul (uint8_t)"},
    [HAL_TEST_MTRX_WMUL_U16] = {HAL_TEST_MTRX_WMUL_U16, test_hal_mtrx_vwmul_u16, "MTRX_WMUL_U16",
                               "Matrix Mul (uint16_t)"},
    [HAL_TEST_MTRX_WMUL_U32] = {HAL_TEST_MTRX_WMUL_U32, test_hal_mtrx_vwmul_u32, "MTRX_WMUL_U32",
                               "Matrix Mul (uint32_t)"},
    [HAL_TEST_MTRX_WMUL_U64] = {HAL_TEST_MTRX_WMUL_U64, test_hal_mtrx_vwmul_u64, "MTRX_WMUL_U64",
                               "Matrix Mul (uint64_t)"},
    [HAL_TEST_MTRX_WMUL_U128] = {HAL_TEST_MTRX_WMUL_U128, test_hal_mtrx_vwmul_u128, "MTRX_WMUL_U128",
                                "Matrix Mul (uint128_t)"},

    [HAL_TEST_MTRX_MUL_TILED_I8] = {HAL_TEST_MTRX_MUL_TILED_I8, test_hal_mtrx_vmul_tiled_i8,
                                    "MTRX_MUL_TILED_I8", "Matrix Mul tiled (int8_t)"},
    [HAL_TEST_MTRX_MUL_TILED_I16] = {HAL_TEST_MTRX_MUL_TILED_I16, test_hal_mtrx_vmul_tiled_i16,
                                     "MTRX_MUL_TILED_I16", "Matrix Mul tiled (int16_t)"},
    [HAL_TEST_MTRX_MUL_TILED_I32] = {HAL_TEST_MTRX_MUL_TILED_I32, test_hal_mtrx_vmul_tiled_i32,
                                     "MTRX_MUL_TILED_I32", "Matrix Mul tiled (int32_t)"},
    [HAL_TEST_MTRX_MUL_TILED_I64] = {HAL_TEST_MTRX_MUL_TILED_I64, test_hal_mtrx_vmul_tiled_i64,
                                     "MTRX_MUL_TILED_I64", "Matrix Mul tiled (int64_t)"},
    [HAL_TEST_MTRX_MUL_TILED_I128] = {HAL_TEST_MTRX_MUL_TILED_I128, test_hal_mtrx_vmul_tiled_i128,
                                      "MTRX_MUL_TILED_I128", "Matrix Mul tiled (int128_t)"},

    [HAL_TEST_MTRX_MUL_TILED_U8] = {HAL_TEST_MTRX_MUL_TILED_U8, test_hal_mtrx_vmul_tiled_u8,
                                    "MTRX_MUL_TILED_U8", "Matrix Mul tiled (uint8_t)"},
    [HAL_TEST_MTRX_MUL_TILED_U16] = {HAL_TEST_MTRX_MUL_TILED_U16, test_hal_mtrx_vmul_tiled_u16,
                                     "MTRX_MUL_TILED_U16", "Matrix Mul tiled (uint16_t)"},
    [HAL_TEST_MTRX_MUL_TILED_U32] = {HAL_TEST_MTRX_MUL_TILED_U32, test_hal_mtrx_vmul_tiled_u32,
                                     "MTRX_MUL_TILED_U32", "Matrix Mul tiled (uint32_t)"},
    [HAL_TEST_MTRX_MUL_TILED_U64] = {HAL_TEST_MTRX_MUL_TILED_U64, test_hal_mtrx_vmul_tiled_u64,
                                     "MTRX_MUL_TILED_U64", "Matrix Mul tiled (uint64_t)"},
    [HAL_TEST_MTRX_MUL_TILED_U128] = {HAL_TEST_MTRX_MUL_TILED_U128, test_hal_mtrx_vmul_tiled_u128,
                                      "MTRX_MUL_TILED_U128", "Matrix Mul tiled (uint128_t)"},

    [HAL_TEST_MTRX_WMUL_TILED_I8] = {HAL_TEST_MTRX_WMUL_TILED_I8, test_hal_mtrx_vwmul_tiled_i8,
                                    "MTRX_WMUL_TILED_I8", "Matrix Mul tiled (int8_t)"},
    [HAL_TEST_MTRX_WMUL_TILED_I16] = {HAL_TEST_MTRX_WMUL_TILED_I16, test_hal_mtrx_vwmul_tiled_i16,
                                     "MTRX_WMUL_TILED_I16", "Matrix Mul tiled (int16_t)"},
    [HAL_TEST_MTRX_WMUL_TILED_I32] = {HAL_TEST_MTRX_WMUL_TILED_I32, test_hal_mtrx_vwmul_tiled_i32,
                                     "MTRX_WMUL_TILED_I32", "Matrix Mul tiled (int32_t)"},
    [HAL_TEST_MTRX_WMUL_TILED_I64] = {HAL_TEST_MTRX_WMUL_TILED_I64, test_hal_mtrx_vwmul_tiled_i64,
                                     "MTRX_WMUL_TILED_I64", "Matrix Mul tiled (int64_t)"},
    [HAL_TEST_MTRX_WMUL_TILED_I128] = {HAL_TEST_MTRX_WMUL_TILED_I128, test_hal_mtrx_vwmul_tiled_i128,
                                      "MTRX_WMUL_TILED_I128", "Matrix Mul tiled (int128_t)"},

    [HAL_TEST_MTRX_WMUL_TILED_U8] = {HAL_TEST_MTRX_WMUL_TILED_U8, test_hal_mtrx_vwmul_tiled_u8,
                                    "MTRX_WMUL_TILED_U8", "Matrix Mul tiled (uint8_t)"},
    [HAL_TEST_MTRX_WMUL_TILED_U16] = {HAL_TEST_MTRX_WMUL_TILED_U16, test_hal_mtrx_vwmul_tiled_u16,
                                     "MTRX_WMUL_TILED_U16", "Matrix Mul tiled (uint16_t)"},
    [HAL_TEST_MTRX_WMUL_TILED_U32] = {HAL_TEST_MTRX_WMUL_TILED_U32, test_hal_mtrx_vwmul_tiled_u32,
                                     "MTRX_WMUL_TILED_U32", "Matrix Mul tiled (uint32_t)"},
    [HAL_TEST_MTRX_WMUL_TILED_U64] = {HAL_TEST_MTRX_WMUL_TILED_U64, test_hal_mtrx_vwmul_tiled_u64,
                                     "MTRX_WMUL_TILED_U64", "Matrix Mul tiled (uint64_t)"},
    [HAL_TEST_MTRX_WMUL_TILED_U128] = {HAL_TEST_MTRX_WMUL_TILED_U128, test_hal_mtrx_vwmul_tiled_u128,
                                      "MTRX_WMUL_TILED_U128", "Matrix Mul tiled (uint128_t)"},

    [HAL_TEST_VMIN_U8] = {HAL_TEST_VMIN_U8, test_hal_vmin_u8, "VMIN_U8", "Vector min (uint8_t)"},
    [HAL_TEST_VMIN_U16] = {HAL_TEST_VMIN_U16, test_hal_vmin_u16, "VMIN_U16",
                           "Vector min (uint16_t)"},
    [HAL_TEST_VMIN_U32] = {HAL_TEST_VMIN_U32, test_hal_vmin_u32, "VMIN_U32",
                           "Vector min (uint32_t)"},
    [HAL_TEST_VMIN_U64] = {HAL_TEST_VMIN_U64, test_hal_vmin_u64, "VMIN_U64",
                           "Vector min (uint64_t)"},
    [HAL_TEST_VMIN_U128] = {HAL_TEST_VMIN_U128, test_hal_vmin_u128, "VMIN_U128",
                            "Vector min (uint128_t)"},

    [HAL_TEST_VMIN_I8] = {HAL_TEST_VMIN_I8, test_hal_vmin_i8, "VMIN_I8", "Vector min (int8_t)"},
    [HAL_TEST_VMIN_I16] = {HAL_TEST_VMIN_I16, test_hal_vmin_i16, "VMIN_I16",
                           "Vector min (int16_t)"},
    [HAL_TEST_VMIN_I32] = {HAL_TEST_VMIN_I32, test_hal_vmin_i32, "VMIN_I32",
                           "Vector min (int32_t)"},
    [HAL_TEST_VMIN_I64] = {HAL_TEST_VMIN_I64, test_hal_vmin_i64, "VMIN_I64",
                           "Vector min (int64_t)"},
    [HAL_TEST_VMIN_I128] = {HAL_TEST_VMIN_I128, test_hal_vmin_i128, "VMIN_I128",
                            "Vector min (int128_t)"},

    [HAL_TEST_VMAX_U8] = {HAL_TEST_VMAX_U8, test_hal_vmax_u8, "VMAX_U8", "Vector max (uint8_t)"},
    [HAL_TEST_VMAX_U16] = {HAL_TEST_VMAX_U16, test_hal_vmax_u16, "VMAX_U16",
                           "Vector max (uint16_t)"},
    [HAL_TEST_VMAX_U32] = {HAL_TEST_VMAX_U32, test_hal_vmax_u32, "VMAX_U32",
                           "Vector max (uint32_t)"},
    [HAL_TEST_VMAX_U64] = {HAL_TEST_VMAX_U64, test_hal_vmax_u64, "VMAX_U64",
                           "Vector max (uint64_t)"},
    [HAL_TEST_VMAX_U128] = {HAL_TEST_VMAX_U128, test_hal_vmax_u128, "VMAX_U128",
                            "Vector max (uint128_t)"},

    [HAL_TEST_VMAX_I8] = {HAL_TEST_VMAX_I8, test_hal_vmax_i8, "VMAX_I8", "Vector max (int8_t)"},
    [HAL_TEST_VMAX_I16] = {HAL_TEST_VMAX_I16, test_hal_vmax_i16, "VMAX_I16",
                           "Vector max (int16_t)"},
    [HAL_TEST_VMAX_I32] = {HAL_TEST_VMAX_I32, test_hal_vmax_i32, "VMAX_I32",
                           "Vector max (int32_t)"},
    [HAL_TEST_VMAX_I64] = {HAL_TEST_VMAX_I64, test_hal_vmax_i64, "VMAX_I64",
                           "Vector max (int64_t)"},
    [HAL_TEST_VMAX_I128] = {HAL_TEST_VMAX_I128, test_hal_vmax_i128, "VMAX_I128",
                            "Vector max (int128_t)"},

    // Floating-point test
    [HAL_TEST_ADD_F32] = {HAL_TEST_ADD_F32, test_hal_vadd_f32, "VADD_F32", "Vector add (float)"},
    [HAL_TEST_SUB_F32] = {HAL_TEST_SUB_F32, test_hal_vsub_f32, "VSUB_F32", "Vector sub (float)"},
    [HAL_TEST_MUL_F32] = {HAL_TEST_MUL_F32, test_hal_vmul_f32, "VMUL_F32", "Vector mul (float)"},
    [HAL_TEST_MAC_F32] = {HAL_TEST_MAC_F32, test_hal_vmac_f32, "VMAC_F32", "Vector mac (float)"},
    [HAL_TEST_DIV_F32] = {HAL_TEST_DIV_F32, test_hal_vdiv_f32, "VDIV_F32", "Vector div (float)"},
    [HAL_TEST_DOT_F32] = {HAL_TEST_DOT_F32, test_hal_vdot_f32, "VDOT_F32",
                          "Vector dot product (float)"},
    [HAL_TEST_MTRX_MUL_F32] = {HAL_TEST_MTRX_MUL_F32, test_hal_mtrx_vmul_f32, "MTRX_MUL_F32",
                               "Matrix Mul (float)"},
    [HAL_TEST_MTRX_MUL_TILED_F32] = {HAL_TEST_MTRX_MUL_TILED_F32, test_hal_mtrx_vmul_tiled_f32,
                                     "MTRX_MUL_TILED_F32", "Matrix Mul tiled (float)"},
    [HAL_TEST_VMIN_F32] = {HAL_TEST_VMIN_F32, test_hal_vmin_f32, "VMIN_F32", "Vector min (float)"},

    [HAL_TEST_VMAX_F32] = {HAL_TEST_VMAX_F32, test_hal_vmax_f32, "VMAX_F32", "Vector max (float)"},

    [HAL_TEST_VSQRT_F32] = {HAL_TEST_VSQRT_F32, test_hal_vsqrt_f32, "VSQRT_F32",
                            "Vector sqrt (float)"},
    [HAL_TEST_VRSQRT7_F32] = {HAL_TEST_VRSQRT7_F32, test_hal_vrsqrt7_f32, "VRSQRT7_F32",
                              "Vector rsqrt7(estimate to 7 bits) (float)"},
    // widening
    [HAL_TEST_WADD_F32] = {HAL_TEST_WADD_F32, test_hal_vwadd_f32, "VWADD_F32", "Vector wadd (float)"},
    [HAL_TEST_WSUB_F32] = {HAL_TEST_WSUB_F32, test_hal_vwsub_f32, "VWSUB_F32", "Vector wsub (float)"},
    [HAL_TEST_WMUL_F32] = {HAL_TEST_WMUL_F32, test_hal_vwmul_f32, "VWMUL_F32", "Vector wmul (float)"},
    [HAL_TEST_WMAC_F32] = {HAL_TEST_WMAC_F32, test_hal_vwmac_f32, "VWMAC_F32", "Vector wmac (float)"},
    [HAL_TEST_WDOT_F32] = {HAL_TEST_WDOT_F32, test_hal_vwdot_f32, "VWDOT_F32",
                          "Widening Vector dot product (float)"},
    [HAL_TEST_MTRX_WMUL_F32] = {HAL_TEST_MTRX_WMUL_F32, test_hal_mtrx_vwmul_f32, "MTRX_WMUL_F32",
                               "Matrix WMul (float)"},
    [HAL_TEST_MTRX_WMUL_TILED_F32] = {HAL_TEST_MTRX_WMUL_TILED_F32, test_hal_mtrx_vwmul_tiled_f32,
                                     "MTRX_WMUL_TILED_F32", "Matrix WMul tiled (float)"},

    [HAL_TEST_VAND_U8] = {HAL_TEST_VAND_U8, test_hal_vand_u8, "VAND_U8", "Vector and (uint8_t)"},
    [HAL_TEST_VAND_U16] = {HAL_TEST_VAND_U16, test_hal_vand_u16, "VAND_U16",
                           "Vector and (uint16_t)"},
    [HAL_TEST_VAND_U32] = {HAL_TEST_VAND_U32, test_hal_vand_u32, "VAND_U32",
                           "Vector and (uint32_t)"},
    [HAL_TEST_VAND_U64] = {HAL_TEST_VAND_U64, test_hal_vand_u64, "VAND_U64",
                           "Vector and (uint64_t)"},
    [HAL_TEST_VAND_U128] = {HAL_TEST_VAND_U128, test_hal_vand_u128, "VAND_U128",
                            "Vector and (uint128_t)"},

    [HAL_TEST_VAND_I8] = {HAL_TEST_VAND_I8, test_hal_vand_i8, "VAND_I8", "Vector and (int8_t)"},
    [HAL_TEST_VAND_I16] = {HAL_TEST_VAND_I16, test_hal_vand_i16, "VAND_I16",
                           "Vector and (int16_t)"},
    [HAL_TEST_VAND_I32] = {HAL_TEST_VAND_I32, test_hal_vand_i32, "VAND_I32",
                           "Vector and (int32_t)"},
    [HAL_TEST_VAND_I64] = {HAL_TEST_VAND_I64, test_hal_vand_i64, "VAND_I64",
                           "Vector and (int64_t)"},
    [HAL_TEST_VAND_I128] = {HAL_TEST_VAND_I128, test_hal_vand_i128, "VAND_I128",
                            "Vector and (int128_t)"},

    [HAL_TEST_VOR_U8] = {HAL_TEST_VOR_U8, test_hal_vor_u8, "VOR_U8", "Vector or (uint8_t)"},
    [HAL_TEST_VOR_U16] = {HAL_TEST_VOR_U16, test_hal_vor_u16, "VOR_U16",
                           "Vector or (uint16_t)"},
    [HAL_TEST_VOR_U32] = {HAL_TEST_VOR_U32, test_hal_vor_u32, "VOR_U32",
                           "Vector or (uint32_t)"},
    [HAL_TEST_VOR_U64] = {HAL_TEST_VOR_U64, test_hal_vor_u64, "VOR_U64",
                           "Vector or (uint64_t)"},
    [HAL_TEST_VOR_U128] = {HAL_TEST_VOR_U128, test_hal_vor_u128, "VOR_U128",
                            "Vector or (uint128_t)"},

    [HAL_TEST_VOR_I8] = {HAL_TEST_VOR_I8, test_hal_vor_i8, "VOR_I8", "Vector or (int8_t)"},
    [HAL_TEST_VOR_I16] = {HAL_TEST_VOR_I16, test_hal_vor_i16, "VOR_I16",
                           "Vector or (int16_t)"},
    [HAL_TEST_VOR_I32] = {HAL_TEST_VOR_I32, test_hal_vor_i32, "VOR_I32",
                           "Vector or (int32_t)"},
    [HAL_TEST_VOR_I64] = {HAL_TEST_VOR_I64, test_hal_vor_i64, "VOR_I64",
                           "Vector or (int64_t)"},
    [HAL_TEST_VOR_I128] = {HAL_TEST_VOR_I128, test_hal_vor_i128, "VOR_I128",
                            "Vector or (int128_t)"},

    [HAL_TEST_VXOR_U8] = {HAL_TEST_VXOR_U8, test_hal_vxor_u8, "VXOR_U8", "Vector xor (uint8_t)"},
    [HAL_TEST_VXOR_U16] = {HAL_TEST_VXOR_U16, test_hal_vxor_u16, "VXOR_U16",
                           "Vector xor (uint16_t)"},
    [HAL_TEST_VXOR_U32] = {HAL_TEST_VXOR_U32, test_hal_vxor_u32, "VXOR_U32",
                           "Vector xor (uint32_t)"},
    [HAL_TEST_VXOR_U64] = {HAL_TEST_VXOR_U64, test_hal_vxor_u64, "VXOR_U64",
                           "Vector xor (uint64_t)"},
    [HAL_TEST_VXOR_U128] = {HAL_TEST_VXOR_U128, test_hal_vxor_u128, "VXOR_U128",
                            "Vector xor (uint128_t)"},

    [HAL_TEST_VXOR_I8] = {HAL_TEST_VXOR_I8, test_hal_vxor_i8, "VXOR_I8", "Vector xor (int8_t)"},
    [HAL_TEST_VXOR_I16] = {HAL_TEST_VXOR_I16, test_hal_vxor_i16, "VXOR_I16",
                           "Vector xor (int16_t)"},
    [HAL_TEST_VXOR_I32] = {HAL_TEST_VXOR_I32, test_hal_vxor_i32, "VXOR_I32",
                           "Vector xor (int32_t)"},
    [HAL_TEST_VXOR_I64] = {HAL_TEST_VXOR_I64, test_hal_vxor_i64, "VXOR_I64",
                           "Vector xor (int64_t)"},
    [HAL_TEST_VXOR_I128] = {HAL_TEST_VXOR_I128, test_hal_vxor_i128, "VXOR_I128",
                            "Vector xor (int128_t)"},

    [HAL_TEST_VNOT_U8] = {HAL_TEST_VNOT_U8, test_hal_vnot_u8, "VNOT_U8", "Vector not (uint8_t)"},
    [HAL_TEST_VNOT_U16] = {HAL_TEST_VNOT_U16, test_hal_vnot_u16, "VNOT_U16",
                           "Vector not (uint16_t)"},
    [HAL_TEST_VNOT_U32] = {HAL_TEST_VNOT_U32, test_hal_vnot_u32, "VNOT_U32",
                           "Vector not (uint32_t)"},
    [HAL_TEST_VNOT_U64] = {HAL_TEST_VNOT_U64, test_hal_vnot_u64, "VNOT_U64",
                           "Vector not (uint64_t)"},
    [HAL_TEST_VNOT_U128] = {HAL_TEST_VNOT_U128, test_hal_vnot_u128, "VNOT_U128",
                            "Vector not (uint128_t)"},

    [HAL_TEST_VNOT_I8] = {HAL_TEST_VNOT_I8, test_hal_vnot_i8, "VNOT_I8", "Vector not (int8_t)"},
    [HAL_TEST_VNOT_I16] = {HAL_TEST_VNOT_I16, test_hal_vnot_i16, "VNOT_I16",
                           "Vector not (int16_t)"},
    [HAL_TEST_VNOT_I32] = {HAL_TEST_VNOT_I32, test_hal_vnot_i32, "VNOT_I32",
                           "Vector not (int32_t)"},
    [HAL_TEST_VNOT_I64] = {HAL_TEST_VNOT_I64, test_hal_vnot_i64, "VNOT_I64",
                           "Vector not (int64_t)"},
    [HAL_TEST_VNOT_I128] = {HAL_TEST_VNOT_I128, test_hal_vnot_i128, "VNOT_I128",
                            "Vector not (int128_t)"},
};

/* * Execute all tests and return the number of failed tests.
 * A return value of 0 means All Pass.
 */
int hal_do_all_test(void) {
    int fail_count = 0;
    int pass_count = 0;

    for (int i = 0; i < HAL_TEST_MAX; i++) {
        if (hal_test_lists[i].func == NULL)
            continue;

        int ret = hal_test_lists[i].func();

        if (ret == -1) { // <--- Modification: Treat as failure only when it is exactly -1, not just
                         // any negative number
            printf("[FAIL] %-15s : %s\n", hal_test_lists[i].name, hal_test_lists[i].description);
            fail_count++;
        } else { // Unconditional success if not -1 & print cycle count
            printf("[PASS] %-15s : %s ", hal_test_lists[i].name, hal_test_lists[i].description);

            // Print hardware cycles only in non-host environments
            if (ret != 0) {
                // <--- Modification: Interpret overflowed int as unsigned int for correct output
                printf("\033[0;36m(%llu cycles)\033[0m\n", (unsigned long long)ret);
            } else {
                printf("\n");
            }
            pass_count++;
        }
    }

    // Print final summary
    printf("\n====================================\n");
    printf(" TEST SUMMARY: %d PASSED, %d FAILED\n", pass_count, fail_count);
    printf("====================================\n");

    printf("\n--- Edge Case Suites ---\n");
    hal_print_test_result("EDGE_OVERFLOW_I32", test_hal_edge_overflow_i32());
    hal_print_test_result("EDGE_WIDENING_I32", test_hal_widening_i32());
    hal_print_test_result("EDGE_WIDENING_U32", test_hal_widening_u32());
    hal_print_test_result("EDGE_FLOAT_LIMITS", test_hal_edge_float_limits());
    hal_print_test_result("EDGE_UNALIGNED_ACCESS", test_hal_edge_unaligned_access());

    return fail_count;
}

#if 0
static int test_hal_vwadd_i8(void) {
    hal_status_t ret = HAL_OK;
    int8_t a[4] = {127, 120, 50,  127};
    int8_t b[4] = {10,  120, 150, 127};
    int16_t c_hw[4] = {0}, c_sw[4] = {0};
    for (int i = 0; i < 4; i++)
        c_sw[i] = (int16_t)a[i] + (int16_t)b[i];

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwadd_i8(c_hw, a, b, 4);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    for (int i=0; i<4; i++) {
        printf("[%d] S=%6d : H=%6d\n", i, c_sw[i], c_hw[i]);
    }
    return 0;
}

static int test_hal_vwadd_u8(void) {
    hal_status_t ret = HAL_OK;
    uint8_t a[4] = {255, 200, 50,  255};
    uint8_t b[4] = {10,  100, 150, 255};
    uint16_t c_hw[4] = {0}, c_sw[4] = {0};
    for (int i = 0; i < 4; i++)
        c_sw[i] = (uint16_t)a[i] + (uint16_t)b[i];

    /* --- Profiling Start --- */
    uint64_t start = get_mcycle();
    ret = hal_vwadd_u8(c_hw, a, b, 4);
    uint64_t end = get_mcycle();
    /* --- Profiling End --- */
    if (ret != HAL_OK) {
        printf("    -> [ERROR] Unexpected error code %d in %s\n", ret, __func__);
    }
    for (int i=0; i<4; i++) {
        printf("[%d] S=%6d : H=%6d\n", i, c_sw[i], c_hw[i]);
    }
    return 0;
}
#endif

int main() {
    // return test_hal_vdiv_f32_1();
    // return test_hal_vwadd_i8();
    // return test_hal_vwadd_u8();
    return hal_do_all_test();
}
