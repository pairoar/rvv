#include "hal_basic_math.h"
#include <stddef.h>
#include <string.h>

/*
    addition
*/
/* i8/u8 */
hal_status_t hal_vadd_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

OUT:
    return ret;
}

hal_status_t hal_vadd_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

OUT:
    return ret;
}

/* i16/u16 */
hal_status_t hal_vadd_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

OUT:
    return ret;
}

hal_status_t hal_vadd_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

OUT:
    return ret;
}

/* i32/u32 */
hal_status_t hal_vadd_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

OUT:
    return ret;
}

hal_status_t hal_vadd_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

OUT:
    return ret;
}

/* i64/u64 */
hal_status_t hal_vadd_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

OUT:
    return ret;
}

hal_status_t hal_vadd_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

OUT:
    return ret;
}

/* i128/u128 */
hal_status_t hal_vadd_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = hal_add_i128(a[i], b[i]);
    }

OUT:
    return ret;
}

hal_status_t hal_vadd_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = hal_add_u128(a[i], b[i]);
    }

OUT:
    return ret;
}

/*
    subtraction
*/
/* i8/u8 */
hal_status_t hal_vsub_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

OUT:
    return ret;
}

hal_status_t hal_vsub_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

OUT:
    return ret;
}

/* i16/u16 */
hal_status_t hal_vsub_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

OUT:
    return ret;
}

hal_status_t hal_vsub_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

OUT:
    return ret;
}

/* i32/u32 */
hal_status_t hal_vsub_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

OUT:
    return ret;
}

hal_status_t hal_vsub_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

OUT:
    return ret;
}

/* i64/u64 */
hal_status_t hal_vsub_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

OUT:
    return ret;
}

hal_status_t hal_vsub_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

OUT:
    return ret;
}

/* i128/u128 */
hal_status_t hal_vsub_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = hal_sub_i128(a[i], b[i]);
    }

OUT:
    return ret;
}

hal_status_t hal_vsub_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = hal_sub_u128(a[i], b[i]);
    }

OUT:
    return ret;
}

/*
    multiplication
*/
/* i8/u8 */
hal_status_t hal_vmul_i8(int16_t *c, const int8_t *a, const int8_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = (int16_t)a[i] * b[i];
    }

OUT:
    return ret;
}

hal_status_t hal_vmul_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = (uint16_t)a[i] * b[i];
    }

OUT:
    return ret;
}

/* i16/u16 */
hal_status_t hal_vmul_i16(int32_t *c, const int16_t *a, const int16_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = (int32_t)a[i] * b[i];
    }

OUT:
    return ret;
}

hal_status_t hal_vmul_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = (uint32_t)a[i] * b[i];
    }

OUT:
    return ret;
}

/* i32/u32 */
hal_status_t hal_vmul_i32(int64_t *c, const int32_t *a, const int32_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = (int64_t)a[i] * b[i];
    }

OUT:
    return ret;
}

hal_status_t hal_vmul_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = (uint64_t)a[i] * b[i];
    }

OUT:
    return ret;
}

/* i64/u64 */
hal_status_t hal_vmul_i64(int128_t *c, const int64_t *a, const int64_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = hal_mul_i64(a[i], b[i]);
    }

OUT:
    return ret;
}

hal_status_t hal_vmul_u64(uint128_t *c, const uint64_t *a, const uint64_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = hal_mul_u64(a[i], b[i]);
    }

OUT:
    return ret;
}

/*
    multiplication and accumulation
*/
/* i8/u8 */
hal_status_t hal_vmac_i8(int16_t *c, const int8_t *a, const int8_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] += (int16_t)a[i] * b[i];
    }

OUT:
    return ret;
}

hal_status_t hal_vmac_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] += (uint16_t)a[i] * b[i];
    }

OUT:
    return ret;
}

/* i16/u16 */
hal_status_t hal_vmac_i16(int32_t *c, const int16_t *a, const int16_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] += (int32_t)a[i] * b[i];
    }

OUT:
    return ret;
}

hal_status_t hal_vmac_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] += (uint32_t)a[i] * b[i];
    }

OUT:
    return ret;
}

/* i32/u32 */
hal_status_t hal_vmac_i32(int64_t *c, const int32_t *a, const int32_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] += (int64_t)a[i] * b[i];
    }

OUT:
    return ret;
}

hal_status_t hal_vmac_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] += (uint64_t)a[i] * b[i];
    }

OUT:
    return ret;
}

/*
    division
*/
/* i8/u8 */
hal_status_t hal_vdiv_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
OUT:
    return ret;
}

