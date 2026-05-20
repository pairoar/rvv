#ifndef HAL_INTERNAL_MATH_H
#define HAL_INTERNAL_MATH_H

#include "hal_types.h"
#include "math.h"
#include <stdio.h>


// =============================================================================
// HAL Macro functins
// =============================================================================
#define ADD(a, b) ((a) + (b))
#define SUB(a, b) ((a) - (b))
#define MUL(a, b) ((a) * (b))
#define DIV(a, b) ((a) / (b))

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define AND(a, b) ((a) & (b))
#define OR(a, b) ((a) | (b))
#define XOR(a, b) ((a) ^ (b))
#define NOT(a) (~(a))

// when we need explicit type casting ...
#define WADD(type, a, b) ((type)(a) + (type)(b))
#define WSUB(type, a, b) ((type)(a) - (type)(b))


// -----------------------------------------------------------------------------
// HAL Internal Math: 128/256-bit Software Emulation
// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// basic 
// -----------------------------------------------------------------------------
inline static int hal_is_zero_u128(const uint128_t a) {
    return (a.l == 0) && (a.u == 0);
}


inline static int hal_is_zero_i128(const int128_t a) {
    return (a.l == 0) && (a.u == 0);
}


/* Unsigned 128-bit comparison (returns 1 if a >= b, otherwise 0) */
inline static int _cmp_ge_u128(const uint128_t a, const uint128_t b) {
    if (a.u > b.u)
        return 1;
    if (a.u < b.u)
        return 0;
    return (a.l >= b.l) ? 1 : 0;
}

// -----------------------------------------------------------------------------
// add
// -----------------------------------------------------------------------------
inline static uint128_t hal_add_u128_u64(const uint128_t a, const uint64_t b) {
    uint128_t res;

    res.l = a.l + b;
    res.u = a.u + ((res.l < a.l) ? 1 : 0); // Add 1 to u if a carry occurs
    return res;
}

inline static uint128_t hal_add_u128(const uint128_t a, const uint128_t b) {
    uint128_t c;

    c.l = a.l + b.l;
    c.u = a.u + b.u + ((c.l < a.l) ? 1 : 0);
    return c;
}


inline static int128_t hal_add_i128(const int128_t a, const int128_t b) {
    uint128_t c = hal_add_u128(*(const uint128_t *)&a, *(const uint128_t *)&b);
    return *(int128_t *)&c;
}


inline static uint256_t hal_add_u256(const uint256_t a, const uint256_t b) {
    uint256_t c = {0};
    uint64_t carry = 0;

    for (uint32_t i = 0; i < 4; i++) {
        uint64_t sum = a.d[i] + b.d[i] + carry;
        // Overflow occurs in a.d[i] + b.d[i]: sum < a.d[i]
        // Overflow occurs when adding the carry : carry && sum == a.d[i]
        carry = (sum < a.d[i]) || (carry && sum == a.d[i]) ? 1 : 0;
        c.d[i] = sum;
    }
    return c;
}


inline static int256_t hal_add_i256(const int256_t a, const int256_t b) {
    uint256_t c = hal_add_u256(*(const uint256_t *)&a, *(const uint256_t *)&b);
    return *(int256_t *)&c;
}


inline static uint256_t hal_add_u256_u128(const uint256_t a, const uint128_t b) {
    uint256_t res = {0};
    uint64_t carry = 0;

    // 1. add lower 64 bits (d[0])
    res.d[0] = a.d[0] + b.l;
    carry = (res.d[0] < a.d[0]) ? 1 : 0;

    // 2. add the second 64 bit (d[1]) (b's top 64 bit + previous carry)
    res.d[1] = a.d[1] + b.u + carry;
    // calculate new caries with and without caries
    if (carry) {
        carry = (res.d[1] <= a.d[1]) ? 1 : 0;
    } else {
        carry = (res.d[1] < a.d[1]) ? 1 : 0;
    }

    // 3. 3rd 64bit (d[2]) - b is 0 so only carry propagates
    res.d[2] = a.d[2] + carry;
    carry = (res.d[2] < a.d[2]) ? 1 : 0;

    // 4. 네 번째 64비트 (d[3]) - 최종 carry 전파
    res.d[3] = a.d[3] + carry;

    return res;
}


