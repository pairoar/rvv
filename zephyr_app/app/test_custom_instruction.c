/**
 * @file   test_custom_instruction.c
 * @brief  test custom instruction
 * @author Seongjin Oh
 * @date   03-17-2026
 */

/* ======================================================================== */
/* Standard Library Headers                                                 */
/* ======================================================================== */
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Linux 시간 측정을 위한 헤더
/* ======================================================================== */
/* User-Defined Headers                                                     */
/* ======================================================================== */
#include "lab_test.h"
#include "lab_util.h"
#include "vmath.h"

/* ======================================================================== */
/*  Macros (#define)                                                        */
/* ======================================================================== */
#define SAFE_FREE(ptr)                                                                             \
    if (ptr != NULL) {                                                                             \
        free(ptr);                                                                                 \
        ptr = NULL;                                                                                \
    }

/*
    - MAT_A, MAT_B must be continouly allocated
    - MAT_A : random generated spars array
    - RATIO : non-zero value ratio
              100 - ratio : 0
              ratio       : non zero
    - rand() + 1 : prevent for setting 0
    - MAT_B : sequencial value
    - Usage : perform below and then call this macro
               INIT_MATRIX(...);
*/
#define INIT_MATRIX(TYPE, MAT_A, MAT_B, N, R)                                                      \
    srand((unsigned int)time(NULL));                                                               \
    int RATIO = R % 100;                                                                           \
    for (size_t i = 0; i < N; i++) {                                                               \
        MAT_A[i] = (rand() % 100 < RATIO) ? (TYPE)(rand() + 1) : (TYPE)0;                          \
        MAT_B[i] = (TYPE)((i % 100) + 1);                                                          \
    }

#define INIT_MATRIX_RND(TYPE, MAT_A, MAT_B, N)                                                     \
    srand((unsigned int)time(NULL));                                                               \
    for (size_t i = 0; i < N; i++) {                                                               \
        MAT_A[i] = (TYPE)rand();                                                                   \
        MAT_B[i] = (TYPE)((i % 100) + 1);                                                          \
    }

/* ======================================================================== */
/* Local Types (enum, struct, typedef)                                      */
/* ======================================================================== */
enum {
    VECTOR_SIZE_NONE = 0,
    VECTOR_SIZE_4 = 4,
    VECTOR_SIZE_5 = 5,
    VECTOR_SIZE_6 = 6,
    VECTOR_SIZE_7 = 7,
    VECTOR_SIZE_8 = 8,
    VECTOR_SIZE_16 = 16,
    VECTOR_SIZE_32 = 32,
    VECTOR_SIZE_64 = 64,
    VECTOR_SIZE_128 = 128,
    VECTOR_SIZE_256 = 256,
    VECTOR_SIZE_UNKNOWN,
};

/* ======================================================================== */
/* Global Variables (Minimize usage recommended)                            */
/* ======================================================================== */

/* ======================================================================== */
/* Static Variables (File-scope global variables)                           */
/* ======================================================================== */

/* ======================================================================== */
/* Global Function Prototypes (Forward declarations)                        */
/* ======================================================================== */
uint64_t mac(uint32_t *a, uint32_t *b, const int i, const int j, const int rows_b);
uint64_t mac2(uint32_t **a, uint32_t **b, const int i, const int j, const int rows_b);
void lab_do_all_custom_inst_test(void);

/* ======================================================================== */
/* Static Function Prototypes (Forward declarations)                        */
/* ======================================================================== */
static void _matrix_mul8(uint16_t *c, uint8_t *a, uint8_t *b, const uint32_t a_row,
                         const uint32_t a_col, const uint32_t b_row, const uint32_t b_col);

/* ======================================================================== */
/* Public Functions (API Implementations)                                   */
/* ======================================================================== */

/* ======================================================================== */
/* Static Functions (Internal implementations)                              */
/* ======================================================================== */
/*
    Initialize test data
*/
static void init_vector_u8_i2(uint8_t **a, uint8_t **b, const uint32_t n) {
    // allocate
    *a = (uint8_t *)calloc(n, sizeof(uint8_t));
    *b = (uint8_t *)calloc(n, sizeof(uint8_t));

    if (*a == NULL || *b == NULL) {
        if (*a != NULL)
            free(*a);
        if (*b != NULL)
            free(*b);
        *a = NULL;
        *b = NULL;
        return;
    }

    // initialize
    for (uint32_t i = 0; i < n; i++) {
        (*a)[i] = (uint8_t)i;       // 0, 1, 2...
        (*b)[i] = (uint8_t)(i * 2); // 0, 2, 4...
    }
}
#if 0
static void init_vector_u8_o2(uint8_t **c_hw, uint8_t **c_sw, const uint32_t n) {
    // allocate
    *c_hw = (uint8_t *)malloc(sizeof(uint8_t) * n);
    *c_sw = (uint8_t *)malloc(sizeof(uint8_t) * n);

    memset(*c_hw, 0, sizeof(uint8_t) * n);
    memset(*c_sw, 0, sizeof(uint8_t) * n);
}
#endif
static void init_vector_u8_i2_o2(uint8_t **a, uint8_t **b, uint8_t **c_hw, uint8_t **c_sw,
                                 const uint32_t n) {
    // allocate
    *a = (uint8_t *)calloc(n, sizeof(uint8_t));
    *b = (uint8_t *)calloc(n, sizeof(uint8_t));
    *c_hw = (uint8_t *)calloc(n, sizeof(uint8_t));
    *c_sw = (uint8_t *)calloc(n, sizeof(uint8_t));

    if (*a == NULL || *b == NULL || *c_hw == NULL || *c_sw == NULL) {
        if (*a != NULL)
            free(*a);
        if (*b != NULL)
            free(*b);
        if (*c_hw != NULL)
            free(*c_hw);
        if (*c_sw != NULL)
            free(*c_sw);
        *a = NULL;
        *b = NULL;
        *c_hw = NULL;
        *c_sw = NULL;
        return;
    }

    // initialize
    for (uint8_t i = 0; i < n; i++) {
        (*a)[i] = i;     // 0, 1, 2...
        (*b)[i] = i * 2; // 0, 2, 4...
    }
}

static void init_vector_u16_i2(uint16_t **a, uint16_t **b, const uint32_t n) {
    // allocate
    *a = (uint16_t *)calloc(n, sizeof(uint16_t));
    *b = (uint16_t *)calloc(n, sizeof(uint16_t));

    if (*a == NULL || *b == NULL) {
        if (*a != NULL)
            free(*a);
        if (*b != NULL)
            free(*b);
        *a = NULL;
        *b = NULL;
        return;
    }

    // initialize
    for (uint16_t i = 0; i < n; i++) {
        (*a)[i] = i;     // 0, 1, 2...
        (*b)[i] = i * 2; // 0, 2, 4...
    }
}

static void init_vector_u16_o2(uint16_t **c_hw, uint16_t **c_sw, const uint32_t n) {
    // allocate
    *c_hw = (uint16_t *)calloc(n, sizeof(uint16_t));
    *c_sw = (uint16_t *)calloc(n, sizeof(uint16_t));

    if (*c_hw == NULL || *c_sw == NULL) {
        if (*c_hw != NULL)
            free(*c_hw);
        if (*c_sw != NULL)
            free(*c_sw);
        *c_hw = NULL;
        *c_sw = NULL;
        return;
    }
}

static void init_vector_u16_4(uint16_t **a, uint16_t **b, uint16_t **c_hw, uint16_t **c_sw,
                              const uint32_t n) {
    // allocate
    *a = (uint16_t *)calloc(n, sizeof(uint16_t));
    *b = (uint16_t *)calloc(n, sizeof(uint16_t));
    *c_hw = (uint16_t *)calloc(n, sizeof(uint16_t));
    *c_sw = (uint16_t *)calloc(n, sizeof(uint16_t));

    if (*a == NULL || *b == NULL || *c_hw == NULL || *c_sw == NULL) {
        if (*a != NULL)
            free(*a);
        if (*b != NULL)
            free(*b);
        if (*c_hw != NULL)
            free(*c_hw);
        if (*c_sw != NULL)
            free(*c_sw);
        *a = NULL;
        *b = NULL;
        *c_hw = NULL;
        *c_sw = NULL;
        return;
    }

    // initialize
    for (uint16_t i = 0; i < n; i++) {
        (*a)[i] = i;     // 0, 1, 2...
        (*b)[i] = i * 2; // 0, 2, 4...
    }
}
#if 0
static void init_vector_u32_i2(uint32_t **a, uint32_t **b, const uint32_t n) {
    // allocate
    *a = (uint32_t *)malloc(sizeof(uint32_t) * n);
    *b = (uint32_t *)malloc(sizeof(uint32_t) * n);

    // initialize
    for (uint32_t i = 0; i < n; i++) {
        (*a)[i] = i;     // 0, 1, 2...
        (*b)[i] = i * 2; // 0, 2, 4...
    }
}
#endif
static void init_vector_u32_o2(uint32_t **c_hw, uint32_t **c_sw, const uint32_t n) {
    // allocate
    *c_hw = (uint32_t *)calloc(n, sizeof(uint32_t));
    *c_sw = (uint32_t *)calloc(n, sizeof(uint32_t));

    if (*c_hw == NULL || *c_sw == NULL) {
        if (*c_hw != NULL)
            free(*c_hw);
        if (*c_sw != NULL)
            free(*c_sw);
        *c_hw = NULL;
        *c_sw = NULL;
        return;
    }
}
#if 0
static void init_vector_u32_4(uint32_t **a, uint32_t **b, uint32_t **c_hw, uint32_t **c_sw,
                              const uint32_t n) {
    // allocate
    *a = (uint32_t *)malloc(sizeof(uint32_t) * n);
    *b = (uint32_t *)malloc(sizeof(uint32_t) * n);
    *c_hw = (uint32_t *)malloc(sizeof(uint32_t) * n);
    *c_sw = (uint32_t *)malloc(sizeof(uint32_t) * n);

    // initialize
    for (uint32_t i = 0; i < n; i++) {
        (*a)[i] = i;     // 0, 1, 2...
        (*b)[i] = i * 2; // 0, 2, 4...
    }
    memset(*c_hw, 0, sizeof(uint32_t) * n);
    memset(*c_sw, 0, sizeof(uint32_t) * n);
}
#endif
static void init_vector_u64_i2(uint64_t **a, uint64_t **b, const uint32_t n) {
    // allocate
    *a = (uint64_t *)calloc(n, sizeof(uint64_t));
    *b = (uint64_t *)calloc(n, sizeof(uint64_t));

    if (*a == NULL || *b == NULL) {
        if (*a != NULL)
            free(*a);
        if (*b != NULL)
            free(*b);
        *a = NULL;
        *b = NULL;
        return;
    }

    // initialize
    for (uint32_t i = 0; i < n; i++) {
        (*a)[i] = i;     // 0, 1, 2...
        (*b)[i] = i * 2; // 0, 2, 4...
    }
}
#if 0
static void init_vector_u64_o2(uint64_t **c_hw, uint64_t **c_sw, const uint32_t n) {
    // allocate
    *c_hw = (uint64_t *)malloc(sizeof(uint64_t) * n);
    *c_sw = (uint64_t *)malloc(sizeof(uint64_t) * n);

    memset(*c_hw, 0, sizeof(uint64_t) * n);
    memset(*c_sw, 0, sizeof(uint64_t) * n);
}

static void init_vector_u64_4(uint64_t **a, uint64_t **b, uint64_t **c_hw, uint64_t **c_sw,
                              const uint32_t n) {
    // allocate
    *a = (uint64_t *)malloc(sizeof(uint64_t) * n);
    *b = (uint64_t *)malloc(sizeof(uint64_t) * n);
    *c_hw = (uint64_t *)malloc(sizeof(uint64_t) * n);
    *c_sw = (uint64_t *)malloc(sizeof(uint64_t) * n);

    // initialize
    for (uint32_t i = 0; i < n; i++) {
        (*a)[i] = i;     // 0, 1, 2...
        (*b)[i] = i * 2; // 0, 2, 4...
    }
    memset(*c_hw, 0, sizeof(uint64_t) * n);
    memset(*c_sw, 0, sizeof(uint64_t) * n);
}
#endif
static void init_vector_u128_i2(uint128_t **a, uint128_t **b, uint32_t n) {
    *a = (uint128_t *)calloc(n, sizeof(uint128_t));
    *b = (uint128_t *)calloc(n, sizeof(uint128_t));

    if (*a == NULL || *b == NULL) {
        if (*a != NULL)
            free(*a);
        if (*b != NULL)
            free(*b);
        *a = NULL;
        *b = NULL;
        return;
    }

    for (uint32_t i = 0; i < n; i++) {
        (*a)[i].l = i;     // 0, 1, 2...
        (*b)[i].l = i * 2; // 0, 2, 4...
    }
}

