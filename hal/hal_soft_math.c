#include "hal_soft_math.h"
#include "hal_basic_math.h"
#include <string.h>

#define HAL_FALLBACK __attribute__((weak))

// -----------------------------------------------------------------------------
// HAL Internal Math: 128/256-bit Software Emulation
// -----------------------------------------------------------------------------
int hal_is_zero_u128(const uint128_t a) { return (a.l == 0) && (a.u == 0); }
int hal_is_zero_i128(const int128_t a) { return (a.l == 0) && (a.u == 0); }

/* --- 128-bit Addition --- */
uint128_t hal_add_u128_u64(const uint128_t a, const uint64_t b) {
    uint128_t res;
    res.l = a.l + b;
    res.u = a.u + ((res.l < a.l) ? 1 : 0); // Add 1 to u if a carry occurs
    return res;
}

uint128_t hal_add_u128(const uint128_t a, const uint128_t b) {
    uint128_t c;
    c.l = a.l + b.l;
    c.u = a.u + b.u + ((c.l < a.l) ? 1 : 0);
    return c;
}

int128_t hal_add_i128(const int128_t a, const int128_t b) {
    uint128_t c = hal_add_u128(*(const uint128_t *)&a, *(const uint128_t *)&b);
    return *(int128_t *)&c;
}

