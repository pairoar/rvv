#include "hal_soft_math.h"
#include "hal_internal_math.h"
#include <string.h>


#define HAL_FALLBACK __attribute__((weak))


// -----------------------------------------------------------------------------
// HAL Public C API
// -----------------------------------------------------------------------------
// Pure C O(N^3) matrix multiplication for benchmarking
HAL_FALLBACK hal_status_t hal_matrix_vmul_c_f32(double *c, const float *a, const float *b, int M, int N, int K) {
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
    return HAL_OK;
}

// -----------------------------------------------------------------------------
// HAL Public API: 128/256-bit Software Emulation Wrappers
// -----------------------------------------------------------------------------

/* i128/u128 */
HAL_FALLBACK hal_status_t hal_vmul_i128(int256_t *c, const int128_t *a, const int128_t *b,
                                        const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = hal_mul_i128(a[i], b[i]);
    }
    return HAL_OK;
}

HAL_FALLBACK hal_status_t hal_vmul_u128(uint256_t *c, const uint128_t *a, const uint128_t *b,
                                        const size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = hal_mul_u128(a[i], b[i]);
    }
return HAL_OK;
}

/* i128/u128 */
HAL_FALLBACK hal_status_t hal_vdiv_i128(int128_t *c, const int128_t *a, const int128_t *b,
                                        const size_t n) {
    hal_status_t ret = HAL_OK;
    
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

    return ret;
}

HAL_FALLBACK hal_status_t hal_vdiv_u128(uint128_t *c, const uint128_t *a, const uint128_t *b,
                                        const size_t n) {
    hal_status_t ret = HAL_OK;

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

    return ret;
}