hal_status_t hal_vdiv_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
OUT:
    return ret;
}

/* i16/u16 */
hal_status_t hal_vdiv_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }
    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
OUT:
    return ret;
}

hal_status_t hal_vdiv_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
OUT:
    return ret;
}

/* i32/u32 */
hal_status_t hal_vdiv_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
OUT:
    return ret;
}

hal_status_t hal_vdiv_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;
    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
OUT:
    return ret;
}

/* i64/u64 */
hal_status_t hal_vdiv_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
OUT:
    return ret;
}

hal_status_t hal_vdiv_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        ret = HAL_OK;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
OUT:
    return ret;
}

/*
    dot product
*/
/* i8/u8 */
hal_status_t hal_vdot_i8(int16_t *result, const int8_t *a, const int8_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || result == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        *result = 0;
        ret = HAL_OK;
        goto OUT;
    }

    int16_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (int16_t)a[i] * b[i]; // Prevent overflow using explicit casting
    }
    *result = sum;

OUT:
    return ret;
}

hal_status_t hal_vdot_u8(uint16_t *result, const uint8_t *a, const uint8_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || result == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        *result = 0;
        ret = HAL_OK;
        goto OUT;
    }

    uint16_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (uint16_t)a[i] * b[i];
    }
    *result = sum;

OUT:
    return ret;
}

/* i16/u16 */
hal_status_t hal_vdot_i16(int32_t *result, const int16_t *a, const int16_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || result == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        *result = 0;
        ret = HAL_OK;
        goto OUT;
    }

    int32_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (int32_t)a[i] * b[i];
    }
    *result = sum;

OUT:
    return ret;
}

hal_status_t hal_vdot_u16(uint32_t *result, const uint16_t *a, const uint16_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || result == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        *result = 0;
        ret = HAL_OK;
        goto OUT;
    }

    uint32_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (uint32_t)a[i] * b[i];
    }
    *result = sum;

OUT:
    return ret;
}

/* i32/u32 */
hal_status_t hal_vdot_i32(int64_t *result, const int32_t *a, const int32_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || result == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        *result = 0;
        ret = HAL_OK;
        goto OUT;
    }

    int64_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (int64_t)a[i] * b[i];
    }
    *result = sum;

OUT:
    return ret;
}

hal_status_t hal_vdot_u32(uint64_t *result, const uint32_t *a, const uint32_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || result == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (n == 0) {
        *result = 0;
        ret = HAL_OK;
        goto OUT;
    }

    uint64_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (uint64_t)a[i] * b[i];
    }
    *result = sum;

OUT:
    return ret;
}

/*
    matrix multiplication
*/
/* i8/u8 */
hal_status_t hal_matrix_vmul_i8(int16_t *c, const int8_t *a, const int8_t *b, int M, int N, int K) {
    hal_status_t ret = HAL_OK;

    // VLA mapping
    int16_t (*pc)[N] = (void *)c;
    int8_t (*pa)[K] = (void *)a;
    int8_t (*pb)[N] = (void *)b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    memset(c, 0, sizeof(int16_t) * M * N);

    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // 3. Operations performed in the order i-k-j (Cache Optimization and Sparse Support)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    pc[i][j] += (int16_t)pa[i][k] * pb[k][j];
                }
            }
        }
    }

OUT:
    return ret;
}

hal_status_t hal_matrix_vmul_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, int M, int N,
                                int K) {
    hal_status_t ret = HAL_OK;

    // VLA mapping
    uint16_t (*pc)[N] = (void *)c;
    uint8_t (*pa)[K] = (void *)a;
    uint8_t (*pb)[N] = (void *)b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    memset(c, 0, sizeof(uint16_t) * M * N);

    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // 3. Operations performed in the order i-k-j (Cache Optimization and Sparse Support)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    pc[i][j] += (uint16_t)pa[i][k] * pb[k][j];
                }
            }
        }
    }

OUT:
    return ret;
}

/* i16/u16 */
hal_status_t hal_matrix_vmul_i16(int32_t *c, const int16_t *a, const int16_t *b, int M, int N,
                                 int K) {
    hal_status_t ret = HAL_OK;

    // VLA mapping
    int32_t (*pc)[N] = (void *)c;
    int16_t (*pa)[K] = (void *)a;
    int16_t (*pb)[N] = (void *)b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    memset(c, 0, sizeof(int32_t) * M * N);

    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // 3. Operations performed in the order i-k-j (Cache Optimization and Sparse Support)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    pc[i][j] += (int32_t)pa[i][k] * pb[k][j];
                }
            }
        }
    }