uint256_t hal_add_u256(const uint256_t a, const uint256_t b) {
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

int256_t hal_add_i256(const int256_t a, const int256_t b) {
    uint256_t c = hal_add_u256(*(const uint256_t *)&a, *(const uint256_t *)&b);
    return *(int256_t *)&c;
}

uint256_t hal_add_u256_u128(const uint256_t a, const uint128_t b) {
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

uint128_t hal_mul_u64(const uint64_t a, const uint64_t b) {
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

/* --- 128-bit Subtraction --- */

/**
 * @brief unsigned 128bit subtracrtion (a - b)
 */
uint128_t hal_sub_u128(const uint128_t a, const uint128_t b) {
    uint128_t res;

    // low 64bit subtraction
    res.l = a.l - b.l;

    // 2. check for the occurrence of a borrow
    //  If a.l is less than b.l, the subtraction operation must borrow the value of the high bit
    //  from the higher-order operation.
    uint64_t borrow = (a.l > a.l + b.l) ? 1 : 0; // or (a.l < b.l) ? 1 : 0;
    // More Precise Borrow Determination : (a.l < b.l)
    borrow = (a.l < b.l) ? 1 : 0;

    // Upper 64-bit Operation (Incorporating Borrow)
    res.u = a.u - b.u - borrow;

    return res;
}

/**
 * @brief  signed 128 bit subtraction (a - b)
 */
int128_t hal_sub_i128(const int128_t a, const int128_t b) {
    // In the two's complement system, the bit-level logic for subtraction is identical to that of
    // unsigned arithmetic.
    uint128_t res = hal_sub_u128(*(const uint128_t *)&a, *(const uint128_t *)&b);
    return *(int128_t *)&res;
}

/* --- 256-bit Subtraction (if required) --- */
uint256_t hal_sub_u256(const uint256_t a, const uint256_t b) {
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

int128_t hal_mul_i64(const int64_t a, const int64_t b) {
    uint128_t res_u = hal_mul_u64((uint64_t)a, (uint64_t)b);

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

/* Single Large Multiplication: 128bit * 128bit -> 256bit */
uint256_t hal_mul_u128(const uint128_t a, const uint128_t b) {
    uint256_t res = {{0, 0, 0, 0}};

    // 4개의 부분 곱 (Partial products)
    uint128_t p00 = hal_mul_u64(a.l, b.l);
    uint128_t p01 = hal_mul_u64(a.l, b.u);
    uint128_t p10 = hal_mul_u64(a.u, b.l);
    uint128_t p11 = hal_mul_u64(a.u, b.u);

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

/* 128-bit x 128-bit -> 256-bit signed multiplication */
int256_t hal_mul_i128(int128_t a, int128_t b) {
    // 1. 먼저 부호를 무시하고 Unsigned 256비트 곱셈을 수행합니다.
    uint256_t res_u = hal_mul_u128(*(uint128_t *)&a, *(uint128_t *)&b);
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

/* Unsigned 128-bit comparison (returns 1 if a >= b, otherwise 0) */
static int _cmp_ge_u128(const uint128_t a, const uint128_t b) {
    if (a.u > b.u)
        return 1;
    if (a.u < b.u)
        return 0;
    return (a.l >= b.l) ? 1 : 0;
}

/* 128-bit Software Division (Unsigned) */
uint128_t hal_div_u128(uint128_t n, uint128_t d) {
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

/* 128-bit Software Division (Signed) */
int128_t hal_div_i128(const int128_t n, const int128_t d) {
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
// HAL Public C API
// -----------------------------------------------------------------------------
// Pure C O(N^3) matrix multiplication for benchmarking
hal_status_t hal_matrix_vmul_c_f32(double *c, const float *a, const float *b, int M, int N, int K) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }
    if (K <= 0) {
        memset(c, 0, sizeof(double) * M * N);
        ret = HAL_OK;
        goto OUT;
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0; // Use double for accumulation to reduce precision loss
            // float sum = 0.0f; --- IGNORE ---
            for (int k = 0; k < K; k++) {
                sum += a[i * K + k] * b[k * N + j];
            }
            c[i * N + j] = sum;
        }
    }
OUT:
    return ret;
}

// -----------------------------------------------------------------------------
// HAL Public API: 128/256-bit Software Emulation Wrappers
// -----------------------------------------------------------------------------

/* i128/u128 */
HAL_FALLBACK hal_status_t hal_vmul_i128(int256_t *c, const int128_t *a, const int128_t *b,
                                        const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = hal_mul_i128(a[i], b[i]);
    }
OUT:
    return ret;
}

HAL_FALLBACK hal_status_t hal_vmul_u128(uint256_t *c, const uint128_t *a, const uint128_t *b,
                                        const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = hal_mul_u128(a[i], b[i]);
    }
OUT:
    return ret;
}

/* i128/u128 */
HAL_FALLBACK hal_status_t hal_vdiv_i128(int128_t *c, const int128_t *a, const int128_t *b,
                                        const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        // Check for Divide by Zero (verify if both l and u of the struct are 0)
        if (b[i].l == 0 && b[i].u == 0) {
            c[i].l = 0; // Set to 0 as requested
            c[i].u = 0;

            // Record status in error variable (to be evaluated externally later)
            ret |= HAL_ERR_DIV_BY_ZERO;
            continue; // Continue processing the next array elements even if an error occurs
        }

        // Perform safe software division
        c[i] = hal_div_i128(a[i], b[i]);
    }
OUT:
    return ret;
}

HAL_FALLBACK hal_status_t hal_vdiv_u128(uint128_t *c, const uint128_t *a, const uint128_t *b,
                                        const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        // Check for Divide by Zero
        if (b[i].l == 0 && b[i].u == 0) {
            c[i].l = 0; // Set to 0
            c[i].u = 0;

            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }

        // Perform safe software division
        c[i] = hal_div_u128(a[i], b[i]);
    }
OUT:
    return ret;
}

/*
    matrix multiplication with tiled
*/
/* i8/u8 */
HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_i8(int16_t *c, const int8_t *a, const int8_t *b,
                                                   int M, int N, int K, int tile_size) {
    hal_status_t ret = HAL_OK;

    // VLA Mapping
    int16_t (*pc)[N] = (int16_t (*)[N])c;
    const int8_t (*pa)[K] = (const int8_t (*)[K])a;
    const int8_t (*pb)[N] = (const int8_t (*)[N])b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // Initialize output buffer
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pc[i][j] = 0;
        }
    }

    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // Outer loops: Move by tile size
    for (int i = 0; i < M; i += tile_size) {
        for (int j = 0; j < N; j += tile_size) {
            for (int k = 0; k < K; k += tile_size) {

                int i_end = MIN(i + tile_size, M);
                int j_end = MIN(j + tile_size, N);
                int k_end = MIN(k + tile_size, K);

                // Inner loops: Operations within the tile
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
OUT:
    return ret;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_u8(uint16_t *c, const uint8_t *a, const uint8_t *b,
                                                   int M, int N, int K, int tile_size) {
    hal_status_t ret = HAL_OK;

    // VLA Mapping
    uint16_t (*pc)[N] = (uint16_t (*)[N])c;
    const uint8_t (*pa)[K] = (const uint8_t (*)[K])a;
    const uint8_t (*pb)[N] = (const uint8_t (*)[N])b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // Initialize output buffer
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pc[i][j] = 0;
        }
    }

    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // Outer loops: Move by tile size
    for (int i = 0; i < M; i += tile_size) {
        for (int j = 0; j < N; j += tile_size) {
            for (int k = 0; k < K; k += tile_size) {

                int i_end = MIN(i + tile_size, M);
                int j_end = MIN(j + tile_size, N);
                int k_end = MIN(k + tile_size, K);

                // Inner loops: Operations within the tile
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
OUT:
    return ret;
}

/* i16/u16 */
HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_i16(int32_t *c, const int16_t *a, const int16_t *b,
                                                    int M, int N, int K, int tile_size) {
    hal_status_t ret = HAL_OK;

    // VLA Mapping
    int32_t (*pc)[N] = (int32_t (*)[N])c;
    const int16_t (*pa)[K] = (const int16_t (*)[K])a;
    const int16_t (*pb)[N] = (const int16_t (*)[N])b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // Initialize output buffer
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pc[i][j] = 0;
        }
    }

    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // Outer loops: Move by tile size
    for (int i = 0; i < M; i += tile_size) {
        for (int j = 0; j < N; j += tile_size) {
            for (int k = 0; k < K; k += tile_size) {

                int i_end = MIN(i + tile_size, M);
                int j_end = MIN(j + tile_size, N);
                int k_end = MIN(k + tile_size, K);

                // Inner loops: Operations within the tile
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
OUT:
    return ret;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_u16(uint32_t *c, const uint16_t *a,
                                                    const uint16_t *b, int M, int N, int K,
                                                    int tile_size) {
    hal_status_t ret = HAL_OK;

    // VLA Mapping
    uint32_t (*pc)[N] = (uint32_t (*)[N])c;
    const uint16_t (*pa)[K] = (const uint16_t (*)[K])a;
    const uint16_t (*pb)[N] = (const uint16_t (*)[N])b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // Initialize output buffer
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pc[i][j] = 0;
        }
    }

    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // Outer loops: Move by tile size
    for (int i = 0; i < M; i += tile_size) {
        for (int j = 0; j < N; j += tile_size) {
            for (int k = 0; k < K; k += tile_size) {

                int i_end = MIN(i + tile_size, M);
                int j_end = MIN(j + tile_size, N);
                int k_end = MIN(k + tile_size, K);

                // Inner loops: Operations within the tile
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
OUT:
    return ret;
}

/* i32/u32 */
HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_i32(int64_t *c, const int32_t *a, const int32_t *b,
                                                    int M, int N, int K, int tile_size) {
    hal_status_t ret = HAL_OK;

    // VLA Mapping
    int64_t (*pc)[N] = (int64_t (*)[N])c;
    const int32_t (*pa)[K] = (const int32_t (*)[K])a;
    const int32_t (*pb)[N] = (const int32_t (*)[N])b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // Initialize output buffer
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pc[i][j] = 0.0;
        }
    }

    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // Outer loops: Move by tile size
    for (int i = 0; i < M; i += tile_size) {
        for (int j = 0; j < N; j += tile_size) {
            for (int k = 0; k < K; k += tile_size) {

                int i_end = MIN(i + tile_size, M);
                int j_end = MIN(j + tile_size, N);
                int k_end = MIN(k + tile_size, K);

                // Inner loops: Operations within the tile
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
OUT:
    return ret;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_u32(uint64_t *c, const uint32_t *a,
                                                    const uint32_t *b, int M, int N, int K,
                                                    int tile_size) {
    hal_status_t ret = HAL_OK;

    // VLA Mapping
    uint64_t (*pc)[N] = (uint64_t (*)[N])c;
    const uint32_t (*pa)[K] = (const uint32_t (*)[K])a;
    const uint32_t (*pb)[N] = (const uint32_t (*)[N])b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // Initialize output buffer
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pc[i][j] = 0.0;
        }
    }

    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // Outer loops: Move by tile size
    for (int i = 0; i < M; i += tile_size) {
        for (int j = 0; j < N; j += tile_size) {
            for (int k = 0; k < K; k += tile_size) {

                int i_end = MIN(i + tile_size, M);
                int j_end = MIN(j + tile_size, N);
                int k_end = MIN(k + tile_size, K);

                // Inner loops: Operations within the tile
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
OUT:
    return ret;
}

/* 64 */
HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_i64(int128_t *c, const int64_t *a, const int64_t *b,
                                                    int M, int N, int K, int tile_size) {
    hal_status_t ret = HAL_OK;

    int128_t(*pc)[N] = (int128_t(*)[N])c;
    const int64_t (*pa)[K] = (const int64_t (*)[K])a;
    const int64_t (*pb)[N] = (const int64_t (*)[N])b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    memset(c, 0, sizeof(int128_t) * M * N);

    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

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
                            pc[ii][jj] = hal_add_i128(pc[ii][jj], hal_mul_i64(prod, pb[kk][jj]));
                        }
                    }
                }
            }
        }
    }
OUT:
    return ret;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_u64(uint128_t *c, const uint64_t *a,
                                                    const uint64_t *b, int M, int N, int K,
                                                    int tile_size) {
    hal_status_t ret = HAL_OK;

    uint128_t(*pc)[N] = (uint128_t(*)[N])c;
    const uint64_t (*pa)[K] = (const uint64_t (*)[K])a;
    const uint64_t (*pb)[N] = (const uint64_t (*)[N])b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    memset(c, 0, sizeof(uint128_t) * M * N);

    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

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
                            pc[ii][jj] = hal_add_u128(pc[ii][jj], hal_mul_u64(prod, pb[kk][jj]));
                        }
                    }
                }
            }
        }
    }
OUT:
    return ret;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_i128(int256_t *c, const int128_t *a,
                                                     const int128_t *b, int M, int N, int K,
                                                     int tile_size) {
    hal_status_t ret = HAL_OK;

    int256_t(*pc)[N] = (int256_t(*)[N])c;
    const int128_t(*pa)[K] = (const int128_t(*)[K])a;
    const int128_t(*pb)[N] = (const int128_t(*)[N])b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    memset(c, 0, sizeof(int256_t) * M * N);

    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

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
                            pc[ii][jj] = hal_add_i256(pc[ii][jj], hal_mul_i128(prod, pb[kk][jj]));
                        }
                    }
                }
            }
        }
    }
