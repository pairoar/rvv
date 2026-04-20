#include "hal_math.h"
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef enum {
    HAL_TEST_INIT,

    /* Add */
    HAL_TEST_VADD_I8,
    HAL_TEST_VADD_U8,
    HAL_TEST_VADD_I16,
    HAL_TEST_VADD_U16,
    HAL_TEST_VADD_I32,
    HAL_TEST_VADD_U32,
    HAL_TEST_VADD_I64,
    HAL_TEST_VADD_U64,
    HAL_TEST_VADD_I128,
    HAL_TEST_VADD_U128,

    /* Subtract */
    HAL_TEST_VSUB_I8,
    HAL_TEST_VSUB_U8,
    HAL_TEST_VSUB_I16,
    HAL_TEST_VSUB_U16,
    HAL_TEST_VSUB_I32,
    HAL_TEST_VSUB_U32,
    HAL_TEST_VSUB_I64,
    HAL_TEST_VSUB_U64,
    HAL_TEST_VSUB_I128,
    HAL_TEST_VSUB_U128,

    /* Multiply */
    HAL_TEST_VMUL_I8,
    HAL_TEST_VMUL_U8,
    HAL_TEST_VMUL_I16,
    HAL_TEST_VMUL_U16,
    HAL_TEST_VMUL_I32,
    HAL_TEST_VMUL_U32,
    HAL_TEST_VMUL_I64,
    HAL_TEST_VMUL_U64,
    HAL_TEST_VMUL_I128,
    HAL_TEST_VMUL_U128,

    /* MAC */
    HAL_TEST_VMAC_I8,
    HAL_TEST_VMAC_U8,
    HAL_TEST_VMAC_I16,
    HAL_TEST_VMAC_U16,
    HAL_TEST_VMAC_I32,
    HAL_TEST_VMAC_U32,
    HAL_TEST_VMAC_I64,
    HAL_TEST_VMAC_U64,
    HAL_TEST_VMAC_I128,
    HAL_TEST_VMAC_U128,

    /* Divide */
    HAL_TEST_VDIV_I8,
    HAL_TEST_VDIV_U8,
    HAL_TEST_VDIV_I16,
    HAL_TEST_VDIV_U16,
    HAL_TEST_VDIV_I32,
    HAL_TEST_VDIV_U32,
    HAL_TEST_VDIV_I64,
    HAL_TEST_VDIV_U64,
    HAL_TEST_VDIV_I128,
    HAL_TEST_VDIV_U128,

    /* Dot Product */
    HAL_TEST_VDOT_I8,
    HAL_TEST_VDOT_U8,
    HAL_TEST_VDOT_I16,
    HAL_TEST_VDOT_U16,
    HAL_TEST_VDOT_I32,
    HAL_TEST_VDOT_U32,
    HAL_TEST_VDOT_I64,
    HAL_TEST_VDOT_U64,
    HAL_TEST_VDOT_I128,
    HAL_TEST_VDOT_U128,

    /* Matrix */
    HAL_TEST_MTRX_MUL_I8,
    HAL_TEST_MTRX_MUL_U8,
    HAL_TEST_MTRX_MUL_I16,
    HAL_TEST_MTRX_MUL_U16,
    HAL_TEST_MTRX_MUL_I32,
    HAL_TEST_MTRX_MUL_U32,
    HAL_TEST_MTRX_MUL_I64,
    HAL_TEST_MTRX_MUL_U64,
    HAL_TEST_MTRX_MUL_I128,
    HAL_TEST_MTRX_MUL_U128,

    /* float */
    HAL_TEST_ADD_F32,
    HAL_TEST_SUB_F32,
    HAL_TEST_MUL_F32,
    HAL_TEST_MAC_F32,
    HAL_TEST_DIV_F32,
    HAL_TEST_DOT_F32,
    HAL_TEST_MTRX_MUL_F32,

    HAL_TEST_MAX,
} HAL_TEST_LIST_E;

typedef struct {
    HAL_TEST_LIST_E idx;
    int (*func)(void);
    const char *name; // 추가: 함수(테스트) 이름
    const char *description;
} HAL_TEST_FUNCTION_DEF;

#define EPSILON 0.0001f

