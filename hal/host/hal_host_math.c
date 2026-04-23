#include "hal_math.h"
#include <stddef.h>
#include <string.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/*
    static functions
*/

/* --- 128-bit Addition --- */
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

/* --- 256-bit Addition --- */
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

/* 단일 곱셈: 64bit * 64bit -> 128bit */
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


/* --- 128-bit Subtraction --- */

/**
 * @brief 부호 없는 128비트 뺄셈 (a - b)
 */
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

/**
 * @brief 부호 있는 128비트 뺄셈 (a - b)
 */
static int128_t _sub_i128(const int128_t a, const int128_t b) {
    // 2의 보수 체계에서 뺄셈 비트 로직은 Unsigned와 동일함
    uint128_t res = _sub_u128(*(const uint128_t*)&a, *(const uint128_t*)&b);
    return *(int128_t*)&res;
}

/* --- 256-bit Subtraction (필요시) --- */
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

/* 단일 대형 곱셈: 128bit * 128bit -> 256bit */
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


/* 부호 없는 128비트 크기 비교 (a >= b 이면 1, 아니면 0) */
static int _cmp_ge_u128(const uint128_t a, const uint128_t b) {
    if (a.u > b.u) return 1;
    if (a.u < b.u) return 0;
    return (a.l >= b.l) ? 1 : 0;
}

/* 128-bit 소프트웨어 나눗셈 (Unsigned) */
static uint128_t _div_u128(uint128_t n, uint128_t d) {
    uint128_t q = {0, 0}; // 몫 (Quotient)
    uint128_t r = {0, 0}; // 나머지 (Remainder)

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

/* 128-bit 소프트웨어 나눗셈 (Signed) */
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


void hal_init_hw(void) { }

/*
    addition
*/
/* i8/u8 */
void hal_add_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

void hal_add_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

/* i16/u16 */
void hal_add_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

void hal_add_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

/* i32/u32 */
void hal_add_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

void hal_add_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

/* i64/u64 */
void hal_add_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

void hal_add_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

/* i128/u128 */
void hal_add_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = _add_i128(a[i], b[i]);
    }
}

void hal_add_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = _add_u128(a[i], b[i]);
    }
}


/*
    subtraction
*/
/* i8/u8 */
void hal_sub_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

void hal_sub_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

/* i16/u16 */
void hal_sub_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

void hal_sub_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

/* i32/u32 */
void hal_sub_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

void hal_sub_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

/* i64/u64 */
void hal_sub_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

void hal_sub_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

/* i128/u128 */
void hal_sub_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = _sub_i128(a[i], b[i]);
    }
}

void hal_sub_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = _sub_u128(a[i], b[i]);
    }
}

/*
    multiplication
*/
/* i8/u8 */
void hal_mul_i8(int16_t *c, const int8_t *a, const int8_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = (int16_t)a[i] * b[i];
    }
}

void hal_mul_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = (uint16_t)a[i] * b[i];
    }
}

/* i16/u16 */
void hal_mul_i16(int32_t *c, const int16_t *a, const int16_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = (int32_t)a[i] * b[i];
    }
}

void hal_mul_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = (uint32_t)a[i] * b[i];
    }
}

/* i32/u32 */
void hal_mul_i32(int64_t *c, const int32_t *a, const int32_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = (int64_t)a[i] * b[i];
    }
}

void hal_mul_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = (uint64_t)a[i] * b[i];
    }
}

/* i64/u64 */
void hal_mul_i64(int128_t *c, const int64_t *a, const int64_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = _mul_i64(a[i], b[i]);
    }
}

void hal_mul_u64(uint128_t *c, const uint64_t *a, const uint64_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = _mul_u64(a[i], b[i]);
    }
}

/* i128/u128 */
void hal_mul_i128(int256_t *c, const int128_t *a, const int128_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = _mul_i128(a[i], b[i]);
    }
}

void hal_mul_u128(uint256_t *c, const uint128_t *a, const uint128_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = _mul_u128(a[i], b[i]);
    }
}

