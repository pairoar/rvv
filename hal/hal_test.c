#include "hal_math.h"
#include "hal_test_mat_data.h"
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SAFE_FREE(ptr)                                                                             \
    do {                                                                                           \
        if (ptr != NULL) {                                                                         \
            free(ptr);                                                                             \
            ptr = NULL;                                                                            \
        }                                                                                          \
    } while (0)

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

    /* Matrix Tiled */
    HAL_TEST_MTRX_MUL_TILED_I8,
    HAL_TEST_MTRX_MUL_TILED_U8,
    HAL_TEST_MTRX_MUL_TILED_I16,
    HAL_TEST_MTRX_MUL_TILED_U16,
    HAL_TEST_MTRX_MUL_TILED_I32,
    HAL_TEST_MTRX_MUL_TILED_U32,
    HAL_TEST_MTRX_MUL_TILED_I64,
    HAL_TEST_MTRX_MUL_TILED_U64,
    HAL_TEST_MTRX_MUL_TILED_I128,
    HAL_TEST_MTRX_MUL_TILED_U128,

    /* float */
    HAL_TEST_ADD_F32,
    HAL_TEST_SUB_F32,
    HAL_TEST_MUL_F32,
    HAL_TEST_MAC_F32,
    HAL_TEST_DIV_F32,
    HAL_TEST_DOT_F32,
    HAL_TEST_MTRX_MUL_F32,
    HAL_TEST_MTRX_MUL_TILED_F32,

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
       __asm__ volatile("csrr %0, cycleh" : "=r"(hi) :: "memory");
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

// -----------------------------------------------------------------------------
// internal 64/128 operations
// -----------------------------------------------------------------------------
static uint128_t _add_u128_u64(const uint128_t a, const uint64_t b) {
    uint128_t res;
    res.l = a.l + b;
    res.u = a.u + ((res.l < a.l) ? 1 : 0); // Carry 발생 시 u에 1 추가
    return res;
}

static int128_t _add_i128_i64(const int128_t a, const int64_t b) {
    // 비트 연산은 Unsigned와 동일하므로 캐스팅하여 재사용 (코드 중복 제거)
    uint128_t res = _add_u128_u64(*(const uint128_t*)&a, (uint64_t)b);
    return *(int128_t*)&res;
}

static uint128_t _add_u128(const uint128_t a, const uint128_t b) {
    uint128_t c;
    c.l = a.l + b.l;
    c.u = a.u + b.u + ((c.l < a.l) ? 1 : 0);
    return c;
}

static int128_t _add_i128(const int128_t a, const int128_t b) {
    uint128_t c = _add_u128(*(const uint128_t*)&a, *(const uint128_t*)&b);
    return *(int128_t*)&c;
}

static uint256_t _add_u256(const uint256_t a, const uint256_t b) {
    uint256_t c = {0};
    uint64_t carry = 0;

    for (uint32_t i = 0; i < 4; i++) {
        uint64_t sum = a.d[i] + b.d[i] + carry;
        // 1. a.d[i] + b.d[i]에서 오버플로우 발생: sum < a.d[i]
        // 2. carry를 더해서 오버플로우 발생: carry && sum == a.d[i]
        carry = (sum < a.d[i]) || (carry && sum == a.d[i]) ? 1 : 0;
        c.d[i] = sum;
    }
    return c;
}

static int256_t _add_i256(const int256_t a, const int256_t b) {
    uint256_t c = _add_u256(*(const uint256_t*)&a, *(const uint256_t*)&b);
    return *(int256_t*)&c;
}

static uint128_t _mul_u64(const uint64_t a, const uint64_t b) {
    uint128_t res;
    uint64_t a_lo = (uint32_t)a, a_hi = a >> 32;
    uint64_t b_lo = (uint32_t)b, b_hi = b >> 32;

    uint64_t p0 = a_lo * b_lo;
    uint64_t p1 = a_lo * b_hi;
    uint64_t p2 = a_hi * b_lo;
    uint64_t p3 = a_hi * b_hi;

    uint64_t mid = (p0 >> 32) + (uint32_t)p1 + (uint32_t)p2;

    res.l = (p0 & 0xFFFFFFFFULL) | (mid << 32);
    res.u = p3 + (p1 >> 32) + (p2 >> 32) + (mid >> 32);

    return res;
}

static uint128_t _sub_u128(const uint128_t a, const uint128_t b) {
    uint128_t res;

    // 1. 하위 64비트 뺄셈
    res.l = a.l - b.l;

    // 2. 빌림수(Borrow) 발생 여부 확인
    // a.l이 b.l보다 작으면 하위 연산에서 상위 비트의 값을 빌려와야 함
    uint64_t borrow = (a.l > a.l + b.l) ? 1 : 0; // 또는 (a.l < b.l) ? 1 : 0;
    // 보다 명확한 borrow 판별: (a.l < b.l)
    borrow = (a.l < b.l) ? 1 : 0;

    // 3. 상위 64비트 연산 (빌림수 반영)
    res.u = a.u - b.u - borrow;

    return res;
}


static int128_t _sub_i128(const int128_t a, const int128_t b) {
    // 2의 보수 체계에서 뺄셈 비트 로직은 Unsigned와 동일함
    uint128_t res = _sub_u128(*(const uint128_t*)&a, *(const uint128_t*)&b);
    return *(int128_t*)&res;
}


static uint256_t _sub_u256(const uint256_t a, const uint256_t b) {
    uint256_t c = {0};
    uint64_t borrow = 0;

    for (int i = 0; i < 4; i++) {
        uint64_t sub = a.d[i] - b.d[i] - borrow;
        // 빌림수 발생 조건:
        // 1. a.d[i] < b.d[i] 인 경우
        // 2. borrow가 있었는데 결과적으로 a.d[i]와 b.d[i]가 같아져서 더 빌려와야 하는 경우
        if (borrow) {
            borrow = (a.d[i] <= b.d[i]) ? 1 : 0;
        } else {
            borrow = (a.d[i] < b.d[i]) ? 1 : 0;
        }
        c.d[i] = sub;
    }
    return c;
}

static int128_t _mul_i64(const int64_t a, const int64_t b) {
    // 1. 먼저 부호 없는 곱셈 수행
    uint128_t res_u = _mul_u64((uint64_t)a, (uint64_t)b);
    int128_t res = { (int64_t)res_u.l, (int64_t)res_u.u };

    // 2. 음수일 경우 2의 보수 체계에 맞게 상위 비트(u) 보정
    if (a < 0) res.u -= b;
    if (b < 0) res.u -= a;

    return res;
}

