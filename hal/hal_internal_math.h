#ifndef HAL_INTERNAL_MATH_H
#define HAL_INTERNAL_MATH_H

#include "hal_math.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/* 128/256-bit Arithmetic Helpers */
int128_t _add_i128(const int128_t a, const int128_t b);
uint128_t _add_u128(const uint128_t a, const uint128_t b);
uint256_t _add_u256(const uint256_t a, const uint256_t b);
int256_t _add_i256(const int256_t a, const int256_t b);

int128_t _sub_i128(const int128_t a, const int128_t b);
uint128_t _sub_u128(const uint128_t a, const uint128_t b);
int256_t _sub_i256(const int256_t a, const int256_t b);
uint256_t _sub_u256(const uint256_t a, const uint256_t b);

uint128_t _mul_u64(const uint64_t a, const uint64_t b);
int128_t _mul_i64(const int64_t a, const int64_t b);
uint256_t _mul_u128(const uint128_t a, const uint128_t b);
int256_t _mul_i128(const int128_t a, const int128_t b);

int128_t _div_i128(const int128_t n, const int128_t d);
uint128_t _div_u128(uint128_t n, uint128_t d);

#ifdef __cplusplus
}
#endif

#endif