/*
    multiplication and accumulation
*/
/* i8/u8 */
void hal_mac_i8(int16_t *c, const int8_t *a, const int8_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] += (int16_t)a[i] * b[i];
    }
}

void hal_mac_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] += (uint16_t)a[i] * b[i];
    }
}

/* i16/u16 */
void hal_mac_i16(int32_t *c, const int16_t *a, const int16_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] += (int32_t)a[i] * b[i];
    }
}

void hal_mac_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] += (uint32_t)a[i] * b[i];
    }
}

/* i32/u32 */
void hal_mac_i32(int64_t *c, const int32_t *a, const int32_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] += (int64_t)a[i] * b[i];
    }
}

void hal_mac_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] += (uint64_t)a[i] * b[i];
    }
}

/* i64/u64 */
void hal_mac_i64(int128_t *c, const int64_t *a, const int64_t *b, const size_t n) {
    if (a == NULL || b == NULL || n == 0) return;
    for (size_t i = 0; i < n; i++) {
        int128_t prod = _mul_i64(a[i], b[i]);
        c[i] = _add_i128(c[i], prod);
    }
}

void hal_mac_u64(uint128_t *c, const uint64_t *a, const uint64_t *b, const size_t n) {
    if (a == NULL || b == NULL || n == 0) return;
    for (size_t i = 0; i < n; i++) {
        uint128_t prod = _mul_u64(a[i], b[i]);
        c[i] = _add_u128(c[i], prod);
    }
}


/* i128/u128 */
void hal_mac_i128(int256_t *c, const int128_t *a, const int128_t *b, const size_t n) {
    if (a == NULL || b == NULL || n == 0)
        return;

    for (uint32_t i = 0; i < n; i++) {
        // 1. 128bit x 128bit -> 256bit 단일 곱셈 (음수 부호 확장 로직 자동 적용)
        int256_t prod = _mul_i128(a[i], b[i]);

        // 2. 256bit 덧셈 누산 (비트 레벨 덧셈은 부호 여부와 무관하므로 안전함)
        c[i] = _add_i256(c[i], prod);
    }
}

void hal_mac_u128(uint256_t *c, const uint128_t *a, const uint128_t *b, const size_t n) {
    if (a == NULL || b == NULL || n == 0)
        return;

    for (uint32_t i = 0; i < n; i++) {
        uint256_t prod = _mul_u128(a[i], b[i]);
        c[i] = _add_u256(c[i], prod);
    }
}


/*
    division
*/
/* i8/u8 */
void hal_div_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL) return;
    if (ret) *ret = HAL_MATH_SUCCESS; // 에러 초기화

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            if (ret) *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

void hal_div_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL) return;
    if (ret) *ret = HAL_MATH_SUCCESS; // 에러 초기화

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            if (ret) *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

/* i16/u16 */
void hal_div_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL) return;
    if (ret) *ret = HAL_MATH_SUCCESS; // 에러 초기화

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            if (ret) *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

void hal_div_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL) return;
    if (ret) *ret = HAL_MATH_SUCCESS; // 에러 초기화

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            if (ret) *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

/* i32/u32 */
void hal_div_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL) return;
    if (ret) *ret = HAL_MATH_SUCCESS; // 에러 초기화

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            if (ret) *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

void hal_div_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL) return;
    if (ret) *ret = HAL_MATH_SUCCESS; // 에러 초기화

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            if (ret) *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

/* i64/u64 */
void hal_div_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL) return;
    if (ret) *ret = HAL_MATH_SUCCESS; // 에러 초기화

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            if (ret) *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

void hal_div_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL) return;
    if (ret) *ret = HAL_MATH_SUCCESS; // 에러 초기화

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            if (ret) *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

/* i128/u128 */
void hal_div_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL) return;

    if (ret) *ret = HAL_MATH_SUCCESS; // 에러 초기화

    for (size_t i = 0; i < n; i++) {
        // Divide by Zero 체크 (구조체의 l, u가 모두 0인지 확인)
        if (b[i].l == 0 && b[i].u == 0) {
            c[i].l = 0; // 요청하신 대로 0으로 세팅
            c[i].u = 0;

            // 에러 변수에 상태 기록 (나중에 외부에서 이 값을 보고 판단)
            if (ret) *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue; // 에러가 나도 다음 배열 요소의 연산은 계속 진행
        }

        // 안전한 소프트웨어 나눗셈 수행
        c[i] = _div_i128(a[i], b[i]);
    }
}