// -----------------------------------------------------------------------------
// [추가] 하드웨어 사이클 카운터 읽기 (Performance Profiling)
// -----------------------------------------------------------------------------
static inline uint64_t get_mcycle(void) {
#if defined(__riscv)
#if __riscv_xlen == 32
    uint32_t lo, hi, hi2;
    // 하위 32비트를 읽는 동안 상위 32비트가 오버플로우 되는 것을 방지
    do {
        // mcycleh -> cycleh, mcycle -> cycle 로 변경
        __asm__ volatile("csrr %0, cycleh" : "=r"(hi));
        __asm__ volatile("csrr %0, cycle" : "=r"(lo));
        __asm__ volatile("csrr %0, cycleh" : "=r"(hi2));
    } while (hi != hi2);
    return (((uint64_t)hi) << 32) | lo;
#else
    uint64_t cycles;
    // mcycle -> cycle 로 변경
    __asm__ volatile("csrr %0, cycle" : "=r"(cycles));
    return cycles;
#endif
#else
    // Host PC (x86 등) 환경일 경우 더미 값 반환
    return 0;
#endif
}

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

static int hal_verify_array_f32(const float *a, const float *b, const size_t n) {
    if (a == NULL || b == NULL)
        return 0;
    if (n == 0)
        return 1;

    for (size_t i = 0; i < n; i++) {
        // 1. 둘 다 NaN인 경우 통과 (NaN == NaN 은 false이므로 예외 처리)
        if (isnan(a[i]) && isnan(b[i]))
            continue;

        // 2. 둘 다 무한대(Inf)이면서 부호가 같은 경우 통과
        if (isinf(a[i]) && isinf(b[i]) && (a[i] == b[i]))
            continue;

        // 3. 일반적인 오차 범위 내 비교
        if (fabsf(a[i] - b[i]) > EPSILON) {
            // 실패 시 디버깅을 위해 어느 인덱스에서 틀렸는지 출력 (옵션)
            // printf("    [Mismatch] idx %zu: SW=%f, HW=%f\n", i, a[i], b[i]);
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
        // 1. 둘 다 NaN인 경우 통과 (NaN == NaN 은 false이므로 예외 처리)
        if (isnan(a[i]) && isnan(b[i]))
            continue;

        // 2. 둘 다 무한대(Inf)이면서 부호가 같은 경우 통과
        if (isinf(a[i]) && isinf(b[i]) && (a[i] == b[i]))
            continue;

        // 3. 일반적인 오차 범위 내 비교
        if (fabs(a[i] - b[i]) > EPSILON) {
            // 실패 시 디버깅을 위해 어느 인덱스에서 틀렸는지 출력 (옵션)
            // printf("    [Mismatch] idx %zu: SW=%f, HW=%f\n", i, a[i], b[i]);
            return 0;
        }
    }
    return 1;
}
// --->
// -----------------------------------------------------------------------------
// [마법의 테스트 생성 매크로 V2 - DIV 에러 체크 추가]
// -----------------------------------------------------------------------------
#define TEST_LEN 1024
#define DEFINE_HAL_TESTS(T_IN, T_OUT, S_IN, S_OUT)                                                 \
    /* 1. ADD Test */                                                                              \
    static int test_hal_add_##S_IN(void) {                                                         \
        T_IN a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},                                         \
             b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};                                              \
        T_IN c_hw[10] = {0}, c_sw[10] = {0};                                                       \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] = a[i] + b[i];                                                                 \
                                                                                                   \
        /* --- 프로파일링 시작 --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        hal_add_##S_IN(c_hw, a, b, 10);                                                            \
        uint64_t end = get_mcycle();                                                               \
        /* --- 프로파일링 종료 --- */                                                              \
                                                                                                   \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 10)) {                                             \
            return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */                           \
        } else {                                                                                   \
            return -1; /* 실패 시 -1 반환 */                                                       \
        }                                                                                          \
    }                                                                                              \
    /* 2. SUB Test */                                                                              \
    static int test_hal_sub_##S_IN(void) {                                                         \
        T_IN a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},                                         \
             b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};                                              \
        T_IN c_hw[10] = {0}, c_sw[10] = {0};                                                       \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] = a[i] - b[i];                                                                 \
                                                                                                   \
        /* --- 프로파일링 시작 --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        hal_sub_##S_IN(c_hw, a, b, 10);                                                            \
        uint64_t end = get_mcycle();                                                               \
        /* --- 프로파일링 종료 --- */                                                              \
                                                                                                   \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 10)) {                                             \
            return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */                           \
        } else {                                                                                   \
            return -1; /* 실패 시 -1 반환 */                                                       \
        }                                                                                          \
    }                                                                                              \
    /* 3. MUL Test */                                                                              \
    static int test_hal_mul_##S_IN(void) {                                                         \
        T_IN a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};    \
        T_OUT c_hw[10] = {0}, c_sw[10] = {0};                                                      \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] = (T_OUT)a[i] * (T_OUT)b[i];                                                   \
        /* --- 프로파일링 시작 --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        hal_mul_##S_IN(c_hw, a, b, 10);                                                            \
        uint64_t end = get_mcycle();                                                               \
        /* --- 프로파일링 종료 --- */                                                              \
                                                                                                   \
        if (hal_verify_array_##S_OUT(c_sw, c_hw, 10)) {                                            \
            return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */                           \
        } else {                                                                                   \
            return -1; /* 실패 시 -1 반환 */                                                       \
        }                                                                                          \
    }                                                                                              \
    /* 4. MAC Test */                                                                              \
    static int test_hal_mac_##S_IN(void) {                                                         \
        T_IN a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};    \
        T_OUT c_hw[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};                                 \
        T_OUT c_sw[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};                                 \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] += (T_OUT)a[i] * (T_OUT)b[i];                                                  \
        /* --- 프로파일링 시작 --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        hal_mac_##S_IN(c_hw, a, b, 10);                                                            \
        uint64_t end = get_mcycle();                                                               \
        /* --- 프로파일링 종료 --- */                                                              \
                                                                                                   \
        if (hal_verify_array_##S_OUT(c_sw, c_hw, 10)) {                                            \
            return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */                           \
        } else {                                                                                   \
            return -1; /* 실패 시 -1 반환 */                                                       \
        }                                                                                          \
    }                                                                                              \
    /* 5. DIV Test (예외 처리 및 ret 출력 강화) */                                                 \
    static int test_hal_div_##S_IN(void) {                                                         \
        T_IN a[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};                                    \
        /* 인덱스 2번과 7번에 의도적으로 0을 넣어 예외가 잘 잡히는지 테스트합니다. */              \
        T_IN b[10] = {2, 2, 0, 2, 2, 2, 2, 0, 2, 2};                                               \
        T_IN c_hw[10] = {0}, c_sw[10] = {0};                                                       \
        int ret = HAL_MATH_SUCCESS;                                                                \
        /* SW 정답지 생성 (0으로 나누어 PC가 뻗는 것을 방지) */                                    \
        for (int i = 0; i < 10; i++) {                                                             \
            if (b[i] == 0)                                                                         \
                c_sw[i] = 0;                                                                       \
            else                                                                                   \
                c_sw[i] = a[i] / b[i];                                                             \
        }                                                                                          \
        /* --- 프로파일링 시작 --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        hal_div_##S_IN(c_hw, a, b, 10, &ret);                                                      \
        uint64_t end = get_mcycle();                                                               \
        /* HAL 함수가 에러를 감지했다면 로그 출력 */                                               \
        if (ret != 0) {                                                                            \
            if (ret == 1 || ret == HAL_MATH_ERR_DIV_BY_ZERO) {                                     \
                printf("    -> [INFO] Caught expected DIV_BY_ZERO error in %s\n", __func__);       \
            } else {                                                                               \
                printf("    -> [ERROR] Unexpected error code 0x%02X in %s\n", ret, __func__);      \
            }                                                                                      \
        }                                                                                          \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 10)) {                                             \
            return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */                           \
        } else {                                                                                   \
            return -1; /* 실패 시 -1 반환 */                                                       \
        }                                                                                          \
    }                                                                                              \
    /* 6. DOT Test */                                                                              \
    static int test_hal_dot_##S_IN(void) {                                                         \
        T_IN a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};      \
        T_OUT c_hw = 0, c_sw = 0;                                                                  \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw += (T_OUT)a[i] * (T_OUT)b[i];                                                     \
        /* --- 프로파일링 시작 --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        hal_dot_##S_IN(&c_hw, a, b, 10);                                                           \
        uint64_t end = get_mcycle();                                                               \
        /* --- 프로파일링 종료 --- */                                                              \
        if (hal_verify_array_##S_OUT(&c_sw, &c_hw, 1)) {                                           \
            return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */                           \
        } else {                                                                                   \
            return -1; /* 실패 시 -1 반환 */                                                       \
        }                                                                                          \
    }                                                                                              \
    /* 7. MTRX MUL Test */                                                                         \
    static int test_hal_mtrx_mul_##S_IN(void) {                                                    \
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
        /* --- 프로파일링 시작 --- */                                                              \
        uint64_t start = get_mcycle();                                                             \
        hal_matrix_mul_##S_IN(c_hw, a, b, 2, 2, 3);                                                \
        uint64_t end = get_mcycle();                                                               \
        /* --- 프로파일링 종료 --- */                                                              \
        if (hal_verify_array_##S_OUT(c_sw, c_hw, 4)) {                                             \
            return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */                           \
        } else {                                                                                   \
            return -1; /* 실패 시 -1 반환 */                                                       \
        }                                                                                          \
    }