static void init_vector_u128_o2(uint128_t **c_hw, uint128_t **c_sw, const uint32_t n) {
    // allocate
    *c_hw = (uint128_t *)calloc(n, sizeof(uint128_t));
    *c_sw = (uint128_t *)calloc(n, sizeof(uint128_t));

    if (*c_hw == NULL || *c_sw == NULL) {
        if (*c_hw != NULL)
            free(*c_hw);
        if (*c_sw != NULL)
            free(*c_sw);
        *c_hw = NULL;
        *c_sw = NULL;
        return;
    }
}
#if 0
static void init_vector_u128_4(uint128_t **a, uint128_t **b, uint128_t **c_hw, uint128_t **c_sw,
                               const uint32_t n) {
    // allocate
    *a = (uint128_t *)malloc(sizeof(uint128_t) * n);
    *b = (uint128_t *)malloc(sizeof(uint128_t) * n);
    *c_hw = (uint128_t *)malloc(sizeof(uint128_t) * n);
    *c_sw = (uint128_t *)malloc(sizeof(uint128_t) * n);

    // initialize
    for (uint32_t i = 0; i < n; i++) {
        (*a)[i].l = i;     // 0, 1, 2...
        (*b)[i].l = i * 2; // 0, 2, 4...
    }
    memset(*c_hw, 0, sizeof(uint128_t) * n);
    memset(*c_sw, 0, sizeof(uint128_t) * n);
}

static void init_vector_u256_i2(uint256_t **a, uint256_t **b, const uint32_t n) {
    // allocate
    *a = (uint256_t *)malloc(sizeof(uint256_t) * n);
    *b = (uint256_t *)malloc(sizeof(uint256_t) * n);

    // initialize
    for (uint32_t i = 0; i < n; i++) {
        (*a)[i].d[0] = i;     // 0, 1, 2...
        (*b)[i].d[0] = i * 2; // 0, 2, 4...
    }
}
#endif
static void init_vector_u256_o2(uint256_t **c_hw, uint256_t **c_sw, const uint32_t n) {
    // allocate
    *c_hw = (uint256_t *)calloc(n, sizeof(uint256_t));
    *c_sw = (uint256_t *)calloc(n, sizeof(uint256_t));

    if (*c_hw == NULL || *c_sw == NULL) {
        if (*c_hw != NULL)
            free(*c_hw);
        if (*c_sw != NULL)
            free(*c_sw);
        *c_hw = NULL;
        *c_sw = NULL;
        return;
    }
}
#if 0
static void init_vector_u256_4(uint256_t **a, uint256_t **b, uint256_t **c_hw, uint256_t **c_sw,
                               const uint32_t n) {
    // allocate
    *a = (uint256_t *)malloc(sizeof(uint256_t) * n);
    *b = (uint256_t *)malloc(sizeof(uint256_t) * n);
    *c_hw = (uint256_t *)malloc(sizeof(uint256_t) * n);
    *c_sw = (uint256_t *)malloc(sizeof(uint256_t) * n);

    // initialize
    for (uint32_t i = 0; i < n; i++) {
        (*a)[i].d[0] = i;     // 0, 1, 2...
        (*b)[i].d[0] = i * 2; // 0, 2, 4...
    }
    memset(*c_hw, 0, sizeof(uint256_t) * n);
    memset(*c_sw, 0, sizeof(uint256_t) * n);
}
#endif
static void init_vector_u32(uint32_t **a, uint32_t **b, uint32_t **c_hw, uint32_t **c_sw,
                            const uint32_t n) {
    // allocate
    *a = (uint32_t *)calloc(n, sizeof(uint32_t));
    *b = (uint32_t *)calloc(n, sizeof(uint32_t));
    *c_hw = (uint32_t *)calloc(n, sizeof(uint32_t));
    *c_sw = (uint32_t *)calloc(n, sizeof(uint32_t));

    if (*a == NULL || *b == NULL || *c_hw == NULL || *c_sw == NULL) {
        if (*a != NULL)
            free(*a);
        if (*b != NULL)
            free(*b);
        if (*c_hw != NULL)
            free(*c_hw);
        if (*c_sw != NULL)
            free(*c_sw);
        *a = NULL;
        *b = NULL;
        *c_hw = NULL;
        *c_sw = NULL;
        return;
    }

    // initialize
    for (uint32_t i = 0; i < n; i++) {
        (*a)[i] = i;     // 0, 1, 2...
        (*b)[i] = i * 2; // 0, 2, 4...
    }
}

static void init_vector_u64(uint64_t **a, uint64_t **b, uint64_t **c_hw, uint64_t **c_sw,
                            const uint32_t n) {
    // allocate
    *a = (uint64_t *)calloc(n, sizeof(uint64_t));
    *b = (uint64_t *)calloc(n, sizeof(uint64_t));
    *c_hw = (uint64_t *)calloc(n, sizeof(uint64_t));
    *c_sw = (uint64_t *)calloc(n, sizeof(uint64_t));

    if (*a == NULL || *b == NULL || *c_hw == NULL || *c_sw == NULL) {
        if (*a != NULL)
            free(*a);
        if (*b != NULL)
            free(*b);
        if (*c_hw != NULL)
            free(*c_hw);
        if (*c_sw != NULL)
            free(*c_sw);
        *a = NULL;
        *b = NULL;
        *c_hw = NULL;
        *c_sw = NULL;
        return;
    }

    // initialize
    for (uint32_t i = 0; i < n; i++) {
        (*a)[i] = i;     // 0, 1, 2...
        (*b)[i] = i * 2; // 0, 2, 4...
    }
}
#if 0
static void init_vector_u128(uint128_t **a, uint128_t **b, uint128_t **c_hw, uint128_t **c_sw,
                             const uint32_t n) {
    // allocate
    *a = (uint128_t *)malloc(sizeof(uint128_t) * n);
    *b = (uint128_t *)malloc(sizeof(uint128_t) * n);
    *c_hw = (uint128_t *)malloc(sizeof(uint128_t) * n);
    *c_sw = (uint128_t *)malloc(sizeof(uint128_t) * n);

    // initialize
    for (uint32_t i = 0; i < n; i++) {
        (*a)[i].l = i;     // 0, 1, 2...
        (*b)[i].l = i * 2; // 0, 2, 4...
    }
    memset(*c_hw, 0, sizeof(uint128_t) * n);
    memset(*c_sw, 0, sizeof(uint128_t) * n);
}
#endif
// init_vectors2(uint32_t** in1, uint32_t** in2, uint32_t n)
static void init_vector2(uint32_t **a, uint32_t **b, const uint32_t n) {
    // allocate
    *a = (uint32_t *)calloc(n, sizeof(uint32_t));
    *b = (uint32_t *)calloc(n, sizeof(uint32_t));

    if (*a == NULL || *b == NULL) {
        if (*a != NULL)
            free(*a);
        if (*b != NULL)
            free(*b);
        *a = NULL;
        *b = NULL;
        return;
    }

    // initialize
    for (uint32_t i = 0; i < n; i++) {
        (*a)[i] = i;     // 0, 1, 2...
        (*b)[i] = i * 2; // 0, 2, 4...
    }
}

static void init_vector3(uint32_t **a, uint32_t **b, const uint32_t n) {
    // allocate
    *a = (uint32_t *)calloc(n, sizeof(uint32_t));
    *b = (uint32_t *)calloc(n, sizeof(uint32_t));

    if (*a == NULL || *b == NULL) {
        if (*a != NULL)
            free(*a);
        if (*b != NULL)
            free(*b);
        *a = NULL;
        *b = NULL;
        return;
    }

    // initialize
    for (uint32_t i = 0; i < n; i++) {
        (*a)[i] = i * 100; // 0, 1, 2...
        (*b)[i] = i * 200; // 0, 2, 4...
    }
}

static void init_vector4(uint64_t **a, uint64_t **b, const uint32_t n) {
    // allocate
    *a = (uint64_t *)calloc(n, sizeof(uint64_t));
    *b = (uint64_t *)calloc(n, sizeof(uint64_t));

    if (*a == NULL || *b == NULL) {
        if (*a != NULL)
            free(*a);
        if (*b != NULL)
            free(*b);
        *a = NULL;
        *b = NULL;
        return;
    }
}
#if 0
static void init_vector5(uint64_t **a, uint64_t **b, const uint32_t n) {
    // allocate
    *a = (uint64_t *)malloc(sizeof(uint64_t) * n);
    *b = (uint64_t *)malloc(sizeof(uint64_t) * n);

    // initialize
    memset(*a, 0, sizeof(uint32_t) * n);
    memset(*b, 0, sizeof(uint32_t) * n);
}
#endif
//
static uint8_t *init_mat_u8(const uint32_t row, const uint32_t col) {
    uint8_t *mat = (uint8_t *)malloc(row * col);
    if (mat == NULL) {
        return NULL;
    }

    for (uint32_t i = 0; i < row; i++) {
        for (uint32_t j = 0; j < col; j++) {
            uint32_t idx = i * col + j;
            if (i == 0) {
                mat[idx] = (uint8_t)(i * 2 + j);
            } else {
                mat[idx] = (uint8_t)(i * 2 + j + 1);
            }
        }
    }
    return mat;
}

static uint16_t *init_mat_u16(const uint32_t row, const uint32_t col) {
    uint16_t *mat = (uint16_t *)malloc(sizeof(uint16_t) * row * col);
    if (mat == NULL) {
        return NULL;
    }
    uint16_t (*ptr)[col] = (void *)mat;

    for (uint32_t i = 0; i < row; i++) {
        for (uint32_t j = 0; j < col; j++) {
            if (i == 0) {
                ptr[i][j] = (uint16_t)(i * 2 + j); // 0, 1, 2...
            } else {
                ptr[i][j] = (uint16_t)(i * 2 + j + 1);
            }
        }
    }
    return mat;
}

static uint32_t *init_mat_u32(const uint32_t row, const uint32_t col) {
    uint32_t *mat = (uint32_t *)malloc(sizeof(uint32_t) * row * col);
    if (mat == NULL) {
        return NULL;
    }
    uint32_t (*ptr)[col] = (void *)mat;

    for (uint32_t i = 0; i < row; i++) {
        for (uint32_t j = 0; j < col; j++) {
            if (i == 0) {
                ptr[i][j] = i * 2 + j; // 0, 1, 2...
            } else {
                ptr[i][j] = i * 2 + j + 1;
            }
        }
    }
    return mat;
}

static uint128_t *init_mat_u128(const uint32_t row, const uint32_t col) {
    // 1. allocate memory to uint128_t size
    uint128_t *mat = (uint128_t *)malloc(sizeof(uint128_t) * row * col);
    if (mat == NULL) {
        return mat;
    }

    // 2. set pointers for accessing like a two-dimensional array
    uint128_t(*ptr)[col] = (uint128_t(*)[col])mat;

    // 3. initialize
    memset(mat, 0, sizeof(uint128_t) * row * col);

    for (uint32_t i = 0; i < row; i++) {
        for (uint32_t j = 0; j < col; j++) {
            if (i == 0) {
                ptr[i][j].l = (uint64_t)i * 2 + j;
                ptr[i][j].u = 0;
            } else {
                ptr[i][j].l = (uint64_t)i * 2 + j + 1;
                ptr[i][j].u = 0;
            }
        }
    }
    return mat;
}

static uint256_t *init_mat_u256(const uint32_t row, const uint32_t col) {
    // 1. allocate memory to uint128_t size
    uint256_t *mat = (uint256_t *)malloc(sizeof(uint256_t) * row * col);
    if (mat == NULL) {
        return mat;
    }

    // 2. set pointers for accessing like a two-dimensional array
    uint256_t(*ptr)[col] = (uint256_t(*)[col])mat;

    // 3. initialize
    memset(mat, 0, sizeof(uint256_t) * row * col);

    for (uint32_t i = 0; i < row; i++) {
        for (uint32_t j = 0; j < col; j++) {
            if (i == 0) {
                ptr[i][j].d[0] = (uint64_t)i * 2 + j;
                ptr[i][j].d[1] = 0;
                ptr[i][j].d[2] = 0;
                ptr[i][j].d[3] = 0;
            } else {
                ptr[i][j].d[0] = (uint64_t)i * 2 + j + 1;
                ptr[i][j].d[1] = 0;
                ptr[i][j].d[2] = 0;
                ptr[i][j].d[3] = 0;
            }
        }
    }
    return mat;
}
#if 0
static void print_matrix_uint32(uint32_t **mat, const uint32_t row, const uint32_t col) {
    for (uint32_t i = 0; i < row; i++) {
        for (uint32_t j = 0; j < col; j++) {
            printf("[%d][%d]=%u\n", i, j, mat[i][j]);
        }
    }
}
#endif
// Verify (Compare with Golden Model)
static int verify_result_u8(uint8_t *hw, uint8_t *sw, const uint32_t n) {
    if (hw == NULL || sw == NULL) {
        return 0;
    }

    for (uint32_t i = 0; i < n; i++) {
        if (hw[i] != sw[i]) {
            // printf("[FAIL] Index %d: HW(%d) != SW(%d)\n", i, hw[i], sw[i]);
            return 0;
        }
    }
    return 1;
}