void hal_div_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL) return;

    if (ret) *ret = HAL_MATH_SUCCESS;

    for (size_t i = 0; i < n; i++) {
        // Divide by Zero 체크
        if (b[i].l == 0 && b[i].u == 0) {
            c[i].l = 0; // 0으로 세팅
            c[i].u = 0;

            if (ret) *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }

        // 안전한 소프트웨어 나눗셈 수행
        c[i] = _div_u128(a[i], b[i]);
    }
}

/*
    dot product
*/
/* i8/u8 */
void hal_dot_i8(int16_t* result, const int8_t* a, const int8_t* b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL) return;
    int16_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (int16_t)a[i] * b[i]; // 명시적 캐스팅으로 오버플로우 방지
    }
    *result = sum;
}

void hal_dot_u8(uint16_t* result, const uint8_t* a, const uint8_t* b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL) return;
    uint16_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (uint16_t)a[i] * b[i];
    }
    *result = sum;
}

/* i16/u16 */
void hal_dot_i16(int32_t* result, const int16_t* a, const int16_t* b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL) return;
    int32_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (int32_t)a[i] * b[i];
    }
    *result = sum;
}

void hal_dot_u16(uint32_t* result, const uint16_t* a, const uint16_t* b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL) return;
    uint32_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (uint32_t)a[i] * b[i];
    }
    *result = sum;
}

/* i32/u32 */
void hal_dot_i32(int64_t* result, const int32_t* a, const int32_t* b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL) return;
    int64_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (int64_t)a[i] * b[i];
    }
    *result = sum;
}

void hal_dot_u32(uint64_t* result, const uint32_t* a, const uint32_t* b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL) return;
    uint64_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (uint64_t)a[i] * b[i];
    }
    *result = sum;
}

/* i64/u64 */
void hal_dot_i64(int128_t* result, const int64_t* a, const int64_t* b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL) return;
    int128_t sum = {0, 0};
    for (size_t i = 0; i < n; i++) {
        if (a[i] == 0 || b[i] == 0) continue; // 0-Skip 최적화
        int128_t prod = _mul_i64(a[i], b[i]);
        sum = _add_i128(sum, prod);
    }
    *result = sum;
}

void hal_dot_u64(uint128_t* result, const uint64_t* a, const uint64_t* b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL) return;
    uint128_t sum = {0, 0};
    for (size_t i = 0; i < n; i++) {
        if (a[i] == 0 || b[i] == 0) continue;
        uint128_t prod = _mul_u64(a[i], b[i]);
        sum = _add_u128(sum, prod);
    }
    *result = sum;
}

/* i128/u128 */
void hal_dot_i128(int256_t* result, const int128_t* a, const int128_t* b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL) return;
    int256_t sum = {0};

    for (size_t i = 0; i < n; i++) {
        // 구조체 0-Skip 최적화 (한쪽이라도 0이면 곱셈 결과는 0)
        if ((a[i].l == 0 && a[i].u == 0) || (b[i].l == 0 && b[i].u == 0)) continue;

        int256_t prod = _mul_i128(a[i], b[i]);
        sum = _add_i256(sum, prod);
    }
    *result = sum;
}

void hal_dot_u128(uint256_t* result, const uint128_t* a, const uint128_t* b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL) return;
    uint256_t sum = {{0, 0, 0, 0}};

    for (size_t i = 0; i < n; i++) {
        // 구조체 0-Skip 최적화
        if ((a[i].l == 0 && a[i].u == 0) || (b[i].l == 0 && b[i].u == 0)) continue;

        uint256_t prod = _mul_u128(a[i], b[i]);
        sum = _add_u256(sum, prod);
    }
    *result = sum;
}