OUT:
    return ret;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_u128(uint256_t *c, const uint128_t *a,
                                                     const uint128_t *b, int M, int N, int K,
                                                     int tile_size) {
    hal_status_t ret = HAL_OK;

    uint256_t(*pc)[N] = (uint256_t(*)[N])c;
    const uint128_t(*pa)[K] = (const uint128_t(*)[K])a;
    const uint128_t(*pb)[N] = (const uint128_t(*)[N])b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    memset(c, 0, sizeof(uint256_t) * M * N);

    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

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
                            pc[ii][jj] = hal_add_u256(pc[ii][jj], hal_mul_u128(prod, pb[kk][jj]));
                        }
                    }
                }
            }
        }
    }
OUT:
    return ret;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_f32(double *c, const float *a, const float *b,
                                                    int M, int N, int K, int tile_size) {
    hal_status_t ret = HAL_OK;

    // VLA Mapping
    double (*pc)[N] = (double (*)[N])c;
    const float (*pa)[K] = (const float (*)[K])a;
    const float (*pb)[N] = (const float (*)[N])b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // Initialize output buffer
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pc[i][j] = 0.0f;
        }
    }

    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // Outer loops: Move by tile size
    for (int i = 0; i < M; i += tile_size) {
        for (int j = 0; j < N; j += tile_size) {
            for (int k = 0; k < K; k += tile_size) {

                int i_end = MIN(i + tile_size, M);
                int j_end = MIN(j + tile_size, N);
                int k_end = MIN(k + tile_size, K);

                // Inner loops: Operations within the tile
                for (int ii = i; ii < i_end; ii++) {
                    for (int jj = j; jj < j_end; jj++) {
                        float sum = pc[ii][jj];
                        for (int kk = k; kk < k_end; kk++) {
                            // Accumulate float * float -> float
                            sum += (float)pa[ii][kk] * (float)pb[kk][jj];
                        }
                        pc[ii][jj] = sum;
                    }
                }
            }
        }
    }