static int verify_result_u16(uint16_t *hw, uint16_t *sw, const uint32_t n) {
    if (hw == NULL || sw == NULL) {
        return 0;
    }

    for (uint32_t i = 0; i < n; i++) {
        if (hw[i] != sw[i]) {
            // printf("[FAIL] Index %d: HW(%d) != SW(%d)\n", i, hw[i], sw[i]);
            return 0;
        }
    }
    return 1;
}

static int verify_result_u32(uint32_t *hw, uint32_t *sw, const uint32_t n) {
    if (hw == NULL || sw == NULL) {
        return 0;
    }

    for (uint32_t i = 0; i < n; i++) {
        if (hw[i] != sw[i]) {
            // printf("[FAIL] Index %d: HW(%d) != SW(%d)\n", i, hw[i], sw[i]);
            return 0;
        }
    }
    return 1;
}

static int verify_result_u64(uint64_t *hw, uint64_t *sw, const uint32_t n) {
    if (hw == NULL || sw == NULL) {
        return 0;
    }

    for (uint32_t i = 0; i < n; i++) {
        if (hw[i] != sw[i]) {
#ifdef CONFIG_SUPPORT_64
            printf("[FAIL] Index %d: HW(0x%lx) != SW(0x%lx)\n", i, hw[i], sw[i]);
#else
            printf("[FAIL] Index %d: HW(0x%llx) != SW(0x%llx)\n", i, hw[i], sw[i]);
#endif
            return 0;
        }
    }
    return 1;
}

#if 0
static int verify_result_u128_one(const uint128_t hw, const uint128_t sw) {
    if (hw.l != sw.l || hw.u != sw.u) {
#ifdef CONFIG_SUPPORT_64
        printf("[FAIL] Index %d: HW(0x%lx) != SW(0x%lx)\n", 0, hw.l, sw.l);
#else
        printf("[FAIL] Index %d: HW(0x%llx) != SW(0x%llx)\n", 0, hw.l, sw.l);
#endif
        return 0;
    }
    return 1;
}
#endif
static int verify_result_u128(const uint128_t *hw, const uint128_t *sw, const uint32_t n) {
    if (hw == NULL || sw == NULL || n <= 0) {
        return 0;
    }

    for (uint32_t i = 0; i < n; i++) {
        if (hw[i].l != sw[i].l || hw[i].u != sw[i].u) {
#ifdef CONFIG_SUPPORT_64
            // printf("[FAIL] Index %d: HW(0x%lx) != SW(0x%lx)\n", i, hw[i].l, sw[i].l);
#else
            // printf("[FAIL] Index %d: HW(0x%llx) != SW(0x%llx)\n", i, hw[i].l, sw[i].l);
#endif
            return 0;
        }
    }
    return 1;
}

static int verify_result_u256(uint256_t *hw, uint256_t *sw, const uint32_t n) {
    if (hw == NULL || sw == NULL) {
        return 0;
    }

    for (uint32_t i = 0; i < n; i++) {
        if (hw[i].d[0] != sw[i].d[0] || hw[i].d[1] != sw[i].d[1] || hw[i].d[2] != sw[i].d[2] ||
            hw[i].d[3] != sw[i].d[3]) {
#ifdef CONFIG_SUPPORT_64
            printf("[FAIL] Index %d: HW{d[0]=0x%lx, d[1]=0x%lx, d[2]=0x%lx, "
                   "d[3]=0x%lx} != SW{d[0]=0x%lx, d[1]=0x%lx, d[2]=0x%lx, "
                   "d[3]=0x%lx}\n",
                   i, hw[i].d[0], hw[i].d[1], hw[i].d[2], hw[i].d[3], sw[i].d[0], sw[i].d[1],
                   sw[i].d[2], sw[i].d[3]);
#else
            printf("[FAIL] Index %d: HW{d[0]=0x%llx, d[1]=0x%llx, d[2]=0x%llx, "
                   "d[3]=0x%llx} != SW{d[0]=0x%llx, d[1]=0x%llx, d[2]=0x%llx, "
                   "d[3]=0x%llx}\n",
                   i, hw[i].d[0], hw[i].d[1], hw[i].d[2], hw[i].d[3], sw[i].d[0], sw[i].d[1],
                   sw[i].d[2], sw[i].d[3]);
#endif
            return 0;
        }
    }
    return 1;
}

/* add128 */
static uint128_t _add128_64(const uint128_t a, const uint64_t b) {
    uint128_t res = {0};
    uint64_t carry = 0;

    // add lower 64 bits (l)
    res.l = a.l + b;
    carry = (res.l < a.l) ? 1 : 0;

    // add the second 64 bit (u) (b's top 64 bit + previous carry)
    res.u = a.u + carry;

    return res;
}

static uint128_t _add128(const uint128_t a, const uint128_t b) {
    uint128_t c = {0};

    // 1. add lower 64 bits first
    c.l = a.l + b.l;

    // 2. confirm carry occurrence
    // if the added result is less than one of the added values, an overflow has
    // occurred
    uint64_t carry = (c.l < a.l) ? 1 : 0;

    // 3. top 64 bits combined with carry
    c.u = a.u + b.u + carry;

    return c;
}

static uint256_t _add256_128(const uint256_t a, const uint128_t b) {
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

    // 4th 64-bit (d[3]) - final carriage propagation
    res.d[3] = a.d[3] + carry;

    return res;
}

static uint256_t _add256(const uint256_t a, const uint256_t b) {
    uint256_t c = {0};
    uint64_t carry = 0;

    for (uint32_t i = 0; i < 4; i++) {
        uint64_t val_a = a.d[i];
        uint64_t val_b = b.d[i];

        // Calculate the sum of the current digits (including the carry of the
        // previous digits)
        c.d[i] = val_a + val_b + carry;

        // 1. Overflow occurs in process a + b: (c.d[i] <val_a)
        // 2. Or I added carry and it fits perfectly, causing overflow: (carry &
        // c.d[i] == val_a) When the above two conditions are combined, they can be
        // expressed concisely as follows.
        if (carry) {
            carry = (c.d[i] <= val_a) ? 1 : 0;
        } else {
            carry = (c.d[i] < val_a) ? 1 : 0;
        }
    }

    return c;
}

/* mac */
static uint128_t _mac64(const uint64_t a, const uint64_t b) {
    uint128_t res = {0};
    uint64_t a_lo = a & 0xFFFFFFFFUL;
    uint64_t a_hi = a >> 32;
    uint64_t b_lo = b & 0xFFFFFFFFUL;
    uint64_t b_hi = b >> 32;

    uint64_t p0 = a_lo * b_lo;
    uint64_t p1 = a_lo * b_hi;
    uint64_t p2 = a_hi * b_lo;
    uint64_t p3 = a_hi * b_hi;

    // 중간 합산 및 Carry 처리
    uint64_t mid = (p0 >> 32) + (p1 & 0xFFFFFFFFUL) + (p2 & 0xFFFFFFFFUL);

    res.l = (p0 & 0xFFFFFFFF) | (mid << 32);
    res.u = p3 + (p1 >> 32) + (p2 >> 32) + (mid >> 32);

    return res;
}

static uint128_t _mac64_arr(uint64_t arr_a[], uint64_t arr_b[], const uint32_t n) {
    uint128_t total = {0, 0};
    if (arr_a == NULL || arr_b == NULL || n <= 0) {
        return total;
    }

    for (uint32_t i = 0; i < n; i++) {
        // 1. multiplication of current elements (64x64 -> 128)
        uint128_t prod = _mac64(arr_a[i], arr_b[i]);

        // 2. accumulated sum (128 + 128)
        uint64_t prev_l = total.l;
        total.l += prod.l;

        // check for carry occurrence in the lower 64 Bit
        uint64_t carry = (total.l < prev_l) ? 1 : 0;

        // summing the top bit and carry of the multiplication result to the top 64
        // bits
        total.u += prod.u + carry;
    }

    return total;
}

static uint256_t _mac128(const uint128_t a, const uint128_t b) {
    uint256_t res = {{0, 0, 0, 0}};

    // Four partial products (128-bit results each)
    // Affect a.l * b.l -> d[0], d[1]
    // a.l * b.u -> affecting d[1], d[2]
    // a.u * b.l -> affect d[1], d[2]
    // a.u * b.u -> affect d[2], d[3]

    uint128_t p00 = _mac64(a.l, b.l);
    uint128_t p01 = _mac64(a.l, b.u);
    uint128_t p10 = _mac64(a.u, b.l);
    uint128_t p11 = _mac64(a.u, b.u);

    // lower 64-bit determination
    res.d[0] = p00.l;

    // d[1] Calculation and Carry Management
    uint64_t carry = 0;
    res.d[1] = p00.u;

    // Add p01.l to d[1]
    res.d[1] += p01.l;
    if (res.d[1] < p01.l) {
        carry++;
    }

    // d[1] plus p10.l
    res.d[1] += p10.l;
    if (res.d[1] < p10.l) {
        carry++;
    }

    // calculate d[2]
    res.d[2] = p11.l + p01.u + p10.u + carry;

    // carry calculation from d[2] (very rare but for safety)
    // uint64_t carry2 = 0;
    if (res.d[2] < p11.l || (res.d[2] == p11.l && (p01.u | p10.u | carry))) {
        // It's actually 64 bit addition multiple times, so a precise check is
        // required
    }

    // d[3] Calculation (including top carry)
    // More precise implementation for accurate carriage in d[2]: manual carriage
    // check instead of unsigned__int128
    uint64_t c1 = 0;

    res.d[2] = p11.l;
    res.d[2] += p01.u;
    if (res.d[2] < p01.u) {
        c1++;
    }

    res.d[2] += p10.u;
    if (res.d[2] < p10.u) {
        c1++;
    }
    res.d[2] += carry;
    if (res.d[2] < carry) {
        c1++;
    }
    res.d[3] = p11.u + c1;

    return res;
}

static uint256_t mac128_arr(uint128_t arr_a[], uint128_t arr_b[], const uint32_t n) {
    uint256_t total = {{0, 0, 0, 0}};
    if (arr_a == NULL || arr_b == NULL || n <= 0) {
        return total;
    }

    for (uint32_t i = 0; i < n; i++) {
        // 1. Perform multiplication of the current elements (128x128 -> 256)
        uint256_t prod = _mac128(arr_a[i], arr_b[i]);

        // 2. 256-bit accumulation (4-stage carry propagation)
        uint64_t carry = 0;
        for (uint32_t j = 0; j < 4; j++) {
            uint64_t prev = total.d[j];
            total.d[j] += prod.d[j] + carry;

            // Carry conditions:
            // 1. Summed result is smaller than the previous value (standard overflow)
            // 2. Or the result equals the carry value after adding it (when the
            // previous value was at its maximum)
            if (carry > 0) {
                carry = (total.d[j] <= prev) ? 1 : 0;
            } else {
                carry = (total.d[j] < prev) ? 1 : 0;
            }
        }
    }

    return total;
}

