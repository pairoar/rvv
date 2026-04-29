#include "hal_basic_math.h"
#include <stddef.h>
#include <string.h>

/*
    addition
*/
/* i8/u8 */
void hal_vadd_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

void hal_vadd_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

/* i16/u16 */
void hal_vadd_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

void hal_vadd_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

/* i32/u32 */
void hal_vadd_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

void hal_vadd_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

/* i64/u64 */
void hal_vadd_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

void hal_vadd_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

/* i128/u128 */
void hal_vadd_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = hal_add_i128(a[i], b[i]);
    }
}

void hal_vadd_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = hal_add_u128(a[i], b[i]);
    }
}

/*
    subtraction
*/
/* i8/u8 */
void hal_vsub_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

void hal_vsub_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

/* i16/u16 */
void hal_vsub_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

void hal_vsub_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

/* i32/u32 */
void hal_vsub_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

void hal_vsub_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

/* i64/u64 */
void hal_vsub_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

void hal_vsub_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

/* i128/u128 */
void hal_vsub_i128(int128_t *c, const int128_t *a, const int128_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = hal_sub_i128(a[i], b[i]);
    }
}

void hal_vsub_u128(uint128_t *c, const uint128_t *a, const uint128_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = hal_sub_u128(a[i], b[i]);
    }
}

/*
    multiplication
*/
/* i8/u8 */
void hal_vmul_i8(int16_t *c, const int8_t *a, const int8_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = (int16_t)a[i] * b[i];
    }
}

void hal_vmul_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = (uint16_t)a[i] * b[i];
    }
}

/* i16/u16 */
void hal_vmul_i16(int32_t *c, const int16_t *a, const int16_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = (int32_t)a[i] * b[i];
    }
}

void hal_vmul_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = (uint32_t)a[i] * b[i];
    }
}

/* i32/u32 */
void hal_vmul_i32(int64_t *c, const int32_t *a, const int32_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = (int64_t)a[i] * b[i];
    }
}

void hal_vmul_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = (uint64_t)a[i] * b[i];
    }
}

/* i64/u64 */
void hal_vmul_i64(int128_t *c, const int64_t *a, const int64_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = hal_mul_i64(a[i], b[i]);
    }
}

void hal_vmul_u64(uint128_t *c, const uint64_t *a, const uint64_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = hal_mul_u64(a[i], b[i]);
    }
}

/*
    multiplication and accumulation
*/
/* i8/u8 */
void hal_vmac_i8(int16_t *c, const int8_t *a, const int8_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] += (int16_t)a[i] * b[i];
    }
}

void hal_vmac_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] += (uint16_t)a[i] * b[i];
    }
}

/* i16/u16 */
void hal_vmac_i16(int32_t *c, const int16_t *a, const int16_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] += (int32_t)a[i] * b[i];
    }
}

void hal_vmac_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] += (uint32_t)a[i] * b[i];
    }
}

/* i32/u32 */
void hal_vmac_i32(int64_t *c, const int32_t *a, const int32_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] += (int64_t)a[i] * b[i];
    }
}

void hal_vmac_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] += (uint64_t)a[i] * b[i];
    }
}

/*
    division
*/
/* i8/u8 */
void hal_vdiv_i8(int8_t *c, const int8_t *a, const int8_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL)
        return;
    if (ret)
        *ret = HAL_MATH_SUCCESS;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            if (ret)
                *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

void hal_vdiv_u8(uint8_t *c, const uint8_t *a, const uint8_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL)
        return;
    if (ret)
        *ret = HAL_MATH_SUCCESS;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            if (ret)
                *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

/* i16/u16 */
void hal_vdiv_i16(int16_t *c, const int16_t *a, const int16_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL)
        return;
    if (ret)
        *ret = HAL_MATH_SUCCESS;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            if (ret)
                *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

void hal_vdiv_u16(uint16_t *c, const uint16_t *a, const uint16_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL)
        return;
    if (ret)
        *ret = HAL_MATH_SUCCESS;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            if (ret)
                *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

/* i32/u32 */
void hal_vdiv_i32(int32_t *c, const int32_t *a, const int32_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL)
        return;
    if (ret)
        *ret = HAL_MATH_SUCCESS;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            if (ret)
                *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

void hal_vdiv_u32(uint32_t *c, const uint32_t *a, const uint32_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL)
        return;
    if (ret)
        *ret = HAL_MATH_SUCCESS;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            if (ret)
                *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

/* i64/u64 */
void hal_vdiv_i64(int64_t *c, const int64_t *a, const int64_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL)
        return;
    if (ret)
        *ret = HAL_MATH_SUCCESS;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            if (ret)
                *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

void hal_vdiv_u64(uint64_t *c, const uint64_t *a, const uint64_t *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL)
        return;
    if (ret)
        *ret = HAL_MATH_SUCCESS;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0) {
            c[i] = 0;

            if (ret)
                *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

/*
    dot product
*/
/* i8/u8 */
void hal_vdot_i8(int16_t *result, const int8_t *a, const int8_t *b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL)
        return;
    int16_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (int16_t)a[i] * b[i]; // Prevent overflow using explicit casting
    }
    *result = sum;
}

