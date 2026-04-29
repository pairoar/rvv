#ifndef HAL_BASIC_MATH_H
#define HAL_BASIC_MATH_H

#include "hal_math.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#ifdef __cplusplus
extern "C" {
#endif

/* 128/256-bit Arithmetic Helpers */
int128_t hal_add_i128(const int128_t a, const int128_t b);
uint128_t hal_add_u128(const uint128_t a, const uint128_t b);
uint256_t hal_add_u256(const uint256_t a, const uint256_t b);
int256_t hal_add_i256(const int256_t a, const int256_t b);

int128_t hal_sub_i128(const int128_t a, const int128_t b);
uint128_t hal_sub_u128(const uint128_t a, const uint128_t b);
int256_t hal_sub_i256(const int256_t a, const int256_t b);
uint256_t hal_sub_u256(const uint256_t a, const uint256_t b);

uint128_t hal_mul_u64(const uint64_t a, const uint64_t b);
int128_t hal_mul_i64(const int64_t a, const int64_t b);
uint256_t hal_mul_u128(const uint128_t a, const uint128_t b);
int256_t hal_mul_i128(const int128_t a, const int128_t b);

int128_t hal_div_i128(const int128_t n, const int128_t d);
uint128_t hal_div_u128(uint128_t n, uint128_t d);

#ifdef __cplusplus
}
#endif

#endif
