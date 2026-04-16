#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

#include "hal_rvv.h"

#define EPSILON 0.0001f

static int hal_verify_result_i32(const int32_t a, const int32_t b) {
    return a == b;
}

static int hal_verify_result_u32(const uint32_t a, const uint32_t b) {
    return a == b;
}

static int hal_verify_result_f32(const float a, const float b) {
    return (fabsf(a - b) < EPSILON);
}

static int hal_verify_array_i32(const int32_t *arr1, const int32_t *arr2, const size_t n) {
    int result = 0;

    if (arr1 == NULL || arr2 == NULL)
        goto RET;

    if (n == 0) {
        result = 1;
        goto RET;
    }

    for (size_t i = 0; i < n; i++) {
        if (arr1[i] != arr2[i]) {
            goto RET;
        }
    }
    result = 1;

RET:
    return result;
}

static int hal_verify_array_u32(const uint32_t *arr1, const uint32_t *arr2, const size_t n) {
    int result = 0;

    if (arr1 == NULL || arr2 == NULL)
        goto RET;

    if (n == 0) {
        result = 1;
        goto RET;
    }

    for (size_t i = 0; i < n; i++) {
        if (arr1[i] != arr2[i]) {
            goto RET;
        }
    }
    result = 1;

RET:
    return result;
}


static int hal_verify_array_f32(const float *arr1, const float *arr2, const size_t n) {
    int result = 0;

    if (arr1 == NULL || arr2 == NULL) {
        goto RET;
    }

    if (n == 0) {
        result = 1;
        goto RET;
    }

    for (size_t i = 0; i < n; i++) {
        if (fabsf(arr1[i] - arr2[i]) > EPSILON) {
            result = 0;
            goto RET;
        }
    }
    result = 1;

RET:
    return result;
}


// 하드웨어 유닛 전원 켜기 (베어메탈 전용)
static void enable_hardware_units() {
    unsigned int mstatus;
    // RISC-V mstatus 레지스터 읽기
    asm volatile("csrr %0, mstatus" : "=r"(mstatus));
    
    // FS 비트(13,14)와 VS 비트(9,10)를 Dirty(3) 상태로 켜기
    mstatus |= (3 << 13) | (3 << 9); 
    
    // 변경된 값을 mstatus 레지스터에 쓰기
    asm volatile("csrw mstatus, %0" : : "r"(mstatus));
}

static void hal_rvv_test_int32_add_1(void) {
    printf("\n[%s]\n", __func__);
    const size_t LEN = 10;
    int32_t a[10] = {-1, -2, -3, -4, 5, -6, -7, -8, -9, -10};
    int32_t b[10] = {-10, -20, -30, -40, 50, -60, -70, -80, -90, -100};
    int32_t c_rvv[10] = {0};
    int32_t c_sw[10] = {-11, -22, -33, -44, 55, -66, -77, -88, -99, -110};

    hal_rvv_add_i32(c_rvv, a, b, LEN);

    int ret = hal_verify_array_i32(c_sw, c_rvv, LEN);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    printf("=====================================\n");
    printf("\tS/W\t\t\t\tRVV\n");
    printf("-------------------------------------\n");
    for (int i=0; i<LEN; ++i){
        printf("%d\t\t%d\t: %s\n", c_sw[i], c_rvv[i], hal_verify_result_i32(c_sw[i], c_rvv[i]) ? "OK" : "Bad");
    }
    printf("-------------------------------------\n\n");
}


