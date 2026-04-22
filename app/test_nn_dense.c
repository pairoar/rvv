#include "nn_layers.h"
#include <stdio.h>

void test_nn_dense(void) {
    printf("--- AI Neural Network Inference Started ---\n");

    // [시나리오] 센서에서 들어온 3개의 데이터로 2개의 결과를 추론하는 단일 레이어
    int in_features = 3;
    int out_features = 2;

    // 입력 데이터 (센서 값 등)
    float X[3] = {1.5f, -0.5f, 2.0f};

    // 가중치 (미리 학습된 딥러닝 모델의 파라미터)
    float W[6] = {
        0.5f,  -0.2f, // 첫 번째 특성 가중치
        1.0f,  0.8f,  // 두 번째 특성 가중치
        -0.5f, 1.2f   // 세 번째 특성 가중치
    };

    // 편향
    float B[2] = {0.1f, -0.5f};

    // 추론 결과를 담을 버퍼
    float Y[2] = {0};

    // 하드웨어 가속기를 이용한 신경망 추론!
    nn_dense_layer_f32(Y, X, W, B, in_features, out_features);

    // 결과 출력
    printf("Input  : [%.2f, %.2f, %.2f]\n", X[0], X[1], X[2]);
    printf("Output : [%.2f, %.2f]\n", Y[0], Y[1]);
}