/*
    matrix multiplication with tiled
*/
/* i8/u8 */
HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_i8(int16_t *c, const int8_t *a, const int8_t *b,
                                                   int M, int N, int K, int tile_size) {
    // VLA Mapping
    int16_t (*pc)[N] = (int16_t (*)[N])c;
    const int8_t (*pa)[K] = (const int8_t (*)[K])a;
    const int8_t (*pb)[N] = (const int8_t (*)[N])b;

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
    return HAL_OK;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_u8(uint16_t *c, const uint8_t *a, const uint8_t *b,
                                                   int M, int N, int K, int tile_size) {
    // VLA Mapping
    uint16_t (*pc)[N] = (uint16_t (*)[N])c;
    const uint8_t (*pa)[K] = (const uint8_t (*)[K])a;
    const uint8_t (*pb)[N] = (const uint8_t (*)[N])b;

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
    return HAL_OK;
}

/* i16/u16 */
HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_i16(int32_t *c, const int16_t *a, const int16_t *b,
                                                    int M, int N, int K, int tile_size) {
    // VLA Mapping
    int32_t (*pc)[N] = (int32_t (*)[N])c;
    const int16_t (*pa)[K] = (const int16_t (*)[K])a;
    const int16_t (*pb)[N] = (const int16_t (*)[N])b;

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
    return HAL_OK;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_u16(uint32_t *c, const uint16_t *a,
                                                    const uint16_t *b, int M, int N, int K,
                                                    int tile_size) {
    // VLA Mapping
    uint32_t (*pc)[N] = (uint32_t (*)[N])c;
    const uint16_t (*pa)[K] = (const uint16_t (*)[K])a;
    const uint16_t (*pb)[N] = (const uint16_t (*)[N])b;

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
    return HAL_OK;
}

/* i32/u32 */
HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_i32(int64_t *c, const int32_t *a, const int32_t *b,
                                                    int M, int N, int K, int tile_size) {
    // VLA Mapping
    int64_t (*pc)[N] = (int64_t (*)[N])c;
    const int32_t (*pa)[K] = (const int32_t (*)[K])a;
    const int32_t (*pb)[N] = (const int32_t (*)[N])b;

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
    return HAL_OK;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_u32(uint64_t *c, const uint32_t *a,
                                                    const uint32_t *b, int M, int N, int K,
                                                    int tile_size) {

    // VLA Mapping
    uint64_t (*pc)[N] = (uint64_t (*)[N])c;
    const uint32_t (*pa)[K] = (const uint32_t (*)[K])a;
    const uint32_t (*pb)[N] = (const uint32_t (*)[N])b;

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
    return HAL_OK;
}

/* 64 */
HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_i64(int128_t *c, const int64_t *a, const int64_t *b,
                                                    int M, int N, int K, int tile_size) {
    int128_t(*pc)[N] = (int128_t(*)[N])c;
    const int64_t (*pa)[K] = (const int64_t (*)[K])a;
    const int64_t (*pb)[N] = (const int64_t (*)[N])b;

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
    return HAL_OK;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_u64(uint128_t *c, const uint64_t *a,
                                                    const uint64_t *b, int M, int N, int K,
                                                    int tile_size) {
    uint128_t(*pc)[N] = (uint128_t(*)[N])c;
    const uint64_t (*pa)[K] = (const uint64_t (*)[K])a;
    const uint64_t (*pb)[N] = (const uint64_t (*)[N])b;

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
    return HAL_OK;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_i128(int256_t *c, const int128_t *a,
                                                     const int128_t *b, int M, int N, int K,
                                                     int tile_size) {
    int256_t(*pc)[N] = (int256_t(*)[N])c;
    const int128_t(*pa)[K] = (const int128_t(*)[K])a;
    const int128_t(*pb)[N] = (const int128_t(*)[N])b;

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
    return HAL_OK;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_u128(uint256_t *c, const uint128_t *a,
                                                     const uint128_t *b, int M, int N, int K,
                                                     int tile_size) {
    uint256_t(*pc)[N] = (uint256_t(*)[N])c;
    const uint128_t(*pa)[K] = (const uint128_t(*)[K])a;
    const uint128_t(*pb)[N] = (const uint128_t(*)[N])b;

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
    return HAL_OK;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_tiled_f32(double *c, const float *a, const float *b,
                                                    int M, int N, int K, int tile_size) {

    // VLA Mapping
    double (*pc)[N] = (double (*)[N])c;
    const float (*pa)[K] = (const float (*)[K])a;
    const float (*pb)[N] = (const float (*)[N])b;

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
    return HAL_OK;
}

/* i64/u64 */
HAL_FALLBACK hal_status_t hal_vdot_i64(int128_t *result, const int64_t *a, const int64_t *b,
                                       const size_t n) {
    int128_t sum = {0, 0};
    for (size_t i = 0; i < n; i++) {
        if (a[i] == 0 || b[i] == 0)
            continue; // 0-Skip optimization
        int128_t prod = hal_mul_i64(a[i], b[i]);
        sum = hal_add_i128(sum, prod);
    }
    *result = sum;

    return HAL_OK;
}

HAL_FALLBACK hal_status_t hal_vdot_u64(uint128_t *result, const uint64_t *a, const uint64_t *b,
                                       const size_t n) {
    uint128_t sum = {0, 0};

    for (size_t i = 0; i < n; i++) {
        if (a[i] == 0 || b[i] == 0)
            continue;
        uint128_t prod = hal_mul_u64(a[i], b[i]);
        sum = hal_add_u128(sum, prod);
    }
    *result = sum;
    return HAL_OK;
}

/* i128/u128 */
HAL_FALLBACK hal_status_t hal_vdot_i128(int256_t *result, const int128_t *a, const int128_t *b,
                                        const size_t n) {
    int256_t sum = {0};

    for (size_t i = 0; i < n; i++) {
        // Struct 0-Skip optimization (if either is 0, the multiplication result is 0)
        if ((a[i].l == 0 && a[i].u == 0) || (b[i].l == 0 && b[i].u == 0))
            continue;

        int256_t prod = hal_mul_i128(a[i], b[i]);
        sum = hal_add_i256(sum, prod);
    }
    *result = sum;
    return HAL_OK;
}

HAL_FALLBACK hal_status_t hal_vdot_u128(uint256_t *result, const uint128_t *a, const uint128_t *b,
                                        const size_t n) {
    uint256_t sum = {{0, 0, 0, 0}};

    for (size_t i = 0; i < n; i++) {
        // Struct 0-Skip optimization
        if ((a[i].l == 0 && a[i].u == 0) || (b[i].l == 0 && b[i].u == 0))
            continue;

        uint256_t prod = hal_mul_u128(a[i], b[i]);
        sum = hal_add_u256(sum, prod);
    }
    *result = sum;
    return HAL_OK;
}

/* i64/u64 */
HAL_FALLBACK hal_status_t hal_vmac_i64(int128_t *c, const int64_t *a, const int64_t *b,
                                       const size_t n) {

    for (size_t i = 0; i < n; i++) {
        int128_t prod = hal_mul_i64(a[i], b[i]);
        c[i] = hal_add_i128(c[i], prod);
    }
    return HAL_OK;
}

HAL_FALLBACK hal_status_t hal_vmac_u64(uint128_t *c, const uint64_t *a, const uint64_t *b,
                                       const size_t n) {
    for (size_t i = 0; i < n; i++) {
        uint128_t prod = hal_mul_u64(a[i], b[i]);
        c[i] = hal_add_u128(c[i], prod);
    }
    return HAL_OK;
}

/* i128/u128 */
HAL_FALLBACK hal_status_t hal_vmac_i128(int256_t *c, const int128_t *a, const int128_t *b,
                                        const size_t n) {
    for (uint32_t i = 0; i < n; i++) {
        // 1. 128bit x 128bit -> 256bit single multiplication (negative sign extension logic applied
        // automatically)
        int256_t prod = hal_mul_i128(a[i], b[i]);

        // 2. 256bit addition accumulation (bit-level addition is safe regardless of sign)
        c[i] = hal_add_i256(c[i], prod);
    }
    return HAL_OK;
}

HAL_FALLBACK hal_status_t hal_vmac_u128(uint256_t *c, const uint128_t *a, const uint128_t *b,
                                        const size_t n) {

    for (uint32_t i = 0; i < n; i++) {
        uint256_t prod = hal_mul_u128(a[i], b[i]);
        c[i] = hal_add_u256(c[i], prod);
    }
    return HAL_OK;
}

/* i64/u64 */
HAL_FALLBACK hal_status_t hal_matrix_vmul_i64(int128_t *c, const int64_t *a, const int64_t *b,
                                              int M, int N, int K) {
    // VLA mapping
    int128_t(*pc)[N] = (void *)c;
    int64_t (*pa)[K] = (void *)a;
    int64_t (*pb)[N] = (void *)b;

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
    return HAL_OK;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_u64(uint128_t *c, const uint64_t *a, const uint64_t *b,
                                              int M, int N, int K) {

    // VLA mapping
    uint128_t(*pc)[N] = (void *)c;
    uint64_t (*pa)[K] = (void *)a;
    uint64_t (*pb)[N] = (void *)b;

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
    return HAL_OK;
}

/* i128/u128 */
HAL_FALLBACK hal_status_t hal_matrix_vmul_i128(int256_t *c, const int128_t *a, const int128_t *b,
                                               int M, int N, int K) {
    // VLA mapping
    int256_t(*pc)[N] = (void *)c;
    int128_t(*pa)[K] = (void *)a;
    int128_t(*pb)[N] = (void *)b;

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
    return HAL_OK;
}

HAL_FALLBACK hal_status_t hal_matrix_vmul_u128(uint256_t *c, const uint128_t *a, const uint128_t *b,
                                               int M, int N, int K) {
    // VLA mapping
    uint256_t(*pc)[N] = (void *)c;
    uint128_t(*pa)[K] = (void *)a;
    uint128_t(*pb)[N] = (void *)b;

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
    return HAL_OK;
}