OUT:
    return ret;
}

/* i64/u64 */
HAL_FALLBACK hal_status_t hal_vdot_i64(int128_t *result, const int64_t *a, const int64_t *b,
                                       const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || result == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    int128_t sum = {0, 0};
    for (size_t i = 0; i < n; i++) {
        if (a[i] == 0 || b[i] == 0)
            continue; // 0-Skip optimization
        int128_t prod = hal_mul_i64(a[i], b[i]);
        sum = hal_add_i128(sum, prod);
    }
    *result = sum;

OUT:
    return ret;
}

HAL_FALLBACK hal_status_t hal_vdot_u64(uint128_t *result, const uint64_t *a, const uint64_t *b,
                                       const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || result == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    uint128_t sum = {0, 0};
    for (size_t i = 0; i < n; i++) {
        if (a[i] == 0 || b[i] == 0)
            continue;
        uint128_t prod = hal_mul_u64(a[i], b[i]);
        sum = hal_add_u128(sum, prod);
    }
    *result = sum;

OUT:
    return ret;
}

/* i128/u128 */
HAL_FALLBACK hal_status_t hal_vdot_i128(int256_t *result, const int128_t *a, const int128_t *b,
                                        const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || result == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    int256_t sum = {0};

    for (size_t i = 0; i < n; i++) {
        // Struct 0-Skip optimization (if either is 0, the multiplication result is 0)
        if ((a[i].l == 0 && a[i].u == 0) || (b[i].l == 0 && b[i].u == 0))
            continue;

        int256_t prod = hal_mul_i128(a[i], b[i]);
        sum = hal_add_i256(sum, prod);
    }
    *result = sum;

OUT:
    return ret;
}