static uint256_t _mul_u128(const uint128_t a, const uint128_t b) {
    uint256_t res = {{0, 0, 0, 0}};

    // 4개의 부분 곱 (각각 128비트)
    uint128_t p00 = _mul_u64(a.l, b.l);
    uint128_t p01 = _mul_u64(a.l, b.u);
    uint128_t p10 = _mul_u64(a.u, b.l);
    uint128_t p11 = _mul_u64(a.u, b.u);

    // [d0] 하위 64비트
    res.d[0] = p00.l;

    // [d1] Carry 관리 및 계산
    uint64_t carry = 0;
    uint64_t sum = p00.u + p01.l;
    carry = (sum < p00.u) ? 1 : 0;

    sum += p10.l;
    carry += (sum < p10.l) ? 1 : 0;
    res.d[1] = sum;

    // [d2] Carry 관리 및 계산
    uint64_t carry2 = 0;
    sum = p11.l + p01.u;
    carry2 = (sum < p11.l) ? 1 : 0;

    sum += p10.u;
    carry2 += (sum < p10.u) ? 1 : 0;

    sum += carry; // 이전 d[1]에서 넘어온 carry 더하기
    carry2 += (sum < carry) ? 1 : 0;
    res.d[2] = sum;

    // [d3] 최상위 64비트 계산
    res.d[3] = p11.u + carry2;

    return res;
}

static int256_t _mul_i128(const int128_t a, const int128_t b) {
    // 1. 부호 없는 256비트 곱셈
    uint256_t res_u = _mul_u128(*(const uint128_t*)&a, *(const uint128_t*)&b);
    int256_t res = *(int256_t*)&res_u;

    // 2. 음수 입력에 대한 부호 확장(Sign Extension) 보정
    // 128비트 부호 있는 정수 곱셈에서도 2의 보수 보정 로직은 동일하게 상위 비트 뺄셈으로 처리됩니다.
    if (a.u < 0) {
        res.d[2] -= b.l;
        int64_t borrow = (res.d[2] > (uint64_t)(res.d[2] + b.l)) ? 1 : 0;
        res.d[3] -= (b.u + borrow);
    }
    if (b.u < 0) {
        res.d[2] -= a.l;
        int64_t borrow = (res.d[2] > (uint64_t)(res.d[2] + a.l)) ? 1 : 0;
        res.d[3] -= (a.u + borrow);
    }

    return res;
}

static int _cmp_ge_u128(const uint128_t a, const uint128_t b) {
    if (a.u > b.u) return 1;
    if (a.u < b.u) return 0;
    return (a.l >= b.l) ? 1 : 0;
}

static uint128_t _div_u128(uint128_t n, uint128_t d) {
    uint128_t q = {0, 0}; // 몫 (Quotient)
    uint128_t r = {0, 0}; // 나머지 (Remainder)

    //  0으로 나누기 방어
    if (d.l == 0 && d.u == 0) return q;

    // 127번 비트부터 0번 비트까지 Shift-and-Subtract 수행
    for (int i = 127; i >= 0; i--) {
        // r을 왼쪽으로 1칸 Shift
        r.u = (r.u << 1) | (r.l >> 63);
        r.l = (r.l << 1);

        // n의 i번째 비트를 r의 최하위 비트에 삽입
        uint64_t bit = (i >= 64) ? ((n.u >> (i - 64)) & 1) : ((n.l >> i) & 1);
        r.l |= bit;

        // r >= d 이면 r에서 d를 빼고 몫의 해당 비트를 1로 세팅
        if (_cmp_ge_u128(r, d)) {
            // r -= d (128비트 뺄셈)
            uint64_t prev_l = r.l;
            r.l -= d.l;
            r.u -= d.u + ((r.l > prev_l) ? 1 : 0); // 빌림수(Borrow) 처리

            // q의 i번째 비트 세팅
            if (i >= 64) q.u |= (1ULL << (i - 64));
            else         q.l |= (1ULL << i);
        }
    }
    return q;
}