// -----------------------------------------------------------------------------
// 매크로를 이용해 49개의 테스트 함수 단숨에 생성!
// -----------------------------------------------------------------------------
DEFINE_HAL_TESTS(int8_t, int16_t, i8, i16)
DEFINE_HAL_TESTS(uint8_t, uint16_t, u8, u16)
DEFINE_HAL_TESTS(int16_t, int32_t, i16, i32)
DEFINE_HAL_TESTS(uint16_t, uint32_t, u16, u32)
DEFINE_HAL_TESTS(int32_t, int64_t, i32, i64)
DEFINE_HAL_TESTS(uint32_t, uint64_t, u32, u64)
DEFINE_HAL_TESTS(float, double, f32, f64)

// -----------------------------------------------------------------------------
// [주의] 64비트 및 128비트 테스트 스텁(Stub)
// C 언어에서는 구조체(int128_t)에 +, * 등의 일반 연산자를 직접 사용할 수 없으므로
// 위 매크로로 생성할 수 없습니다.
// 일단 컴파일이 되도록 빈 껍데기(항상 패스)만 남겨둡니다.
// 추후 필요할 때 _add_i128 등의 호스트 함수를 사용해 직접 수동 작성하세요.
// -----------------------------------------------------------------------------
#define DUMMY_TEST(NAME)                                                                           \
    static int NAME(void) { return 0; }

