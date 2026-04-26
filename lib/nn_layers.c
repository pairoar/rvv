#include "hal_soft_math.h"
#include "vmath_driver.h"
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
    double temp_mac[out_features];
    for (int i = 0; i < out_features; i++) {
        temp_mac[i] = 0.0;
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
