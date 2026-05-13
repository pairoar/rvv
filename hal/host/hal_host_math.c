#include "hal_internal_math.h"
#include <stddef.h>
#include <string.h>
#include <math.h>

/*
    addition
*/
/* i8/u8 */
hal_status_t hal_vadd_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
    return HAL_OK;
}

hal_status_t hal_vadd_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

    return HAL_OK;
}

/* i16/u16 */
hal_status_t hal_vadd_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

    return HAL_OK;
}

hal_status_t hal_vadd_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

    return HAL_OK;
}

/* i32/u32 */
hal_status_t hal_vadd_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

    return HAL_OK;
}

hal_status_t hal_vadd_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

    return HAL_OK;
}

/* i64/u64 */
hal_status_t hal_vadd_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

    return HAL_OK;
}

hal_status_t hal_vadd_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

    return HAL_OK;
}

/* i128/u128 */
hal_status_t hal_vadd_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = hal_add_i128(a[i], b[i]);
    }

    return HAL_OK;
}

hal_status_t hal_vadd_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = hal_add_u128(a[i], b[i]);
    }

    return HAL_OK;
}

/*
    subtraction
*/
/* i8/u8 */
hal_status_t hal_vsub_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

    return HAL_OK;
}

hal_status_t hal_vsub_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

    return HAL_OK;
}

/* i16/u16 */
hal_status_t hal_vsub_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

    return HAL_OK;
}

hal_status_t hal_vsub_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

    return HAL_OK;
}

/* i32/u32 */
hal_status_t hal_vsub_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

    return HAL_OK;
}

hal_status_t hal_vsub_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

    return HAL_OK;
}

/* i64/u64 */
hal_status_t hal_vsub_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

    return HAL_OK;
}

hal_status_t hal_vsub_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

    return HAL_OK;
}

/* i128/u128 */
hal_status_t hal_vsub_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = hal_sub_i128(a[i], b[i]);
    }

    return HAL_OK;
}

hal_status_t hal_vsub_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = hal_sub_u128(a[i], b[i]);
    }

    return HAL_OK;
}

/*
    multiplication
*/
/* i8/u8 */
hal_status_t hal_vmul_i8(int16_t *c, const int8_t *a, const int8_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = (int16_t)a[i] * b[i];
    }

    return HAL_OK;
}

hal_status_t hal_vmul_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = (uint16_t)a[i] * b[i];
    }

    return HAL_OK;
}

/* i16/u16 */
hal_status_t hal_vmul_i16(int32_t *c, const int16_t *a, const int16_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = (int32_t)a[i] * b[i];
    }

    return HAL_OK;
}

hal_status_t hal_vmul_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = (uint32_t)a[i] * b[i];
    }

    return HAL_OK;
}

/* i32/u32 */
hal_status_t hal_vmul_i32(int64_t *c, const int32_t *a, const int32_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = (int64_t)a[i] * b[i];
    }

    return HAL_OK;
}

hal_status_t hal_vmul_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = (uint64_t)a[i] * b[i];
    }

    return HAL_OK;
}

/* i64/u64 */
hal_status_t hal_vmul_i64(int128_t *c, const int64_t *a, const int64_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = hal_mul_i64(a[i], b[i]);
    }

    return HAL_OK;
}

hal_status_t hal_vmul_u64(uint128_t *c, const uint64_t *a, const uint64_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = hal_mul_u64(a[i], b[i]);
    }

    return HAL_OK;
}

/*
    multiplication and accumulation
*/
/* i8/u8 */
hal_status_t hal_vmac_i8(int16_t *c, const int8_t *a, const int8_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] += (int16_t)a[i] * b[i];
    }

    return HAL_OK;
}

hal_status_t hal_vmac_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] += (uint16_t)a[i] * b[i];
    }

    return HAL_OK;
}

/* i16/u16 */
hal_status_t hal_vmac_i16(int32_t *c, const int16_t *a, const int16_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] += (int32_t)a[i] * b[i];
    }

    return HAL_OK;
}

