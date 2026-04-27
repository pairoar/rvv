// --- zephyr_app/src/main.c ---
#include "hal_math.h"
#include "hal_soft_math.h" // C버전 함수가 있는 헤더
#include "image_filter.h"
#include "mnist_data.h" // [추가] 실제 데이터 및 가중치 헤더
#include "nn_layers.h"
#include "vmath_driver.h"
#include <stdio.h>
#include <zephyr/kernel.h>

#define MATRIX_SIZE 128 // 256x256 행렬 곱셈 테스트

// 기존 애플리케이션 계층 함수들 선언
extern void test_mlp_xor(void);
extern void test_dsp_iir(void);
extern void test_image_conv2d(void);

// ========================================================
// TinyML 파이프라인 버퍼 (BSS 영역 할당 - 스택 오버플로우 방지)
// ========================================================
static float input_image[28 * 28];
static float conv_output[28 * 28];
static float pool_output[14 * 14];
static float dense_output[10];
static float conv_kernel[9];
static float dense_weight[14 * 14 * 10];
static float dense_bias[10];

void run_mnist_cnn_pipeline(void) {
    printf("\n🚀 --- Real TinyML CNN (MNIST Digit '7') Pipeline --- 🚀\n");

    // 1. [핵심 변경] 실제 숫자 '7' 데이터와 학습된 가중치 로드
    for (int i = 0; i < 28 * 28; i++) {
        input_image[i] = mnist_digit_7[i];
    }
    load_trained_weights(conv_kernel, dense_weight, dense_bias);

    // 2. 파이프라인 관통 (Conv -> MaxPool -> Dense -> Softmax)
    printf("[1/4] Running Conv2D (28x28)...\n");
    image_conv2d_f32(conv_output, input_image, 28, 28, conv_kernel, 3);

    printf("[2/4] Running Max Pooling (14x14)...\n");
    nn_maxpool_2d_f32(pool_output, conv_output, 28, 28);

    printf("[3/4] Running Dense Layer (196 -> 10)...\n");
    nn_dense_layer_f32(dense_output, pool_output, dense_weight, dense_bias, 14 * 14, 10);

    printf("[4/4] Running Softmax...\n");
    nn_softmax_f32(dense_output, 10);

    // 3. 결과 판별 및 출력
    printf("\n--- Final Classification Probabilities ---\n");

    int best_digit = 0;
    float best_prob = 0.0f;
    float sum = 0.0f;

    for (int i = 0; i < 10; i++) {
        printf("Digit %d: %6.2f %%\n", i, dense_output[i] * 100.0f);
        sum += dense_output[i];

        // 가장 확률이 높은 숫자(Top-1) 찾기
        if (dense_output[i] > best_prob) {
            best_prob = dense_output[i];
            best_digit = i;
        }
    }

    printf("------------------------------------------\n");
    printf("✅ AI Classification Result: It's a '%d' !!! (%.1f%% confidence)\n", best_digit,
           best_prob * 100.0f);
    printf("✅ Total Probability Sum: %.2f\n", sum);
    printf("------------------------------------------\n");
}

void run_performance_benchmark(void) {
    printf("\n--- RISC-V Hardware Cycle Profiling Started ---\n");
    // VMath 하드웨어 드라이버 초기화 (뮤텍스 준비 등)
    vmath_drv_init();

    // 256x256 테스트 데이터 할당
    static float A[MATRIX_SIZE * MATRIX_SIZE];
    static float B[MATRIX_SIZE * MATRIX_SIZE];
    static float Out_C[MATRIX_SIZE * MATRIX_SIZE];
    static float Out_RVV[MATRIX_SIZE * MATRIX_SIZE];

    // 데이터 초기화 (1.0으로 통일)
    for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
        A[i] = 1.0f;
        B[i] = 1.0f;
    }

    uint32_t start_cycles, end_cycles;
    uint32_t cycles_c, cycles_rvv;

    // 1. C버전 (시간이 좀 걸릴 수 있습니다)
    start_cycles = k_cycle_get_32();
    hal_matrix_mul_c_f32(Out_C, A, B, MATRIX_SIZE, MATRIX_SIZE, MATRIX_SIZE);
    end_cycles = k_cycle_get_32();
    cycles_c = end_cycles - start_cycles;
    printf("[Scalar C] Execution Cycles: %u\n", cycles_c);

    // 2. RVV버전
    vmath_drv_lock();
    start_cycles = k_cycle_get_32();
    hal_matrix_mul_tiled_f32(Out_RVV, A, B, MATRIX_SIZE, MATRIX_SIZE, MATRIX_SIZE, 16);
    end_cycles = k_cycle_get_32();
    vmath_drv_unlock();

    cycles_rvv = end_cycles - start_cycles;
    printf("[RVV 128b] Execution Cycles: %u\n", cycles_rvv);

    float speedup = (float)cycles_c / (float)cycles_rvv;
    printf("\n🚀 Total Speedup: %.2f x Faster!\n", speedup);
    printf("-----------------------------------------------\n");
}

void test_app(void) {
    // VMath 하드웨어 드라이버 초기화 (뮤텍스 준비 등)
    vmath_drv_init();

    printf("==================================\n");
    printf(" VMath on Zephyr RTOS Booted!\n");
    printf("==================================\n");

    // -------------------------------------------------------------
    // [핵심 해킹 포인트]
    // mstatus 레지스터의 VS(Vector Status, 23~24번 비트)를
    // 01(Initial 상태)로 강제로 덮어써서 벡터 가속기 전원을 켭니다!
    // -------------------------------------------------------------
    // __asm__ volatile("csrs mstatus, %0" ::"r"(1 << 23));

    // Zephyr 커널 부팅 완료 후 진입
    printf("==================================\n");
    printf(" VMath on Zephyr RTOS Booted!\n");
    printf("==================================\n");

    // RTOS 환경 위에서 하드웨어 가속기 테스트 실행
    test_mlp_xor();

    // 약간의 딜레이(Sleep)를 주며 멀티태스킹 흉내내기
    k_msleep(1000);

    test_dsp_iir();
    k_msleep(1000);

    test_image_conv2d();

    printf("\n--- All Tasks Completed Successfully ---\n");
}

int main(void) {
    printf("Starting Zephyr Application...\n");

    // test_app();
    // run_performance_benchmark();
    run_mnist_cnn_pipeline();

    return 0;
}
