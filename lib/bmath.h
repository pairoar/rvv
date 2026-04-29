#ifndef BMATH_H
#define BMATH_H

#include "hal_basic_math.h"

// 치환 방향 수정: math_ 함수를 hal_ 함수로 연결
#define math_add_i128    hal_add_i128
#define math_add_u128    hal_add_u128
#define math_add_u256    hal_add_u256
#define math_add_i256    hal_add_i256

#define math_sub_i128    hal_sub_i128
#define math_sub_u128    hal_sub_u128
#define math_sub_i256    hal_sub_i256
#define math_sub_u256    hal_sub_u256

#define math_mul_u64     hal_mul_u64
#define math_mul_i64     hal_mul_i64
#define math_mul_u128    hal_mul_u128
#define math_mul_i128    hal_mul_i128

#define math_div_i128    hal_div_i128
#define math_div_u128    hal_div_u128

#endif // BMATH_H