/*
    matrix multiplication
*/
/* i8/u8 */
void hal_matrix_mul_i8(int16_t *c, const int8_t *a, const int8_t *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    int16_t (*pc)[N] = (void *)c;
    int8_t (*pa)[K] = (void *)a;
    int8_t (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(int16_t) * M * N);

    // 3. i-k-j 순서로 연산 (캐시 최적화 및 Sparse 대응)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    pc[i][j] += (int16_t)pa[i][k] * pb[k][j];
                }
            }
        }
    }
}


void hal_matrix_mul_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    uint16_t (*pc)[N] = (void *)c;
    uint8_t (*pa)[K] = (void *)a;
    uint8_t (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(uint16_t) * M * N);

    // 3. i-k-j 순서로 연산 (캐시 최적화 및 Sparse 대응)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    pc[i][j] += (uint16_t)pa[i][k] * pb[k][j];
                }
            }
        }
    }
}

/* i16/u16 */
void hal_matrix_mul_i16(int32_t *c, const int16_t *a, const int16_t *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    int32_t (*pc)[N] = (void *)c;
    int16_t (*pa)[K] = (void *)a;
    int16_t (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(int32_t) * M * N);

    // 3. i-k-j 순서로 연산 (캐시 최적화 및 Sparse 대응)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    pc[i][j] += (int32_t)pa[i][k] * pb[k][j];
                }
            }
        }
    }
}

void hal_matrix_mul_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    uint32_t (*pc)[N] = (void *)c;
    uint16_t (*pa)[K] = (void *)a;
    uint16_t (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(uint32_t) * M * N);

    // 3. i-k-j 순서로 연산 (캐시 최적화 및 Sparse 대응)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    pc[i][j] += (uint32_t)pa[i][k] * pb[k][j];
                }
            }
        }
    }
}

/* i32/u32 */
void hal_matrix_mul_i32(int64_t *c, const int32_t *a, const int32_t *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    int64_t (*pc)[N] = (void *)c;
    int32_t (*pa)[K] = (void *)a;
    int32_t (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(int64_t) * M * N);

    // 3. i-k-j 순서로 연산 (캐시 최적화 및 Sparse 대응)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    pc[i][j] += (int64_t)pa[i][k] * pb[k][j];
                }
            }
        }
    }
}

// a_row : M, a_col = K
// b_row : K , b_col = N
void hal_matrix_mul_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    uint64_t (*pc)[N] = (void *)c;
    uint32_t (*pa)[K] = (void *)a;
    uint32_t (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(uint64_t) * M * N);

    // 3. i-k-j 순서로 연산 (캐시 최적화 및 Sparse 대응)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    pc[i][j] += (uint64_t)pa[i][k] * pb[k][j];
                }
            }
        }
    }
}

/* i64/u64 */
void hal_matrix_mul_i64(int128_t *c, const int64_t *a, const int64_t *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    int128_t (*pc)[N] = (void *)c;
    int64_t (*pa)[K] = (void *)a;
    int64_t (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(int128_t) * M * N);

    // 3. i-k-j 순서로 연산 (캐시 최적화 및 Sparse 대응)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    int128_t prod = _mul_i64(pa[i][k], pb[k][j]);
                    pc[i][j] = _add_i128(pc[i][j], prod);
                }
            }
        }
    }
}


void hal_matrix_mul_u64(uint128_t *c, const uint64_t *a, const uint64_t *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    uint128_t (*pc)[N] = (void *)c;
    uint64_t (*pa)[K] = (void *)a;
    uint64_t (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(uint128_t) * M * N);

    // 3. i-k-j 순서로 연산 (캐시 최적화 및 Sparse 대응)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    uint128_t prod = _mul_u64(pa[i][k], pb[k][j]);
                    pc[i][j] = _add_u128(pc[i][j], prod);
                }
            }
        }
    }
}

/* i128/u128 */
void hal_matrix_mul_i128(int256_t *c, const int128_t *a, const int128_t *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    int256_t (*pc)[N] = (void *)c;
    int128_t (*pa)[K] = (void *)a;
    int128_t (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(int256_t) * M * N);

    // 3. i-k-j 순서로 연산 (캐시 최적화 및 Sparse 대응)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            // if (pa[i][k] != 0) {
            if ((pa[i][k].u != 0) || (pa[i][k].l != 0)) {
                for (int j = 0; j < N; j++) {
                    int256_t prod = _mul_i128(pa[i][k], pb[k][j]);
                    pc[i][j] = _add_i256(pc[i][j], prod);
                }
            }
        }
    }
}