static int128_t _div_i128(const int128_t n, const int128_t d) {
    // 1. 부호 판별
    int sign_n = (n.u < 0) ? -1 : 1;
    int sign_d = (d.u < 0) ? -1 : 1;
    int sign_res = sign_n * sign_d;

    // 2. 2의 보수 체계를 이용해 절대값(Unsigned)으로 변환
    uint128_t un = *(uint128_t*)&n;
    if (sign_n < 0) {
        un.l = ~un.l; un.u = ~un.u;
        un.l += 1;
        if (un.l == 0) un.u += 1;
    }

    uint128_t ud = *(uint128_t*)&d;
    if (sign_d < 0) {
        ud.l = ~ud.l; ud.u = ~ud.u;
        ud.l += 1;
        if (ud.l == 0) ud.u += 1;
    }

    // 3. 부호 없는 나눗셈 수행
    uint128_t uq = _div_u128(un, ud);

    // 4. 결과에 부호 적용 (음수여야 하면 다시 2의 보수 취함)
    if (sign_res < 0) {
        uq.l = ~uq.l; uq.u = ~uq.u;
        uq.l += 1;
        if (uq.l == 0) uq.u += 1;
    }

    return *(int128_t*)&uq;
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
        if ((a[i].d[3] != b[i].d[3]) || (a[i].d[2] != b[i].d[2]) || (a[i].d[1] != b[i].d[1]) || (a[i].d[0] != b[i].d[0])) {
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

static int hal_verify_array_u256(const uint256_t *a, const uint256_t *b, const size_t n) {
    int result = 0;

    if (a == NULL || b == NULL)
        goto RET;

    if (n == 0) {
        result = 1;
        goto RET;
    }

    for (size_t i = 0; i < n; i++) {
        if ((a[i].d[3] != b[i].d[3]) || (a[i].d[2] != b[i].d[2]) || (a[i].d[1] != b[i].d[1]) || (a[i].d[0] != b[i].d[0])) {
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
        /* --- 프로파일링 시작 --- */                                                                  \
        uint64_t start = get_mcycle();                                                             \
        hal_add_##S_IN(c_hw, a, b, 10);                                                            \
        uint64_t end = get_mcycle();                                                               \
        /* --- 프로파일링 종료 --- */                                                                  \
                                                                                                   \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 10)) {                                             \
            return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */                                 \
        } else {                                                                                   \
            return -1; /* 실패 시 -1 반환 */                                                          \
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
        /* --- 프로파일링 시작 --- */                                                                  \
        uint64_t start = get_mcycle();                                                             \
        hal_sub_##S_IN(c_hw, a, b, 10);                                                            \
        uint64_t end = get_mcycle();                                                               \
        /* --- 프로파일링 종료 --- */                                                                  \
                                                                                                   \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 10)) {                                             \
            return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */                                 \
        } else {                                                                                   \
            return -1; /* 실패 시 -1 반환 */                                                          \
        }                                                                                          \
    }                                                                                              \
    /* 3. MUL Test */                                                                              \
    static int test_hal_mul_##S_IN(void) {                                                         \
        T_IN a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};    \
        T_OUT c_hw[10] = {0}, c_sw[10] = {0};                                                      \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] = (T_OUT)a[i] * (T_OUT)b[i];                                                   \
        /* --- 프로파일링 시작 --- */                                                                  \
        uint64_t start = get_mcycle();                                                             \
        hal_mul_##S_IN(c_hw, a, b, 10);                                                            \
        uint64_t end = get_mcycle();                                                               \
        /* --- 프로파일링 종료 --- */                                                                  \
                                                                                                   \
        if (hal_verify_array_##S_OUT(c_sw, c_hw, 10)) {                                            \
            return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */                                 \
        } else {                                                                                   \
            return -1; /* 실패 시 -1 반환 */                                                          \
        }                                                                                          \
    }                                                                                              \
    /* 4. MAC Test */                                                                              \
    static int test_hal_mac_##S_IN(void) {                                                         \
        T_IN a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};    \
        T_OUT c_hw[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};                                 \
        T_OUT c_sw[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};                                 \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw[i] += (T_OUT)a[i] * (T_OUT)b[i];                                                  \
        /* --- 프로파일링 시작 --- */                                                                  \
        uint64_t start = get_mcycle();                                                             \
        hal_mac_##S_IN(c_hw, a, b, 10);                                                            \
        uint64_t end = get_mcycle();                                                               \
        /* --- 프로파일링 종료 --- */                                                                  \
                                                                                                   \
        if (hal_verify_array_##S_OUT(c_sw, c_hw, 10)) {                                            \
            return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */                                 \
        } else {                                                                                   \
            return -1; /* 실패 시 -1 반환 */                                                          \
        }                                                                                          \
    }                                                                                              \
    /* 5. DIV Test (예외 처리 및 ret 출력 강화) */                                                      \
    static int test_hal_div_##S_IN(void) {                                                         \
        T_IN a[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};                                    \
        /* 인덱스 2번과 7번에 의도적으로 0을 넣어 예외가 잘 잡히는지 테스트합니다. */                              \
        T_IN b[10] = {2, 2, 0, 2, 2, 2, 2, 0, 2, 2};                                               \
        T_IN c_hw[10] = {0}, c_sw[10] = {0};                                                       \
        int ret = HAL_MATH_SUCCESS;                                                                \
        /* SW 정답지 생성 (0으로 나누어 PC가 뻗는 것을 방지) */                                             \
        for (int i = 0; i < 10; i++) {                                                             \
            if (b[i] == 0)                                                                         \
                c_sw[i] = 0;                                                                       \
            else                                                                                   \
                c_sw[i] = a[i] / b[i];                                                             \
        }                                                                                          \
        /* --- 프로파일링 시작 --- */                                                                  \
        uint64_t start = get_mcycle();                                                             \
        hal_div_##S_IN(c_hw, a, b, 10, &ret);                                                      \
        uint64_t end = get_mcycle();                                                               \
        /* HAL 함수가 에러를 감지했다면 로그 출력 */                                                       \
        if (ret != 0) {                                                                            \
            if (ret == 1 || ret == HAL_MATH_ERR_DIV_BY_ZERO) {                                     \
                printf("    -> [INFO] Caught expected DIV_BY_ZERO error in %s\n", __func__);       \
            } else {                                                                               \
                printf("    -> [ERROR] Unexpected error code 0x%02X in %s\n", ret, __func__);      \
            }                                                                                      \
        }                                                                                          \
        if (hal_verify_array_##S_IN(c_sw, c_hw, 10)) {                                             \
            return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */                                 \
        } else {                                                                                   \
            return -1; /* 실패 시 -1 반환 */                                                          \
        }                                                                                          \
    }                                                                                              \
    /* 6. DOT Test */                                                                              \
    static int test_hal_dot_##S_IN(void) {                                                         \
        T_IN a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};      \
        T_OUT c_hw = 0, c_sw = 0;                                                                  \
        for (int i = 0; i < 10; i++)                                                               \
            c_sw += (T_OUT)a[i] * (T_OUT)b[i];                                                     \
        /* --- 프로파일링 시작 --- */                                                                  \
        uint64_t start = get_mcycle();                                                             \
        hal_dot_##S_IN(&c_hw, a, b, 10);                                                           \
        uint64_t end = get_mcycle();                                                               \
        /* --- 프로파일링 종료 --- */                                                                  \
        if (hal_verify_array_##S_OUT(&c_sw, &c_hw, 1)) {                                           \
            return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */                                 \
        } else {                                                                                   \
            return -1; /* 실패 시 -1 반환 */                                                          \
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
        /* --- 프로파일링 시작 --- */                                                                  \
        uint64_t start = get_mcycle();                                                             \
        hal_matrix_mul_##S_IN(c_hw, a, b, 2, 2, 3);                                                \
        uint64_t end = get_mcycle();                                                               \
        /* --- 프로파일링 종료 --- */                                                                  \
        if (hal_verify_array_##S_OUT(c_sw, c_hw, 4)) {                                             \
            return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */                                 \
        } else {                                                                                   \
            return -1; /* 실패 시 -1 반환 */                                                          \
        }                                                                                          \
    }                                                                                              \
    /* 8. MTRX MUL Tiled Test */                                                                   \
    static int test_hal_mtrx_mul_tiled_##S_IN(void) {                                              \
        int result = -1;                                                                           \
        int M = 131, N = 71, K = 79;                                                               \
        int tile_size = 64;                                                                        \
        T_IN *A = (T_IN *)malloc(sizeof(T_IN) * M * K);                                            \
        T_IN *B = (T_IN *)malloc(sizeof(T_IN) * K * N);                                            \
        T_OUT *CS = (T_OUT *)malloc(sizeof(T_OUT) * M * N);                                        \
        T_OUT *CH = (T_OUT *)malloc(sizeof(T_OUT) * M * N);                                        \
        T_IN (*pa)[K] = (T_IN (*)[K])A;                                                            \
        T_IN (*pb)[N] = (T_IN (*)[N])B;                                                            \
        T_OUT (*ps)[N] = (T_OUT (*)[N])CS;                                                         \
        T_OUT (*ph)[N] = (T_OUT (*)[N])CH;                                                         \
        for(int _idx = 0; _idx < M * K; _idx++) A[_idx] = (T_IN)1;                                 \
        for(int _idx = 0; _idx < K * N; _idx++) B[_idx] = (T_IN)2;                                 \
        memset(CS, 0, sizeof(T_OUT) * M * N);                                                      \
        memset(CH, 0, sizeof(T_OUT) * M * N);                                                      \
        /* 3. i-k-j 순서로 연산 (캐시 최적화 및 Sparse 대응) */                                           \
        for (int i = 0; i < M; i++) {                                                              \
            for (int k = 0; k < K; k++) {                                                          \
                if (pa[i][k] != 0) {                                                               \
                    for (int j = 0; j < N; j++) {                                                  \
                        ps[i][j] += (T_OUT)pa[i][k] * pb[k][j];                                    \
                    }                                                                              \
                }                                                                                  \
            }                                                                                      \
        }                                                                                          \
        /* --- 프로파일링 시작 --- */                                                                  \
        uint64_t start_hal = get_mcycle();                                                         \
        hal_matrix_mul_tiled_##S_IN(CH, A, B, M, N, K, tile_size);                                 \
        uint64_t end_hal = get_mcycle();                                                           \
        /* --- 프로파일링 종료 --- */                                                                  \
        if (hal_verify_array_##S_OUT(CS, CH, M*N)) {                                               \
            result = (int)(end_hal - start_hal); /* 성공 시 소요된 클럭 수 반환 */                       \
        }                                                                                          \
        SAFE_FREE(A);                                                                              \
        SAFE_FREE(B);                                                                              \
        SAFE_FREE(CS);                                                                             \
        SAFE_FREE(CH);                                                                             \
        return result;                                                                             \
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
// i64/u64 operations
// -----------------------------------------------------------------------------