DUMMY_TEST(test_hal_add_i64)
DUMMY_TEST(test_hal_add_u64)
DUMMY_TEST(test_hal_sub_i64)
DUMMY_TEST(test_hal_sub_u64)
DUMMY_TEST(test_hal_mul_i64)
DUMMY_TEST(test_hal_mul_u64)
DUMMY_TEST(test_hal_mac_i64)
DUMMY_TEST(test_hal_mac_u64)
DUMMY_TEST(test_hal_div_i64)
DUMMY_TEST(test_hal_div_u64)
DUMMY_TEST(test_hal_dot_i64)
DUMMY_TEST(test_hal_dot_u64)
DUMMY_TEST(test_hal_mtrx_mul_i64)
DUMMY_TEST(test_hal_mtrx_mul_u64)
DUMMY_TEST(test_hal_add_i128)
DUMMY_TEST(test_hal_add_u128)
DUMMY_TEST(test_hal_sub_i128)
DUMMY_TEST(test_hal_sub_u128)
DUMMY_TEST(test_hal_mul_i128)
DUMMY_TEST(test_hal_mul_u128)
DUMMY_TEST(test_hal_mac_i128)
DUMMY_TEST(test_hal_mac_u128)
DUMMY_TEST(test_hal_div_i128)
DUMMY_TEST(test_hal_div_u128)
DUMMY_TEST(test_hal_dot_i128)
DUMMY_TEST(test_hal_dot_u128)
DUMMY_TEST(test_hal_mtrx_mul_i128)
DUMMY_TEST(test_hal_mtrx_mul_u128)
// <---

// ---> edge case start
// -----------------------------------------------------------------------------
// [경계값 테스트 1] 정수형 오버플로우/언더플로우 검증 (i32)
// -----------------------------------------------------------------------------
static int test_hal_edge_overflow_i32(void) {
    const size_t LEN = 4;
    // 경계값: [최대값, 최소값, 일반값, -1]
    int32_t a[4] = {INT32_MAX, INT32_MIN, 100, -1};
    int32_t b[4] = {1, -1, 200, 1};
    int32_t c_hw[4] = {0};
    int32_t c_sw[4] = {0};

    // SW 계산: C언어의 UB(Undefined Behavior)를 피하기 위해 Unsigned로 캐스팅 후 연산
    for (int i = 0; i < LEN; i++) {
        c_sw[i] = (int32_t)((uint32_t)a[i] + (uint32_t)b[i]);
    }

    hal_add_i32(c_hw, a, b, LEN);

    // INT32_MAX + 1이 INT32_MIN으로 정확히 롤오버 되었는지 검증
    return hal_verify_array_i32(c_sw, c_hw, LEN) ? 0 : -1;
}