void hal_matrix_mul_u128(uint256_t *c, const uint128_t *a, const uint128_t *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    uint256_t (*pc)[N] = (void *)c;
    uint128_t (*pa)[K] = (void *)a;
    uint128_t (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(uint256_t) * M * N);

    // 3. i-k-j 순서로 연산 (캐시 최적화 및 Sparse 대응)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if ((pa[i][k].u != 0) || (pa[i][k].l != 0)) {
                for (int j = 0; j < N; j++) {
                    uint256_t prod = _mul_u128(pa[i][k], pb[k][j]);
                    pc[i][j] = _add_u256(pc[i][j], prod);
                }
            }
        }
    }
}


/*
    matrix multiplication with tiled
*/
/* i8/u8 */
void hal_matrix_mul_tiled_i8(int16_t* c, const int8_t* a, const int8_t* b, int M, int N, int K, int tile_size) {
    // VLA Mapping
    int16_t (*pc)[N] = (int16_t (*)[N])c;
    const int8_t (*pa)[K] = (const int8_t (*)[K])a;
    const int8_t (*pb)[N] = (const int8_t (*)[N])b;

    // 출력 버퍼 초기화
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pc[i][j] = 0;
        }
    }

    // Outer loops: Tile 단위 이동
    for (int i = 0; i < M; i += tile_size) {
        for (int j = 0; j < N; j += tile_size) {
            for (int k = 0; k < K; k += tile_size) {

                int i_end = MIN(i + tile_size, M);
                int j_end = MIN(j + tile_size, N);
                int k_end = MIN(k + tile_size, K);

                // Inner loops: Tile 내부 연산
                for (int ii = i; ii < i_end; ii++) {
                    for (int jj = j; jj < j_end; jj++) {
                        int16_t sum = pc[ii][jj];
                        for (int kk = k; kk < k_end; kk++) {
                            sum += (int16_t)pa[ii][kk] * (int16_t)pb[kk][jj];
                        }
                        pc[ii][jj] = sum;
                    }
                }
            }
        }
    }
}


void hal_matrix_mul_tiled_u8(uint16_t* c, const uint8_t* a, const uint8_t* b, int M, int N, int K, int tile_size) {
    // VLA Mapping
    uint16_t (*pc)[N] = (uint16_t (*)[N])c;
    const uint8_t (*pa)[K] = (const uint8_t (*)[K])a;
    const uint8_t (*pb)[N] = (const uint8_t (*)[N])b;

    // 출력 버퍼 초기화
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pc[i][j] = 0;
        }
    }

    // Outer loops: Tile 단위 이동
    for (int i = 0; i < M; i += tile_size) {
        for (int j = 0; j < N; j += tile_size) {
            for (int k = 0; k < K; k += tile_size) {

                int i_end = MIN(i + tile_size, M);
                int j_end = MIN(j + tile_size, N);
                int k_end = MIN(k + tile_size, K);

                // Inner loops: Tile 내부 연산
                for (int ii = i; ii < i_end; ii++) {
                    for (int jj = j; jj < j_end; jj++) {
                        uint16_t sum = pc[ii][jj];
                        for (int kk = k; kk < k_end; kk++) {
                            sum += (uint16_t)pa[ii][kk] * (uint16_t)pb[kk][jj];
                        }
                        pc[ii][jj] = sum;
                    }
                }
            }
        }
    }
}