inline static uint128_t hal_wadd_u64(const uint64_t a, const uint64_t b) {
    uint128_t c;
    
    c.l = a + b;
    c.u = c.l < a ? 1 : 0;
    return c;
}


inline static int128_t hal_wadd_i64(const int64_t a, const int64_t b) {
    int128_t c;

    // 1. 하위 64비트 연산: C언어의 미정의 동작(UB)을 피하기 위해 Unsigned로 캐스팅 후 덧셈
    uint64_t au = (uint64_t)a;
    uint64_t bu = (uint64_t)b;
    c.l = au + bu;

    // 2. 오버플로우(Carry) 판별
    uint64_t carry = (c.l < au) ? 1 : 0;

    // 3. 상위 64비트 연산 (부호 확장 + Carry)
    // 부호 있는 64비트 정수를 63번 우측으로 밀면(ASR), 
    // 양수는 0x0000000000000000 (0)이 되고, 음수는 0xFFFFFFFFFFFFFFFF (-1)이 됩니다.
    c.u = (a >> 63) + (b >> 63) + carry;

    return c;
}


inline static uint256_t hal_wadd_u128(const uint128_t a, const uint128_t b) {
    uint256_t res = {0};
    uint64_t carry = 0;
    // uint64_t t = 0;

    // 1. add lower 64 bits (d[0])
    res.d[0] = a.l + b.l;
    carry = (res.d[0] < a.l) ? 1 : 0;

    // 2. add the second 64 bit (d[1]) (b's top 64 bit + previous carry)
    res.d[1] = a.u + b.u + carry;
    carry = (res.d[1] < a.u) || (carry && res.d[1] == a.u) ? 1 : 0;

    // 3. 3rd 64bit (d[2]) only carry propagates
    res.d[2] = carry;

    return res;
}


inline static int256_t hal_wadd_i128(const int128_t a, const int128_t b) {
    int256_t res;
    uint64_t carry = 0;

    // 1. 하위 64비트 (d[0]) 덧셈
    res.d[0] = a.l + b.l;
    carry = (res.d[0] < a.l) ? 1 : 0;

    // 2. 상위 64비트 (d[1]) 덧셈 (부호를 무시하고 Unsigned로 연산)
    uint64_t au = (uint64_t)a.u;
    uint64_t bu = (uint64_t)b.u;
    res.d[1] = au + bu + carry;
    carry = (res.d[1] < au) || (carry && res.d[1] == au) ? 1 : 0;

    // 3. 부호 확장 (Sign Extension) 마스크 생성
    // a.u 또는 b.u가 음수(< 0)이면 확장 영역을 0xFFFFFFFFFFFFFFFF로, 양수면 0으로 채움
    // (컴파일러가 산술 우측 시프트(ASR) 명령어로 분기문 없이 매우 빠르게 최적화해 줍니다)
    uint64_t a_ext = (a.u < 0) ? 0xFFFFFFFFFFFFFFFFULL : 0;
    uint64_t b_ext = (b.u < 0) ? 0xFFFFFFFFFFFFFFFFULL : 0;

    // 4. 확장 영역 덧셈 (d[2], d[3])
    res.d[2] = a_ext + b_ext + carry;
    carry = (res.d[2] < a_ext) || (carry && res.d[2] == a_ext) ? 1 : 0;

    res.d[3] = a_ext + b_ext + carry;
    // 256비트 결과이므로 d[3]에서 발생하는 마지막 Carry는 자연스럽게 버려집니다(Wrap-around).

    return res;
}


