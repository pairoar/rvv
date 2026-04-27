#include "hal_math.h"
#include "hal_soft_math.h"
#include "vmath_driver.h"
#include <float.h>
#include <math.h> // expf() 사용을 위해 추가
#include <stddef.h>

// 캐시 라인이나 하드웨어 특성에 맞는 최적의 타일 사이즈 정의
#define VMATH_TILE_SIZE 16

/**
 * @brief 신경망의 완전 연결 계층 (Dense Layer) 추론 함수
 * @param output 결과가 저장될 버퍼 (크기: out_features)
 * @param input  입력 특징 벡터 (크기: in_features)
 * @param weight 가중치 행렬 (크기: in_features * out_features)
 * @param bias   편향 벡터 (크기: out_features)
 */
void nn_dense_layer_f32(float *output, const float *input, const float *weight, const float *bias,
                        int in_features, int out_features) {
    // 하드웨어 가속기 점유 (멀티스레드 충돌 방지!)
    vmath_drv_lock();

    // [수정된 부분] RVV MAC 연산을 위해 메모리를 명시적으로 0.0으로 초기화!
    // double temp_mac[out_features];
    // for (int i = 0; i < out_features; i++) {
    //     temp_mac[i] = 0.0;
    // }
    float temp_mac[out_features];
    for (int i = 0; i < out_features; i++) {
        temp_mac[i] = 0.0f;
    }

    // M=1, N=out_features, K=in_features
    // hal_matrix_mul_f32(temp_mac, input, weight, 1, out_features, in_features);
    hal_matrix_mul_tiled_f32(temp_mac, input, weight, 1, out_features, in_features,
                             VMATH_TILE_SIZE);

    // Bias 덧셈 (B) 및 ReLU 활성화 함수 적용
    for (int i = 0; i < out_features; i++) {
        float val = (float)temp_mac[i] + bias[i];
        output[i] = (val > 0.0f) ? val : 0.0f;
    }

    // 연산 완료 후 가속기 반납
    vmath_drv_unlock();
}

/**
 * @brief 2D Max Pooling 함수 (2x2 윈도우)
 */
void nn_maxpool_2d_f32(float *output, const float *input, int width, int height) {
    vmath_drv_lock(); // 가속기 ON

    int out_width = width / 2;
    int out_height = height / 2;
    float window[4]; // 2x2 윈도우 버퍼

    for (int y = 0; y < out_height; y++) {
        for (int x = 0; x < out_width; x++) {
            int in_y = y * 2;
            int in_x = x * 2;

            // 4개의 픽셀 데이터를 연속된 배열로 복사
            window[0] = input[(in_y + 0) * width + (in_x + 0)];
            window[1] = input[(in_y + 0) * width + (in_x + 1)];
            window[2] = input[(in_y + 1) * width + (in_x + 0)];
            window[3] = input[(in_y + 1) * width + (in_x + 1)];

            // [RVV 가속] 4개의 데이터 중 최댓값을 벡터 리덕션으로 한 번에 추출!
            output[y * out_width + x] = hal_max_array_f32(window, 4);
        }
    }
    vmath_drv_unlock(); // 가속기 OFF
}

/**
 * @brief Softmax 활성화 함수
 */
void nn_softmax_f32(float *io_buffer, int length) {
    vmath_drv_lock(); // 가속기 ON

    // 1. [RVV 가속] 10개(length)의 출력값 중 최댓값(Max)을 벡터로 초고속 탐색
    float max_val = hal_max_array_f32(io_buffer, length);

    // 2. 오버플로우 방지를 위해 최댓값을 빼주고 지수(exp) 취하기
    for (int i = 0; i < length; i++) {
        io_buffer[i] = expf(io_buffer[i] - max_val);
    }

    // 3. [RVV 가속] 지수가 취해진 10개 데이터의 총합을 벡터로 초고속 연산
    float sum_exp = hal_sum_array_f32(io_buffer, length);

    // 4. 총합으로 나누어 최종 확률 계산 (확률의 합은 1.0)
    float inv_sum = 1.0f / sum_exp;
    for (int i = 0; i < length; i++) {
        io_buffer[i] *= inv_sum;
    }

    vmath_drv_unlock(); // 가속기 OFF
}