// add_i64
static int test_hal_add_i64(void) {
        int64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},
             b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int64_t c_hw[10] = {0}, c_sw[10] = {0};
        for (int i = 0; i < 10; i++)
            c_sw[i] = a[i] + b[i];

        /* --- 프로파일링 시작 --- */
        uint64_t start = get_mcycle();
        hal_add_i64(c_hw, a, b, 10);
        uint64_t end = get_mcycle();
        /* --- 프로파일링 종료 --- */

        if (hal_verify_array_i64(c_sw, c_hw, 10)) {
            return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
        } else {
            return -1; /* 실패 시 -1 반환 */
        }
}

// add_u64
static int test_hal_add_u64(void) {
        uint64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},
             b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        uint64_t c_hw[10] = {0}, c_sw[10] = {0};
        for (int i = 0; i < 10; i++)
            c_sw[i] = a[i] + b[i];

        /* --- 프로파일링 시작 --- */
        uint64_t start = get_mcycle();
        hal_add_u64(c_hw, a, b, 10);
        uint64_t end = get_mcycle();
        /* --- 프로파일링 종료 --- */

        if (hal_verify_array_u64(c_sw, c_hw, 10)) {
            return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
        } else {
            return -1; /* 실패 시 -1 반환 */
        }
}

// sub_i64
static int test_hal_sub_i64(void) {
    int64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},
            b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = a[i] - b[i];

    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_sub_i64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */

    if (hal_verify_array_i64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// sub_u64
static int test_hal_sub_u64(void) {
    uint64_t a[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20},
            b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint64_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = a[i] - b[i];

    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_sub_u64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */

    if (hal_verify_array_u64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// mul_i64
static int test_hal_mul_i64(void) {
    int64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = _mul_i64(a[i], b[i]);
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_mul_i64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */

    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// mul_u64
static int test_hal_mul_u64(void) {
    uint64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    uint128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = _mul_u64(a[i], b[i]);
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_mul_u64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */

    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// mac_i64
static int test_hal_mac_i64(void) {
    int64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int128_t c_hw[10] = {{0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}};
    int128_t c_sw[10] = {{0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}};

    for (int i = 0; i < 10; i++) {
        int128_t prod = _mul_i64(a[i], b[i]);
        c_sw[i] = _add_i128(c_sw[i], prod);
    }
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_mac_i64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */

    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// mac_u64
static int test_hal_mac_u64(void) {
    uint64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    uint128_t c_hw[10] = {{0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}};
    uint128_t c_sw[10] = {{0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}, {0,10}};
    
    for (int i = 0; i < 10; i++) {
        uint128_t prod = _mul_u64(a[i], b[i]);
        c_sw[i] = _add_u128(c_sw[i], prod);
    }
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_mac_u64(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */

    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// div_i64
static int test_hal_div_i64(void) {
    int64_t a[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    /* 인덱스 2번과 7번에 의도적으로 0을 넣어 예외가 잘 잡히는지 테스트합니다. */
    int64_t b[10] = {2, 2, 0, 2, 2, 2, 2, 0, 2, 2};
    int64_t c_hw[10] = {0}, c_sw[10] = {0};
    int ret = HAL_MATH_SUCCESS;
    /* SW 정답지 생성 (0으로 나누어 PC가 뻗는 것을 방지) */
    for (int i = 0; i < 10; i++) {
        if (b[i] == 0)
            c_sw[i] = 0;
        else
            c_sw[i] = a[i] / b[i];
    }
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_div_i64(c_hw, a, b, 10, &ret);
    uint64_t end = get_mcycle();
    /* HAL 함수가 에러를 감지했다면 로그 출력 */
    if (ret != 0) {
        if (ret == 1 || ret == HAL_MATH_ERR_DIV_BY_ZERO) {
            printf("    -> [INFO] Caught expected DIV_BY_ZERO error in %s\n", __func__);
        } else {
            printf("    -> [ERROR] Unexpected error code 0x%02X in %s\n", ret, __func__);
        }
    }
    if (hal_verify_array_i64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// div_u64
static int test_hal_div_u64(void) {
    uint64_t a[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    /* 인덱스 2번과 7번에 의도적으로 0을 넣어 예외가 잘 잡히는지 테스트합니다. */
    uint64_t b[10] = {2, 2, 0, 2, 2, 2, 2, 0, 2, 2};
    uint64_t c_hw[10] = {0}, c_sw[10] = {0};
    int ret = HAL_MATH_SUCCESS;
    /* SW 정답지 생성 (0으로 나누어 PC가 뻗는 것을 방지) */
    for (int i = 0; i < 10; i++) {
        if (b[i] == 0)
            c_sw[i] = 0;
        else
            c_sw[i] = a[i] / b[i];
    }
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_div_u64(c_hw, a, b, 10, &ret);
    uint64_t end = get_mcycle();
    /* HAL 함수가 에러를 감지했다면 로그 출력 */
    if (ret != 0) {
        if (ret == 1 || ret == HAL_MATH_ERR_DIV_BY_ZERO) {
            printf("    -> [INFO] Caught expected DIV_BY_ZERO error in %s\n", __func__);
        } else {
            printf("    -> [ERROR] Unexpected error code 0x%02X in %s\n", ret, __func__);
        }
    }
    if (hal_verify_array_u64(c_sw, c_hw, 10)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// dot_i64
static int test_hal_dot_i64(void) {
    int64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int128_t c_hw = {0}, c_sw = {0};
    for (int i = 0; i < 10; i++) {
        int128_t prod = _mul_i64(a[i], b[i]);
        c_sw = _add_i128(c_sw, prod);
    }
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_dot_i64(&c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */
    if (hal_verify_array_i128(&c_sw, &c_hw, 1)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// dot_u64
static int test_hal_dot_u64(void) {
    uint64_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, b[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    uint128_t c_hw = {0}, c_sw = {0};
    for (int i = 0; i < 10; i++) {
        uint128_t prod = _mul_u64(a[i], b[i]);
        c_sw = _add_u128(c_sw, prod);
    }
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_dot_u64(&c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */
    if (hal_verify_array_u128(&c_sw, &c_hw, 1)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// matrix_mul_i64
static int test_hal_mtrx_mul_i64(void) {
    int64_t a[6] = {1, 2, 3, 4, 5, 6}, b[6] = {1, 2, 3, 4, 5, 6};
    int128_t c_hw[4] = {0}, c_sw[4] = {0};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int128_t sum = {0};
            for (int k = 0; k < 3; k++) {
                sum = _add_i128(_mul_i64(a[i * 3 + k], b[k * 2 + j]), sum);
            }
            c_sw[i * 2 + j] = sum;
        }
    }
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_matrix_mul_i64(c_hw, a, b, 2, 2, 3);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */
    if (hal_verify_array_i128(c_sw, c_hw, 4)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// matrix_mul_u64
static int test_hal_mtrx_mul_u64(void) {
    uint64_t a[6] = {1, 2, 3, 4, 5, 6}, b[6] = {1, 2, 3, 4, 5, 6};
    uint128_t c_hw[4] = {0}, c_sw[4] = {0};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            uint128_t sum = {0};
            for (int k = 0; k < 3; k++) {
                sum = _add_u128(_mul_u64(a[i * 3 + k], b[k * 2 + j]), sum);
            }
            c_sw[i * 2 + j] = sum;
        }
    }
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_matrix_mul_u64(c_hw, a, b, 2, 2, 3);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */
    if (hal_verify_array_u128(c_sw, c_hw, 4)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}


// mtrx_mul_tile_i64
static int test_hal_mtrx_mul_tiled_i64(void) {
    int result = -1;
    int M = 131, N = 71, K = 79;
    int tile_size = 64;
    int64_t *A = (int64_t *)malloc(sizeof(int64_t) * M * K);
    int64_t *B = (int64_t *)malloc(sizeof(int64_t) * K * N);
    int128_t *CS = (int128_t *)malloc(sizeof(int128_t) * M * N);
    int128_t *CH = (int128_t *)malloc(sizeof(int128_t) * M * N);
    int64_t (*pa)[K] = (int64_t (*)[K])A;
    int64_t (*pb)[N] = (int64_t (*)[N])B;
    int128_t (*ps)[N] = (int128_t (*)[N])CS;
    int128_t (*ph)[N] = (int128_t (*)[N])CH;
    memset(A, 0, sizeof(int64_t) * M * K);
    memset(B, 0, sizeof(int64_t) * K * N);
    memset(CS, 0, sizeof(int128_t) * M * N);
    memset(CH, 0, sizeof(int128_t) * M * N);
    
    const int8_t (*ta)[K] = (const int8_t (*)[K])test_mat_a;
    const int8_t (*tb)[K] = (const int8_t (*)[N])test_mat_b;
    const int16_t (*tc)[K] = (const int16_t (*)[N])test_mat_c;

    // fill matrix
    int i, j;
    // a[]
    for (i=0; i<M; i++) {
        for (j=0; j<K;j++) {
            pa[i][j] = (int64_t)ta[i][j];
        }
    }
    // b[]
    for (i=0; i<K; i++) {
        for (j=0; j<N;j++) {
            pb[i][j] = (int64_t)tb[i][j];
        }
    }
    // c[]
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    int128_t prod = _mul_i64(pa[i][k], pb[k][j]);
                    ps[i][j] = _add_i128(ps[i][j], prod);
                }
            }
        }
    }

    /* --- 프로파일링 시작 --- */
    uint64_t start_hal = get_mcycle();
    hal_matrix_mul_tiled_i64(CH, A, B, M, N, K, tile_size);
    uint64_t end_hal = get_mcycle();
    /* --- 프로파일링 종료 --- */
    if (hal_verify_array_i128(CS, CH, M*N)) {
        result = (int)(end_hal - start_hal); /* 성공 시 소요된 클럭 수 반환 */
    }
    SAFE_FREE(A);
    SAFE_FREE(B);
    SAFE_FREE(CS);
    SAFE_FREE(CH);
    return result;
}


// mtrx_mul_tile_u64
static int test_hal_mtrx_mul_tiled_u64(void) {
    int result = -1;
    int M = 131, N = 71, K = 79;
    int tile_size = 64;
    uint64_t *A = (uint64_t *)malloc(sizeof(uint64_t) * M * K);
    uint64_t *B = (uint64_t *)malloc(sizeof(uint64_t) * K * N);
    uint128_t *CS = (uint128_t *)malloc(sizeof(uint128_t) * M * N);
    uint128_t *CH = (uint128_t *)malloc(sizeof(uint128_t) * M * N);
    uint64_t (*pa)[K] = (uint64_t (*)[K])A;
    uint64_t (*pb)[N] = (uint64_t (*)[N])B;
    uint128_t (*ps)[N] = (uint128_t (*)[N])CS;
    uint128_t (*ph)[N] = (uint128_t (*)[N])CH;
    memset(A, 0, sizeof(uint64_t) * M * K);
    memset(B, 0, sizeof(uint64_t) * K * N);
    memset(CS, 0, sizeof(uint128_t) * M * N);
    memset(CH, 0, sizeof(uint128_t) * M * N);
    
    const int8_t (*ta)[K] = (const int8_t (*)[K])test_mat_a;
    const int8_t (*tb)[K] = (const int8_t (*)[N])test_mat_b;
    const int16_t (*tc)[K] = (const int16_t (*)[N])test_mat_c;

    // fill matrix
    int i, j;
    // a[]
    for (i=0; i<M; i++) {
        for (j=0; j<K;j++) {
            pa[i][j] = (uint64_t)ta[i][j];
        }
    }
    // b[]
    for (i=0; i<K; i++) {
        for (j=0; j<N;j++) {
            pb[i][j] = (uint64_t)tb[i][j];
        }
    }
    // c[]
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    uint128_t prod = _mul_u64(pa[i][k], pb[k][j]);
                    ps[i][j] = _add_u128(ps[i][j], prod);
                }
            }
        }
    }

    /* --- 프로파일링 시작 --- */
    uint64_t start_hal = get_mcycle();
    hal_matrix_mul_tiled_u64(CH, A, B, M, N, K, tile_size);
    uint64_t end_hal = get_mcycle();
    /* --- 프로파일링 종료 --- */
    if (hal_verify_array_u128(CS, CH, M*N)) {
        result = (int)(end_hal - start_hal); /* 성공 시 소요된 클럭 수 반환 */
    } 

    SAFE_FREE(A);
    SAFE_FREE(B);
    SAFE_FREE(CS);
    SAFE_FREE(CH);

    return result;
}

// -----------------------------------------------------------------------------
// i128/u128 operations
// -----------------------------------------------------------------------------

// add_i128
static int test_hal_add_i128(void) {
    int128_t a[10] = {{0,2}, {0,4}, {0,6}, {0,8}, {0,10}, {0,12}, {0,14}, {0,16}, {0,18}, {0,20}},
            b[10] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10}};
    int128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = _add_i128(a[i], b[i]);

    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_add_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */

    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// add_u128
static int test_hal_add_u128(void) {
        uint128_t a[10] = {{0,2}, {0,4}, {0,6}, {0,8}, {0,10}, {0,12}, {0,14}, {0,16}, {0,18}, {0,20}},
            b[10] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10}};
        uint128_t c_hw[10] = {0}, c_sw[10] = {0};
        for (int i = 0; i < 10; i++)
            c_sw[i] = _add_u128(a[i], b[i]);

        /* --- 프로파일링 시작 --- */
        uint64_t start = get_mcycle();
        hal_add_u128(c_hw, a, b, 10);
        uint64_t end = get_mcycle();
        /* --- 프로파일링 종료 --- */

        if (hal_verify_array_u128(c_sw, c_hw, 10)) {
            return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
        } else {
            return -1; /* 실패 시 -1 반환 */
        }
}

// sub_i128
static int test_hal_sub_i128(void) {
    int128_t a[10] = {{0,2}, {0,4}, {0,6}, {0,8}, {0,10}, {0,12}, {0,14}, {0,16}, {0,18}, {0,20}},
            b[10] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10}};
    int128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = _sub_i128(a[i], b[i]);

    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_sub_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */

    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// sub_u128
static int test_hal_sub_u128(void) {
    uint128_t a[10] = {{0,2}, {0,4}, {0,6}, {0,8}, {0,10}, {0,12}, {0,14}, {0,16}, {0,18}, {0,20}},
            b[10] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10}};
    uint128_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = _sub_u128(a[i], b[i]);

    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_sub_u128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */

    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// mul_i128
static int test_hal_mul_i128(void) {
    int128_t a[10] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10}}, b[10] = {{0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10}, {0,11}};
    int256_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = _mul_i128(a[i], b[i]);
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_mul_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */

    if (hal_verify_array_i256(c_sw, c_hw, 10)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// mul_u128
static int test_hal_mul_u128(void) {
    uint128_t a[10] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10}}, b[10] = {{0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10}, {0,11}};
    uint256_t c_hw[10] = {0}, c_sw[10] = {0};
    for (int i = 0; i < 10; i++)
        c_sw[i] = _mul_u128(a[i], b[i]);
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_mul_u128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */

    if (hal_verify_array_u256(c_sw, c_hw, 10)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// mac_i128
static int test_hal_mac_i128(void) {
    int128_t a[10] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10}}, b[10] = {{0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10}, {0,11}};
    int256_t c_hw[10] = {{0,{0,0,10}}, {0,{0,0,10}}, {0,{0,0,10}}, {0,{0,0,10}}, {0,{0,0,10}}, {0,{0,0,10}}, {0,{0,0,10}}, {0,{0,0,10}}, {0,{0,0,10}}, {0,{0,0,10}}};
    int256_t c_sw[10] = {{0,{0,0,10}}, {0,{0,0,10}}, {0,{0,0,10}}, {0,{0,0,10}}, {0,{0,0,10}}, {0,{0,0,10}}, {0,{0,0,10}}, {0,{0,0,10}}, {0,{0,0,10}}, {0,{0,0,10}}};

    for (int i = 0; i < 10; i++) {
        int256_t prod = _mul_i128(a[i], b[i]);
        c_sw[i] = _add_i256(c_sw[i], prod);
    }
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_mac_i128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */

    if (hal_verify_array_i256(c_sw, c_hw, 10)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// mac_u128
static int test_hal_mac_u128(void) {
    uint128_t a[10] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10}}, b[10] = {{0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10}, {0,11}};
    uint256_t c_hw[10] = {{0,0,0,10}, {0,0,0,10}, {0,0,0,10}, {0,0,0,10}, {0,0,0,10}, {0,0,0,10}, {0,0,0,10}, {0,0,0,10}, {0,0,0,10}, {0,0,0,10}};
    uint256_t c_sw[10] = {{0,0,0,10}, {0,0,0,10}, {0,0,0,10}, {0,0,0,10}, {0,0,0,10}, {0,0,0,10}, {0,0,0,10}, {0,0,0,10}, {0,0,0,10}, {0,0,0,10}};
    
    for (int i = 0; i < 10; i++) {
        uint256_t prod = _mul_u128(a[i], b[i]);
        c_sw[i] = _add_u256(c_sw[i], prod);
    }
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_mac_u128(c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */

    if (hal_verify_array_u256(c_sw, c_hw, 10)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// div_i64
static int test_hal_div_i128(void) {
    int128_t a[10] = {{0,10}, {0,20}, {0,30}, {0,40}, {0,50}, {0,60}, {0,70}, {0,80}, {0,90}, {0,100}};
    /* 인덱스 2번과 7번에 의도적으로 0을 넣어 예외가 잘 잡히는지 테스트합니다. */
    int128_t b[10] = {{0,2}, {0,2}, {0}, {0,2}, {0,2}, {0,2}, {0,2}, {0}, {0,2}, {0,2}};
    int128_t c_hw[10] = {0}, c_sw[10] = {0};
    int ret = HAL_MATH_SUCCESS;
    /* SW 정답지 생성 (0으로 나누어 PC가 뻗는 것을 방지) */
    for (int i = 0; i < 10; i++) {
        if ((b[i].u == 0) && (b[i].l == 0)) {
            c_sw[i].u = 0;
            c_sw[i].l = 0;
        } else {
            c_sw[i] = _div_i128(a[i], b[i]);
        }
    }
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_div_i128(c_hw, a, b, 10, &ret);
    uint64_t end = get_mcycle();
    /* HAL 함수가 에러를 감지했다면 로그 출력 */
    if (ret != 0) {
        if (ret == 1 || ret == HAL_MATH_ERR_DIV_BY_ZERO) {
            printf("    -> [INFO] Caught expected DIV_BY_ZERO error in %s\n", __func__);
        } else {
            printf("    -> [ERROR] Unexpected error code 0x%02X in %s\n", ret, __func__);
        }
    }
    if (hal_verify_array_i128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// div_u128
static int test_hal_div_u128(void) {
    uint128_t a[10] = {{0,10}, {0,20}, {0,30}, {0,40}, {0,50}, {0,60}, {0,70}, {0,80}, {0,90}, {0,100}};
    /* 인덱스 2번과 7번에 의도적으로 0을 넣어 예외가 잘 잡히는지 테스트합니다. */
    uint128_t b[10] = {{0,2}, {0,2}, {0}, {0,2}, {0,2}, {0,2}, {0,2}, {0}, {0,2}, {0,2}};
    uint128_t c_hw[10] = {0}, c_sw[10] = {0};
    int ret = HAL_MATH_SUCCESS;
    /* SW 정답지 생성 (0으로 나누어 PC가 뻗는 것을 방지) */
    for (int i = 0; i < 10; i++) {
        if ((b[i].u == 0) && (b[i].l == 0)) {
            c_sw[i].u = 0;
            c_sw[i].l = 0;
        } else {
            c_sw[i] = _div_u128(a[i], b[i]);
        }
    }
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_div_u128(c_hw, a, b, 10, &ret);
    uint64_t end = get_mcycle();
    /* HAL 함수가 에러를 감지했다면 로그 출력 */
    if (ret != 0) {
        if (ret == 1 || ret == HAL_MATH_ERR_DIV_BY_ZERO) {
            printf("    -> [INFO] Caught expected DIV_BY_ZERO error in %s\n", __func__);
        } else {
            printf("    -> [ERROR] Unexpected error code 0x%02X in %s\n", ret, __func__);
        }
    }
    if (hal_verify_array_u128(c_sw, c_hw, 10)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// dot_i128
static int test_hal_dot_i128(void) {
    int128_t a[10] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10}}, b[10] = {{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}};
    int256_t c_hw = {0}, c_sw = {0};
    for (int i = 0; i < 10; i++) {
        int256_t prod = _mul_i128(a[i], b[i]);
        c_sw = _add_i256(c_sw, prod);
    }
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_dot_i128(&c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */
    if (hal_verify_result_i256(c_sw, c_hw)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// dot_u128
static int test_hal_dot_u128(void) {
    uint128_t a[10] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10}}, b[10] = {{0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}, {0,1}};
    uint256_t c_hw = {0}, c_sw = {0};
    for (int i = 0; i < 10; i++) {
        uint256_t prod = _mul_u128(a[i], b[i]);
        c_sw = _add_u256(c_sw, prod);
    }
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_dot_u128(&c_hw, a, b, 10);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */
    if (hal_verify_result_u256(c_sw, c_hw)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// matrix_mul_i128
static int test_hal_mtrx_mul_i128(void) {
    int128_t a[6] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}}, b[6] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}};
    int256_t c_hw[4] = {0}, c_sw[4] = {0};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int256_t sum = {0};
            for (int k = 0; k < 3; k++) {
                sum = _add_i256(_mul_i128(a[i * 3 + k], b[k * 2 + j]), sum);
            }
            c_sw[i * 2 + j] = sum;
        }
    }
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_matrix_mul_i128(c_hw, a, b, 2, 2, 3);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */
    if (hal_verify_array_i256(c_sw, c_hw, 4)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}

// matrix_mul_u128
static int test_hal_mtrx_mul_u128(void) {
    uint128_t a[6] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}}, b[6] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}};
    uint256_t c_hw[4] = {0}, c_sw[4] = {0};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            uint256_t sum = {0};
            for (int k = 0; k < 3; k++) {
                sum = _add_u256(_mul_u128(a[i * 3 + k], b[k * 2 + j]), sum);
            }
            c_sw[i * 2 + j] = sum;
        }
    }
    /* --- 프로파일링 시작 --- */
    uint64_t start = get_mcycle();
    hal_matrix_mul_u128(c_hw, a, b, 2, 2, 3);
    uint64_t end = get_mcycle();
    /* --- 프로파일링 종료 --- */
    if (hal_verify_array_u256(c_sw, c_hw, 4)) {
        return (int)(end - start); /* 성공 시 소요된 클럭 수 반환 */
    } else {
        return -1; /* 실패 시 -1 반환 */
    }
}


// mtrx_mul_tile_i128
static int test_hal_mtrx_mul_tiled_i128(void) {
    int result = -1;
    int M = 131, N = 71, K = 79;
    int tile_size = 64;
    int128_t *A = (int128_t *)malloc(sizeof(int128_t) * M * K);
    int128_t *B = (int128_t *)malloc(sizeof(int128_t) * K * N);
    int256_t *CS = (int256_t *)malloc(sizeof(int256_t) * M * N);
    int256_t *CH = (int256_t *)malloc(sizeof(int256_t) * M * N);
    int128_t (*pa)[K] = (int128_t (*)[K])A;
    int128_t (*pb)[N] = (int128_t (*)[N])B;
    int256_t (*ps)[N] = (int256_t (*)[N])CS;
    int256_t (*ph)[N] = (int256_t (*)[N])CH;
    memset(A, 0, sizeof(int128_t) * M * K);
    memset(B, 0, sizeof(int128_t) * K * N);
    memset(CS, 0, sizeof(int256_t) * M * N);
    memset(CH, 0, sizeof(int256_t) * M * N);
    
    const int8_t (*ta)[K] = (const int8_t (*)[K])test_mat_a;
    const int8_t (*tb)[K] = (const int8_t (*)[N])test_mat_b;
    const int16_t (*tc)[K] = (const int16_t (*)[N])test_mat_c;

    // fill matrix
    int i, j;
    // a[]
    for (i=0; i<M; i++) {
        for (j=0; j<K;j++) {
            pa[i][j].l = (int64_t)ta[i][j];
            pa[i][j].u = (ta[i][j] < 0) ? -1 : 0;
        }
    }
    // b[]
    for (i=0; i<K; i++) {
        for (j=0; j<N;j++) {
            pb[i][j].l = (int64_t)tb[i][j];
            pb[i][j].u = (tb[i][j] < 0) ? -1 : 0;
        }
    }
    // c[]
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k].l != 0 || pa[i][k].u != 0) {
                for (int j = 0; j < N; j++) {
                    int256_t prod = _mul_i128(pa[i][k], pb[k][j]);
                    ps[i][j] = _add_i256(ps[i][j], prod);
                }
            }
        }
    }

    /* --- 프로파일링 시작 --- */
    uint64_t start_hal = get_mcycle();
    hal_matrix_mul_tiled_i128(CH, A, B, M, N, K, tile_size);
    uint64_t end_hal = get_mcycle();
    /* --- 프로파일링 종료 --- */
    if (hal_verify_array_i256(CS, CH, M*N)) {
        result = (int)(end_hal - start_hal); /* 성공 시 소요된 클럭 수 반환 */
    }

    SAFE_FREE(A);
    SAFE_FREE(B);
    SAFE_FREE(CS);
    SAFE_FREE(CH);

    return result;
}