// -----------------------------------------------------------------------------
// subtraction
// -----------------------------------------------------------------------------
inline static uint128_t hal_sub_u128(const uint128_t a, const uint128_t b) {
    uint128_t res;

    // low 64bit subtraction
    res.l = a.l - b.l;

    // 2. check for the occurrence of a borrow
    //  If a.l is less than b.l, the subtraction operation must borrow the value of the high bit
    //  from the higher-order operation.
    uint64_t borrow = (a.l < b.l) ? 1 : 0;

    // Upper 64-bit Operation (Incorporating Borrow)
    res.u = a.u - b.u - borrow;

    return res;
}


inline static int128_t hal_sub_i128(const int128_t a, const int128_t b) {
    // In the two's complement system, the bit-level logic for subtraction is identical to that of
    // unsigned arithmetic.
    uint128_t res = hal_sub_u128(*(const uint128_t *)&a, *(const uint128_t *)&b);
    return *(int128_t *)&res;
}


inline static uint256_t hal_sub_u256(const uint256_t a, const uint256_t b) {
    uint256_t c = {0};
    uint64_t borrow = 0;

    for (int i = 0; i < 4; i++) {
        uint64_t sub = a.d[i] - b.d[i] - borrow;
        // Condition for Borrowing:
        // 1. a.d[i] < b.d[i]
        // 2. A borrow operation occurred, but as a result, `a.d[i]` and `b.d[i]` became equal,
        // necessitating a further borrow.
        if (borrow) {
            borrow = (a.d[i] <= b.d[i]) ? 1 : 0;
        } else {
            borrow = (a.d[i] < b.d[i]) ? 1 : 0;
        }
        c.d[i] = sub;
    }
    return c;
}


inline static int256_t hal_sub_i256(const int256_t a, const int256_t b) {
    int256_t res = {0};
    uint8_t borrow = 0; // 하위 비트에서 발생한 빌림수

    // 1. 최하위 64비트 (ll)
    res.d[0] = a.d[0] - b.d[0];
    // a.d[0]이 b.d[0]보다 작으면 빌림수 1 발생
    borrow = (a.d[0] < b.d[0]) ? 1 : 0; 

    // 2. 하위-중간 64비트 (lh)
    res.d[1] = a.d[1] - b.d[1] - borrow;
    // 현재 자리에서 뺄셈을 감당할 수 없거나, (값이 같은데 이전 자리에서 빌림수가 넘어온 경우) 빌림수 발생
    borrow = (a.d[1] < b.d[1]) || (a.d[1] == b.d[1] && borrow) ? 1 : 0;

    // 3. 상위-중간 64비트 (hl)
    res.d[2] = a.d[2] - b.d[2] - borrow;
    borrow = (a.d[2] < b.d[2]) || (a.d[2] == b.d[2] && borrow) ? 1 : 0;

    // 4. 최상위 64비트 (hh)
    // C언어에서 Signed 정수의 오버플로우/언더플로우는 미정의 동작(Undefined Behavior)입니다.
    // 이를 방지하기 위해 강제로 Unsigned로 캐스팅하여 연산 후 Signed로 다시 변환합니다.
    // (2의 보수 체계이므로 비트 단위 연산 결과는 동일합니다)
    res.d[3] = (int64_t)((uint64_t)a.d[3] - (uint64_t)b.d[3] - borrow);

    return res;
}


inline static uint128_t hal_wsub_u64(const uint64_t a, const uint64_t b) {
    uint128_t c;

    // 1. 하위 64비트 뺄셈
    c.l = a - b;

    // 2. 빌림수(Borrow) 판별 및 상위 비트 채움
    // a < b 이면 언더플로우가 발생하므로, 상위 64비트는 모두 1(-1)이 됩니다.
    c.u = (a < b) ? 0xFFFFFFFFFFFFFFFFULL : 0;

    return c;
}