hal_status_t hal_vmac_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] += (uint32_t)a[i] * b[i];
    }

    return HAL_OK;
}

/* i32/u32 */
hal_status_t hal_vmac_i32(int64_t *c, const int32_t *a, const int32_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] += (int64_t)a[i] * b[i];
    }

    return HAL_OK;
}

hal_status_t hal_vmac_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] += (uint64_t)a[i] * b[i];
    }

    return HAL_OK;
}

/*
    division
*/
/* i8/u8 */
hal_status_t hal_vdiv_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;
            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
    return ret;
}

hal_status_t hal_vdiv_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;
            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
    return ret;
}

/* i16/u16 */
hal_status_t hal_vdiv_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;
            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
    return ret;
}

hal_status_t hal_vdiv_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;
            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
    return ret;
}

/* i32/u32 */
hal_status_t hal_vdiv_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;
            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
    return ret;
}

hal_status_t hal_vdiv_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;
            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
    return ret;
}

/* i64/u64 */
hal_status_t hal_vdiv_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;
            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
    return ret;
}

hal_status_t hal_vdiv_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;
            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
    return ret;
}

/*
    dot product
*/
/* i8/u8 */
hal_status_t hal_vdot_i8(int16_t *result, const int8_t *a, const int8_t *b, const size_t n) {
    int16_t sum = 0;

    for (size_t i = 0; i < n; i++) {
        sum += (int16_t)a[i] * b[i]; // Prevent overflow using explicit casting
    }
    *result = sum;
    return HAL_OK;
}

hal_status_t hal_vdot_u8(uint16_t *result, const uint8_t *a, const uint8_t *b, const size_t n) {
    uint16_t sum = 0;

    for (size_t i = 0; i < n; i++) {
        sum += (uint16_t)a[i] * b[i];
    }
    *result = sum;

    return HAL_OK;
}

/* i16/u16 */
hal_status_t hal_vdot_i16(int32_t *result, const int16_t *a, const int16_t *b, const size_t n) {
    int32_t sum = 0;

    for (size_t i = 0; i < n; i++) {
        sum += (int32_t)a[i] * b[i];
    }
    *result = sum;

    return HAL_OK;
}

hal_status_t hal_vdot_u16(uint32_t *result, const uint16_t *a, const uint16_t *b, const size_t n) {
    uint32_t sum = 0;

    for (size_t i = 0; i < n; i++) {
        sum += (uint32_t)a[i] * b[i];
    }
    *result = sum;

    return HAL_OK;
}

/* i32/u32 */
hal_status_t hal_vdot_i32(int64_t *result, const int32_t *a, const int32_t *b, const size_t n) {
    int64_t sum = 0;

    for (size_t i = 0; i < n; i++) {
        sum += (int64_t)a[i] * b[i];
    }
    *result = sum;

    return HAL_OK;
}

hal_status_t hal_vdot_u32(uint64_t *result, const uint32_t *a, const uint32_t *b, const size_t n) {
    uint64_t sum = 0;

    for (size_t i = 0; i < n; i++) {
        sum += (uint64_t)a[i] * b[i];
    }
    *result = sum;

    return HAL_OK;
}

/*
    matrix multiplication
*/
/* i8/u8 */
hal_status_t hal_matrix_vmul_i8(int16_t *c, const int8_t *a, const int8_t *b, int M, int N, int K) {
    // VLA mapping
    int16_t (*pc)[N] = (void *)c;
    int8_t (*pa)[K] = (void *)a;
    int8_t (*pb)[N] = (void *)b;

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

    return HAL_OK;
}

hal_status_t hal_matrix_vmul_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, int M, int N,
                                int K) {
    // VLA mapping
    uint16_t (*pc)[N] = (void *)c;
    uint8_t (*pa)[K] = (void *)a;
    uint8_t (*pb)[N] = (void *)b;

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

    return HAL_OK;
}