static void hal_rvv_test_uint32_add_1(void) {
    printf("\n[%s]\n", __func__);
    const size_t LEN = 10;
    uint32_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint32_t b[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    uint32_t c_rvv[10] = {0};
    uint32_t c_sw[10] = {11, 22, 33, 44, 55, 66, 77, 88, 99, 110};

    hal_rvv_add_u32(c_rvv, a, b, LEN);

    int ret = hal_verify_array_u32(c_sw, c_rvv, LEN);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    printf("=====================================\n");
    printf("\tS/W\t\t\t\tRVV\n");
    printf("-------------------------------------\n");
    for (int i=0; i<LEN; ++i){
        printf("%d\t\t%d\t: %s\n", c_sw[i], c_rvv[i], hal_verify_result_u32(c_sw[i], c_rvv[i]) ? "OK" : "Bad");
    }
    printf("-------------------------------------\n\n");

}


static void hal_rvv_test_int32_sub_1(void) {
    printf("\n[%s]\n", __func__);
    const size_t LEN = 10;
    int32_t a[10] = {-1, -2, -3, -4, 5, -6, -7, -8, -9, -10};
    int32_t b[10] = {-10, -20, -30, -40, 50, -60, -70, -80, -90, -100};
    int32_t c_rvv[10] = {0};
    int32_t c_sw[10] = {9, 18, 27, 36, -45, 54, 63, 72, 81, 90};

    hal_rvv_sub_i32(c_rvv, a, b, LEN);

    int ret = hal_verify_array_i32(c_sw, c_rvv, LEN);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    printf("=====================================\n");
    printf("\tS/W\t\t\tRVV\n");
    printf("-------------------------------------\n");
    for (int i=0; i<LEN; ++i){
        printf("\t%d\t\t\t%d\t\t: %s\n", c_sw[i], c_rvv[i], hal_verify_result_i32(c_sw[i], c_rvv[i]) ? "OK" : "Bad");
    }
    printf("-------------------------------------\n\n");

}


static void hal_rvv_test_uint32_sub_1(void) {
    printf("\n[%s]\n", __func__);
    const size_t LEN = 10;
    uint32_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint32_t b[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    uint32_t c_rvv[10] = {0};
    uint32_t c_sw[10] = {-9, -18, -27, -36, -45, -54, -63, -72, -81, -90};

    hal_rvv_sub_u32(c_rvv, a, b, LEN);

    int ret = hal_verify_array_u32(c_sw, c_rvv, LEN);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    printf("=====================================\n");
    printf("\tS/W\t\t\t\tRVV\n");
    printf("-------------------------------------\n");
    for (int i=0; i<LEN; ++i){
        printf("%d\t\t%d\t: %s\n", c_sw[i], c_rvv[i], hal_verify_result_u32(c_sw[i], c_rvv[i]) ? "OK" : "Bad");
    }
    printf("-------------------------------------\n\n");

}


static void hal_rvv_test_int32_mul_1(void) {
    printf("\n[%s]\n", __func__);
    const size_t LEN = 10;
    int32_t a[10] = {-1, -2, -3, 4, 5, 6, -7, -8, -9, 10};
    int32_t b[10] = {10, 20, -30, -40, 50, 60, -70, -80, 90, 100};
    int32_t c_rvv[10] = {0};
    int32_t c_sw[10] = {-10, -40, 90, -160, 250, 360, 490, 640, -810, 1000};

    hal_rvv_mul_i32(c_rvv, a, b, LEN);

    int ret = hal_verify_array_i32(c_sw, c_rvv, LEN);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    printf("=====================================\n");
    printf("\tS/W\t\t\tRVV\n");
    printf("-------------------------------------\n");
    for (int i=0; i<LEN; ++i){
        printf("\t%d\t\t\t%d\t\t: %s\n", c_sw[i], c_rvv[i], hal_verify_result_i32(c_sw[i], c_rvv[i]) ? "OK" : "Bad");
    }
    printf("-------------------------------------\n\n");

}

static void hal_rvv_test_uint32_mul_1(void) {
    printf("\n[%s]\n", __func__);
    const size_t LEN = 10;
    int32_t a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int32_t b[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int32_t c_rvv[10] = {0};
    int32_t c_sw[10] = {10, 40, 90, 160, 250, 360, 490, 640, 810, 1000};

    hal_rvv_mul_u32(c_rvv, a, b, LEN);

    int ret = hal_verify_array_u32(c_sw, c_rvv, LEN);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    printf("=====================================\n");
    printf("\tS/W\t\t\t\tRVV\n");
    printf("-------------------------------------\n");
    for (int i=0; i<LEN; ++i){
        printf("%d\t\t%d\t: %s\n", c_sw[i], c_rvv[i], hal_verify_result_u32(c_sw[i], c_rvv[i]) ? "OK" : "Bad");
    }
    printf("-------------------------------------\n\n");

}


void hal_rvv_test_int32_mac_1() {
    const size_t LEN = 5;
    
    int32_t a[5] = {1, 2, 3, 4, 5};
    int32_t b[5] = {5, 5, 5, 5, 5};
    
    // C 배열에 이미 10.0f 라는 값이 들어있다고 가정합니다.
    int32_t c_rvv[5] = {10, 10, 10, 10, 10};
    int32_t c_sw[5] = {15, 20, 25, 30, 35}; 

    // 연산 수행: C[i] = C[i] + (A[i] * B[i])
    hal_rvv_mac_i32(c_rvv, a, b, LEN);

    int ret = hal_verify_array_i32(c_sw, c_rvv, LEN);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    printf("=====================================\n");
    printf("\tS/W\t\t\tRVV\n");
    printf("-------------------------------------\n");
    for (int i=0; i<LEN; ++i){
        printf("\t%d\t\t\t%d\t\t: %s\n", c_sw[i], c_rvv[i], hal_verify_result_i32(c_sw[i], c_rvv[i]) ? "OK" : "Bad");
    }
    printf("-------------------------------------\n\n");    
}


void hal_rvv_test_uint32_mac_1() {
    const size_t LEN = 5;
    
    uint32_t a[5] = {1, 2, 3, 4, 5};
    uint32_t b[5] = {5, 5, 5, 5, 5};
    
    // C 배열에 이미 10.0f 라는 값이 들어있다고 가정합니다.
    uint32_t c_rvv[5] = {10, 10, 10, 10, 10};
    uint32_t c_sw[5] = {15, 20, 25, 30, 35}; 

    // 연산 수행: C[i] = C[i] + (A[i] * B[i])
    hal_rvv_mac_u32(c_rvv, a, b, LEN);

    int ret = hal_verify_array_u32(c_sw, c_rvv, LEN);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    printf("=====================================\n");
    printf("\tS/W\t\t\tRVV\n");
    printf("-------------------------------------\n");
    for (int i=0; i<LEN; ++i){
        printf("\t%u\t\t\t%u\t\t: %s\n", c_sw[i], c_rvv[i], hal_verify_result_i32(c_sw[i], c_rvv[i]) ? "OK" : "Bad");
    }
    printf("-------------------------------------\n\n");    
}


static void hal_rvv_test_int32_div_1(void) {
    printf("\n[%s]\n", __func__);
    const size_t LEN = 10;
    int32_t a[10] = {1, 19, -15, -12, 12, 19, -23, -29, 19, 120};
    int32_t b[10] = {1, 2, 3, 4, 5, -6, -7, -8, -9, -10};
    int32_t c_rvv[10] = {0};
    int32_t c_sw[10] = {1, 9, -5, -3, 2, -3, 3, 3, -2, -12};

    hal_rvv_div_i32(c_rvv, a, b, LEN);

    int ret = hal_verify_array_i32(c_sw, c_rvv, LEN);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    printf("=====================================\n");
    printf("\tS/W\t\t\tRVV\n");
    printf("-------------------------------------\n");
    for (int i=0; i<LEN; ++i){
        printf("\t%d\t\t\t%d\t\t: %s\n", c_sw[i], c_rvv[i], hal_verify_result_i32(c_sw[i], c_rvv[i]) ? "OK" : "Bad");
    }
    printf("-------------------------------------\n\n");

}

static void hal_rvv_test_uint32_div_1(void) {
    printf("\n[%s]\n", __func__);
    const size_t LEN = 10;
    int32_t a[10] = {1, 19, -15, -12, 12, 19, -23, -29, 19, 120};
    int32_t b[10] = {1, 2, 3, 4, 5, -6, -7, -8, -9, -10};
    int32_t c_rvv[10] = {0};
    int32_t c_sw[10] = {1, 9, -5, -3, 2, -3, 3, 3, -2, -12};

    hal_rvv_div_u32(c_rvv, a, b, LEN);

    int ret = hal_verify_array_u32(c_sw, c_rvv, LEN);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    printf("=====================================\n");
    printf("\tS/W\t\t\t\tRVV\n");
    printf("-------------------------------------\n");
    for (int i=0; i<LEN; ++i){
        printf("%d\t\t%d\t: %s\n", c_sw[i], c_rvv[i], hal_verify_result_u32(c_sw[i], c_rvv[i]) ? "OK" : "Bad");
    }
    printf("-------------------------------------\n\n");

}


void hal_rvv_test_int32_mtrx_dot_1(void) {
    const size_t LEN = 5;
    const int32_t expected_val = 70;

    int32_t a[5] = {1, 2, 3, 4, 5};
    int32_t b[5] = {2, 3, 4, 5, 6};
    
    // 결과를 저장할 단일 스칼라 변수
    int32_t result = 0;
    printf("\n[%s]\n", __func__);

    // 내적 연산 수행
    hal_rvv_dot_i32(&result, a, b, LEN);
    int ret = hal_verify_result_i32(expected_val, result);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    // 수동 계산 결과와 맞는지 검증
    // (1*2) + (2*3) + (3*4) + (4*5) + (5*6) 
    // = 2 + 6 + 12 + 20 + 30 = 70.0f
    printf("=====================================\n");
    printf("\tS/W\t\t\tRVV\n");
    printf("-------------------------------------\n");
    printf("\t%d\t\t\t%d\n", expected_val, result);
    printf("-------------------------------------\n\n");

}


void hal_rvv_test_uint32_mtrx_dot_1(void) {
    const size_t LEN = 5;
    const uint32_t expected_val = 70;

    uint32_t a[5] = {1, 2, 3, 4, 5};
    uint32_t b[5] = {2, 3, 4, 5, 6};
    
    // 결과를 저장할 단일 스칼라 변수
    uint32_t result = 0;
    printf("\n[%s]\n", __func__);

    // 내적 연산 수행
    hal_rvv_dot_u32(&result, a, b, LEN);
    int ret = hal_verify_result_u32(expected_val, result);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    // 수동 계산 결과와 맞는지 검증
    // (1*2) + (2*3) + (3*4) + (4*5) + (5*6) 
    // = 2 + 6 + 12 + 20 + 30 = 70.0f
    printf("=====================================\n");
    printf("\tS/W\t\t\tRVV\n");
    printf("-------------------------------------\n");
    printf("\t%d\t\t\t%d\n", expected_val, result);
    printf("-------------------------------------\n\n");

}

void hal_rvv_test_int32_mtrx_mul_1(void) {
    size_t M=2, N=2, K=3;

    int32_t a[6] = {1, 2, 3, 
                  4, 5, 6};
    int32_t b[6] = {7, 8, 
                  9, 10, 
                  11, 12};
    int32_t c_sw[4] = {58, 64, 139, 154};
    int32_t c_rvv[4] = {0};

    printf("\n[%s]\n", __func__);

    hal_rvv_matrix_mul_i32(c_rvv, a, b, M, N, K);
    int ret = hal_verify_array_i32(c_sw, c_rvv, M*N);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    // 수동 계산 결과와 맞는지 검증
    // (1*2) + (2*3) + (3*4) + (4*5) + (5*6) 
    // = 2 + 6 + 12 + 20 + 30 = 70.0f
    printf("=====================================\n");
    printf("\tS/W\t\t\t\tRVV\n");
    printf("-------------------------------------\n");
    for (int i=0; i < M*N; ++i){
        printf("\t%d\t\t\t%d\t\t\t: %s\n", c_sw[i], c_rvv[i], hal_verify_result_i32(c_sw[i], c_rvv[i]) ? "OK" : "Bad");
    }
    printf("-------------------------------------\n\n");
}

void hal_rvv_test_uint32_mtrx_mul_1(void) {
    size_t M=2, N=2, K=3;

    uint32_t a[6] = {1, 2, 3, 
                  4, 5, 6};
    uint32_t b[6] = {7, 8, 
                  9, 10, 
                  11, 12};
    uint32_t c_sw[4] = {58, 64, 139, 154};
    uint32_t c_rvv[4] = {0};

    printf("\n[%s]\n", __func__);

    hal_rvv_matrix_mul_u32(c_rvv, a, b, M, N, K);
    int ret = hal_verify_array_u32(c_sw, c_rvv, M*N);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    // 수동 계산 결과와 맞는지 검증
    // (1*2) + (2*3) + (3*4) + (4*5) + (5*6) 
    // = 2 + 6 + 12 + 20 + 30 = 70.0f
    printf("=====================================\n");
    printf("\tS/W\t\t\tRVV\n");
    printf("-------------------------------------\n");
    for (int i=0; i < M*N; ++i){
        printf("\t%u\t\t\t%u\t\t\t: %s\n", c_sw[i], c_rvv[i], hal_verify_result_i32(c_sw[i], c_rvv[i]) ? "OK" : "Bad");
    }
    printf("-------------------------------------\n\n");

}


static void hal_rvv_test_float_add_1(void) {
    printf("\n[%s]\n", __func__);
    const size_t LEN = 10;
    float a[10] = {1.1, 2.1, 3.1, 4.1, 5.1, 6.1, 7.1, 8.1, 9.1, 10.1};
    float b[10] = {10.2, 20.2, 30.2, 40.2, 50.2, 60.2, 70.2, 80.2, 90.2, 100.2};
    float c_rvv[10] = {0};
    float c_sw[10] = {11.3, 22.3, 33.3, 44.3, 55.3, 66.3, 77.3, 88.3, 99.3, 110.3};

    hal_rvv_add_f32(c_rvv, a, b, LEN);

    int ret = hal_verify_array_f32(c_sw, c_rvv, LEN);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    printf("=====================================\n");
    printf("\tS/W\t\t\t\tRVV\n");
    printf("-------------------------------------\n");
    for (int i=0; i<LEN; ++i){
        printf("%f\t\t%f\t: %s\n", c_sw[i], c_rvv[i], hal_verify_result_f32(c_sw[i], c_rvv[i]) ? "OK" : "Bad");
    }
    printf("-------------------------------------\n\n");
}

static void hal_rvv_test_float_sub_1(void) {
    printf("\n[%s]\n", __func__);
    const size_t LEN = 10;
    float a[10] = {1.1, 2.1, 3.1, 4.1, 5.1, 6.1, 7.1, 8.1, 9.1, 10.1};
    float b[10] = {10.2, 20.2, 30.2, 40.2, 50.2, 60.2, 70.2, 80.2, 90.2, 100.2};
    float c_rvv[10] = {0};
    float c_sw[10] = {-9.1, -18.1, -27.1, -36.1, -45.1, -54.1, -63.1, -72.1, -81.1, -90.1};

    hal_rvv_sub_f32(c_rvv, a, b, LEN);

    int ret = hal_verify_array_f32(c_sw, c_rvv, LEN);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    printf("=====================================\n");
    printf("\tS/W\t\t\t\tRVV\n");
    printf("-------------------------------------\n");
    for (int i=0; i<LEN; ++i){
        printf("%f\t\t%f\t: %s\n", c_sw[i], c_rvv[i], hal_verify_result_f32(c_sw[i], c_rvv[i]) ? "OK" : "Bad");
    }
    printf("-------------------------------------\n\n");
}

static void hal_rvv_test_float_mul_1(void) {
    printf("\n[%s]\n", __func__);
    const size_t LEN = 10;
    float a[10] = {1.1, 2.1, 3.1, 4.1, 5.1, 6.1, 7.1, 8.1, 9.1, 10.1};
    float b[10] = {10.2, 20.2, 30.2, 40.2, 50.2, 60.2, 70.2, 80.2, 90.2, 100.2};
    float c_rvv[10] = {0};
    float c_sw[10] = {11.22, 42.42, 93.62, 164.82, 256.02, 367.22, 498.42, 649.62, 820.82, 1012.02};

    hal_rvv_mul_f32(c_rvv, a, b, LEN);

    int ret = hal_verify_array_f32(c_sw, c_rvv, LEN);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    printf("=====================================\n");
    printf("\tS/W\t\t\t\tRVV\n");
    printf("-------------------------------------\n");
    for (int i=0; i<LEN; ++i){
        printf("%f\t\t%f\t: %s\n", c_sw[i], c_rvv[i], hal_verify_result_f32(c_sw[i], c_rvv[i]) ? "OK" : "Bad");
    }
    printf("-------------------------------------\n\n");
}

static void hal_rvv_test_float_div_1(void) {
    printf("\n[%s]\n", __func__);
    const size_t LEN = 10;
    float a[10] = {1.1, 2.1, 3.1, 4.1, 5.1, 6.1, 7.1, 8.1, 9.1, 10.1};
    float b[10] = {10.2, 20.2, 30.2, 40.2, 50.2, 60.2, 70.2, 80.2, 90.2, 100.2};
    float c_rvv[10] = {0};
    float c_sw[10] = {0.107843137, 0.103960396, 0.102649007, 0.10199005, 0.101593625, 0.101328904, 0.101139601, 0.100997506, 0.100886918, 0.100798403};

    hal_rvv_div_f32(c_rvv, a, b, LEN);

    int ret = hal_verify_array_f32(c_sw, c_rvv, LEN);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    printf("=====================================\n");
    printf("\tS/W\t\t\t\tRVV\n");
    printf("-------------------------------------\n");
    for (int i=0; i<LEN; ++i){
        printf("%f\t\t%f\t: %s\n", c_sw[i], c_rvv[i], hal_verify_result_f32(c_sw[i], c_rvv[i]) ? "OK" : "Bad");
    }
    printf("-------------------------------------\n\n");
}


void hal_rvv_test_float_mtrx_dot_1(void) {
    const size_t LEN = 5;
    const float expected_val = 70.0f;

    float a[5] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    float b[5] = {2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    
    // 결과를 저장할 단일 스칼라 변수
    float result = 0.0f;
    printf("\n[%s]\n", __func__);

    // 내적 연산 수행
    hal_rvv_dot_f32(&result, a, b, LEN);
    int ret = hal_verify_result_f32(expected_val, result);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    // 수동 계산 결과와 맞는지 검증
    // (1*2) + (2*3) + (3*4) + (4*5) + (5*6) 
    // = 2 + 6 + 12 + 20 + 30 = 70.0f
    printf("=====================================\n");
    printf("\tS/W\t\t\t\tRVV\n");
    printf("-------------------------------------\n");
    printf("%f\t\t%f\n", expected_val, result);
    printf("-------------------------------------\n\n");

}


void hal_rvv_test_float_mtrx_mul_1(void) {
    const size_t LEN = 5;
    const float expected_val = 70.0f;
    size_t M=2, N=2, K=3;

    float a[6] = {1, 2, 3, 
                  4, 5, 6};
    float b[6] = {7, 8, 
                  9, 10, 
                  11, 12};
    float c_sw[4] = {58, 64, 139, 154};
    float c_rvv[4] = {0};

    // 결과를 저장할 단일 스칼라 변수
    float result = 0.0f;
    printf("\n[%s]\n", __func__);

    // 내적 연산 수행
    // hal_rvv_matrix_mul_f32(a, b, c_rvv, M, N, K);
    hal_rvv_matrix_mul_f32(c_rvv, a, b, M, N, K);
    int ret = hal_verify_array_f32(c_sw, c_rvv, M*N);
    printf("operation is %s\n", ret ? "OK" : "FAIL");

    // 수동 계산 결과와 맞는지 검증
    // (1*2) + (2*3) + (3*4) + (4*5) + (5*6) 
    // = 2 + 6 + 12 + 20 + 30 = 70.0f
    printf("=====================================\n");
    printf("\tS/W\t\t\t\tRVV\n");
    printf("-------------------------------------\n");
    for (int i=0; i < M*N; ++i){
        printf("%f\t\t%f\t: %s\n", c_sw[i], c_rvv[i], hal_verify_result_f32(c_sw[i], c_rvv[i]) ? "OK" : "Bad");
    }
    printf("-------------------------------------\n\n");

}


int main() {
    // enable_hardware_units();

    printf("=======================================\n");
    printf("  RVV HAL Layer Test\n");
    printf("=======================================\n");

    // hal_rvv_test_int32_add_1();
    // hal_rvv_test_uint32_add_1();

    // hal_rvv_test_int32_sub_1();
    // hal_rvv_test_uint32_sub_1();

    // hal_rvv_test_int32_mul_1();
    // hal_rvv_test_uint32_mul_1();

    // hal_rvv_test_int32_mac_1();
    // hal_rvv_test_uint32_mac_1();

    // hal_rvv_test_int32_div_1();
    // hal_rvv_test_uint32_div_1();

    // hal_rvv_test_int32_mtrx_dot_1();
    hal_rvv_test_uint32_mtrx_dot_1();

    // hal_rvv_test_int32_mtrx_mul_1();
    // hal_rvv_test_uint32_mtrx_mul_1();

    // hal_rvv_test_float_add_1();
    // hal_rvv_test_float_sub_1();
    // hal_rvv_test_float_mul_1();
    // hal_rvv_test_float_div_1();

    // hal_rvv_test_float_mtrx_dot_1();
    // hal_rvv_test_float_mtrx_mul_1();

    printf("  All tests completed.\n");
    return 0;
}