inline static int128_t hal_wsub_i64(const int64_t a, const int64_t b) {
    int128_t c;

    // 1. 하위 64비트 뺄셈 (C언어 UB 방지를 위해 Unsigned 캐스팅)
    uint64_t au = (uint64_t)a;
    uint64_t bu = (uint64_t)b;
    c.l = au - bu;

    // 2. 빌림수(Borrow) 판별
    uint64_t borrow = (au < bu) ? 1 : 0;

    // 3. 상위 64비트 뺄셈 (부호 확장 값에서 뺌)
    c.u = (a >> 63) - (b >> 63) - borrow;

    return c;
}


inline static uint256_t hal_wsub_u128(const uint128_t a, const uint128_t b) {
    uint256_t res;
    uint64_t borrow = 0;

    // 1. 하위 64비트 (d[0])
    res.d[0] = a.l - b.l;
    borrow = (a.l < b.l) ? 1 : 0;

    // 2. 상위 64비트 (d[1])
    res.d[1] = a.u - b.u - borrow;
    // a.u < b.u 이거나, (a.u == b.u 인데 하위에서 borrow가 올라와서 더 빼야 하는 경우)
    borrow = (a.u < b.u) || (borrow && a.u == b.u) ? 1 : 0;

    // 3. 확장 영역 (Widening - d[2], d[3])
    // Unsigned 확장이므로 원본의 상위 128비트는 0입니다. (0 - 0 - borrow)
    uint64_t ext_val = borrow ? 0xFFFFFFFFFFFFFFFFULL : 0;
    res.d[2] = ext_val;
    res.d[3] = ext_val;

    return res;
}


inline static int256_t hal_wsub_i128(const int128_t a, const int128_t b) {
    int256_t res;
    uint64_t borrow = 0;

    // 1. 하위 64비트 (d[0])
    res.d[0] = a.l - b.l;
    borrow = (a.l < b.l) ? 1 : 0;

    // 2. 상위 64비트 (d[1]) (Unsigned 처리)
    uint64_t au = (uint64_t)a.u;
    uint64_t bu = (uint64_t)b.u;
    res.d[1] = au - bu - borrow;
    borrow = (au < bu) || (borrow && au == bu) ? 1 : 0;

    // 3. 부호 확장(Sign Extension) 마스크 생성
    uint64_t a_ext = (a.u < 0) ? 0xFFFFFFFFFFFFFFFFULL : 0;
    uint64_t b_ext = (b.u < 0) ? 0xFFFFFFFFFFFFFFFFULL : 0;

    // 4. 확장 영역 뺄셈 (d[2], d[3])
    res.d[2] = a_ext - b_ext - borrow;
    borrow = (a_ext < b_ext) || (borrow && a_ext == b_ext) ? 1 : 0;

    res.d[3] = a_ext - b_ext - borrow;
    // d[3]에서 발생하는 마지막 borrow는 버려짐(Wrap-around)

    return res;
}


// -----------------------------------------------------------------------------
// multiplication
// -----------------------------------------------------------------------------
inline static uint64_t hal_mul_u64(const uint64_t a, const uint64_t b) {
    return a * b;
}

inline static int64_t hal_mul_i64(const int64_t a, const int64_t b) {
    return a * b;
}


inline static uint128_t hal_wmul_u64(const uint64_t a, const uint64_t b) {
    uint128_t res;
    uint64_t a_lo = a & 0xFFFFFFFF;
    uint64_t a_hi = a >> 32;
    uint64_t b_lo = b & 0xFFFFFFFF;
    uint64_t b_hi = b >> 32;

    uint64_t p0 = a_lo * b_lo;
    uint64_t p1 = a_lo * b_hi;
    uint64_t p2 = a_hi * b_lo;
    uint64_t p3 = a_hi * b_hi;

    uint64_t mid = (p0 >> 32) + (p1 & 0xFFFFFFFF) + (p2 & 0xFFFFFFFF);

    res.l = (p0 & 0xFFFFFFFF) | (mid << 32);
    res.u = p3 + (p1 >> 32) + (p2 >> 32) + (mid >> 32);

    return res;
}