// mtrx_mul_tile_u128
static int test_hal_mtrx_mul_tiled_u128(void) {
    int result = -1;
    int M = 131, N = 71, K = 79;
    int tile_size = 64;
    uint128_t *A = (uint128_t *)malloc(sizeof(uint128_t) * M * K);
    uint128_t *B = (uint128_t *)malloc(sizeof(uint128_t) * K * N);
    uint256_t *CS = (uint256_t *)malloc(sizeof(uint256_t) * M * N);
    uint256_t *CH = (uint256_t *)malloc(sizeof(uint256_t) * M * N);
    uint128_t (*pa)[K] = (uint128_t (*)[K])A;
    uint128_t (*pb)[N] = (uint128_t (*)[N])B;
    uint256_t (*ps)[N] = (uint256_t (*)[N])CS;
    uint256_t (*ph)[N] = (uint256_t (*)[N])CH;
    memset(A, 0, sizeof(uint128_t) * M * K);
    memset(B, 0, sizeof(uint128_t) * K * N);
    memset(CS, 0, sizeof(uint256_t) * M * N);
    memset(CH, 0, sizeof(uint256_t) * M * N);
    
    const int8_t (*ta)[K] = (const int8_t (*)[K])test_mat_a;
    const int8_t (*tb)[K] = (const int8_t (*)[N])test_mat_b;
    const int16_t (*tc)[K] = (const int16_t (*)[N])test_mat_c;

    // fill matrix
    int i, j;
    // a[]
    for (i=0; i<M; i++) {
        for (j=0; j<K;j++) {
            pa[i][j].u = 0;
            pa[i][j].l = (uint64_t)ta[i][j];
        }
    }
    // b[]
    for (i=0; i<K; i++) {
        for (j=0; j<N;j++) {
            pb[i][j].l = (uint64_t)tb[i][j];
        }
    }
    
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k].l != 0 || pa[i][k].u != 0) {
                for (int j = 0; j < N; j++) {
                    uint256_t prod = _mul_u128(pa[i][k], pb[k][j]);
                    ps[i][j] = _add_u256(ps[i][j], prod);
                }
            }
        }
    }

    /* --- 프로파일링 시작 --- */
    uint64_t start_hal = get_mcycle();
    hal_matrix_mul_tiled_u128(CH, A, B, M, N, K, tile_size);
    uint64_t end_hal = get_mcycle();
    /* --- 프로파일링 종료 --- */
    if (hal_verify_array_u256(CS, CH, M*N)) {
        result = (int)(end_hal - start_hal); /* 성공 시 소요된 클럭 수 반환 */
    }

    SAFE_FREE(A);
    SAFE_FREE(B);
    SAFE_FREE(CS);
    SAFE_FREE(CH);

    return result;
}