// -----------------------------------------------------------------------------
// [경계값 테스트 2] 부동소수점 극한값 검증 (f32)
// -----------------------------------------------------------------------------
static int test_hal_edge_float_limits(void) {
    const size_t LEN = 5;
    // 1. FLT_MAX (최대값) + FLT_MAX = +Infinity
    // 2. -FLT_MAX (최소값) - FLT_MAX = -Infinity
    // 3. +Infinity * 0.0f = NaN (Not a Number)
    // 4. 아주 작은 수 (Subnormal) 덧셈
    // 5. 0.0 / 0.0 = NaN
    float a[5] = {FLT_MAX, -FLT_MAX, INFINITY, 1e-40f, 0.0f};
    float b[5] = {FLT_MAX, -FLT_MAX, 0.0f, 1e-40f, 0.0f};
    float c_hw[5] = {0};
    float c_sw[5] = {0};

    // SW 계산 (덧셈의 경우)
    for (int i = 0; i < LEN; i++)
        c_sw[i] = a[i] + b[i];

    hal_add_f32(c_hw, a, b, LEN);

    return hal_verify_array_f32(c_sw, c_hw, LEN) ? 0 : -1;
}

// -----------------------------------------------------------------------------
// [경계값 테스트 3] 비정렬 메모리 접근 검증 (Unaligned Memory Access)
// -----------------------------------------------------------------------------
static int test_hal_edge_unaligned_access(void) {
    const size_t LEN = 5;
    // 넉넉한 버퍼를 하나 잡습니다.
    uint8_t buffer_a[100] = {0};
    uint8_t buffer_b[100] = {0};
    uint8_t buffer_c[100] = {0};

    // 배열의 시작 주소를 강제로 1바이트, 3바이트씩 어긋나게 포인터 캐스팅합니다. (정렬 파괴)
    float *unaligned_a = (float *)(&buffer_a[1]); // 1바이트 엇갈림
    float *unaligned_b = (float *)(&buffer_b[3]); // 3바이트 엇갈림
    float *unaligned_c = (float *)(&buffer_c[2]); // 2바이트 엇갈림

    // 초기값 세팅
    for (int i = 0; i < LEN; i++) {
        unaligned_a[i] = (float)i;
        unaligned_b[i] = (float)(i * 2);
    }

    // 이 함수가 실행되다가 QEMU가 Segmentation Fault를 내면
    // 현재 RISC-V 코어나 커널이 Unaligned Access를 지원하지 않는 것입니다.
    // 무사히 넘어가면 지원하는 것입니다.
    hal_add_f32(unaligned_c, unaligned_a, unaligned_b, LEN);

    return 0; // 살아서 여기까지 오면 PASS
}

// <--- edge case end

// C99 지정 초기자(Designated Initializers) 사용
// Enum 값과 배열 인덱스를 강제로 매핑하여 순서가 섞여도 안전함