inline static int128_t hal_wmul_i64(const int64_t a, const int64_t b) {
    uint128_t res_u = hal_wmul_u64((uint64_t)a, (uint64_t)b);

    int128_t res;
    res.u = (int64_t)res_u.u;
    res.l = res_u.l;

    // If the value is negative, correct the high-order bit (u) in accordance with the two's
    // complement system.
    if (a < 0)
        res.u -= b;
    if (b < 0)
        res.u -= a;

    return res;
}


inline static uint256_t hal_wmul_u128(const uint128_t a, const uint128_t b) {
    uint256_t res = {{0, 0, 0, 0}};

    // 4개의 부분 곱 (Partial products)
    uint128_t p00 = hal_wmul_u64(a.l, b.l);
    uint128_t p01 = hal_wmul_u64(a.l, b.u);
    uint128_t p10 = hal_wmul_u64(a.u, b.l);
    uint128_t p11 = hal_wmul_u64(a.u, b.u);

    // [0] 하위 64비트 확정
    res.d[0] = p00.l;

    // [1] 계산 및 Carry 관리
    uint64_t carry = 0;
    res.d[1] = p00.u;

    res.d[1] += p01.l;
    if (res.d[1] < p01.l)
        carry++;

    res.d[1] += p10.l;
    if (res.d[1] < p10.l)
        carry++;

    // [2] 계산 및 Carry 관리
    uint64_t c1 = 0;
    res.d[2] = p11.l;

    res.d[2] += p01.u;
    if (res.d[2] < p01.u)
        c1++;

    res.d[2] += p10.u;
    if (res.d[2] < p10.u)
        c1++;

    res.d[2] += carry;
    if (res.d[2] < carry)
        c1++;

    // [3] 최상위 64비트 계산 (Carry 포함)
    res.d[3] = p11.u + c1;

    return res;
}


inline static int256_t hal_wmul_i128(int128_t a, int128_t b) {
    // 1. 먼저 부호를 무시하고 Unsigned 256비트 곱셈을 수행합니다.
    uint256_t res_u = hal_wmul_u128(*(uint128_t *)&a, *(uint128_t *)&b);
    int256_t res = *(int256_t *)&res_u;

    // 2. 2의 보수 수학적 특성에 따른 상위 128비트 보정(Correction)

    // a가 음수일 경우: 256비트 결과의 상위 128비트(d[2], d[3])에서 b를 빼줍니다.
    if (a.u < 0) {
        uint64_t prev_d2 = res.d[2];
        res.d[2] -= (uint64_t)b.l;
        uint64_t borrow = (res.d[2] > prev_d2) ? 1 : 0; // 언더플로우 발생 시 빌림수 1
        res.d[3] -= (uint64_t)b.u + borrow;
    }

    // b가 음수일 경우: 256비트 결과의 상위 128비트(d[2], d[3])에서 a를 빼줍니다.
    if (b.u < 0) {
        uint64_t prev_d2 = res.d[2];
        res.d[2] -= (uint64_t)a.l;
        uint64_t borrow = (res.d[2] > prev_d2) ? 1 : 0; // 언더플로우 발생 시 빌림수 1
        res.d[3] -= (uint64_t)a.u + borrow;
    }

    return res;
}


// 1. Unsigned 일반 128비트 곱셈 (128x128 -> 128)
inline static uint128_t hal_mul_u128(const uint128_t a, const uint128_t b) {
    // 1단계: 하위 64비트끼리의 곱셈 (결과는 128비트)
    // 앞서 정리한 확장 곱셈(wmul) 함수를 재사용합니다.
    uint128_t res = hal_wmul_u64(a.l, b.l);

    // 2단계: 교차 곱(Cross Product) 누산
    // (a.u * b.l)과 (a.l * b.u)의 결과는 128비트의 상위 64비트(u) 자리에만 영향을 미칩니다.
    // 여기서 발생하는 오버플로우는 128비트를 벗어나므로 자연스럽게 버려집니다.
    res.u += (a.u * b.l) + (a.l * b.u);

    // a.u * b.u 는 계산하지 않습니다! (128비트를 초과하므로 0이 됨)
    return res;
}