HAL_FALLBACK hal_status_t hal_vdot_u128(uint256_t *result, const uint128_t *a, const uint128_t *b,
                                        const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || result == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    uint256_t sum = {{0, 0, 0, 0}};

    for (size_t i = 0; i < n; i++) {
        // Struct 0-Skip optimization
        if ((a[i].l == 0 && a[i].u == 0) || (b[i].l == 0 && b[i].u == 0))
            continue;

        uint256_t prod = hal_mul_u128(a[i], b[i]);
        sum = hal_add_u256(sum, prod);
    }
    *result = sum;

OUT:
    return ret;
}

/* i64/u64 */
HAL_FALLBACK hal_status_t hal_vmac_i64(int128_t *c, const int64_t *a, const int64_t *b,
                                       const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || n == 0) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        int128_t prod = hal_mul_i64(a[i], b[i]);
        c[i] = hal_add_i128(c[i], prod);
    }
OUT:
    return ret;
}

HAL_FALLBACK hal_status_t hal_vmac_u64(uint128_t *c, const uint64_t *a, const uint64_t *b,
                                       const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || n == 0) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        uint128_t prod = hal_mul_u64(a[i], b[i]);
        c[i] = hal_add_u128(c[i], prod);
    }
OUT:
    return ret;
}

/* i128/u128 */
HAL_FALLBACK hal_status_t hal_vmac_i128(int256_t *c, const int128_t *a, const int128_t *b,
                                        const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || n == 0) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    for (uint32_t i = 0; i < n; i++) {
        // 1. 128bit x 128bit -> 256bit single multiplication (negative sign extension logic applied
        // automatically)
        int256_t prod = hal_mul_i128(a[i], b[i]);

        // 2. 256bit addition accumulation (bit-level addition is safe regardless of sign)
        c[i] = hal_add_i256(c[i], prod);
    }
OUT:
    return ret;
}