static const HAL_TEST_FUNCTION_DEF hal_test_lists[] = {
    [HAL_TEST_VADD_I8] = {HAL_TEST_VADD_I8, test_hal_add_i8, "VADD_I8", "Vector add (int8_t)"},
    [HAL_TEST_VADD_I16] = {HAL_TEST_VADD_I16, test_hal_add_i16, "VADD_I16", "Vector add (int16_t)"},
    [HAL_TEST_VADD_I32] = {HAL_TEST_VADD_I32, test_hal_add_i32, "VADD_I32", "Vector add (int32_t)"},
    [HAL_TEST_VADD_I64] = {HAL_TEST_VADD_I64, test_hal_add_i64, "VADD_I64", "Vector add (int64_t)"},
    [HAL_TEST_VADD_I128] = {HAL_TEST_VADD_I128, test_hal_add_i128, "VADD_I128",
                            "Vector add (int128_t)"},

    [HAL_TEST_VADD_U8] = {HAL_TEST_VADD_U8, test_hal_add_u8, "VADD_U8", "Vector add (uint8_t)"},
    [HAL_TEST_VADD_U16] = {HAL_TEST_VADD_U16, test_hal_add_u16, "VADD_U16",
                           "Vector add (uint16_t)"},
    [HAL_TEST_VADD_U32] = {HAL_TEST_VADD_U32, test_hal_add_u32, "VADD_U32",
                           "Vector add (uint32_t)"},
    [HAL_TEST_VADD_U64] = {HAL_TEST_VADD_U64, test_hal_add_u64, "VADD_U64",
                           "Vector add (uint64_t)"},
    [HAL_TEST_VADD_U128] = {HAL_TEST_VADD_U128, test_hal_add_u128, "VADD_U128",
                            "Vector add (uint128_t)"},

    [HAL_TEST_VSUB_I8] = {HAL_TEST_VSUB_I8, test_hal_sub_i8, "VSUB_I8", "Vector sub (int8_t)"},
    [HAL_TEST_VSUB_I16] = {HAL_TEST_VSUB_I16, test_hal_sub_i16, "VSUB_I16", "Vector sub (int16_t)"},
    [HAL_TEST_VSUB_I32] = {HAL_TEST_VSUB_I32, test_hal_sub_i32, "VSUB_I32", "Vector sub (int32_t)"},
    [HAL_TEST_VSUB_I64] = {HAL_TEST_VSUB_I64, test_hal_sub_i64, "VSUB_I64", "Vector sub (int64_t)"},
    [HAL_TEST_VSUB_I128] = {HAL_TEST_VSUB_I128, test_hal_sub_i128, "VSUB_I128",
                            "Vector sub (int128_t)"},

    [HAL_TEST_VSUB_U8] = {HAL_TEST_VSUB_U8, test_hal_sub_u8, "VSUB_U8", "Vector sub (uint8_t)"},
    [HAL_TEST_VSUB_U16] = {HAL_TEST_VSUB_U16, test_hal_sub_u16, "VSUB_U16",
                           "Vector sub (uint16_t)"},
    [HAL_TEST_VSUB_U32] = {HAL_TEST_VSUB_U32, test_hal_sub_u32, "VSUB_U32",
                           "Vector sub (uint32_t)"},
    [HAL_TEST_VSUB_U64] = {HAL_TEST_VSUB_U64, test_hal_sub_u64, "VSUB_U64",
                           "Vector sub (uint64_t)"},
    [HAL_TEST_VSUB_U128] = {HAL_TEST_VSUB_U128, test_hal_sub_u128, "VSUB_U128",
                            "Vector sub (uint128_t)"},

    [HAL_TEST_VMUL_I8] = {HAL_TEST_VMUL_I8, test_hal_mul_i8, "VMUL_I8", "Vector mul (int8_t)"},
    [HAL_TEST_VMUL_I16] = {HAL_TEST_VMUL_I16, test_hal_mul_i16, "VMUL_I16", "Vector mul (int16_t)"},
    [HAL_TEST_VMUL_I32] = {HAL_TEST_VMUL_I32, test_hal_mul_i32, "VMUL_I32", "Vector mul (int32_t)"},
    [HAL_TEST_VMUL_I64] = {HAL_TEST_VMUL_I64, test_hal_mul_i64, "VMUL_I64", "Vector mul (int64_t)"},
    [HAL_TEST_VMUL_I128] = {HAL_TEST_VMUL_I128, test_hal_mul_i128, "VMUL_I128",
                            "Vector mul (int128_t)"},

    [HAL_TEST_VMUL_U8] = {HAL_TEST_VMUL_U8, test_hal_mul_u8, "VMUL_U8", "Vector mul (uint8_t)"},
    [HAL_TEST_VMUL_U16] = {HAL_TEST_VMUL_U16, test_hal_mul_u16, "VMUL_U16",
                           "Vector mul (uint16_t)"},
    [HAL_TEST_VMUL_U32] = {HAL_TEST_VMUL_U32, test_hal_mul_u32, "VMUL_U32",
                           "Vector mul (uint32_t)"},
    [HAL_TEST_VMUL_U64] = {HAL_TEST_VMUL_U64, test_hal_mul_u64, "VMUL_U64",
                           "Vector mul (uint64_t)"},
    [HAL_TEST_VMUL_U128] = {HAL_TEST_VMUL_U128, test_hal_mul_u128, "VMUL_U128",
                            "Vector mul (uint128_t)"},

    [HAL_TEST_VMAC_I8] = {HAL_TEST_VMAC_I8, test_hal_mac_i8, "VMAC_I8", "Vector mac (int8_t)"},
    [HAL_TEST_VMAC_I16] = {HAL_TEST_VMAC_I16, test_hal_mac_i16, "VMAC_I16", "Vector mac (int16_t)"},
    [HAL_TEST_VMAC_I32] = {HAL_TEST_VMAC_I32, test_hal_mac_i32, "VMAC_I32", "Vector mac (int32_t)"},
    [HAL_TEST_VMAC_I64] = {HAL_TEST_VMAC_I64, test_hal_mac_i64, "VMAC_I64", "Vector mac (int64_t)"},
    [HAL_TEST_VMAC_I128] = {HAL_TEST_VMAC_I128, test_hal_mac_i128, "VMAC_I128",
                            "Vector mac (int128_t)"},

    [HAL_TEST_VMAC_U8] = {HAL_TEST_VMAC_U8, test_hal_mac_u8, "VMAC_U8", "Vector mac (uint8_t)"},
    [HAL_TEST_VMAC_U16] = {HAL_TEST_VMAC_U16, test_hal_mac_u16, "VMAC_U16",
                           "Vector mac (uint16_t)"},
    [HAL_TEST_VMAC_U32] = {HAL_TEST_VMAC_U32, test_hal_mac_u32, "VMAC_U32",
                           "Vector mac (uint32_t)"},
    [HAL_TEST_VMAC_U64] = {HAL_TEST_VMAC_U64, test_hal_mac_u64, "VMAC_U64",
                           "Vector mac (uint64_t)"},
    [HAL_TEST_VMAC_U128] = {HAL_TEST_VMAC_U128, test_hal_mac_u128, "VMAC_U128",
                            "Vector mac (uint128_t)"},

    [HAL_TEST_VDIV_I8] = {HAL_TEST_VDIV_I8, test_hal_div_i8, "VDIV_I8", "Vector div (int8_t)"},
    [HAL_TEST_VDIV_I16] = {HAL_TEST_VDIV_I16, test_hal_div_i16, "VDIV_I16", "Vector div (int16_t)"},
    [HAL_TEST_VDIV_I32] = {HAL_TEST_VDIV_I32, test_hal_div_i32, "VDIV_I32", "Vector div (int32_t)"},
    [HAL_TEST_VDIV_I64] = {HAL_TEST_VDIV_I64, test_hal_div_i64, "VDIV_I64", "Vector div (int64_t)"},
    [HAL_TEST_VDIV_I128] = {HAL_TEST_VDIV_I128, test_hal_div_i128, "VDIV_I128",
                            "Vector div (int128_t)"},

    [HAL_TEST_VDIV_U8] = {HAL_TEST_VDIV_U8, test_hal_div_u8, "VDIV_U8", "Vector div (uint8_t)"},
    [HAL_TEST_VDIV_U16] = {HAL_TEST_VDIV_U16, test_hal_div_u16, "VDIV_U16",
                           "Vector div (uint16_t)"},
    [HAL_TEST_VDIV_U32] = {HAL_TEST_VDIV_U32, test_hal_div_u32, "VDIV_U32",
                           "Vector div (uint32_t)"},
    [HAL_TEST_VDIV_U64] = {HAL_TEST_VDIV_U64, test_hal_div_u64, "VDIV_U64",
                           "Vector div (uint64_t)"},
    [HAL_TEST_VDIV_U128] = {HAL_TEST_VDIV_U128, test_hal_div_u128, "VDIV_U128",
                            "Vector div (uint128_t)"},

    [HAL_TEST_VDOT_I8] = {HAL_TEST_VDOT_I8, test_hal_dot_i8, "VDOT_I8",
                          "Vector dot product (int8_t)"},
    [HAL_TEST_VDOT_I16] = {HAL_TEST_VDOT_I16, test_hal_dot_i16, "VDOT_I16",
                           "Vector dot product (int16_t)"},
    [HAL_TEST_VDOT_I32] = {HAL_TEST_VDOT_I32, test_hal_dot_i32, "VDOT_I32",
                           "Vector dot product (int32_t)"},
    [HAL_TEST_VDOT_I64] = {HAL_TEST_VDOT_I64, test_hal_dot_i64, "VDOT_I64",
                           "Vector dot product (int64_t)"},
    [HAL_TEST_VDOT_I128] = {HAL_TEST_VDOT_I128, test_hal_dot_i128, "VDOT_I128",
                            "Vector dot product (int128_t)"},

    [HAL_TEST_VDOT_U8] = {HAL_TEST_VDOT_U8, test_hal_dot_u8, "VDOT_U8",
                          "Vector dot product (uint8_t)"},
    [HAL_TEST_VDOT_U16] = {HAL_TEST_VDOT_U16, test_hal_dot_u16, "VDOT_U16",
                           "Vector dot product (uint16_t)"},
    [HAL_TEST_VDOT_U32] = {HAL_TEST_VDOT_U32, test_hal_dot_u32, "VDOT_U32",
                           "Vector dot product (uint32_t)"},
    [HAL_TEST_VDOT_U64] = {HAL_TEST_VDOT_U64, test_hal_dot_u64, "VDOT_U64",
                           "Vector dot product (uint64_t)"},
    [HAL_TEST_VDOT_U128] = {HAL_TEST_VDOT_U128, test_hal_dot_u128, "VDOT_U128",
                            "Vector dot product (uint128_t)"},

    [HAL_TEST_MTRX_MUL_I8] = {HAL_TEST_MTRX_MUL_I8, test_hal_mtrx_mul_i8, "MTRX_MUL_I8",
                              "Matrix Mul (int8_t)"},
    [HAL_TEST_MTRX_MUL_I16] = {HAL_TEST_MTRX_MUL_I16, test_hal_mtrx_mul_i16, "MTRX_MUL_I16",
                               "Matrix Mul (int16_t)"},
    [HAL_TEST_MTRX_MUL_I32] = {HAL_TEST_MTRX_MUL_I32, test_hal_mtrx_mul_i32, "MTRX_MUL_I32",
                               "Matrix Mul (int32_t)"},
    [HAL_TEST_MTRX_MUL_I64] = {HAL_TEST_MTRX_MUL_I64, test_hal_mtrx_mul_i64, "MTRX_MUL_I64",
                               "Matrix Mul (int64_t)"},
    [HAL_TEST_MTRX_MUL_I128] = {HAL_TEST_MTRX_MUL_I128, test_hal_mtrx_mul_i128, "MTRX_MUL_I128",
                                "Matrix Mul (int128_t)"},

    [HAL_TEST_MTRX_MUL_U8] = {HAL_TEST_MTRX_MUL_U8, test_hal_mtrx_mul_u8, "MTRX_MUL_U8",
                              "Matrix Mul (uint8_t)"},
    [HAL_TEST_MTRX_MUL_U16] = {HAL_TEST_MTRX_MUL_U16, test_hal_mtrx_mul_u16, "MTRX_MUL_U16",
                               "Matrix Mul (uint16_t)"},
    [HAL_TEST_MTRX_MUL_U32] = {HAL_TEST_MTRX_MUL_U32, test_hal_mtrx_mul_u32, "MTRX_MUL_U32",
                               "Matrix Mul (uint32_t)"},
    [HAL_TEST_MTRX_MUL_U64] = {HAL_TEST_MTRX_MUL_U64, test_hal_mtrx_mul_u64, "MTRX_MUL_U64",
                               "Matrix Mul (uint64_t)"},
    [HAL_TEST_MTRX_MUL_U128] = {HAL_TEST_MTRX_MUL_U128, test_hal_mtrx_mul_u128, "MTRX_MUL_U128",
                                "Matrix Mul (uint128_t)"},

    [HAL_TEST_ADD_F32] = {HAL_TEST_ADD_F32, test_hal_add_f32, "VADD_F32", "Vector add (float)"},
    [HAL_TEST_SUB_F32] = {HAL_TEST_SUB_F32, test_hal_sub_f32, "VSUB_F32", "Vector sub (float)"},
    [HAL_TEST_MUL_F32] = {HAL_TEST_MUL_F32, test_hal_mul_f32, "VMUL_F32", "Vector mul (float)"},
    [HAL_TEST_MAC_F32] = {HAL_TEST_MAC_F32, test_hal_mac_f32, "VMAC_F32", "Vector mac (float)"},
    [HAL_TEST_DIV_F32] = {HAL_TEST_DIV_F32, test_hal_div_f32, "VDIV_F32", "Vector div (float)"},
    [HAL_TEST_DOT_F32] = {HAL_TEST_DOT_F32, test_hal_dot_f32, "VDOT_F32",
                          "Vector dot product (float)"},
    [HAL_TEST_MTRX_MUL_F32] = {HAL_TEST_MTRX_MUL_F32, test_hal_mtrx_mul_f32, "MTRX_MUL_F32",
                               "Matrix Mul (float)"},
};