/* i16/u16 */
void hal_matrix_mul_tiled_i16(int32_t* c, const int16_t* a, const int16_t* b, int M, int N, int K, int tile_size) {
    // VLA Mapping
    int32_t (*pc)[N] = (int32_t (*)[N])c;
    const int16_t (*pa)[K] = (const int16_t (*)[K])a;
    const int16_t (*pb)[N] = (const int16_t (*)[N])b;

    // 출력 버퍼 초기화
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pc[i][j] = 0;
        }
    }

    // Outer loops: Tile 단위 이동
    for (int i = 0; i < M; i += tile_size) {
        for (int j = 0; j < N; j += tile_size) {
            for (int k = 0; k < K; k += tile_size) {

                int i_end = MIN(i + tile_size, M);
                int j_end = MIN(j + tile_size, N);
                int k_end = MIN(k + tile_size, K);

                // Inner loops: Tile 내부 연산
                for (int ii = i; ii < i_end; ii++) {
                    for (int jj = j; jj < j_end; jj++) {
                        int32_t sum = pc[ii][jj];
                        for (int kk = k; kk < k_end; kk++) {
                            sum += (int32_t)pa[ii][kk] * (int32_t)pb[kk][jj];
                        }
                        pc[ii][jj] = sum;
                    }
                }
            }
        }
    }
}

void hal_matrix_mul_tiled_u16(uint32_t* c, const uint16_t* a, const uint16_t* b, int M, int N, int K, int tile_size) {
    // VLA Mapping
    uint32_t (*pc)[N] = (uint32_t (*)[N])c;
    const uint16_t (*pa)[K] = (const uint16_t (*)[K])a;
    const uint16_t (*pb)[N] = (const uint16_t (*)[N])b;

    // 출력 버퍼 초기화
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pc[i][j] = 0;
        }
    }

    // Outer loops: Tile 단위 이동
    for (int i = 0; i < M; i += tile_size) {
        for (int j = 0; j < N; j += tile_size) {
            for (int k = 0; k < K; k += tile_size) {

                int i_end = MIN(i + tile_size, M);
                int j_end = MIN(j + tile_size, N);
                int k_end = MIN(k + tile_size, K);

                // Inner loops: Tile 내부 연산
                for (int ii = i; ii < i_end; ii++) {
                    for (int jj = j; jj < j_end; jj++) {
                        uint32_t sum = pc[ii][jj];
                        for (int kk = k; kk < k_end; kk++) {
                            sum += (uint32_t)pa[ii][kk] * (uint32_t)pb[kk][jj];
                        }
                        pc[ii][jj] = sum;
                    }
                }
            }
        }
    }
}


/* i32/u32 */
void hal_matrix_mul_tiled_i32(int64_t* c, const int32_t* a, const int32_t* b, int M, int N, int K, int tile_size) {
    // VLA Mapping
    int64_t (*pc)[N] = (int64_t (*)[N])c;
    const int32_t (*pa)[K] = (const int32_t (*)[K])a;
    const int32_t (*pb)[N] = (const int32_t (*)[N])b;

    // 출력 버퍼 초기화
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pc[i][j] = 0.0;
        }
    }

    // Outer loops: Tile 단위 이동
    for (int i = 0; i < M; i += tile_size) {
        for (int j = 0; j < N; j += tile_size) {
            for (int k = 0; k < K; k += tile_size) {

                int i_end = MIN(i + tile_size, M);
                int j_end = MIN(j + tile_size, N);
                int k_end = MIN(k + tile_size, K);

                // Inner loops: Tile 내부 연산
                for (int ii = i; ii < i_end; ii++) {
                    for (int jj = j; jj < j_end; jj++) {
                        int64_t sum = pc[ii][jj];
                        for (int kk = k; kk < k_end; kk++) {
                            sum += (int64_t)pa[ii][kk] * (int64_t)pb[kk][jj];
                        }
                        pc[ii][jj] = sum;
                    }
                }
            }
        }
    }
}


