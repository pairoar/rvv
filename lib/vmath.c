/**
 * @file   aermob_lib_hw.c
 * @brief  Library
 * @author Seongjin Oh
 * @date   03-17-2026
 */

/* ======================================================================== */
/* Standard Library Headers                                                 */
/* ======================================================================== */
#include <stdio.h>

/* ======================================================================== */
/* User-Defined Headers                                                     */
/* ======================================================================== */
#include "vmath_types.h"
#include "hal_soft_math.h"
#include "hal_math.h"

/* ======================================================================== */
/*  Macros (#define)                                                        */
/* ======================================================================== */

/* ======================================================================== */
/* Local Types (enum, struct, typedef)                                      */
/* ======================================================================== */

/* ======================================================================== */
/* Global Variables (Minimize usage recommended)                            */
/* ======================================================================== */

/* ======================================================================== */
/* Static Variables (File-scope global variables)                           */
/* ======================================================================== */

/* ======================================================================== */
/* Static Function Prototypes (Forward declarations)                        */
/* ======================================================================== */

/* ======================================================================== */
/* Public Functions (API Implementations)                                   */
/* ======================================================================== */

// --> new start

/* addition */
vmath_status_t vmath_add_i8(int8_t *dst, const int8_t *src_a, const int8_t *src_b,
                            const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vadd_i8(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_add_u8(uint8_t *dst, const uint8_t *src_a, const uint8_t *src_b,
                            const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vadd_u8(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_add_i16(int16_t *dst, const int16_t *src_a, const int16_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vadd_i16(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_add_u16(uint16_t *dst, const uint16_t *src_a, const uint16_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vadd_u16(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_add_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vadd_i32(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_add_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vadd_u32(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_add_i64(int64_t *dst, const int64_t *src_a, const int64_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vadd_i64(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_add_u64(uint64_t *dst, const uint64_t *src_a, const uint64_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vadd_u64(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_add_i128(int128_t *dst, const int128_t *src_a, const int128_t *src_b,
                              const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vadd_i128(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_add_u128(uint128_t *dst, const uint128_t *src_a, const uint128_t *src_b,
                              const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vadd_u128(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

/* subtraction */
vmath_status_t vmath_sub_i8(int8_t *dst, const int8_t *src_a, const int8_t *src_b,
                            const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vsub_i8(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_sub_u8(uint8_t *dst, const uint8_t *src_a, const uint8_t *src_b,
                            const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vsub_u8(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_sub_i16(int16_t *dst, const int16_t *src_a, const int16_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vsub_i16(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_sub_u16(uint16_t *dst, const uint16_t *src_a, const uint16_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vsub_u16(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_sub_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vsub_i32(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_sub_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vsub_u32(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_sub_i64(int64_t *dst, const int64_t *src_a, const int64_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vsub_i64(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_sub_u64(uint64_t *dst, const uint64_t *src_a, const uint64_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vsub_u64(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_sub_i128(int128_t *dst, const int128_t *src_a, const int128_t *src_b,
                              const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vsub_i128(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_sub_u128(uint128_t *dst, const uint128_t *src_a, const uint128_t *src_b,
                              const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vsub_u128(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

/* multiplication */
vmath_status_t vmath_mul_i8(int16_t *dst, const int8_t *src_a, const int8_t *src_b,
                            const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vmul_i8(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_mul_u8(uint16_t *dst, const uint8_t *src_a, const uint8_t *src_b,
                            const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vmul_u8(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_mul_i16(int32_t *dst, const int16_t *src_a, const int16_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vmul_i16(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_mul_u16(uint32_t *dst, const uint16_t *src_a, const uint16_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vmul_u16(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_mul_i32(int64_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vmul_i32(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_mul_u32(uint64_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vmul_u32(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_mul_i64(int128_t *dst, const int64_t *src_a, const int64_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vmul_i64(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_mul_u64(uint128_t *dst, const uint64_t *src_a, const uint64_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vmul_u64(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_mul_i128(int256_t *dst, const int128_t *src_a, const int128_t *src_b,
                              const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vmul_i128(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

vmath_status_t vmath_mul_u128(uint256_t *dst, const uint128_t *src_a, const uint128_t *src_b,
                              const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    hal_vmul_u128(dst, src_a, src_b, len);
    return VMATH_SUCCESS;
}

/* mac */
vmath_status_t vmath_mac_i8(int16_t *dst, const int8_t *src_a, const int8_t *src_b,
                            const size_t len)
{
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL)
    {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0)
    {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_vmac_i8(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_mac_u8(uint16_t *dst, const uint8_t *src_a, const uint8_t *src_b,
                            const size_t len)
{
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL)
    {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0)
    {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_vmac_u8(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_mac_i16(int32_t *dst, const int16_t *src_a, const int16_t *src_b,
                             const size_t len)
{
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL)
    {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0)
    {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_vmac_i16(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_mac_u16(uint32_t *dst, const uint16_t *src_a, const uint16_t *src_b,
                             const size_t len)
{
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL)
    {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0)
    {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_vmac_u16(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_mac_i32(int64_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len)
{
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL)
    {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0)
    {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_vmac_i32(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_mac_u32(uint64_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len)
{
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL)
    {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0)
    {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_vmac_u32(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_mac_i64(int128_t *dst, const int64_t *src_a, const int64_t *src_b,
                             const size_t len)
{
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL)
    {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0)
    {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_vmac_i64(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_mac_u64(uint128_t *dst, const uint64_t *src_a, const uint64_t *src_b,
                             const size_t len)
{
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL)
    {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0)
    {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_vmac_u64(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_mac_i128(int256_t *dst, const int128_t *src_a, const int128_t *src_b,
                              const size_t len)
{
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL)
    {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0)
    {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_vmac_i128(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_mac_u128(uint256_t *dst, const uint128_t *src_a, const uint128_t *src_b,
                              const size_t len)
{
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL)
    {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0)
    {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_vmac_u128(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

/* division */
vmath_status_t vmath_div_i8(int8_t *dst, const int8_t *src_a, const int8_t *src_b,
                            const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;
    int ret_code = 0;
    hal_vdiv_i8(dst, src_a, src_b, len, &ret_code);
    if (ret_code != 0)
    {
        // 하드웨어에서 나누기 연산 중 오류 발생 (예: 0으로 나누기)
        return VMATH_ERR_HW_FAULT;
    }
    return VMATH_SUCCESS;
}

vmath_status_t vmath_div_u8(uint8_t *dst, const uint8_t *src_a, const uint8_t *src_b,
                            const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;
    int ret_code = 0;
    hal_vdiv_u8(dst, src_a, src_b, len, &ret_code);
    if (ret_code != 0)
    {
        // 하드웨어에서 나누기 연산 중 오류 발생 (예: 0으로 나누기)
        return VMATH_ERR_HW_FAULT;
    }
    return VMATH_SUCCESS;
}

vmath_status_t vmath_div_i16(int16_t *dst, const int16_t *src_a, const int16_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;
    int ret_code = 0;
    hal_vdiv_i16(dst, src_a, src_b, len, &ret_code);
    if (ret_code != 0)
    {
        // 하드웨어에서 나누기 연산 중 오류 발생 (예: 0으로 나누기)
        return VMATH_ERR_HW_FAULT;
    }
    return VMATH_SUCCESS;
}

vmath_status_t vmath_div_u16(uint16_t *dst, const uint16_t *src_a, const uint16_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;
    int ret_code = 0;
    hal_vdiv_u16(dst, src_a, src_b, len, &ret_code);
    if (ret_code != 0)
    {
        // 하드웨어에서 나누기 연산 중 오류 발생 (예: 0으로 나누기)
        return VMATH_ERR_HW_FAULT;
    }
    return VMATH_SUCCESS;
}

vmath_status_t vmath_div_i32(int32_t *dst, const int32_t *src_a, const int32_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;
    int ret_code = 0;
    hal_vdiv_i32(dst, src_a, src_b, len, &ret_code);
    if (ret_code != 0)
    {
        // 하드웨어에서 나누기 연산 중 오류 발생 (예: 0으로 나누기)
        return VMATH_ERR_HW_FAULT;
    }
    return VMATH_SUCCESS;
}

vmath_status_t vmath_div_u32(uint32_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;
    int ret_code = 0;
    hal_vdiv_u32(dst, src_a, src_b, len, &ret_code);
    if (ret_code != 0)
    {
        // 하드웨어에서 나누기 연산 중 오류 발생 (예: 0으로 나누기)
        return VMATH_ERR_HW_FAULT;
    }
    return VMATH_SUCCESS;
}

vmath_status_t vmath_div_i64(int64_t *dst, const int64_t *src_a, const int64_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;
    int ret_code = 0;
    hal_vdiv_i64(dst, src_a, src_b, len, &ret_code);
    if (ret_code != 0)
    {
        // 하드웨어에서 나누기 연산 중 오류 발생 (예: 0으로 나누기)
        return VMATH_ERR_HW_FAULT;
    }
    return VMATH_SUCCESS;
}

vmath_status_t vmath_div_u64(uint64_t *dst, const uint64_t *src_a, const uint64_t *src_b,
                             const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;
    int ret_code = 0;
    hal_vdiv_u64(dst, src_a, src_b, len, &ret_code);
    if (ret_code != 0)
    {
        // 하드웨어에서 나누기 연산 중 오류 발생 (예: 0으로 나누기)
        return VMATH_ERR_HW_FAULT;
    }
    return VMATH_SUCCESS;
}

vmath_status_t vmath_div_i128(int128_t *dst, const int128_t *src_a, const int128_t *src_b,
                              const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;
    int ret_code = 0;
    hal_vdiv_i128(dst, src_a, src_b, len, &ret_code);
    if (ret_code != 0)
    {
        // 하드웨어에서 나누기 연산 중 오류 발생 (예: 0으로 나누기)
        return VMATH_ERR_HW_FAULT;
    }
    return VMATH_SUCCESS;
}

vmath_status_t vmath_div_u128(uint128_t *dst, const uint128_t *src_a, const uint128_t *src_b,
                              const size_t len)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;
    int ret_code = 0;
    hal_vdiv_u128(dst, src_a, src_b, len, &ret_code);
    if (ret_code != 0)
    {
        // 하드웨어에서 나누기 연산 중 오류 발생 (예: 0으로 나누기)
        return VMATH_ERR_HW_FAULT;
    }
    return VMATH_SUCCESS;
}

/* dot prodcut */
vmath_status_t vmath_matrix_dot_i8(int16_t *result, const int8_t *src_a, const int8_t *src_b,
                                   size_t len)
{
    if (src_a == NULL || src_b == NULL || result == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 초기 쓰레기값이 있을 수 있으므로 방어적으로 0로 초기화
    *result = 0;

    hal_vdot_i8(result, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_dot_u8(uint16_t *result, const uint8_t *src_a, const uint8_t *src_b,
                                   size_t len)
{
    if (src_a == NULL || src_b == NULL || result == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 초기 쓰레기값이 있을 수 있으므로 방어적으로 0로 초기화
    *result = 0;

    hal_vdot_u8(result, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_dot_i16(int32_t *result, const int16_t *src_a, const int16_t *src_b,
                                    size_t len)
{
    if (src_a == NULL || src_b == NULL || result == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 초기 쓰레기값이 있을 수 있으므로 방어적으로 0로 초기화
    *result = 0;

    hal_vdot_i16(result, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_dot_u16(uint32_t *result, const uint16_t *src_a, const uint16_t *src_b,
                                    size_t len)
{
    if (src_a == NULL || src_b == NULL || result == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 초기 쓰레기값이 있을 수 있으므로 방어적으로 0로 초기화
    *result = 0;

    hal_vdot_u16(result, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_dot_i32(int64_t *result, const int32_t *src_a, const int32_t *src_b,
                                    size_t len)
{
    if (src_a == NULL || src_b == NULL || result == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 초기 쓰레기값이 있을 수 있으므로 방어적으로 0로 초기화
    *result = 0;

    hal_vdot_i32(result, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_dot_u32(uint64_t *result, const uint32_t *src_a, const uint32_t *src_b,
                                    size_t len)
{
    if (src_a == NULL || src_b == NULL || result == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 초기 쓰레기값이 있을 수 있으므로 방어적으로 0로 초기화
    *result = 0;

    hal_vdot_u32(result, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_dot_i64(int128_t *result, const int64_t *src_a, const int64_t *src_b,
                                    size_t len)
{
    if (src_a == NULL || src_b == NULL || result == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 초기 쓰레기값이 있을 수 있으므로 방어적으로 0로 초기화
    *result = (int128_t){0, 0};

    hal_vdot_i64(result, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_dot_u64(uint128_t *result, const uint64_t *src_a, const uint64_t *src_b,
                                    size_t len)
{
    if (src_a == NULL || src_b == NULL || result == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 초기 쓰레기값이 있을 수 있으므로 방어적으로 0로 초기화
    *result = (uint128_t){0};

    hal_vdot_u64(result, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_dot_i128(int256_t *result, const int128_t *src_a, const int128_t *src_b,
                                     size_t len)
{
    if (src_a == NULL || src_b == NULL || result == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 초기 쓰레기값이 있을 수 있으므로 방어적으로 0로 초기화
    *result = (int256_t){0};

    hal_vdot_i128(result, src_a, src_b, len);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_dot_u128(uint256_t *result, const uint128_t *src_a,
                                     const uint128_t *src_b, size_t len)
{
    if (src_a == NULL || src_b == NULL || result == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 초기 쓰레기값이 있을 수 있으므로 방어적으로 0로 초기화
    *result = (uint256_t){0};

    hal_vdot_u128(result, src_a, src_b, len);

    return VMATH_SUCCESS;
}

/* matrix multiplication */
vmath_status_t vmath_matrix_mul_i8(int16_t *dst, const int8_t *src_a, const int8_t *src_b, size_t M,
                                   size_t N, size_t K)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (size_t i = 0; i < M * N; i++)
    {
        dst[i] = 0;
    }

    hal_matrix_vmul_i8(dst, src_a, src_b, (int)M, (int)N, (int)K);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_u8(uint16_t *dst, const uint8_t *src_a, const uint8_t *src_b,
                                   size_t M, size_t N, size_t K)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (size_t i = 0; i < M * N; i++)
    {
        dst[i] = 0;
    }

    hal_matrix_vmul_u8(dst, src_a, src_b, (int)M, (int)N, (int)K);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_i16(int32_t *dst, const int16_t *src_a, const int16_t *src_b,
                                    size_t M, size_t N, size_t K)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (size_t i = 0; i < M * N; i++)
    {
        dst[i] = 0;
    }

    hal_matrix_vmul_i16(dst, src_a, src_b, (int)M, (int)N, (int)K);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_u16(uint32_t *dst, const uint16_t *src_a, const uint16_t *src_b,
                                    size_t M, size_t N, size_t K)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (size_t i = 0; i < M * N; i++)
    {
        dst[i] = 0;
    }

    hal_matrix_vmul_u16(dst, src_a, src_b, (int)M, (int)N, (int)K);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_i32(int64_t *dst, const int32_t *src_a, const int32_t *src_b,
                                    size_t M, size_t N, size_t K)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (size_t i = 0; i < M * N; i++)
    {
        dst[i] = 0;
    }

    hal_matrix_vmul_i32(dst, src_a, src_b, (int)M, (int)N, (int)K);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_u32(uint64_t *dst, const uint32_t *src_a, const uint32_t *src_b,
                                    size_t M, size_t N, size_t K)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (size_t i = 0; i < M * N; i++)
    {
        dst[i] = 0;
    }

    hal_matrix_vmul_u32(dst, src_a, src_b, (int)M, (int)N, (int)K);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_i64(int128_t *dst, const int64_t *src_a, const int64_t *src_b,
                                    size_t M, size_t N, size_t K)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (size_t i = 0; i < M * N; i++)
    {
        dst[i] = (int128_t){0};
    }

    hal_matrix_vmul_i64(dst, src_a, src_b, (int)M, (int)N, (int)K);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_u64(uint128_t *dst, const uint64_t *src_a, const uint64_t *src_b,
                                    size_t M, size_t N, size_t K)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (size_t i = 0; i < M * N; i++)
    {
        dst[i] = (uint128_t){0};
    }

    hal_matrix_vmul_u64(dst, src_a, src_b, (int)M, (int)N, (int)K);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_i128(int256_t *dst, const int128_t *src_a, const int128_t *src_b,
                                     size_t M, size_t N, size_t K)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (size_t i = 0; i < M * N; i++)
    {
        dst[i] = (int256_t){0};
    }

    hal_matrix_vmul_i128(dst, src_a, src_b, (int)M, (int)N, (int)K);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_u128(uint256_t *dst, const uint128_t *src_a, const uint128_t *src_b,
                                     size_t M, size_t N, size_t K)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (size_t i = 0; i < M * N; i++)
    {
        dst[i] = (uint256_t){0};
    }

    hal_matrix_vmul_u128(dst, src_a, src_b, (int)M, (int)N, (int)K);

    return VMATH_SUCCESS;
}

/*
    float addtion
*/
vmath_status_t vmath_add_f32(float *dst, const float *src_a, const float *src_b, const size_t len)
{
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL)
    {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0)
    {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_vadd_f32(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

/* float subtraction */
vmath_status_t vmath_sub_f32(float *dst, const float *src_a, const float *src_b, const size_t len)
{
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL)
    {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0)
    {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_vsub_f32(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

/* float multiplicaton */
vmath_status_t vmath_mul_f32(double *dst, const float *src_a, const float *src_b,
                             const size_t len)
{
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL)
    {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0)
    {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_vmul_f32(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

/* float mac */
vmath_status_t vmath_mac_f32(double *dst, const float *src_a, const float *src_b,
                             const size_t len)
{
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL)
    {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0)
    {
        return VMATH_ERR_INVALID_SIZE;
    }

    // 2. HAL 계층 호출 (실제 연산 위임)
    hal_vmac_f32(dst, src_a, src_b, len);

    return VMATH_SUCCESS;
}

/* float division */
vmath_status_t vmath_div_f32(float *dst, const float *src_a, const float *src_b, const size_t len)
{
    // 1. 파라미터 유효성 검사 (안전성 확보)
    if (src_a == NULL || src_b == NULL || dst == NULL)
    {
        return VMATH_ERR_NULL_PTR;
    }
    if (len == 0)
    {
        return VMATH_ERR_INVALID_SIZE;
    }
    int ret_code = 0;
    hal_vdiv_f32(dst, src_a, src_b, len, &ret_code);
    if (ret_code != 0)
    {
        // 하드웨어에서 나누기 연산 중 오류 발생 (예: 0으로 나누기)
        return VMATH_ERR_HW_FAULT;
    }

    return VMATH_SUCCESS;
}

/* float dot product */
vmath_status_t vmath_matrix_dot_f32(double *result, const float *src_a, const float *src_b,
                                    size_t len)
{
    if (src_a == NULL || src_b == NULL || result == NULL)
        return VMATH_ERR_NULL_PTR;
    if (len == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 초기 쓰레기값이 있을 수 있으므로 방어적으로 0.0f로 초기화
    *result = 0.0f;

    hal_vdot_f32(result, src_a, src_b, len);

    return VMATH_SUCCESS;
}

/* float matrix multilication */
vmath_status_t vmath_matrix_mul_f32(double *dst, const float *src_a, const float *src_b, size_t M,
                                    size_t N, size_t K)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (size_t i = 0; i < M * N; i++)
    {
        dst[i] = 0.0f;
    }

    hal_matrix_vmul_f32(dst, src_a, src_b, (int)M, (int)N, (int)K);

    return VMATH_SUCCESS;
}

// --> new end

// --> tiled start
/* Tiled Matrix Multiplication (Signed) */
vmath_status_t vmath_matrix_mul_tiled_i8(int16_t *dst, const int8_t *src_a, const int8_t *src_b,
                                         int M, int N, int K, int tile_size)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (int i = 0; i < M * N; i++)
    {
        dst[i] = 0;
    }

    hal_matrix_vmul_tiled_i8(dst, src_a, src_b, (int)M, (int)N, (int)K, tile_size);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_tiled_i16(int32_t *dst, const int16_t *src_a, const int16_t *src_b,
                                          int M, int N, int K, int tile_size)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (int i = 0; i < M * N; i++)
    {
        dst[i] = 0;
    }

    hal_matrix_vmul_tiled_i16(dst, src_a, src_b, (int)M, (int)N, (int)K, tile_size);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_tiled_i32(int64_t *dst, const int32_t *src_a, const int32_t *src_b,
                                          int M, int N, int K, int tile_size)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (int i = 0; i < M * N; i++)
    {
        dst[i] = 0;
    }

    hal_matrix_vmul_tiled_i32(dst, src_a, src_b, (int)M, (int)N, (int)K, tile_size);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_tiled_i64(int128_t *dst, const int64_t *src_a, const int64_t *src_b,
                                          int M, int N, int K, int tile_size)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (int i = 0; i < M * N; i++)
    {
        dst[i] = (int128_t){0};
    }

    hal_matrix_vmul_tiled_i64(dst, src_a, src_b, (int)M, (int)N, (int)K, tile_size);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_tiled_i128(int256_t *dst, const int128_t *src_a,
                                           const int128_t *src_b, int M, int N, int K,
                                           int tile_size)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (int i = 0; i < M * N; i++)
    {
        dst[i] = (int256_t){0};
    }

    hal_matrix_vmul_tiled_i128(dst, src_a, src_b, (int)M, (int)N, (int)K, tile_size);

    return VMATH_SUCCESS;
}

/* Tiled Matrix Multiplication (Unsigned) */
vmath_status_t vmath_matrix_mul_tiled_u8(uint16_t *dst, const uint8_t *src_a, const uint8_t *src_b,
                                         int M, int N, int K, int tile_size)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (int i = 0; i < M * N; i++)
    {
        dst[i] = 0;
    }

    hal_matrix_vmul_tiled_u8(dst, src_a, src_b, (int)M, (int)N, (int)K, tile_size);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_tiled_u16(uint32_t *dst, const uint16_t *src_a,
                                          const uint16_t *src_b, int M, int N, int K,
                                          int tile_size)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (int i = 0; i < M * N; i++)
    {
        dst[i] = 0;
    }

    hal_matrix_vmul_tiled_u16(dst, src_a, src_b, (int)M, (int)N, (int)K, tile_size);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_tiled_u32(uint64_t *dst, const uint32_t *src_a,
                                          const uint32_t *src_b, int M, int N, int K,
                                          int tile_size)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (int i = 0; i < M * N; i++)
    {
        dst[i] = 0;
    }

    hal_matrix_vmul_tiled_u32(dst, src_a, src_b, (int)M, (int)N, (int)K, tile_size);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_tiled_u64(uint128_t *dst, const uint64_t *src_a,
                                          const uint64_t *src_b, int M, int N, int K,
                                          int tile_size)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (int i = 0; i < M * N; i++)
    {
        dst[i] = (uint128_t){0};
    }

    hal_matrix_vmul_tiled_u64(dst, src_a, src_b, (int)M, (int)N, (int)K, tile_size);

    return VMATH_SUCCESS;
}

vmath_status_t vmath_matrix_mul_tiled_u128(uint256_t *dst, const uint128_t *src_a,
                                           const uint128_t *src_b, int M, int N, int K,
                                           int tile_size)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (int i = 0; i < M * N; i++)
    {
        dst[i] = (uint256_t){0};
    }

    hal_matrix_vmul_tiled_u128(dst, src_a, src_b, (int)M, (int)N, (int)K, tile_size);

    return VMATH_SUCCESS;
}

/* Tiled Matrix Multiplication (Float) */
vmath_status_t vmath_matrix_mul_tiled_f32(double *dst, const float *src_a, const float *src_b,
                                          int M, int N, int K, int tile_size)
{
    if (src_a == NULL || src_b == NULL || dst == NULL)
        return VMATH_ERR_NULL_PTR;
    if (M == 0 || N == 0 || K == 0)
        return VMATH_ERR_INVALID_SIZE;

    // 결과 행렬 dst 초기화 (누산을 위해 필요)
    for (int i = 0; i < M * N; i++)
    {
        dst[i] = 0.0;
    }

    hal_matrix_vmul_tiled_f32(dst, src_a, src_b, (int)M, (int)N, (int)K, tile_size);

    return VMATH_SUCCESS;
}
// --> tile end

/* ======================================================================== */
/* Static Functions (Internal implementations)                              */
/* ======================================================================== */