/* mul */
static uint128_t _mul64(const uint64_t a, const uint64_t b) {
    uint64_t a_low = (uint32_t)a;
    uint64_t a_high = a >> 32;
    uint64_t b_low = (uint32_t)b;
    uint64_t b_high = b >> 32;

    uint64_t p0 = a_low * b_low;
    uint64_t p1 = a_low * b_high;
    uint64_t p2 = a_high * b_low;
    uint64_t p3 = a_high * b_high;

    // Summation of intermediate results and carry handling
    uint64_t mid = p1 + (p0 >> 32) + (uint32_t)p2;

    uint128_t res = {0};
    res.l = (mid << 32) | (uint32_t)p0;
    res.u = p3 + (mid >> 32) + (p2 >> 32);
    return res;
}
#if 0
static uint128_t lab_add128_with_carry(const uint128_t a, const uint128_t b, uint64_t *carry_out) {
    uint128_t res = {0};
    res.l = a.l + b.l;
    uint64_t c1 = (res.l < a.l); // check carry
    res.u = a.u + b.u + c1;
    *carry_out = (res.u < a.u) || (c1 && res.u == a.u);
    return res;
}
#endif
// 64bit * 64bit = 128bit
static uint128_t lab_mul64_to_128(const uint64_t a, const uint64_t b) {
    uint64_t al = (uint32_t)a;
    uint64_t ah = a >> 32;
    uint64_t bl = (uint32_t)b;
    uint64_t bh = b >> 32;

    uint64_t p0 = al * bl;
    uint64_t p1 = al * bh;
    uint64_t p2 = ah * bl;
    uint64_t p3 = ah * bh;

    // Summation of intermediate terms and carry processing
    uint64_t mid = p1 + (p0 >> 32) + (uint32_t)p2;

    uint128_t res = {0};
    res.l = (mid << 32) | (uint32_t)p0;
    res.u = p3 + (mid >> 32) + (p2 >> 32);
    return res;
}

static uint256_t mul128(const uint128_t a, const uint128_t b) {
    // Compute 4 partial products and handle carries during summation.
    uint128_t ll = lab_mul64_to_128(a.l, b.l); // (aL * bL)
    uint128_t lh = lab_mul64_to_128(a.l, b.u); // (aL * bH)
    uint128_t hl = lab_mul64_to_128(a.u, b.l); // (aH * bL)
    uint128_t hh = lab_mul64_to_128(a.u, b.u); // (aH * bH)

    uint256_t res = {0};
    uint64_t carry = 0;
    uint64_t c_temp = 0;

    // Determine the least significant 64 bits.
    res.d[0] = ll.l;

    // Summation for the second 64-bit segment (ll.u + lh.l + hl.l)
    uint64_t mid_low = ll.u + lh.l;
    carry = (mid_low < ll.u);
    res.d[1] = mid_low + hl.l;
    carry += (res.d[1] < mid_low);

    // Summation for the third 64-bit segment (lh.u + hl.u + hh.l + carry)
    uint64_t mid_high = lh.u + hl.u + carry;
    c_temp = (mid_high < lh.u);
    res.d[2] = mid_high + hh.l;
    carry = c_temp + (res.d[2] < mid_high);

    // Most significant 64 bits
    res.d[3] = hh.u + carry;

    return res;
}

static uint128_t add128_with_carry(const uint128_t a, const uint128_t b, uint64_t *carry_out) {
    uint128_t res = {0};
    res.l = a.l + b.l;
    uint64_t c1 = (res.l < a.l); // check carry
    res.u = a.u + b.u + c1;
    *carry_out = (res.u < a.u) || (c1 && res.u == a.u);
    return res;
}

static uint256_t _mul128(const uint128_t a, const uint128_t b) {
    // Split 128-bit into 64-bit H and L to calculate 4 partial products.
    uint128_t ll = lab_mul64_to_128(a.l, b.l); // (aL * bL)
    uint128_t lh = lab_mul64_to_128(a.l, b.u); // (aL * bH)
    uint128_t hl = lab_mul64_to_128(a.u, b.l); // (aH * bL)
    uint128_t hh = lab_mul64_to_128(a.u, b.u); // (aH * bH)

    uint256_t res = {0};
    uint64_t carry = 0;
    uint64_t c_temp = 0;

    // Determine the least significant 64 bits.
    res.d[0] = ll.l;

    // Summation for the second 64-bit segment (ll.u + lh.l + hl.l)
    uint64_t mid_low = ll.u + lh.l;
    carry = (mid_low < ll.u);
    res.d[1] = mid_low + hl.l;
    carry += (res.d[1] < mid_low);

    // Summation for the third 64-bit segment (lh.u + hl.u + hh.l + carry)
    uint64_t mid_high = lh.u + hl.u + carry;
    c_temp = (mid_high < lh.u);
    res.d[2] = mid_high + hh.l;
    carry = c_temp + (res.d[2] < mid_high);

    // MSB 64 bits
    res.d[3] = hh.u + carry;

    return res;
}

static void lab_test_init_hw(void) { aermob_hw_init(); }

uint64_t mac(uint32_t *a, uint32_t *b, const int i, const int j, const int rows_b) {
    uint64_t sum = 0;
    uint32_t (*pa)[j] = (void *)a;
    uint32_t (*pb)[i] = (void *)b;

    if (a == NULL || b == NULL || i == 0 || j == 0 || rows_b == 0) {
        return sum;
    }

    for (int k = 0; k < rows_b; k++) {
        sum += pa[i][k] * pb[k][j];
    }

    return sum;
}

uint64_t mac2(uint32_t **a, uint32_t **b, const int i, const int j, const int rows_b) {
    uint64_t sum = 0;
    if (a == NULL || b == NULL || i == 0 || j == 0 || rows_b == 0) {
        return sum;
    }

    for (int k = 0; k < rows_b; k++) {
        sum += a[i][k] * b[k][j];
    }

    return sum;
}

/**
 * @brief matrix multiplication (naive)
 * @param pa: matrix A's pointer
 * @param pb: matrix B's pointer
 * @return return result pointer when success, other wise NULL
 */
static void _matrix_mul8(uint16_t *c, uint8_t *a, uint8_t *b, const uint32_t a_row,
                         const uint32_t a_col, const uint32_t b_row, const uint32_t b_col) {
    if (c == NULL || a == NULL || b == NULL || a_col != b_row) {
        return;
    }

    uint16_t (*pc)[b_col] = (void *)c;
    uint8_t (*pa)[a_col] = (void *)a;
    uint8_t (*pb)[b_col] = (void *)b;
    memset(c, 0, sizeof(uint16_t) * a_row * b_col);

    for (uint32_t i = 0; i < a_row; i++) {
        for (uint32_t j = 0; j < b_col; j++) {
            pc[i][j] = 0;
            for (uint32_t k = 0; k < a_col; k++) {
                pc[i][j] += (uint16_t)(pa[i][k] * pb[k][j]);
            }
        }
    }
}

/**
 * @brief matrix multiplication(cache and sparse friendly)
 * @param pa: matrix A's pointer
 * @param pb: matrix B's pointer
 * @return return result pointer when success, other wise NULL
 */
static void lab_matrix_mul8b(uint16_t *c, uint8_t *a, uint8_t *b, const uint32_t a_row,
                             const uint32_t a_col, const uint32_t b_row, const uint32_t b_col) {
    if (c == NULL || a == NULL || b == NULL || a_col != b_row) {
        return;
    }

    uint16_t (*pc)[b_col] = (void *)c;
    uint8_t (*pa)[a_col] = (void *)a;
    uint8_t (*pb)[b_col] = (void *)b;
    memset(c, 0, sizeof(uint16_t) * a_row * b_col);

    for (uint32_t i = 0; i < a_row; i++) {
        for (uint32_t k = 0; k < a_col; k++) {
            if (pa[i][k] != 0) {
                for (uint32_t j = 0; j < b_col; j++) {
                    pc[i][j] += pa[i][k] * pb[k][j];
                }
            }
        }
    }
}

static void _matrix_mul16(uint32_t *c, uint16_t *a, uint16_t *b, const uint32_t a_row,
                          const uint32_t a_col, const uint32_t b_row, const uint32_t b_col) {

    if (c == NULL || a == NULL || b == NULL || a_col != b_row) {
        return;
    }

    uint32_t (*pc)[b_col] = (void *)c;
    uint16_t (*pa)[a_col] = (void *)a;
    uint16_t (*pb)[b_col] = (void *)b;
    memset(c, 0, sizeof(uint32_t) * a_row * b_col);

    for (uint32_t i = 0; i < a_row; i++) {
        for (uint32_t j = 0; j < b_col; j++) {
            pc[i][j] = 0;
            for (uint32_t k = 0; k < a_col; k++) {
                pc[i][j] += (uint32_t)pa[i][k] * pb[k][j];
            }
        }
    }
}

static void _matrix_mul16b(uint32_t *c, uint16_t *a, uint16_t *b, const uint32_t a_row,
                           const uint32_t a_col, const uint32_t b_row, const uint32_t b_col) {
    if (c == NULL || a == NULL || b == NULL || a_col != b_row) {
        return;
    }

    uint32_t (*pc)[b_col] = (void *)c;
    uint16_t (*pa)[a_col] = (void *)a;
    uint16_t (*pb)[b_col] = (void *)b;
    memset(c, 0, sizeof(uint32_t) * a_row * b_col);

    for (uint32_t i = 0; i < a_row; i++) {
        for (uint32_t k = 0; k < a_col; k++) {
            if (pc[i][k] != 0) {
                for (uint32_t j = 0; j < b_col; j++) {
                    pc[i][j] += (uint32_t)pa[i][k] * pb[k][j];
                }
            }
        }
    }
}

static void _matrix_mul32(uint64_t *c, uint32_t *a, uint32_t *b, const uint32_t a_row,
                          const uint32_t a_col, const uint32_t b_row, const uint32_t b_col) {
    if (c == NULL || a == NULL || b == NULL || a_col != b_row) {
        return;
    }

    uint64_t (*pc)[b_col] = (void *)c;
    uint32_t (*pa)[a_col] = (void *)a;
    uint32_t (*pb)[b_col] = (void *)b;
    memset(c, 0, sizeof(uint64_t) * a_row * b_col);

    for (uint32_t i = 0; i < a_row; i++) {
        for (uint32_t j = 0; j < b_col; j++) {
            pc[i][j] = 0;
            for (uint32_t k = 0; k < a_col; k++) {
                pc[i][j] += (uint64_t)pa[i][k] * pb[k][j];
            }
        }
    }
}

static void _matrix_mul32b(uint64_t *c, uint32_t *a, uint32_t *b, const uint32_t a_row,
                           const uint32_t a_col, const uint32_t b_row, const uint32_t b_col) {
    if (c == NULL || a == NULL || b == NULL || a_col != b_row) {
        return;
    }
    /* VLA mapping */
    uint64_t (*pc)[b_col] = (void *)c;
    uint32_t (*pa)[a_col] = (void *)a;
    uint32_t (*pb)[b_col] = (void *)b;
    memset(c, 0, sizeof(uint64_t) * a_row * b_col);

    for (uint32_t i = 0; i < a_row; i++) {
        for (uint32_t k = 0; k < a_col; k++) {
            if (pa[i][k] != 0) {
                for (uint32_t j = 0; j < b_col; j++) {
                    pc[i][j] += (uint64_t)pa[i][k] * pb[k][j];
                }
            }
        }
    }
}

static void _matrix_mul64(uint128_t *c, uint64_t *a, uint64_t *b, const uint32_t a_row,
                          const uint32_t a_col, const uint32_t b_row, const uint32_t b_col) {
    if (c == NULL || a == NULL || b == NULL || a_col != b_row) {
        return;
    }

    /* VLA mapping */
    uint128_t(*pc)[b_col] = (uint128_t(*)[b_col])c;
    uint64_t (*pa)[a_col] = (uint64_t (*)[a_col])a;
    uint64_t (*pb)[b_col] = (uint64_t (*)[b_col])b;
    memset(c, 0, sizeof(uint128_t) * a_row * b_col);

    for (uint32_t i = 0; i < a_row; i++) {
        for (uint32_t j = 0; j < b_col; j++) {
            pc[i][j].u = 0, pc[i][j].l = 0;
            for (uint32_t k = 0; k < a_col; k++) {
                uint128_t val_a = _mul64(pa[i][k], pb[k][j]);
                pc[i][j] = _add128(pc[i][j], val_a);
            }
        }
    }
}

static void _matrix_mul64b(uint128_t *c, uint64_t *a, uint64_t *b, const uint32_t a_row,
                           const uint32_t a_col, const uint32_t b_row, const uint32_t b_col) {
    if (c == NULL || a == NULL || b == NULL || a_col != b_row) {
        return;
    }

    /* VLA mapping */
    uint128_t(*pc)[b_col] = (uint128_t(*)[b_col])c;
    uint64_t (*pa)[a_col] = (uint64_t (*)[a_col])a;
    uint64_t (*pb)[b_col] = (uint64_t (*)[b_col])b;
    memset(c, 0, sizeof(uint128_t) * a_row * b_col);

    for (uint32_t i = 0; i < a_row; i++) {
        for (uint32_t k = 0; k < a_col; k++) {
            uint64_t val_a = pa[i][k];
            if (val_a == 0)
                continue;

            for (uint32_t j = 0; j < b_col; j++) {
                uint128_t mul_res = _mul64(pa[i][k], pb[k][j]);
                pc[i][j] = _add128(pc[i][j], mul_res);
            }
        }
    }
}