// 2. Signed 일반 128비트 곱셈 (128x128 -> 128)
inline static int128_t hal_mul_i128(const int128_t a, const int128_t b) {
    // 2의 보수 체계에서는 크기가 유지되는 Same-width 곱셈 시 
    // 부호 확장을 고려할 필요 없이 Unsigned 비트 연산 결과와 100% 동일합니다.
    // 따라서 Unsigned 함수를 호출하고 타입만 Signed로 캐스팅하여 반환합니다.
    uint128_t res = hal_mul_u128(*(const uint128_t *)&a, *(const uint128_t *)&b);
    return *(int128_t *)&res;
}

// -----------------------------------------------------------------------------
// division
// -----------------------------------------------------------------------------
inline static uint128_t hal_div_u128(const uint128_t n, const uint128_t d) {
    uint128_t q = {0, 0}; // Quotient
    uint128_t r = {0, 0}; // Remainder

    if (hal_is_zero_u128(d))
        return r;

    // Perform Shift-and-Subtract from bit 127 down to 0
    for (int i = 127; i >= 0; i--) {
        // Shift r left by 1 bit
        r.u = (r.u << 1) | (r.l >> 63);
        r.l = (r.l << 1);

        // Insert the i-th bit of n into the LSB of r
        uint64_t bit = (i >= 64) ? ((n.u >> (i - 64)) & 1) : ((n.l >> i) & 1);
        r.l |= bit;

        // If r >= d, subtract d from r and set the corresponding bit in the quotient to 1
        if (_cmp_ge_u128(r, d)) {
            // r -= d (128-bit subtraction)
            uint64_t prev_l = r.l;
            r.l -= d.l;
            r.u -= d.u + ((r.l > prev_l) ? 1 : 0); // Handle borrow

            // Set the i-th bit of q
            if (i >= 64)
                q.u |= (1ULL << (i - 64));
            else
                q.l |= (1ULL << i);
        }
    }
    return q;
}


inline static int128_t hal_div_i128(const int128_t n, const int128_t d) {
    if (hal_is_zero_i128(d))
        return (int128_t){0, 0};

    // 1. Determine sign
    int sign_n = (n.u < 0) ? -1 : 1;
    int sign_d = (d.u < 0) ? -1 : 1;
    int sign_res = sign_n * sign_d;

    // 2. Convert to absolute value (Unsigned) using two's complement
    uint128_t un = *(uint128_t *)&n;
    if (sign_n < 0) {
        un.l = ~un.l;
        un.u = ~un.u;
        un.l += 1;
        if (un.l == 0)
            un.u += 1;
    }

    uint128_t ud = *(uint128_t *)&d;
    if (sign_d < 0) {
        ud.l = ~ud.l;
        ud.u = ~ud.u;
        ud.l += 1;
        if (ud.l == 0)
            ud.u += 1;
    }

    // 3. Perform unsigned division
    uint128_t uq = hal_div_u128(un, ud);

    // 4. Apply sign to the result (take two's complement again if it should be negative)
    if (sign_res < 0) {
        uq.l = ~uq.l;
        uq.u = ~uq.u;
        uq.l += 1;
        if (uq.l == 0)
            uq.u += 1;
    }

    return *(int128_t *)&uq;
}

// -----------------------------------------------------------------------------
// min
// -----------------------------------------------------------------------------
inline static int128_t hal_min_i128(const int128_t a, const int128_t b) {
    if (a.u < b.u)
        return a;
    if (a.u == b.u && a.l < b.l)
        return a;
    return b;
}