void hal_vdot_u8(uint16_t *result, const uint8_t *a, const uint8_t *b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL)
        return;
    uint16_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (uint16_t)a[i] * b[i];
    }
    *result = sum;
}

/* i16/u16 */
void hal_vdot_i16(int32_t *result, const int16_t *a, const int16_t *b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL)
        return;
    int32_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (int32_t)a[i] * b[i];
    }
    *result = sum;
}

void hal_vdot_u16(uint32_t *result, const uint16_t *a, const uint16_t *b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL)
        return;
    uint32_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (uint32_t)a[i] * b[i];
    }
    *result = sum;
}

/* i32/u32 */
void hal_vdot_i32(int64_t *result, const int32_t *a, const int32_t *b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL)
        return;
    int64_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (int64_t)a[i] * b[i];
    }
    *result = sum;
}

void hal_vdot_u32(uint64_t *result, const uint32_t *a, const uint32_t *b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL)
        return;
    uint64_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (uint64_t)a[i] * b[i];
    }
    *result = sum;
}

/*
    matrix multiplication
*/
/* i8/u8 */
void hal_matrix_vmul_i8(int16_t *c, const int8_t *a, const int8_t *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    int16_t (*pc)[N] = (void *)c;
    int8_t (*pa)[K] = (void *)a;
    int8_t (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(int16_t) * M * N);

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
}

void hal_matrix_vmul_u8(uint16_t *c, const uint8_t *a, const uint8_t *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    uint16_t (*pc)[N] = (void *)c;
    uint8_t (*pa)[K] = (void *)a;
    uint8_t (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(uint16_t) * M * N);

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
}

/* i16/u16 */
void hal_matrix_vmul_i16(int32_t *c, const int16_t *a, const int16_t *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    int32_t (*pc)[N] = (void *)c;
    int16_t (*pa)[K] = (void *)a;
    int16_t (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(int32_t) * M * N);

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
}

void hal_matrix_vmul_u16(uint32_t *c, const uint16_t *a, const uint16_t *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    uint32_t (*pc)[N] = (void *)c;
    uint16_t (*pa)[K] = (void *)a;
    uint16_t (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(uint32_t) * M * N);

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
}

/* i32/u32 */
void hal_matrix_vmul_i32(int64_t *c, const int32_t *a, const int32_t *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    int64_t (*pc)[N] = (void *)c;
    int32_t (*pa)[K] = (void *)a;
    int32_t (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(int64_t) * M * N);

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
}

// a_row : M, a_col = K
// b_row : K , b_col = N
void hal_matrix_vmul_u32(uint64_t *c, const uint32_t *a, const uint32_t *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    uint64_t (*pc)[N] = (void *)c;
    uint32_t (*pa)[K] = (void *)a;
    uint32_t (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(uint64_t) * M * N);

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
}

/*
    float 32
*/
/*
    addition
*/
void hal_vadd_f32(float *c, const float *a, const float *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

/*
    subtraction
*/
void hal_vsub_f32(float *c, const float *a, const float *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] - b[i];
    }
}

/*
    multiplication
*/
void hal_vmul_f32(double *c, const float *a, const float *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = (double)a[i] * b[i];
    }
}

/*
    multiplication and accumulation
*/
void hal_vmac_f32(double *c, const float *a, const float *b, const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] += (double)a[i] * b[i];
    }
}

/*
    division
*/
void hal_vdiv_f32(float *c, const float *a, const float *b, const size_t n, int *ret) {
    if (a == NULL || b == NULL || c == NULL)
        return;

    if (ret)
        *ret = HAL_MATH_SUCCESS;

    for (size_t i = 0; i < n; i++) {
        if (b[i] == 0.0f) {
            c[i] = 0;

            if (ret)
                *ret |= HAL_MATH_ERR_DIV_BY_ZERO;
            continue;
        }
        c[i] = a[i] / b[i];
    }
}

/*
    dot product
*/
void hal_vdot_f32(double *result, const float *a, const float *b, const size_t n) {
    if (a == NULL || b == NULL || result == NULL)
        return;
    double sum = 0.0;

    for (size_t i = 0; i < n; i++) {
        if (a[i] == 0.0f || b[i] == 0.0f)
            continue;
        sum += (double)a[i] * b[i]; // Guaranteed Double Precision
    }
    *result = sum;
}

/*
    matrix multiplication
*/
void hal_matrix_vmul_f32(double *c, const float *a, const float *b, int M, int N, int K) {
    // Exception Handling
    if (a == NULL || b == NULL) {
        return;
    }

    // VLA mapping
    double (*pc)[N] = (void *)c;
    float (*pa)[K] = (void *)a;
    float (*pb)[N] = (void *)b;
    memset(c, 0, sizeof(double) * M * N);

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
}
