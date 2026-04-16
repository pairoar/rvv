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
    return (abs(a - b) < EPSILON);
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



static void hal_rvv_test_float_add_1(void) {
    printf("\n[%s]\n", __func__);
    const size_t LEN = 10;
    float a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    float b[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    float c_rvv[10] = {0};
    float c_sw[10] = {11, 22, 33, 44, 55, 66, 77, 88, 99, 110};

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

void hal_rvv_test_vector_dot(void) {
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

    hal_rvv_test_int32_div_1();
    hal_rvv_test_uint32_div_1();

    // hal_rvv_test_float_add_1();

    // hal_rvv_test_vector_dot();

    printf("  All tests completed.\n");
    return 0;
}