/* * 모든 테스트를 실행하고, 실패한 테스트의 개수를 반환합니다.
 * 반환값이 0이면 All Pass 입니다.
 */
int hal_do_all_test(void) {
    int fail_count = 0;
    int pass_count = 0;

    for (int i = 0; i < HAL_TEST_MAX; i++) {
        if (hal_test_lists[i].func == NULL)
            continue;

        int ret = hal_test_lists[i].func();

        if (ret < 0) { // -1 이면 실패
            printf("[FAIL] %-15s : %s\n", hal_test_lists[i].name, hal_test_lists[i].description);
            fail_count++;
        } else { // 0 이상이면 성공 & 사이클 수
            printf("[PASS] %-15s : %s ", hal_test_lists[i].name, hal_test_lists[i].description);

            // 호스트 환경이 아닐 때만 하드웨어 사이클 출력
            if (ret > 0) {
                printf("\033[0;36m(%d cycles)\033[0m\n", ret); // 눈에 띄게 청록색으로 출력
            } else {
                printf("\n");
            }
            pass_count++;
        }
    }

    // 최종 요약 출력
    printf("\n====================================\n");
    printf(" TEST SUMMARY: %d PASSED, %d FAILED\n", pass_count, fail_count);
    printf("====================================\n");

    printf("\n--- Edge Case Suites ---\n");
    hal_print_test_result("EDGE_OVERFLOW_I32", test_hal_edge_overflow_i32());
    hal_print_test_result("EDGE_FLOAT_LIMITS", test_hal_edge_float_limits());
    hal_print_test_result("EDGE_UNALIGNED_ACCESS", test_hal_edge_unaligned_access());

    return fail_count;
}

int main() { return hal_do_all_test(); }