// -----------------------------------------------------------------------------
// [주의] 64비트 및 128비트 테스트 스텁(Stub)
// C 언어에서는 구조체(int128_t)에 +, * 등의 일반 연산자를 직접 사용할 수 없으므로
// 위 매크로로 생성할 수 없습니다.
// 일단 컴파일이 되도록 빈 껍데기(항상 패스)만 남겨둡니다.
// 추후 필요할 때 _add_i128 등의 호스트 함수를 사용해 직접 수동 작성하세요.
// -----------------------------------------------------------------------------
#define DUMMY_TEST(NAME)                                                                           \
    static int NAME(void) { return 0; }

// DUMMY_TEST(test_hal_add_i64)
// DUMMY_TEST(test_hal_add_u64)
// DUMMY_TEST(test_hal_sub_i64)
// DUMMY_TEST(test_hal_sub_u64)
// DUMMY_TEST(test_hal_mul_i64)
// DUMMY_TEST(test_hal_mul_u64)
// DUMMY_TEST(test_hal_mac_i64)
// DUMMY_TEST(test_hal_mac_u64)
// DUMMY_TEST(test_hal_div_i64)
// DUMMY_TEST(test_hal_div_u64)
// DUMMY_TEST(test_hal_dot_i64)
// DUMMY_TEST(test_hal_dot_u64)
// DUMMY_TEST(test_hal_mtrx_mul_i64)
// DUMMY_TEST(test_hal_mtrx_mul_u64)
// DUMMY_TEST(test_hal_add_i128)
// DUMMY_TEST(test_hal_add_u128)
// DUMMY_TEST(test_hal_sub_i128)
// DUMMY_TEST(test_hal_sub_u128)
// DUMMY_TEST(test_hal_mul_i128)
// DUMMY_TEST(test_hal_mul_u128)
// DUMMY_TEST(test_hal_mac_i128)
// DUMMY_TEST(test_hal_mac_u128)
// DUMMY_TEST(test_hal_div_i128)
// DUMMY_TEST(test_hal_div_u128)
// DUMMY_TEST(test_hal_dot_i128)
// DUMMY_TEST(test_hal_dot_u128)
// DUMMY_TEST(test_hal_mtrx_mul_i128)
// DUMMY_TEST(test_hal_mtrx_mul_u128)

