#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rvv_math.h"

#define EPSILON 0.0001f

int verify_result(const float a, const float b) {
    return (abs(a - b) < EPSILON);
}

static rvv_math_status_t vec_add_f32(float *c, const float *a, const float *b, const size_t n){
    for (size_t i=0; i<n; ++i){
        c[i] = a[i] + b[i];
    }
    return RVV_MATH_SUCCESS;
}

static rvv_math_status_t vec_sub_f32(float *c, const float *a, const float *b, const size_t n){
    for (size_t i=0; i<n; ++i){
        c[i] = a[i] - b[i];
    }
    return RVV_MATH_SUCCESS;
}

static rvv_math_status_t vec_mul_f32(float *c, const float *a, const float *b, const size_t n){
    for (size_t i=0; i<n; ++i){
        c[i] = a[i] * b[i];
    }
    return RVV_MATH_SUCCESS;
}

static rvv_math_status_t vec_mac_f32(float *c, const float *a, const float *b, const size_t n){
    for (size_t i=0; i<n; ++i){
        c[i] += a[i] * b[i];
    }
    return RVV_MATH_SUCCESS;
}

static void test_vector_add_f32(void) {
    const size_t LEN = 10;
    const float a[10] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f};
    const float b[10] = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f, 90.0f, 100.0f};
    float c_sw[10] = {0};
    float c_vec[10] = {0};

    printf("[APP] Starting Vector Addition via Library...\n");

    // call s/w function
    rvv_math_status_t st_lib = vec_add_f32(c_sw, a, b, LEN);
    if (st_lib != RVV_MATH_SUCCESS) {
        printf("Error: wrong func!\n");
        return ;
    }

    // call library
    rvv_math_status_t st_sw = rvv_math_add_f32(c_vec, a, b, LEN);
    if (st_sw != RVV_MATH_SUCCESS) {
        printf("Error: lib!\n");
        return;
    }

    // verify result (ex c_sw[i] == c_vec[i],  +- epsilon)
    printf("=====================================\n");
    printf("\tS/W\t\t\t\tRVV\n");
    printf("=====================================\n");
    for (int i=0; i<LEN; ++i){
        printf("%f\t\t%f\t: %s\n", c_sw[i], c_vec[i], verify_result(c_sw[i], c_vec[i]) ? "OK" : "Bad");
    }
}

static void test_vector_sub_f32(void) {
    const size_t LEN = 10;
    const float a[10] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f};
    const float b[10] = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f, 90.0f, 100.0f};
    float c_sw[10] = {0};
    float c_vec[10] = {0};

    printf("[APP] Starting Vector Subtraction via Library...\n");

    // call s/w function
    rvv_math_status_t st_lib = vec_sub_f32(c_sw, a, b, LEN);
    if (st_lib != RVV_MATH_SUCCESS) {
        printf("Error: wrong func!\n");
        return ;
    }

    // call library
    rvv_math_status_t st_sw = rvv_math_sub_f32(c_vec, a, b, LEN);
    if (st_sw != RVV_MATH_SUCCESS) {
        printf("Error: lib!\n");
        return;
    }

    // verify result (ex c_sw[i] == c_vec[i],  +- epsilon)
    printf("=====================================\n");
    printf("\tS/W\t\t\t\tRVV\n");
    printf("=====================================\n");
    for (int i=0; i<LEN; ++i){
        printf("%f\t\t%f\t: %s\n", c_sw[i], c_vec[i], verify_result(c_sw[i], c_vec[i]) ? "OK" : "Bad");
    }
}

static void test_vector_mul_f32(void) {
    const size_t LEN = 10;
    const float a[10] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f};
    const float b[10] = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f, 90.0f, 100.0f};
    float c_sw[10] = {0};
    float c_vec[10] = {0};

    printf("[APP] Starting Vector Multiplication via Library...\n");

    // call s/w function
    rvv_math_status_t st_lib = vec_mul_f32(c_sw, a, b, LEN);
    if (st_lib != RVV_MATH_SUCCESS) {
        printf("Error: wrong func!\n");
        return ;
    }

    // call library
    rvv_math_status_t st_sw = rvv_math_mul_f32(c_vec, a, b, LEN);
    if (st_sw != RVV_MATH_SUCCESS) {
        printf("Error: lib!\n");
        return;
    }

    // verify result (ex c_sw[i] == c_vec[i],  +- epsilon)
    printf("=====================================\n");
    printf("\tS/W\t\t\t\tRVV\n");
    printf("=====================================\n");
    for (int i=0; i<LEN; ++i){
        printf("%f\t\t%f\t: %s\n", c_sw[i], c_vec[i], verify_result(c_sw[i], c_vec[i]) ? "OK" : "Bad");
    }
}


static void test_vector_mac_f32(void) {
    const size_t LEN = 10;
    float a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    float b[10] = {1, 1, 1, 1, 1, 2, 2, 2, 2, 2};
    float c_vec[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10}; 
    float c_sw[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10}; 

    printf("[APP] Starting Vector MAC via Library...\n");

    // call s/w function
    rvv_math_status_t st_lib = vec_mac_f32(c_sw, a, b, LEN);
    if (st_lib != RVV_MATH_SUCCESS) {
        printf("Error: wrong func!\n");
        return ;
    }

    // call library
    rvv_math_status_t st_sw = rvv_math_mac_f32(c_vec, a, b, LEN);
    if (st_sw != RVV_MATH_SUCCESS) {
        printf("Error: lib!\n");
        return;
    }

    // verify result (ex c_sw[i] == c_vec[i],  +- epsilon)
    printf("=====================================\n");
    printf("\tS/W\t\t\t\tRVV\n");
    printf("=====================================\n");
    for (int i=0; i<LEN; ++i){
        printf("%f\t\t%f\t: %s\n", c_sw[i], c_vec[i], verify_result(c_sw[i], c_vec[i]) ? "OK" : "Bad");
    }
}

int main() {
    test_vector_add_f32();
    test_vector_sub_f32();
    test_vector_mul_f32();
    test_vector_mac_f32();

    return 0;
}