static void _matrix_mul128(uint256_t *c, uint128_t *a, uint128_t *b, const uint32_t a_row,
                           const uint32_t a_col, const uint32_t b_row, const uint32_t b_col) {
    if (c == NULL || a == NULL || b == NULL || a_col != b_row) {
        return;
    }
    uint256_t(*pc)[b_col] = (uint256_t(*)[b_col])c;
    uint128_t(*pa)[a_col] = (uint128_t(*)[a_col])a;
    uint128_t(*pb)[b_col] = (uint128_t(*)[b_col])b;
    memset(c, 0, sizeof(uint256_t) * a_row * b_col);

    for (uint32_t i = 0; i < a_row; i++) {
        for (uint32_t j = 0; j < b_col; j++) {
            pc[i][j].d[3] = 0, pc[i][j].d[2] = 0, pc[i][j].d[1] = 0, pc[i][j].d[0] = 0;
            for (uint32_t k = 0; k < a_col; k++) {
                uint256_t mul_res = _mul128(pa[i][k], pb[k][j]);
                pc[i][j] = _add256(pc[i][j], mul_res);
            }
        }
    }
}

static void _matrix_mul128b(uint256_t *c, uint128_t *a, uint128_t *b, const uint32_t a_row,
                            const uint32_t a_col, const uint32_t b_row, const uint32_t b_col) {
    if (c == NULL || a == NULL || b == NULL || a_col != b_row) {
        return;
    }

    uint256_t(*matC)[b_col] = (uint256_t(*)[b_col])c;
    uint128_t(*matA)[a_col] = (uint128_t(*)[a_col])a;
    uint128_t(*matB)[b_col] = (uint128_t(*)[b_col])b;
    memset(c, 0, sizeof(uint256_t) * a_row * b_col);

    for (uint32_t i = 0; i < a_row; i++) {
        for (uint32_t k = 0; k < a_col; k++) {
            uint128_t val_a = matA[i][k];
            if (val_a.u == 0 && val_a.l == 0)
                continue;
            for (uint32_t j = 0; j < b_col; j++) {
                uint256_t mul_res = _mul128(matA[i][k], matB[k][j]);
                matC[i][j] = _add256(matC[i][j], mul_res);
            }
        }
    }
}

#if 0
static void matrix_mul2(uint64_t **c, uint32_t **a, uint32_t **b, const uint32_t a_row, const uint32_t a_col,
                        const uint32_t b_row, const uint32_t b_col) {
    (void)b_row;
    uint64_t(*pc)[b_col] = (void *)c;
    uint32_t(*pa)[a_col] = (void *)a;
    // uint32_t (*pb)[b_col]  = (void *)b;

    if (c == NULL || a == NULL || b == NULL) {
        return;
    }

    // clear c
    for (uint32_t i = 0; i < a_row; i++) {
        for (uint32_t j = 0; j < b_col; j++) {
            pc[i][j] = 0;
        }
    }

    for (uint32_t i = 0; i < a_row; i++) {
        for (uint32_t k = 0; k < a_col; k++) {
            if (pa[i][k] != 0) {
                for (uint32_t j = 0; j < b_col; j++) {
                    c[i][j] += (uint64_t)a[i][k] * b[k][j];
                }
            }
        }
    }
}