void hal_matrix_mul_tiled_u32(uint64_t* c, const uint32_t* a, const uint32_t* b, int M, int N, int K, int tile_size) {
    // VLA Mapping
    uint64_t (*pc)[N] = (uint64_t (*)[N])c;
    const uint32_t (*pa)[K] = (const uint32_t (*)[K])a;
    const uint32_t (*pb)[N] = (const uint32_t (*)[N])b;

    // 출력 버퍼 초기화
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pc[i][j] = 0.0;
        }
    }

    // Outer loops: Tile 단위 이동
    for (int i = 0; i < M; i += tile_size) {
        for (int j = 0; j < N; j += tile_size) {
            for (int k = 0; k < K; k += tile_size) {

                int i_end = MIN(i + tile_size, M);
                int j_end = MIN(j + tile_size, N);
                int k_end = MIN(k + tile_size, K);

                // Inner loops: Tile 내부 연산
                for (int ii = i; ii < i_end; ii++) {
                    for (int jj = j; jj < j_end; jj++) {
                        uint64_t sum = pc[ii][jj];
                        for (int kk = k; kk < k_end; kk++) {
                            sum += (uint64_t)pa[ii][kk] * (uint64_t)pb[kk][jj];
                        }
                        pc[ii][jj] = sum;
                    }
                }
            }
        }
    }
}

/* 64 */
void hal_matrix_mul_tiled_i64(int128_t* c, const int64_t* a, const int64_t* b, int M, int N, int K, int tile_size) {
    int128_t (*pc)[N] = (int128_t (*)[N])c;
    const int64_t (*pa)[K] = (const int64_t (*)[K])a;
    const int64_t (*pb)[N] = (const int64_t (*)[N])b;
    memset(c, 0, sizeof(int128_t) * M * N); 

    for (int i = 0; i < M; i += tile_size) {
        for (int k = 0; k < K; k += tile_size) {
            int i_end = MIN(i + tile_size, M);
            int k_end = MIN(k + tile_size, K);

            for (int j = 0; j < N; j += tile_size) {
                int j_end = MIN(j + tile_size, N);
                for (int ii = i; ii < i_end; ii++) {
                    for (int kk = k; kk < k_end; kk++) {
                        int64_t prod = pa[ii][kk];
                        for (int jj = j; jj < j_end; jj++) {
                            pc[ii][jj] = _add_i128(pc[ii][jj], _mul_i64(prod, pb[kk][jj]));
                        }
                    }
                }
            }
        }
    }
}

void hal_matrix_mul_tiled_u64(uint128_t* c, const uint64_t* a, const uint64_t* b, int M, int N, int K, int tile_size) {
    uint128_t (*pc)[N] = (uint128_t (*)[N])c;
    const uint64_t (*pa)[K] = (const uint64_t (*)[K])a;
    const uint64_t (*pb)[N] = (const uint64_t (*)[N])b;
    memset(c, 0, sizeof(uint128_t) * M * N); 

    for (int i = 0; i < M; i += tile_size) {
        for (int k = 0; k < K; k += tile_size) {
            int i_end = MIN(i + tile_size, M);
            int k_end = MIN(k + tile_size, K);

            for (int j = 0; j < N; j += tile_size) {
                int j_end = MIN(j + tile_size, N);
                for (int ii = i; ii < i_end; ii++) {
                    for (int kk = k; kk < k_end; kk++) {
                        uint64_t prod = pa[ii][kk];
                        for (int jj = j; jj < j_end; jj++) {
                            pc[ii][jj] = _add_u128(pc[ii][jj], _mul_u64(prod, pb[kk][jj]));
                        }
                    }
                }
            }
        }
    }
}

void hal_matrix_mul_tiled_i128(int256_t* c, const int128_t* a, const int128_t* b, int M, int N, int K, int tile_size) {
    int256_t (*pc)[N] = (int256_t (*)[N])c;
    const int128_t (*pa)[K] = (const int128_t (*)[K])a;
    const int128_t (*pb)[N] = (const int128_t (*)[N])b;
    memset(c, 0, sizeof(int256_t) * M * N); 

    for (int i = 0; i < M; i += tile_size) {
        for (int k = 0; k < K; k += tile_size) {
            int i_end = MIN(i + tile_size, M);
            int k_end = MIN(k + tile_size, K);

            for (int j = 0; j < N; j += tile_size) {
                int j_end = MIN(j + tile_size, N);
                for (int ii = i; ii < i_end; ii++) {
                    for (int kk = k; kk < k_end; kk++) {
                        int128_t prod = pa[ii][kk];
                        for (int jj = j; jj < j_end; jj++) {
                            pc[ii][jj] = _add_i256(pc[ii][jj], _mul_i128(prod, pb[kk][jj]));
                        }
                    }
                }
            }
        }
    }
}