/* i16/u16 */
hal_status_t hal_matrix_vmul_i16(int32_t *c, const int16_t *a, const int16_t *b, int M, int N,
                                 int K) {
    // VLA mapping
    int32_t (*pc)[N] = (void *)c;
    int16_t (*pa)[K] = (void *)a;
    int16_t (*pb)[N] = (void *)b;

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

    return HAL_OK;
}

hal_status_t hal_matrix_vmul_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, int M, int N,
                                 int K) {
    // VLA mapping
    uint32_t (*pc)[N] = (void *)c;
    uint16_t (*pa)[K] = (void *)a;
    uint16_t (*pb)[N] = (void *)b;

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

    return HAL_OK;
}

/* i32/u32 */
hal_status_t hal_matrix_vmul_i32(int64_t *c, const int32_t *a, const int32_t *b, int M, int N,
                                 int K) {
    // VLA mapping
    int64_t (*pc)[N] = (void *)c;
    int32_t (*pa)[K] = (void *)a;
    int32_t (*pb)[N] = (void *)b;

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

    return HAL_OK;
}

// a_row : M, a_col = K
// b_row : K , b_col = N
hal_status_t hal_matrix_vmul_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, int M, int N,
                                 int K) {
    // VLA mapping
    uint64_t (*pc)[N] = (void *)c;
    uint32_t (*pa)[K] = (void *)a;
    uint32_t (*pb)[N] = (void *)b;

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

    return HAL_OK;
}

/*
    float 32
*/
/*
    addition
*/
hal_status_t hal_vadd_f32(float *c, const float *a, const float *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }

    return HAL_OK;
}

/*
    subtraction
*/
hal_status_t hal_vsub_f32(float *c, const float *a, const float *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }

    return HAL_OK;
}

/*
    multiplication
*/
hal_status_t hal_vmul_f32(double *c, const float *a, const float *b, const size_t n) {
    
    for (size_t i = 0; i < n; i++) {
        c[i] = (double)a[i] * b[i];
    }

    return HAL_OK;
}

/*
    multiplication and accumulation
*/
hal_status_t hal_vmac_f32(double *c, const float *a, const float *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] += (double)a[i] * b[i];
    }

    return HAL_OK;
}

/*
    division
*/
hal_status_t hal_vdiv_f32(float *c, const float *a, const float *b, const size_t n) {
    hal_status_t ret = HAL_OK;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0.0f) {
            c[i] = 0;
            ret |= HAL_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
    return ret;
}

/*
    dot product
*/
hal_status_t hal_vdot_f32(double *result, const float *a, const float *b, const size_t n) {
    double sum = 0.0;

    for (size_t i = 0; i < n; i++) {
        if (a[i] == 0.0f || b[i] == 0.0f)
            continue;
        sum += (double)a[i] * b[i]; // Guaranteed Double Precision
    }
    *result = sum;

    return HAL_OK;
}

/*
    matrix multiplication
*/
hal_status_t hal_matrix_vmul_f32(double *c, const float *a, const float *b, int M, int N, int K) {
    // VLA mapping
    double (*pc)[N] = (void *)c;
    float (*pa)[K] = (void *)a;
    float (*pb)[N] = (void *)b;

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

    return HAL_OK;
}



/*
    min/max
*/
#if 0   // proto type function
hal_status_t hal_vmin_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {

    for (size_t i=0; i < n; i++) {
        c[i] = MIN(a[i], b[i]);
    }
    return HAL_OK;
}

#endif

/*
    OP_N    : operation name
    OP_T    : operation type
    OP_FUNC : function name
    T_IN    : input type
    T_OUT   : output type(result)
    N       : length
    c       : output
    a       : input array1
    b       : input array2

    hal_status_t hal_vmin_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n)
*/

// hal_status_t hal_vmin_f32(float *c, const float *a, const float *b, const size_t n);
// DEFINE_HAL_GENERAL_OPS(vmin, f32, MIN, float, double)
//              hal_vmin_f32()
#define DEFINE_HAL_GENERAL_OPS(OP_N, OP_T, OP_FUNC, T_IN, T_OUT)                    \
    hal_status_t hal_##OP_N##_##OP_T(T_OUT *c, const T_IN *a, const T_IN *b, const size_t n) { \
        for (size_t i = 0; i < n; i++) {                                            \
            c[i] = OP_FUNC(a[i], b[i]);                                             \
        }                                                                           \
        return HAL_OK;                                                              \
    }