static void matrix_mul3(uint64_t *c, uint32_t *a, uint32_t *b, const uint32_t a_row, const uint32_t a_col,
                        const uint32_t b_row, const uint32_t b_col) {
    (void)b_row;
    uint64_t(*pc)[b_col] = (void *)c;
    uint32_t(*pa)[a_col] = (void *)a;
    uint32_t(*pb)[b_col] = (void *)b;

    if (c == NULL || a == NULL || b == NULL) {
        return;
    }

    memset(c, 0, sizeof(uint64_t) * a_row * b_col);

    for (uint32_t i = 0; i < a_row; i++) {
        for (uint32_t k = 0; k < a_col; k++) {
            if (pa[i][k] != 0) {
                for (uint32_t j = 0; j < b_col; j++) {
                    pc[i][j] += (uint64_t)pa[i][k] * pb[k][j];
                }
            }
        }
    }
}
#endif
static int lab_test_inst_aermob_vadd_u8(void) {
    static const char *name = __func__;
    vmath_status_t result = VMATH_SUCCESS;
    int ret = 0;
    uint8_t *a, *b, *c_hw, *c_sw;
    uint8_t len = VECTOR_SIZE_128;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom VADD8 Accelerator Test ---\n");
    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    init_vector_u8_i2_o2(&a, &b, &c_hw, &c_sw,
                         len); // dec : stack, single pointer
    printf("c_hw=%p, a=%p, b=%p\n", (void *)c_hw, (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_vadd8(c_hw, a, b, len); // change malloc

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    for (uint32_t i = 0; i < len; i++) {
        c_sw[i] = a[i] + b[i];
    }

    // 4. Compare results
    ret = verify_result_u8(c_hw, c_sw, len);
    if (ret) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

    if (result != VMATH_SUCCESS
#ifdef CONFIG_DEBUG
        || true
#endif
    ) {
        printf("----------------------------------\n");
        printf("          EXPECTED      COMPUTED\n");
        printf("----------------------------------\n");
        for (uint32_t i = 0; i < len; i++) {
            printf("[%03u]     %8u    %8u\n", i, c_sw[i], c_hw[i]);
        }
    }

    lab_print_result(result, name);

    // free memory
    if (a != NULL) {
        printf("[%s %04d] free(a)\n", __func__, __LINE__);
        free(a);
    }
    if (b != NULL) {
        printf("[%s %04d] free(b)\n", __func__, __LINE__);
        free(b);
    }
    if (c_hw != NULL) {
        printf("[%s %04d] free(c_hw)\n", __func__, __LINE__);
        free(c_hw);
    }
    if (c_sw != NULL) {
        printf("[%s %04d] free(c_sw)\n", __func__, __LINE__);
        free(c_sw);
    }

    return result;
}

static int lab_test_inst_aermob_vadd_u16(void) {
    static const char *name = __func__;
    vmath_status_t result = VMATH_SUCCESS;
    int ret = 0;
    uint16_t *a, *b, *c_hw, *c_sw;
    uint16_t len = VECTOR_SIZE_128;
    // uint16_t len = VECTOR_SIZE_8;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom VADD16 Accelerator Test ---\n");
    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    init_vector_u16_4(&a, &b, &c_hw, &c_sw, len); // dec : stack, single pointer
    printf("c_hw=%p, a=%p, b=%p\n", (void *)c_hw, (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_vadd16(c_hw, a, b, len); // change malloc

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    for (uint32_t i = 0; i < len; i++) {
        c_sw[i] = a[i] + b[i];
    }

    // 4. Compare results
    ret = verify_result_u16(c_hw, c_sw, len);
    if (ret) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

    if (result != VMATH_SUCCESS
#ifdef CONFIG_DEBUG
        || true
#endif
    ) {
        printf("----------------------------------\n");
        printf("          EXPECTED      COMPUTED\n");
        printf("----------------------------------\n");
        for (uint32_t i = 0; i < len; i++) {
            printf("[%03u]     %8u    %8u\n", i, c_sw[i], c_hw[i]);
        }
    }

    lab_print_result(result, name);

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);
    SAFE_FREE(c_hw);
    SAFE_FREE(c_sw);

    return result;
}

static int lab_test_inst_aermob_vadd_u32(void) {
    static const char *name = __func__;
    vmath_status_t result = VMATH_SUCCESS;
    int ret = 0;
    uint32_t *a, *b, *c_hw, *c_sw;
    uint32_t len = VECTOR_SIZE_128;
    // uint32_t len = VECTOR_SIZE_7;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom VADD32 Accelerator Test ---\n");
    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    init_vector_u32(&a, &b, &c_hw, &c_sw, len); // dec : stack, single pointer
    printf("c_hw=%p, a=%p, b=%p\n", (void *)c_hw, (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_vadd32(c_hw, a, b, len);

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    for (uint32_t i = 0; i < len; i++) {
        c_sw[i] = a[i] + b[i];
    }

    // 4. Compare results
    ret = verify_result_u32(c_hw, c_sw, len);
    if (ret) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

    if (result != VMATH_SUCCESS
#ifdef CONFIG_DEBUG
        || true
#endif
    ) {
        printf("----------------------------------\n");
        printf("          EXPECTED      COMPUTED\n");
        printf("----------------------------------\n");
        for (uint32_t i = 0; i < len; i++) {
            printf("[%03u]     %8u    %8u\n", i, c_sw[i], c_hw[i]);
        }
    }

    lab_print_result(result, name);

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);
    SAFE_FREE(c_hw);
    SAFE_FREE(c_sw);

    return result;
}

static int lab_test_inst_aermob_vadd_u64(void) {
    static const char *name = __func__;
    vmath_status_t result = VMATH_SUCCESS;
    int ret = 0;
    uint64_t *a, *b, *c_hw, *c_sw;
    // uint32_t len = VECTOR_SIZE_128;
    uint32_t len = VECTOR_SIZE_7;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom VADD64 Accelerator Test ---\n");
    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    init_vector_u64(&a, &b, &c_hw, &c_sw, len); // dec : stack, single pointer
    printf("c_hw=%p, a=%p, b=%p\n", (void *)c_hw, (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_vadd64(c_hw, a, b, len); // change malloc

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    for (uint32_t i = 0; i < len; i++) {
        c_sw[i] = a[i] + b[i];
    }

    // 4. Compare results
    ret = verify_result_u64(c_hw, c_sw, len);
    if (ret) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

    if (result != VMATH_SUCCESS
#ifdef CONFIG_DEBUG
        || true
#endif
    ) {
        printf("----------------------------------\n");
        printf("          EXPECTED      COMPUTED\n");
        printf("----------------------------------\n");
        for (uint32_t i = 0; i < len; i++) {
#ifdef CONFIG_SUPPORT_64
            printf("[%03u]     %lu    %lu\n", i, c_sw[i], c_hw[i]);
#else
            printf("[%03u]     %llu    %llu\n", i, c_sw[i], c_hw[i]);
#endif
        }
    }

    lab_print_result(result, name);

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);
    SAFE_FREE(c_hw);
    SAFE_FREE(c_sw);

    return result;
}

static int lab_test_inst_aermob_vadd_u128(void) {
    static const char *name = __func__;
    vmath_status_t result = VMATH_SUCCESS;
    int ret = 0;
    uint128_t *a, *b, *c_hw, *c_sw;
    // uint32_t len = VECTOR_SIZE_128;
    uint32_t len = VECTOR_SIZE_7;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom VADD128 Accelerator Test ---\n");
    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    init_vector_u128_i2(&a, &b, len);       // dec : stack, single pointer
    init_vector_u128_o2(&c_hw, &c_sw, len); // dec : stack, single pointer
    printf("c_hw=%p, a=%p, b=%p\n", (void *)c_hw, (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_vadd128(c_hw, a, b, len); // change malloc

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    for (uint32_t i = 0; i < len; i++) {
        c_sw[i] = _add128(a[i], b[i]);
    }

    // 4. Compare results
    ret = verify_result_u128(c_hw, c_sw, len);
    if (ret) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

    if (result != VMATH_SUCCESS
#ifdef CONFIG_DEBUG
        || true
#endif
    ) {
        printf("----------------------------------\n");
        printf("          EXPECTED      COMPUTED\n");
        printf("----------------------------------\n");
        for (uint32_t i = 0; i < len; i++) {
#ifdef CONFIG_SUPPORT_64
            printf("[%03u]     0x%lx:0x%lx\t\t\t\t0x%lx:0x%lx\n", i, c_sw[i].u, c_sw[i].l,
                   c_hw[i].u, c_hw[i].l);
#else
            printf("[%03u]     0x%llx:0x%llx\t\t\t\t0x%llx:0x%llx\n", i, c_sw[i].u, c_sw[i].l,
                   c_hw[i].u, c_hw[i].l);
#endif
        }
    }

    lab_print_result(result, name);

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);
    SAFE_FREE(c_hw);
    SAFE_FREE(c_sw);

    return result;
}

/* mac */
static int lab_test_inst_aermob_vmac8(void) {
    static const char *name = __func__;
    int result = VMATH_SUCCESS;
    int ret = 0;
    uint8_t *a, *b;
    uint16_t h_sum = 0, s_sum = 0;

    // uint32_t len = VECTOR_SIZE_128;
    const uint32_t len = (uint32_t)VECTOR_SIZE_5;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom MAC8 Accelerator Test ---\n");
    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    // a,b
    init_vector_u8_i2(&a, &b, len); // dec : stack, single pointer
    printf("[%s %04d] a=%p, b=%p\n", __func__, __LINE__, (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_vmac_u8(&h_sum, a, b, len);

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    for (uint32_t i = 0; i < len; i++) {
        s_sum += (uint16_t)(a[i] * b[i]);
    }

    // 4. Compare results
    ret = (h_sum == s_sum) ? 1 : 0;
#ifdef CONFIG_DEBUG
    printf("ret=%s, h_sum=%d : s_sum=%d\n", ret ? "success" : "fail", h_sum, s_sum);
#endif
    if (result) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

#ifdef CONFIG_DEBUG
    printf("-----------------------------------------------------------------\n");
    printf("\t\tEXPECTED\t\t\t\tCOMPUTED\n");
    printf("-----------------------------------------------------------------\n");
    printf("\t\t\t%d\t\t\t\t\t\t%d\n", h_sum, s_sum);
#endif

    lab_print_result(result, name);

    // DBG_MID("Done!\n");

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);

    return result;
}

static int lab_test_inst_aermob_vmac16(void) {
    static const char *name = __func__;
    int result = 0;
    int ret = 0;
    uint16_t *a, *b;
    uint32_t h_sum = 0, s_sum = 0;

    // uint32_t len = VECTOR_SIZE_128;
    uint32_t len = VECTOR_SIZE_5;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom MAC16 Accelerator Test ---\n");
    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    // a,b
    init_vector_u16_i2(&a, &b, len); // dec : stack, single pointer
    printf("a=%p, b=%p\n", (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_vmac_u16(&h_sum, a, b, len); // change malloc

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    for (uint32_t i = 0; i < len; i++) {
        s_sum += a[i] * b[i];
    }

    // 4. Compare results
    ret = (h_sum == s_sum) ? 1 : 0;
#ifdef CONFIG_DEBUG
    printf("ret=%s, h_sum=%u : s_sum=%u\n", ret ? "success" : "fail", h_sum, s_sum);
#endif
    if (result) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

#ifdef CONFIG_DEBUG
    printf("-----------------------------------------------------------------\n");
    printf("\t\tEXPECTED\t\t\t\tCOMPUTED\n");
    printf("-----------------------------------------------------------------\n");
    printf("\t\t\t%d\t\t\t\t\t\t%d\n", h_sum, s_sum);
#endif

    lab_print_result(result, name);

    // DBG_MID("Done!\n");

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);

    return result;
}

static int lab_test_inst_aermob_vmac32(void) {
    static const char *name = __func__;
    int result = 0;
    int ret = 0;
    uint32_t *a, *b;
    // uint32_t len = VECTOR_SIZE_128;
    uint32_t len = VECTOR_SIZE_5;
    uint64_t h_sum = 0, s_sum = 0;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom MAC32 Accelerator Test ---\n");
    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    // a,b
    init_vector2(&a, &b, len); // dec : stack, single pointer
    printf("a=%p, b=%p\n", (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_vmac_u32(&h_sum, a, b, len); // change malloc

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    for (uint32_t i = 0; i < len; i++) {
        s_sum += a[i] * b[i];
    }

    // 4. Compare results
    ret = (h_sum == s_sum) ? 1 : 0;
#ifdef CONFIG_DEBUG
#ifdef CONFIG_SUPPORT_64
    printf("ret=%s, h_sum=0x%lx : s_sum=0x%lx\n", result ? "success" : "fail", h_sum, s_sum);
#else
    printf("ret=%s, h_sum=0x%llx : s_sum=0x%llx\n", result ? "success" : "fail", h_sum, s_sum);
#endif
#endif
    if (ret) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

#ifdef CONFIG_DEBUG
    printf("-----------------------------------------------------------------\n");
    printf("\t\tEXPECTED\t\t\t\tCOMPUTED\n");
    printf("-----------------------------------------------------------------\n");
#ifdef CONFIG_SUPPORT_64
    printf("\t\t\t%ld\t\t\t\t\t\t%ld\n", h_sum, s_sum);
#else
    printf("\t\t\t%lld\t\t\t\t\t\t%lld\n", h_sum, s_sum);
#endif
#endif

    lab_print_result(result, name);

    // DBG_MID("Done!\n");

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);

    return result;
}

static int lab_test_inst_aermob_vmac64(void) {
    static const char *name = __func__;
    int result = 0;
    int ret = 0;
    uint64_t *a, *b;
    // uint32_t len = VECTOR_SIZE_128;
    uint32_t len = VECTOR_SIZE_5;
    uint128_t h_sum = {0}, s_sum = {0};

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom MAC64 Accelerator Test ---\n");
    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    // a,b
    init_vector_u64_i2(&a, &b, len); // dec : stack, single pointer
    printf("a=%p, b=%p\n", (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_vmac_u64(&h_sum, a, b, len); // change malloc

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    // for (uint32_t i = 0; i < len; i++) {
    //     // s_sum += a[i] * b[i];
    //     s_sum = _add128(s_sum, lab_mul64_to_128(a[i], b[i]));
    // }
    s_sum = _mac64_arr(a, b, len);

    // 4. Compare results
    ret = (h_sum.u == s_sum.u && h_sum.l == s_sum.l) ? 1 : 0;

#ifdef CONFIG_DEBUG
#ifdef CONFIG_SUPPORT_64
    printf("ret=%s, h_sum=0x%lu:0x%lu : s_sum=0x%lu:0x%lu\n", result ? "success" : "fail", h_sum.u,
           h_sum.l, s_sum.u, s_sum.l);
#else
    printf("ret=%s, h_sum=0x%llu:0x%llu : s_sum=0x%llu:0x%llu\n", result ? "success" : "fail",
           h_sum.u, h_sum.l, s_sum.u, s_sum.l);
#endif
#endif
    if (ret) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

#ifdef CONFIG_DEBUG
    printf("-----------------------------------------------------------------\n");
    printf("\t\tEXPECTED\t\t\t\tCOMPUTED\n");
    printf("-----------------------------------------------------------------\n");
#ifdef CONFIG_SUPPORT_64
    printf("\t\t0x%lu:0x%lu\t\t\t\t0x%lu:0x%lu\n", h_sum.u, h_sum.l, s_sum.u, s_sum.l);
#else
    printf("\t\t0x%llu:0x%llu\t\t\t\t0x%llu:0x%llu\n", h_sum.u, h_sum.l, s_sum.u, s_sum.l);
#endif

#endif

    lab_print_result(result, name);

    // DBG_MID("Done!\n");

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);

    return result;
}

static int lab_test_inst_aermob_vmac128(void) {
    static const char *name = __func__;
    int result = 0;
    int ret = 0;
    uint128_t *a, *b;
    // uint32_t len = VECTOR_SIZE_128;
    uint32_t len = VECTOR_SIZE_5;
    uint256_t h_sum = {0}, s_sum = {0};

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom MAC128 Accelerator Test ---\n");
    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    // a,b
    init_vector_u128_i2(&a, &b, len); // dec : stack, single pointer
    printf("a=%p, b=%p\n", (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_vmac_u128(&h_sum, a, b, len); // change malloc

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    // for (uint32_t i = 0; i < len; i++) {
    //     s_sum = _add256(s_sum, lab_mul128_to_256(a[i], b[i]));
    // }
    s_sum = mac128_arr(a, b, len);

    // 4. Compare results
    ret = (h_sum.d[0] == s_sum.d[0] && h_sum.d[1] == s_sum.d[1] && h_sum.d[2] == s_sum.d[2] &&
           h_sum.d[3] == s_sum.d[3])
              ? 1
              : 0;

#ifdef CONFIG_DEBUG
#ifdef CONFIG_SUPPORT_64
    printf("ret=%s, h_sum=[0x%lx:0x%lx:0x%lx:0x%lx] : "
           "s_sum=[0x%lx:0x%lx:0x%lx:0x%lx]\n",
           ret ? "success" : "fail", h_sum.d[0], h_sum.d[1], h_sum.d[2], h_sum.d[3], s_sum.d[0],
           s_sum.d[1], s_sum.d[2], s_sum.d[3]);
#else
    printf("result=%s, h_sum=[0x%llx:0x%llx:0x%llx:0x%llx] : "
           "s_sum=[0x%llx:0x%llx:0x%llx:0x%llx]\n",
           ret ? "success" : "fail", h_sum.d[0], h_sum.d[1], h_sum.d[2], h_sum.d[3], s_sum.d[0],
           s_sum.d[1], s_sum.d[2], s_sum.d[3]);
#endif
#endif
    if (ret) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

#ifdef CONFIG_DEBUG
    printf("-----------------------------------------------------------------\n");
    printf("\t\tEXPECTED\t\t\t\t\tCOMPUTED\n");
    printf("-----------------------------------------------------------------\n");
#ifdef CONFIG_SUPPORT_64
    printf("\t\t[0x%lx:0x%lx:0x%lx:0x%lx]\t\t\t[0x%lx:0x%lx:0x%lx:"
           "0x%lx]\n",
           h_sum.d[0], h_sum.d[1], h_sum.d[2], h_sum.d[3], s_sum.d[0], s_sum.d[1], s_sum.d[2],
           s_sum.d[3]);
#else
    printf("\t\t[0x%llx:0x%llx:0x%llx:0x%llx]\t\t\t[0x%llx:0x%llx:0x%llx:"
           "0x%llx]\n",
           h_sum.d[0], h_sum.d[1], h_sum.d[2], h_sum.d[3], s_sum.d[0], s_sum.d[1], s_sum.d[2],
           s_sum.d[3]);
#endif
#endif

    lab_print_result(result, name);

    // DBG_MID("Done!\n");

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);

    return result;
}

// vmul
//
static int lab_test_inst_cp_vmul_u8(void) {
    static const char *name = __func__;
    vmath_status_t result = VMATH_SUCCESS;
    int ret = 0;
    uint8_t *a, *b;
    uint16_t *c_hw, *c_sw;
    // uint32_t len = VECTOR_SIZE_128;
    uint32_t len = VECTOR_SIZE_7;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom VMUL8 Accelerator Test ---\n");

    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    init_vector_u8_i2(&a, &b, len);        // input array
    init_vector_u16_o2(&c_hw, &c_sw, len); // output array
    printf("c=%p, a=%p, b=%p\n", (void *)c_hw, (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_vmul_u8(c_hw, a, b, len); // change malloc

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    for (uint32_t i = 0; i < len; i++) {
        c_sw[i] = a[i] * b[i];
    }

    // 4. Compare results
    if ((ret = verify_result_u16(c_hw, c_sw, len))) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

#ifdef CONFIG_DEBUG
    printf("-----------------------------------------------------------------\n");
    printf("\t\tEXPECTED\t\t\t\tCOMPUTED\n");
    printf("-----------------------------------------------------------------\n");
    for (uint32_t i = 0; i < len; i++) {
        printf("[%03u]\t%8d\t\t\t\t%8d\n", i, c_sw[i], c_hw[i]);
    }
#endif

    lab_print_result(result, name);

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);
    SAFE_FREE(c_hw);
    SAFE_FREE(c_sw);

    return result;
}

static int lab_test_inst_cp_vmul_u16(void) {
    static const char *name = __func__;
    vmath_status_t result = VMATH_SUCCESS;
    int ret = 0;
    uint16_t *a = NULL, *b = NULL;
    uint32_t *c_hw = NULL, *c_sw = NULL;
    // uint32_t len = VECTOR_SIZE_128;
    uint32_t len = VECTOR_SIZE_7;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom VMUL16 Accelerator Test ---\n");

    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    init_vector_u16_i2(&a, &b, len);       // input array
    init_vector_u32_o2(&c_hw, &c_sw, len); // output array
    printf("c=%p, a=%p, b=%p\n", (void *)c_hw, (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_vmul_u16(c_hw, a, b, len); // change malloc

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    for (uint32_t i = 0; i < len; i++) {
        c_sw[i] = a[i] * b[i];
    }

    // 4. Compare results
    if ((result = verify_result_u32(c_hw, c_sw, len)) == VMATH_SUCCESS) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

#ifdef CONFIG_DEBUG
    printf("-----------------------------------------------------------------\n");
    printf("\t\tEXPECTED\t\t\t\tCOMPUTED\n");
    printf("-----------------------------------------------------------------\n");
    for (uint32_t i = 0; i < len; i++) {
        printf("[%03u]\t%8d\t\t\t\t%8d\n", i, c_sw[i], c_hw[i]);
    }
#endif

    lab_print_result(result, name);

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);
    SAFE_FREE(c_hw);
    SAFE_FREE(c_sw);

    return result;
}

// vmul
//
static int lab_test_inst_cp_vmul_u32(void) {
    static const char *name = __func__;
    vmath_status_t result = VMATH_SUCCESS;
    int ret = 0;
    uint32_t *a, *b;
    uint64_t *c_hw, *c_sw;
    // uint32_t len = VECTOR_SIZE_128;
    uint32_t len = VECTOR_SIZE_7;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom VMUL32 Accelerator Test ---\n");

    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    init_vector3(&a, &b, len);       // input array
    init_vector4(&c_hw, &c_sw, len); // output array
    printf("c=%p, a=%p, b=%p\n", (void *)c_hw, (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_vmul_u32(c_hw, a, b, len); // change malloc

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    for (uint32_t i = 0; i < len; i++) {
        c_sw[i] = a[i] * b[i];
    }

    // 4. Compare results
    if ((ret = verify_result_u64(c_hw, c_sw, len))) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

#ifdef CONFIG_DEBUG
    printf("-----------------------------------------------------------------\n");
    printf("\t\tEXPECTED\t\t\t\tCOMPUTED\n");
    printf("-----------------------------------------------------------------\n");
    for (uint32_t i = 0; i < len; i++) {
#ifdef CONFIG_SUPPORT_64
        printf("[%03u]\t%8ld\t\t\t\t%8ld\n", i, c_sw[i], c_hw[i]);
#else
        printf("[%03u]\t%8lld\t\t\t\t%8lld\n", i, c_sw[i], c_hw[i]);
#endif
    }
#endif

    lab_print_result(result, name);

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);
    SAFE_FREE(c_hw);
    SAFE_FREE(c_sw);

    return result;
}

// vmul
//
static int lab_test_inst_cp_vmul_u64(void) {
    static const char *name = __func__;
    vmath_status_t result = VMATH_SUCCESS;
    int ret = 0;
    uint64_t *a, *b;
    uint128_t *c_hw, *c_sw;
    // uint32_t len = VECTOR_SIZE_128;
    uint32_t len = VECTOR_SIZE_7;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom VMUL64 Accelerator Test ---\n");

    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    init_vector_u64_i2(&a, &b, len);        // input array
    init_vector_u128_o2(&c_hw, &c_sw, len); // output array
    printf("c=%p, a=%p, b=%p\n", (void *)c_hw, (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_vmul_u64(c_hw, a, b, len); // change malloc

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    for (uint32_t i = 0; i < len; i++) {
        // c_sw[i] = a[i] * b[i];
        c_sw[i] = _mul64(a[i], b[i]);
    }

    // 4. Compare results
    if ((ret = verify_result_u128(c_hw, c_sw, len))) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

#ifdef CONFIG_DEBUG
    printf("-----------------------------------------------------------\n");
    printf("\t\t\t\tEXPECTED\t\t\t\t\tCOMPUTED\n");
    printf("-----------------------------------------------------------\n");
    for (uint32_t i = 0; i < len; i++) {
#ifdef CONFIG_SUPPORT_64
        printf("[%03u]\t\t0x%08ld:%08ld\t\t0x%08ld:%08ld\n", i, c_sw[i].u, c_sw[i].l, c_hw[i].u,
               c_hw[i].l);
#else
        printf("[%03u]\t\t0x%08lld:%08lld\t\t0x%08lld:%08lld\n", i, c_sw[i].u, c_sw[i].l, c_hw[i].u,
               c_hw[i].l);
#endif
    }
#endif

    lab_print_result(result, name);

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);
    SAFE_FREE(c_hw);
    SAFE_FREE(c_sw);

    return result;
}

static int lab_test_inst_cp_vmul_u128(void) {
    static const char *name = __func__;
    vmath_status_t result = VMATH_SUCCESS;
    int ret = 0;
    uint128_t *a, *b;
    uint256_t *c_hw, *c_sw;
    // uint32_t len = VECTOR_SIZE_128;
    uint32_t len = VECTOR_SIZE_7;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom VMUL128 Accelerator Test ---\n");

    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    init_vector_u128_i2(&a, &b, len);       // input array
    init_vector_u256_o2(&c_hw, &c_sw, len); // output array
    printf("c=%p, a=%p, b=%p\n", (void *)c_hw, (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_vmul_u128(c_hw, a, b, len); // change malloc

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    for (uint32_t i = 0; i < len; i++) {
        c_sw[i] = mul128(a[i], b[i]);
    }

    // 4. Compare results
    if ((ret = verify_result_u256(c_hw, c_sw, len))) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

#ifdef CONFIG_DEBUG
    printf("------------------------------------------------------------------------------\n");
    printf("\t\t\t\t\t\tEXPECTED\t\t\t\t\t\t\tCOMPUTED\n");
    printf("------------------------------------------------------------------------------\n");
    for (uint32_t i = 0; i < len; i++) {
#ifdef CONFIG_SUPPORT_64
        printf("[%03u] 0x%lx:0x%lx:0x%lx:0x%lx\t\t\t\t\t0x%lx:0x%lx:0x%lx:0x%lx\n", i, c_sw[i].d[3],
               c_sw[i].d[2], c_sw[i].d[1], c_sw[i].d[0], c_hw[i].d[3], c_hw[i].d[2], c_hw[i].d[1],
               c_hw[i].d[0]);
#else
        printf("[%03u] 0x%llx:0x%llx:0x%llx:0x%llx\t\t\t\t\t0x%llx:0x%llx:0x%llx:0x%llx\n", i,
               c_sw[i].d[3], c_sw[i].d[2], c_sw[i].d[1], c_sw[i].d[0], c_hw[i].d[3], c_hw[i].d[2],
               c_hw[i].d[1], c_hw[i].d[0]);
#endif
    }
#endif

    lab_print_result(result, name);

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);
    SAFE_FREE(c_hw);
    SAFE_FREE(c_sw);

    return result;
}

static int lab_test_inst_aermob_mtrx8(void) {
#define A_ROW_SIZ 2
#define A_COL_SIZ 3
#define B_ROW_SIZ 3
#define B_COL_SIZ 2

    static const char *name = __func__;
    vmath_status_t result = VMATH_SUCCESS;
    int ret = 0;
    uint8_t *a, *b;
    uint16_t *c_hw, *c_sw;
    // uint32_t len = VECTOR_SIZE_128;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom MTRX8 Accelerator Test ---\n");
    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    a = init_mat_u8(A_ROW_SIZ, A_COL_SIZ);     // input array
    b = init_mat_u8(B_ROW_SIZ, B_COL_SIZ);     // input array
    c_hw = init_mat_u16(A_ROW_SIZ, B_COL_SIZ); // output array
    c_sw = init_mat_u16(A_ROW_SIZ, B_COL_SIZ); // output array

    uint8_t (*pa)[A_COL_SIZ] = (void *)a;
    uint8_t (*pb)[B_COL_SIZ] = (void *)b;

    uint16_t (*ph)[B_COL_SIZ] = (void *)c_hw;
    uint16_t (*ps)[B_COL_SIZ] = (void *)c_sw;
    printf("c=%p, a=%p, b=%p\n", (void *)c_hw, (void *)a, (void *)b);

    // test
    printf("A::\n");
    for (int i = 0; i < A_ROW_SIZ; i++) {
        for (int j = 0; j < A_COL_SIZ; j++) {
            printf("%d ", pa[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("B::\n");
    for (int i = 0; i < B_ROW_SIZ; i++) {
        for (int j = 0; j < B_COL_SIZ; j++) {
            printf("%d ", pb[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_mtrx_u8(c_hw, a, b, A_ROW_SIZ, sizeof(pa[0]) / sizeof(pa[0][0]), B_ROW_SIZ,
                        sizeof(pb[0]) / sizeof(pb[0][0]));

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    // _matrix_mul8(c_sw, a, b, A_ROW_SIZ, sizeof(pa[0]) / sizeof(pa[0][0]), B_ROW_SIZ,
    //                 sizeof(pb[0]) / sizeof(pb[0][0]));
    lab_matrix_mul8b(c_sw, a, b, A_ROW_SIZ, sizeof(pa[0]) / sizeof(pa[0][0]), B_ROW_SIZ,
                     sizeof(pb[0]) / sizeof(pb[0][0]));

    // 4. Compare results
    if ((ret = verify_result_u16(c_hw, c_sw, A_ROW_SIZ * B_COL_SIZ))) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

#ifdef CONFIG_DEBUG
    printf("----------------------------------------------------------\n");
    printf("\t\t\tEXPECTED\t\t\t\t\tCOMPUTED\n");
    printf("----------------------------------------------------------\n");

    for (uint32_t i = 0; i < A_ROW_SIZ; i++) {
        for (uint32_t j = 0; j < B_COL_SIZ; j++) {
            printf("[%03u]\t\t%8u\t\t\t\t%8u\n", i, ps[i][j], ph[i][j]);
        }
    }
#endif

    lab_print_result(result, name);

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);
    SAFE_FREE(c_hw);
    SAFE_FREE(c_sw);

#undef A_ROW_SIZ
#undef A_COL_SIZ
#undef B_ROW_SIZ
#undef B_COL_SIZ

    return result;
}

static int lab_test_inst_aermob_mtrx16(void) {
#define A_ROW_SIZ 2
#define A_COL_SIZ 3
#define B_ROW_SIZ 3
#define B_COL_SIZ 2

    static const char *name = __func__;
    vmath_status_t result = VMATH_SUCCESS;
    int ret = 0;
    uint16_t *a, *b;
    uint32_t *c_hw, *c_sw;
    // uint32_t len = VECTOR_SIZE_128;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom MTRX16 Accelerator Test ---\n");
    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    a = init_mat_u16(A_ROW_SIZ, A_COL_SIZ);    // input array
    b = init_mat_u16(B_ROW_SIZ, B_COL_SIZ);    // input array
    c_hw = init_mat_u32(A_ROW_SIZ, B_COL_SIZ); // output array
    c_sw = init_mat_u32(A_ROW_SIZ, B_COL_SIZ); // output array

    uint16_t (*pa)[A_COL_SIZ] = (void *)a;
    uint16_t (*pb)[B_COL_SIZ] = (void *)b;

    uint32_t (*ph)[B_COL_SIZ] = (void *)c_hw;
    uint32_t (*ps)[B_COL_SIZ] = (void *)c_sw;
    printf("c=%p, a=%p, b=%p\n", (void *)c_hw, (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_mtrx_u16(c_hw, a, b, A_ROW_SIZ, sizeof(pa[0]) / sizeof(pa[0][0]), B_ROW_SIZ,
                         sizeof(pb[0]) / sizeof(pb[0][0]));

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    _matrix_mul16(c_sw, a, b, A_ROW_SIZ, sizeof(pa[0]) / sizeof(pa[0][0]), B_ROW_SIZ,
                  sizeof(pb[0]) / sizeof(pb[0][0]));

    // 4. Compare results
    if ((ret = verify_result_u32(c_hw, c_sw, A_ROW_SIZ * B_COL_SIZ))) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

#ifdef CONFIG_DEBUG
    printf("----------------------------------------------------------\n");
    printf("\t\t\tEXPECTED\t\t\t\t\tCOMPUTED\n");
    printf("----------------------------------------------------------\n");
    for (uint32_t i = 0; i < A_ROW_SIZ; i++) {
        for (uint32_t j = 0; j < B_COL_SIZ; j++) {
            printf("[%03u]\t\t%8u\t\t\t\t%8u\n", i, ps[i][j], ph[i][j]);
        }
    }
#endif

    lab_print_result(result, name);

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);
    SAFE_FREE(c_hw);
    SAFE_FREE(c_sw);

#undef A_ROW_SIZ
#undef A_COL_SIZ
#undef B_ROW_SIZ
#undef B_COL_SIZ

    return result;
}

static int lab_test_inst_aermob_mtrx32(void) {
#define A_ROW_SIZ 2
#define A_COL_SIZ 3
#define B_ROW_SIZ 3
#define B_COL_SIZ 2

    static const char *name = __func__;
    vmath_status_t result = VMATH_SUCCESS;
    int ret = 0;
    uint32_t *a, *b;
    uint64_t *c_hw, *c_sw;
    // uint32_t len = VECTOR_SIZE_128;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom MTRX32 Accelerator Test ---\n");
    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    a = init_mat_u32(A_ROW_SIZ, A_COL_SIZ);    // input array
    b = init_mat_u32(B_ROW_SIZ, B_COL_SIZ);    // input array
    c_hw = init_mat_u64(A_ROW_SIZ, B_COL_SIZ); // output array
    c_sw = init_mat_u64(A_ROW_SIZ, B_COL_SIZ); // output array

    uint32_t (*pa)[A_COL_SIZ] = (void *)a;
    uint32_t (*pb)[B_COL_SIZ] = (void *)b;

    uint64_t (*ph)[B_COL_SIZ] = (void *)c_hw;
    uint64_t (*ps)[B_COL_SIZ] = (void *)c_sw;
    printf("c=%p, a=%p, b=%p\n", (void *)c_hw, (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_mtrx_u32(c_hw, a, b, A_ROW_SIZ, sizeof(pa[0]) / sizeof(pa[0][0]), B_ROW_SIZ,
                         sizeof(pb[0]) / sizeof(pb[0][0]));

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    _matrix_mul32(c_sw, a, b, A_ROW_SIZ, sizeof(pa[0]) / sizeof(pa[0][0]), B_ROW_SIZ,
                  sizeof(pb[0]) / sizeof(pb[0][0]));
    // _matrix_mul32b(c_sw, a, b, A_ROW_SIZ, sizeof(pa[0]) / sizeof(pa[0][0]), B_ROW_SIZ,
    //                  sizeof(pb[0]) / sizeof(pb[0][0]));

    // 4. Compare results
    if ((ret = verify_result_u64a(c_hw, c_sw, A_ROW_SIZ, B_COL_SIZ))) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

#ifdef CONFIG_DEBUG
    printf("----------------------------------------------------------\n");
    printf("\t\t\tEXPECTED\t\t\t\t\tCOMPUTED\n");
    printf("----------------------------------------------------------\n");

    for (uint32_t i = 0; i < A_ROW_SIZ; i++) {
        for (uint32_t j = 0; j < B_COL_SIZ; j++) {
#ifdef CONFIG_SUPPORT_64
            printf("[%03u]\t\t%16lu\t\t\t\t%16lu\n", i, ps[i][j], ph[i][j]);
#else
            printf("[%03u]\t\t%16llu\t\t\t\t%16llu\n", i, ps[i][j], ph[i][j]);
#endif
        }
    }
#endif

    lab_print_result(result, name);

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);
    SAFE_FREE(c_hw);
    SAFE_FREE(c_sw);

#undef A_ROW_SIZ
#undef A_COL_SIZ
#undef B_ROW_SIZ
#undef B_COL_SIZ

    return result;
}

static int lab_test_inst_aermob_mtrx64(void) {
#define A_ROW_SIZ 2
#define A_COL_SIZ 3
#define B_ROW_SIZ 3
#define B_COL_SIZ 2

    static const char *name = __func__;
    vmath_status_t result = VMATH_SUCCESS;
    int ret = 0;
    uint64_t *a, *b;
    uint128_t *c_hw, *c_sw;
    // uint32_t len = VECTOR_SIZE_128;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom MTRX64 Accelerator Test ---\n");
    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    a = init_mat_u64(A_ROW_SIZ, A_COL_SIZ);     // input array
    b = init_mat_u64(B_ROW_SIZ, B_COL_SIZ);     // input array
    c_hw = init_mat_u128(A_ROW_SIZ, B_COL_SIZ); // output array
    c_sw = init_mat_u128(A_ROW_SIZ, B_COL_SIZ); // output array

    uint64_t (*pa)[A_COL_SIZ] = (void *)a;
    uint64_t (*pb)[B_COL_SIZ] = (void *)b;

    uint128_t(*ph)[B_COL_SIZ] = (void *)c_hw;
    uint128_t(*ps)[B_COL_SIZ] = (void *)c_sw;
    printf("c=%p, a=%p, b=%p\n", (void *)c_hw, (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_mtrx_u64(c_hw, a, b, A_ROW_SIZ, sizeof(pa[0]) / sizeof(pa[0][0]), B_ROW_SIZ,
                         sizeof(pb[0]) / sizeof(pb[0][0]));

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    // _matrix_mul64(c_sw, a, b, A_ROW_SIZ, sizeof(pa[0]) / sizeof(pa[0][0]), B_ROW_SIZ,
    //                  sizeof(pb[0]) / sizeof(pb[0][0]));
    _matrix_mul64b(c_sw, a, b, A_ROW_SIZ, sizeof(pa[0]) / sizeof(pa[0][0]), B_ROW_SIZ,
                   sizeof(pb[0]) / sizeof(pb[0][0]));

    // 4. Compare results
    if ((ret = verify_result_u128(c_hw, c_sw, A_ROW_SIZ * B_COL_SIZ))) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

#ifdef CONFIG_DEBUG
    printf("----------------------------------\n");
    printf("\t\t\tEXPECTED\t\t\tCOMPUTED\n");
    printf("----------------------------------\n");

    for (uint32_t i = 0; i < A_ROW_SIZ; i++) {
        for (uint32_t j = 0; j < B_COL_SIZ; j++) {
#ifdef CONFIG_SUPPORT_64
            printf("[%03u]\t\t%ld:%ld\t%ld:%ld\n", i, ps[i][j].u, ps[i][j].l, ph[i][j].u,
                   ph[i][j].l);
#else
            printf("[%03u]\t\t%lld:%lld\t%lld:%lld\n", i, ps[i][j].u, ps[i][j].l, ph[i][j].u,
                   ph[i][j].l);
#endif
        }
    }
#endif

    lab_print_result(result, name);

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);
    SAFE_FREE(c_hw);
    SAFE_FREE(c_sw);

#undef A_ROW_SIZ
#undef A_COL_SIZ
#undef B_ROW_SIZ
#undef B_COL_SIZ

    return result;
}

static int lab_test_inst_aermob_mtrx128(void) {
#define A_ROW_SIZ 2
#define A_COL_SIZ 3
#define B_ROW_SIZ 3
#define B_COL_SIZ 2

    static const char *name = __func__;
    vmath_status_t result = VMATH_SUCCESS;
    int ret = 0;
    uint128_t *a, *b;
    uint256_t *c_hw, *c_sw;
    // uint32_t len = VECTOR_SIZE_128;

    printf("\n==================================================================\n");
    printf("--- RISC-V Custom MTRX128 Accelerator Test ---\n");
    // 0. initialize hw
    lab_test_init_hw();

    // 1. prepare data
    a = init_mat_u128(A_ROW_SIZ, A_COL_SIZ);    // input array
    b = init_mat_u128(B_ROW_SIZ, B_COL_SIZ);    // input array
    c_hw = init_mat_u256(A_ROW_SIZ, B_COL_SIZ); // output array
    c_sw = init_mat_u256(A_ROW_SIZ, B_COL_SIZ); // output array

    uint128_t(*pa)[A_COL_SIZ] = (void *)a;
    uint128_t(*pb)[B_COL_SIZ] = (void *)b;

    uint256_t(*ph)[B_COL_SIZ] = (void *)c_hw;
    uint256_t(*ps)[B_COL_SIZ] = (void *)c_sw;
    printf("c=%p, a=%p, b=%p\n", (void *)c_hw, (void *)a, (void *)b);

    // 2. call custom instruction
    printf("Executing Accelerator...\n");
    result = cp_mtrx_u128(c_hw, a, b, A_ROW_SIZ, sizeof(pa[0]) / sizeof(pa[0][0]), B_ROW_SIZ,
                          sizeof(pb[0]) / sizeof(pb[0][0]));

    // 3. CPU(Golden Model) operation (for verification)
    printf("Executing Golden Model (CPU)...\n");
    // _matrix_mul128(c_sw, a, b, A_ROW_SIZ, sizeof(pa[0]) / sizeof(pa[0][0]), B_ROW_SIZ,
    //                   sizeof(pb[0]) / sizeof(pb[0][0]));
    _matrix_mul128b(c_sw, a, b, A_ROW_SIZ, sizeof(pa[0]) / sizeof(pa[0][0]), B_ROW_SIZ,
                    sizeof(pb[0]) / sizeof(pb[0][0]));

    // 4. Compare results
    if ((ret = verify_result_u256(c_hw, c_sw, A_ROW_SIZ * B_COL_SIZ))) {
        printf("[VMATH_SUCCESS] All vector operations are correct!\n");
    } else {
        printf("[FAIL]!\n");
    }

#ifdef CONFIG_DEBUG
    printf("----------------------------------------------------------\n");
    printf("\t\t\tEXPECTED\t\t\t\t\tCOMPUTED\n");
    printf("----------------------------------------------------------\n");

    for (uint32_t i = 0; i < A_ROW_SIZ; i++) {
        for (uint32_t j = 0; j < B_COL_SIZ; j++) {
#ifdef CONFIG_SUPPORT_64
            printf("[%03u]     %ld:%ld:%ld:%ld    "
                   "%ld:%ld:%ld:%ld\n",
                   i, ps[i][j].d[3], ps[i][j].d[2], ps[i][j].d[1], ps[i][j].d[0], ph[i][j].d[3],
                   ph[i][j].d[2], ph[i][j].d[1], ph[i][j].d[0]);
#else
            printf("[%03u]     %lld:%lld:%lld:%lld    "
                   "%lld:%lld:%lld:%lld\n",
                   i, ps[i][j].d[3], ps[i][j].d[2], ps[i][j].d[1], ps[i][j].d[0], ph[i][j].d[3],
                   ph[i][j].d[2], ph[i][j].d[1], ph[i][j].d[0]);
#endif
        }
    }
#endif

    lab_print_result(result, name);

    // free memory
    SAFE_FREE(a);
    SAFE_FREE(b);
    SAFE_FREE(c_hw);
    SAFE_FREE(c_sw);

#undef A_ROW_SIZ
#undef A_COL_SIZ
#undef B_ROW_SIZ
#undef B_COL_SIZ

    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
static const LAB_TEST_INST_FUNCTION_DEF custom_inst_lists[] = {
    {LAB_INST_AERMOB_VADD8, lab_test_inst_aermob_vadd_u8, "vector add(uint8_t)"},
    {LAB_INST_AERMOB_VADD16, lab_test_inst_aermob_vadd_u16, "vector add(uint16_t)"},
    {LAB_INST_AERMOB_VADD32, lab_test_inst_aermob_vadd_u32, "vector add(uint32_t)"},
    {LAB_INST_AERMOB_VADD64, lab_test_inst_aermob_vadd_u64, "vector add(uint64_t)"},
    {LAB_INST_AERMOB_VADD128, lab_test_inst_aermob_vadd_u128, "vector add(uint128_t)"},
    {LAB_INST_AERMOB_MAC8, lab_test_inst_aermob_vmac8, "vector multiply and addition"},
    {LAB_INST_AERMOB_MAC16, lab_test_inst_aermob_vmac16, "vector multiply and addition"},
    {LAB_INST_AERMOB_MAC32, lab_test_inst_aermob_vmac32, "vector multiply and addition"},
    {LAB_INST_AERMOB_MAC64, lab_test_inst_aermob_vmac64, "vector multiply and addition"},
    {LAB_INST_AERMOB_MAC128, lab_test_inst_aermob_vmac128, "vector multiply and addition"},
    {LAB_INST_AERMOB_VMUL8, lab_test_inst_cp_vmul_u8, "vector multiply and addition"},
    {LAB_INST_AERMOB_VMUL16, lab_test_inst_cp_vmul_u16, "vector multiply and addition"},
    {LAB_INST_AERMOB_VMUL32, lab_test_inst_cp_vmul_u32, "vector multiply and addition"},
    {LAB_INST_AERMOB_VMUL64, lab_test_inst_cp_vmul_u64, "vector multiply and addition"},
    {LAB_INST_AERMOB_VMUL128, lab_test_inst_cp_vmul_u128, "vector multiply and addition"},
    {LAB_INST_AERMOB_MTRX8, lab_test_inst_aermob_mtrx8, "matrix multiplication"},
    {LAB_INST_AERMOB_MTRX16, lab_test_inst_aermob_mtrx16, "matrix multiplication"},
    {LAB_INST_AERMOB_MTRX32, lab_test_inst_aermob_mtrx32, "matrix multiplication"},
    {LAB_INST_AERMOB_MTRX64, lab_test_inst_aermob_mtrx64, "matrix multiplication"},
    {LAB_INST_AERMOB_MTRX128, lab_test_inst_aermob_mtrx128, "matrix multiplication"},
};

void lab_do_all_custom_inst_test(void) {
    int result = 0;
    uint32_t siz = (uint32_t)(sizeof(custom_inst_lists) / sizeof(custom_inst_lists[0]));
    // printf("\nTesting for Custom Instruction operation(%zu)::\n",
    //        sizeof(custom_inst_lists) / sizeof(custom_inst_lists[0]));

    for (uint32_t i = 0; i < siz; i++) {
        result = custom_inst_lists[i].test_inst_func();
        printf("[%u] result=%d\n", i, result);
    }
}