void hal_matrix_mul_tiled_u128(uint256_t* c, const uint128_t* a, const uint128_t* b, int M, int N, int K, int tile_size) {
    uint256_t (*pc)[N] = (uint256_t (*)[N])c;
    const uint128_t (*pa)[K] = (const uint128_t (*)[K])a;
    const uint128_t (*pb)[N] = (const uint128_t (*)[N])b;
    memset(c, 0, sizeof(uint256_t) * M * N); 

    for (int i = 0; i < M; i += tile_size) {
        for (int k = 0; k < K; k += tile_size) {
            int i_end = MIN(i + tile_size, M);
            int k_end = MIN(k + tile_size, K);

            for (int j = 0; j < N; j += tile_size) {
                int j_end = MIN(j + tile_size, N);
                for (int ii = i; ii < i_end; ii++) {
                    for (int kk = k; kk < k_end; kk++) {
                        uint128_t prod = pa[ii][kk];
                        for (int jj = j; jj < j_end; jj++) {
                            pc[ii][jj] = _add_u256(pc[ii][jj], _mul_u128(prod, pb[kk][jj]));
                        }
                    }
                }
            }
        }
    }
}


/*
    float 32
*/
/*
    addition
*/
void hal_add_f32(float *c, const float *a, const float *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

/*
    subtraction
*/
void hal_sub_f32(float *c, const float *a, const float *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

/*
    multiplication
*/
void hal_mul_f32(double *c, const float *a, const float *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = (double)a[i] * b[i];
    }
}

/*
    multiplication and accumulation
*/
void hal_mac_f32(double *c, const float *a, const float *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] += (double)a[i] * b[i];
    }
}

/*
    division
*/
void hal_div_f32(float *c, const float *a, const float *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL) return;

    if (ret) *ret = HAL_MATH_SUCCESS; // 에러 초기화

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0.0f) {
            c[i] = 0;

            if (ret) *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

/*
    dot product
*/
void hal_dot_f32(double* result, const float* a, const float* b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL) return;
    double sum = 0.0;

    for (size_t i = 0; i < n; i++) {
        if (a[i] == 0.0f || b[i] == 0.0f) continue;
        sum += (double)a[i] * b[i]; // double 정밀도 보장
    }
    *result = sum;
}

/*
    matrix multiplication
*/
void hal_matrix_mul_f32(double *c, const float *a, const float *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    double (*pc)[N] = (void *)c;
    float (*pa)[K] = (void *)a;
    float (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(double) * M * N);

    // 3. i-k-j 순서로 연산 (캐시 최적화 및 Sparse 대응)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0.0f) {
                for (int j = 0; j < N; j++) {
                    pc[i][j] += (double)pa[i][k] * pb[k][j];
                }
            }
        }
    }
}


void hal_matrix_mul_tiled_f32(double* c, const float* a, const float* b, int M, int N, int K, int tile_size) {
    // VLA Mapping
    double (*pc)[N] = (double (*)[N])c;
    const float (*pa)[K] = (const float (*)[K])a;
    const float (*pb)[N] = (const float (*)[N])b;

    // 출력 버퍼 초기화
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pc[i][j] = 0.0;
        }
    }

    // Outer loops: Tile 단위 이동
    for (int i = 0; i < M; i += tile_size) {
        for (int j = 0; j < N; j += tile_size) {
            for (int k = 0; k < K; k += tile_size) {

                int i_end = MIN(i + tile_size, M);
                int j_end = MIN(j + tile_size, N);
                int k_end = MIN(k + tile_size, K);

                // Inner loops: Tile 내부 연산
                for (int ii = i; ii < i_end; ii++) {
                    for (int jj = j; jj < j_end; jj++) {
                        double sum = pc[ii][jj];
                        for (int kk = k; kk < k_end; kk++) {
                            // float * float -> double 누적
                            sum += (double)pa[ii][kk] * (double)pb[kk][jj];
                        }
                        pc[ii][jj] = sum;
                    }
                }
            }
        }
    }
}