OUT:
    return ret;
}

hal_status_t hal_matrix_vmul_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, int M, int N,
                                 int K) {
    hal_status_t ret = HAL_OK;

    // VLA mapping
    uint32_t (*pc)[N] = (void *)c;
    uint16_t (*pa)[K] = (void *)a;
    uint16_t (*pb)[N] = (void *)b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    memset(c, 0, sizeof(uint32_t) * M * N);

    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // 3. Operations performed in the order i-k-j (Cache Optimization and Sparse Support)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    pc[i][j] += (uint32_t)pa[i][k] * pb[k][j];
                }
            }
        }
    }

OUT:
    return ret;
}

/* i32/u32 */
hal_status_t hal_matrix_vmul_i32(int64_t *c, const int32_t *a, const int32_t *b, int M, int N,
                                 int K) {
    hal_status_t ret = HAL_OK;

    // VLA mapping
    int64_t (*pc)[N] = (void *)c;
    int32_t (*pa)[K] = (void *)a;
    int32_t (*pb)[N] = (void *)b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }
    memset(c, 0, sizeof(int64_t) * M * N);
    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // 3. Operations performed in the order i-k-j (Cache Optimization and Sparse Support)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    pc[i][j] += (int64_t)pa[i][k] * pb[k][j];
                }
            }
        }
    }

OUT:
    return ret;
}

// a_row : M, a_col = K
// b_row : K , b_col = N
hal_status_t hal_matrix_vmul_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, int M, int N,
                                 int K) {
    hal_status_t ret = HAL_OK;

    // VLA mapping
    uint64_t (*pc)[N] = (void *)c;
    uint32_t (*pa)[K] = (void *)a;
    uint32_t (*pb)[N] = (void *)b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    memset(c, 0, sizeof(uint64_t) * M * N);

    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // 3. Operations performed in the order i-k-j (Cache Optimization and Sparse Support)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0) {
                for (int j = 0; j < N; j++) {
                    pc[i][j] += (uint64_t)pa[i][k] * pb[k][j];
                }
            }
        }
    }

OUT:
    return ret;
}

/*
    float 32
*/
/*
    addition
*/
hal_status_t hal_vadd_f32(float *c, const float *a, const float *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

OUT:
    return ret;
}

/*
    subtraction
*/
hal_status_t hal_vsub_f32(float *c, const float *a, const float *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

OUT:
    return ret;
}

/*
    multiplication
*/
hal_status_t hal_vmul_f32(double *c, const float *a, const float *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] = (double)a[i] * b[i];
    }

OUT:
    return ret;
}

/*
    multiplication and accumulation
*/
hal_status_t hal_vmac_f32(double *c, const float *a, const float *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        c[i] += (double)a[i] * b[i];
    }

OUT:
    return ret;
}

/*
    division
*/
hal_status_t hal_vdiv_f32(float *c, const float *a, const float *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0.0f) {
            c[i] = 0;

            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
OUT:
    return ret;
}

/*
    dot product
*/
hal_status_t hal_vdot_f32(double *result, const float *a, const float *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    if (a == NULL || b == NULL || result == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }

    double sum = 0.0;

    for (size_t i = 0; i < n; i++) {
        if (a[i] == 0.0f || b[i] == 0.0f)
            continue;
        sum += (double)a[i] * b[i]; // Guaranteed Double Precision
    }
    *result = sum;

OUT:
    return ret;
}

/*
    matrix multiplication
*/
hal_status_t hal_matrix_vmul_f32(double *c, const float *a, const float *b, int M, int N, int K) {
    hal_status_t ret = HAL_OK;

    // VLA mapping
    double (*pc)[N] = (void *)c;
    float (*pa)[K] = (void *)a;
    float (*pb)[N] = (void *)b;

    if (a == NULL || b == NULL || c == NULL) {
        ret = HAL_ERR_NULL_PTR;
        goto OUT;
    }
    if (M <= 0 || N <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    memset(c, 0, sizeof(double) * M * N);

    if (K <= 0) {
        ret = HAL_OK;
        goto OUT;
    }

    // 3. Operations performed in the order i-k-j (Cache Optimization and Sparse Support)
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            if (pa[i][k] != 0.0f) {
                for (int j = 0; j < N; j++) {
                    pc[i][j] += (double)pa[i][k] * pb[k][j];
                }
            }
        }
    }

OUT:
    return ret;
}