// -----------------------------------------------------------------------------
// Generate functions at once using macros!
// -----------------------------------------------------------------------------
// vmin (uint8_t) -> hal_vmin_u8
DEFINE_HAL_GENERAL_OPS(vmin, u8, MIN, uint8_t, uint8_t)
DEFINE_HAL_GENERAL_OPS(vmin, i8, MIN, int8_t, int8_t)
DEFINE_HAL_GENERAL_OPS(vmin, u16, MIN, uint16_t, uint16_t)
DEFINE_HAL_GENERAL_OPS(vmin, i16, MIN, int16_t, int16_t)
DEFINE_HAL_GENERAL_OPS(vmin, u32, MIN, uint32_t, uint32_t)
DEFINE_HAL_GENERAL_OPS(vmin, i32, MIN, int32_t, int32_t)
DEFINE_HAL_GENERAL_OPS(vmin, u64, MIN, uint64_t, uint64_t)
DEFINE_HAL_GENERAL_OPS(vmin, i64, MIN, int64_t, int64_t)
DEFINE_HAL_GENERAL_OPS(vmin, f32, MIN, float, float)

// vmax (uint8_t) -> hal_vmax_u8
DEFINE_HAL_GENERAL_OPS(vmax, u8, MAX, uint8_t, uint8_t)
DEFINE_HAL_GENERAL_OPS(vmax, i8, MAX, int8_t, int8_t)
DEFINE_HAL_GENERAL_OPS(vmax, u16, MAX, uint16_t, uint16_t)
DEFINE_HAL_GENERAL_OPS(vmax, i16, MAX, int16_t, int16_t)
DEFINE_HAL_GENERAL_OPS(vmax, u32, MAX, uint32_t, uint32_t)
DEFINE_HAL_GENERAL_OPS(vmax, i32, MAX, int32_t, int32_t)
DEFINE_HAL_GENERAL_OPS(vmax, u64, MAX, uint64_t, uint64_t)
DEFINE_HAL_GENERAL_OPS(vmax, i64, MAX, int64_t, int64_t)
DEFINE_HAL_GENERAL_OPS(vmax, f32, MAX, float, float)


hal_status_t hal_vmin_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = hal_min_u128(a[i], b[i]);
    }

    return HAL_OK;
}

hal_status_t hal_vmin_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = hal_min_i128(a[i], b[i]);
    }

    return HAL_OK;
}

hal_status_t hal_vmax_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = hal_max_u128(a[i], b[i]);
    }

    return HAL_OK;
}

hal_status_t hal_vmax_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = hal_max_i128(a[i], b[i]);
    }

    return HAL_OK;
}

#if 0   // proto type function
hal_status_t hal_vmin_f32(float *b, const float *a, const size_t n) {
    for (size_t i=0; i < n; i++) {
        b[i] = sqrtf(a[i]);
    }
    return HAL_OK;
}
#endif

#define DEFINE_HAL_GENERAL_OPS2(OP_N, OP_T, OP_FUNC, T_IN, T_OUT)                   \
    hal_status_t hal_##OP_N##_##OP_T(T_OUT *b, const T_IN *a, const size_t n) {     \
        for (size_t i = 0; i < n; i++) {                                            \
            b[i] = OP_FUNC(a[i]);                                                   \
        }                                                                           \
        return HAL_OK;                                                              \
    }

// -----------------------------------------------------------------------------
// Generate functions at once using macros!
// -----------------------------------------------------------------------------
// sqrt(float) -> hal_vsqrt_f32
DEFINE_HAL_GENERAL_OPS2(vsqrt, f32, sqrtf, float, float)

// rsqrt7(float) -> hal_vrsqrt7_f32
DEFINE_HAL_GENERAL_OPS2(vrsqrt7, f32, hal_rsqrt7_f32, float, float)