HAL_FALLBACK hal_status_t hal_vmac_u128(uint256_t *c, const uint128_t *a, const uint128_t *b,
                                        const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || n == 0) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    for (uint32_t i = 0; i < n; i++) {
        uint256_t prod = hal_mul_u128(a[i], b[i]);
        c[i] = hal_add_u256(c[i], prod);
    }
OUT:
    return ret;
}

/* i64/u64 */
HAL_FALLBACK hal_status_t hal_matrix_vmul_i64(int128_t *c, const int64_t *a, const int64_t *b,
                                              int M, int N, int K) {
    hal_status_t ret = HAL_OK;

    // VLA mapping
    int128_t(*pc)[N] = (void *)c;
    int64_t (*pa)[K] = (void *)a;
    int64_t (*pb)[N] = (void *)b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }
    if (K <= 0) {
        memset(c, 0, sizeof(int128_t) * M * N);
        ret = HAL_OK;
        goto OUT;
    }

    memset(c, 0, sizeof(int128_t) * M * N);

    // 3. i-k-j order operation (Cache optimization and Sparse matrix handling)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    int128_t prod = hal_mul_i64(pa[i][k], pb[k][j]);
                    pc[i][j] = hal_add_i128(pc[i][j], prod);
                }
            }
        }
    }
OUT:
    return ret;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_u64(uint128_t *c, const uint64_t *a, const uint64_t *b,
                                              int M, int N, int K) {
    hal_status_t ret = HAL_OK;

    // VLA mapping
    uint128_t(*pc)[N] = (void *)c;
    uint64_t (*pa)[K] = (void *)a;
    uint64_t (*pb)[N] = (void *)b;
    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }
    if (K <= 0) {
        memset(c, 0, sizeof(uint128_t) * M * N);
        ret = HAL_OK;
        goto OUT;
    }

    memset(c, 0, sizeof(uint128_t) * M * N);

    // 3. i-k-j order operation (Cache optimization and Sparse matrix handling)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    uint128_t prod = hal_mul_u64(pa[i][k], pb[k][j]);
                    pc[i][j] = hal_add_u128(pc[i][j], prod);
                }
            }
        }
    }
OUT:
    return ret;
}

/* i128/u128 */
HAL_FALLBACK hal_status_t hal_matrix_vmul_i128(int256_t *c, const int128_t *a, const int128_t *b,
                                               int M, int N, int K) {
    hal_status_t ret = HAL_OK;

    // VLA mapping
    int256_t(*pc)[N] = (void *)c;
    int128_t(*pa)[K] = (void *)a;
    int128_t(*pb)[N] = (void *)b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }
    if (K <= 0) {
        memset(c, 0, sizeof(int256_t) * M * N);
        ret = HAL_OK;
        goto OUT;
    }

    memset(c, 0, sizeof(int256_t) * M * N);

    // 3. i-k-j order operation (Cache optimization and Sparse matrix handling)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            // if (pa[i][k] != 0) {
            if ((pa[i][k].u != 0) || (pa[i][k].l != 0)) {
                for (int j = 0; j < N; j++) {
                    int256_t prod = hal_mul_i128(pa[i][k], pb[k][j]);
                    pc[i][j] = hal_add_i256(pc[i][j], prod);
                }
            }
        }
    }
OUT:
    return ret;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_u128(uint256_t *c, const uint128_t *a, const uint128_t *b,
                                               int M, int N, int K) {
    hal_status_t ret = HAL_OK;

    // VLA mapping
    uint256_t(*pc)[N] = (void *)c;
    uint128_t(*pa)[K] = (void *)a;
    uint128_t(*pb)[N] = (void *)b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }
    if (K <= 0) {
        memset(c, 0, sizeof(uint256_t) * M * N);
        ret = HAL_OK;
        goto OUT;
    }

    memset(c, 0, sizeof(uint256_t) * M * N);

    // 3. i-k-j order operation (Cache optimization and Sparse matrix handling)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if ((pa[i][k].u != 0) || (pa[i][k].l != 0)) {
                for (int j = 0; j < N; j++) {
                    uint256_t prod = hal_mul_u128(pa[i][k], pb[k][j]);
                    pc[i][j] = hal_add_u256(pc[i][j], prod);
                }
            }
        }
    }
OUT:
    return ret;
}