// DUMMY_TEST(test_hal_mtrx_mul_tiled_i64)
// DUMMY_TEST(test_hal_mtrx_mul_tiled_u64)
// DUMMY_TEST(test_hal_mtrx_mul_tiled_i128)
// DUMMY_TEST(test_hal_mtrx_mul_tiled_u128)
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
    for (size_t i = 0; i < LEN; i++) {
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
    for (size_t i = 0; i < LEN; i++)
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
    for (size_t i = 0; i < LEN; i++) {
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

    [HAL_TEST_MTRX_MUL_TILED_I8] = {HAL_TEST_MTRX_MUL_TILED_I8, test_hal_mtrx_mul_tiled_i8, "MTRX_MUL_TILED_I8",
                              "Matrix Mul tiled (int8_t)"},
    [HAL_TEST_MTRX_MUL_TILED_I16] = {HAL_TEST_MTRX_MUL_TILED_I16, test_hal_mtrx_mul_tiled_i16, "MTRX_MUL_TILED_I16",
                               "Matrix Mul tiled (int16_t)"},
    [HAL_TEST_MTRX_MUL_TILED_I32] = {HAL_TEST_MTRX_MUL_TILED_I32, test_hal_mtrx_mul_tiled_i32, "MTRX_MUL_TILED_I32",
                               "Matrix Mul tiled (int32_t)"},
    [HAL_TEST_MTRX_MUL_TILED_I64] = {HAL_TEST_MTRX_MUL_TILED_I64, test_hal_mtrx_mul_tiled_i64, "MTRX_MUL_TILED_I64",
                               "Matrix Mul tiled (int64_t)"},
    [HAL_TEST_MTRX_MUL_TILED_I128] = {HAL_TEST_MTRX_MUL_TILED_I128, test_hal_mtrx_mul_tiled_i128, "MTRX_MUL_TILED_I128",
                                "Matrix Mul tiled (int128_t)"},

    [HAL_TEST_MTRX_MUL_TILED_U8] = {HAL_TEST_MTRX_MUL_TILED_U8, test_hal_mtrx_mul_tiled_u8, "MTRX_MUL_TILED_U8",
                              "Matrix Mul tiled (uint8_t)"},
    [HAL_TEST_MTRX_MUL_TILED_U16] = {HAL_TEST_MTRX_MUL_TILED_U16, test_hal_mtrx_mul_tiled_u16, "MTRX_MUL_TILED_U16",
                               "Matrix Mul tiled (uint16_t)"},
    [HAL_TEST_MTRX_MUL_TILED_U32] = {HAL_TEST_MTRX_MUL_TILED_U32, test_hal_mtrx_mul_tiled_u32, "MTRX_MUL_TILED_U32",
                               "Matrix Mul tiled (uint32_t)"},
    [HAL_TEST_MTRX_MUL_TILED_U64] = {HAL_TEST_MTRX_MUL_TILED_U64, test_hal_mtrx_mul_tiled_u64, "MTRX_MUL_TILED_U64",
                               "Matrix Mul tiled (uint64_t)"},
    [HAL_TEST_MTRX_MUL_TILED_U128] = {HAL_TEST_MTRX_MUL_TILED_U128, test_hal_mtrx_mul_tiled_u128, "MTRX_MUL_TILED_U128",
                                "Matrix Mul tiled (uint128_t)"},

    [HAL_TEST_ADD_F32] = {HAL_TEST_ADD_F32, test_hal_add_f32, "VADD_F32", "Vector add (float)"},
    [HAL_TEST_SUB_F32] = {HAL_TEST_SUB_F32, test_hal_sub_f32, "VSUB_F32", "Vector sub (float)"},
    [HAL_TEST_MUL_F32] = {HAL_TEST_MUL_F32, test_hal_mul_f32, "VMUL_F32", "Vector mul (float)"},
    [HAL_TEST_MAC_F32] = {HAL_TEST_MAC_F32, test_hal_mac_f32, "VMAC_F32", "Vector mac (float)"},
    [HAL_TEST_DIV_F32] = {HAL_TEST_DIV_F32, test_hal_div_f32, "VDIV_F32", "Vector div (float)"},
    [HAL_TEST_DOT_F32] = {HAL_TEST_DOT_F32, test_hal_dot_f32, "VDOT_F32",
                          "Vector dot product (float)"},
    [HAL_TEST_MTRX_MUL_F32] = {HAL_TEST_MTRX_MUL_F32, test_hal_mtrx_mul_f32, "MTRX_MUL_F32",
                               "Matrix Mul (float)"},
    [HAL_TEST_MTRX_MUL_TILED_F32] = {HAL_TEST_MTRX_MUL_TILED_F32, test_hal_mtrx_mul_tiled_f32, "MTRX_MUL_TILED_F32",
                               "Matrix Mul tiled (float)"},
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