inline static uint128_t hal_min_u128(const uint128_t a, const uint128_t b) {
    if (a.u < b.u)
        return a;
    if (a.u == b.u && a.l < b.l)
        return a;
    return b;
}

// -----------------------------------------------------------------------------
// max
// -----------------------------------------------------------------------------
inline static int128_t hal_max_i128(const int128_t a, const int128_t b) {
    if (a.u > b.u)
        return a;
    if (a.u == b.u && a.l > b.l)
        return a;
    return b;
}

inline static uint128_t hal_max_u128(const uint128_t a, const uint128_t b) {
    if (a.u > b.u)
        return a;
    if (a.u == b.u && a.l > b.l)
        return a;
    return b;
}

/* ------------------------------------------------------------------
 * Bitwise AND (&)
 * ------------------------------------------------------------------ */
inline static uint128_t hal_and_u128(const uint128_t a, const uint128_t b) {
    uint128_t res;
    res.l = a.l & b.l;
    res.u = a.u & b.u;
    return res;
}

inline static int128_t hal_and_i128(const int128_t a, const int128_t b) {
    // 부호 무관하게 비트 패턴이 동일하므로 Unsigned 함수 재사용
    uint128_t res = hal_and_u128(*(const uint128_t *)&a, *(const uint128_t *)&b);
    return *(int128_t *)&res;
}

/* ------------------------------------------------------------------
 * Bitwise OR (|)
 * ------------------------------------------------------------------ */
inline static uint128_t hal_or_u128(const uint128_t a, const uint128_t b) {
    uint128_t res;
    res.l = a.l | b.l;
    res.u = a.u | b.u;
    return res;
}

inline static int128_t hal_or_i128(const int128_t a, const int128_t b) {
    uint128_t res = hal_or_u128(*(const uint128_t *)&a, *(const uint128_t *)&b);
    return *(int128_t *)&res;
}

/* ------------------------------------------------------------------
 * Bitwise XOR (^)
 * ------------------------------------------------------------------ */
inline static uint128_t hal_xor_u128(const uint128_t a, const uint128_t b) {
    uint128_t res;
    res.l = a.l ^ b.l;
    res.u = a.u ^ b.u;
    return res;
}

inline static int128_t hal_xor_i128(const int128_t a, const int128_t b) {
    uint128_t res = hal_xor_u128(*(const uint128_t *)&a, *(const uint128_t *)&b);
    return *(int128_t *)&res;
}

/* ------------------------------------------------------------------
 * Bitwise NOT (~) - unary
 * ------------------------------------------------------------------ */
inline static uint128_t hal_not_u128(const uint128_t a) {
    uint128_t res;
    res.l = ~a.l;
    res.u = ~a.u;
    return res;
}

inline static int128_t hal_not_i128(const int128_t a) {
    uint128_t res = hal_not_u128(*(const uint128_t *)&a);
    return *(int128_t *)&res;
}

// -----------------------------------------------------------------------------
// square root
// -----------------------------------------------------------------------------
inline float hal_rsqrt7_f32(float value) {
    long i;
    float y;
    float x2;
    // const float threehalfs = 1.5F;

    // 0.0f 및 음수 체크
    if (value <= 0.0f) {
        return (value == 0.0f) ? HUGE_VALF : NAN; 
    }

    x2 = value * 0.5F;
    y  = value;
    i  = * ( long * ) &y;                       // 부동 소수점을 정수형으로 해석
    i  = 0x5f3759df - ( i >> 1 );               // "Magic Number"를 이용한 1차 추정
    y  = * ( float * ) &i;

    // --- Newton-Raphson 1회 반복 (정밀도를 높이고 싶을 때 사용) ---
    // 이 줄을 주석 처리하면 순수하게 비트 연산 기반의 낮은 정밀도(약 7~8비트) 추정치가 됩니다.
    // y  = y * ( threehalfs - ( x2 * y * y ) ); 

    return y;
}

#ifdef __cplusplus
}
#endif

#endif
