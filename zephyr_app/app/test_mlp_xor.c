#include "nn_layers.h"
#include <stdio.h>

void test_mlp_xor(void) {
    printf("\n--- AI MLP (Multi-Layer Perceptron) XOR Inference Started ---\n");

    // [네트워크 구조 설계]
    // Input(2) -> Hidden(2) -> Output(1)

    // 1. 은닉층 (Hidden Layer) 가중치와 편향
    // 특징 2개를 받아서 2개의 새로운 차원으로 변환
    float W1[4] = {
        1.0f, 1.0f, // 첫 번째 뉴런: x1 + x2
        1.0f, 1.0f  // 두 번째 뉴런: x1 + x2
    };
    float B1[2] = {0.0f, -1.0f}; // 첫 뉴런은 그대로, 두 번째 뉴런은 임계값을 높임(-1.0)

    // 2. 출력층 (Output Layer) 가중치와 편향
    // 은닉층에서 나온 2개의 값을 받아서 1개의 최종 결과로 압축
    float W2[2] = {
        1.0f, // 첫 번째 뉴런의 값은 더하고
        -2.0f // 두 번째 뉴런의 값은 두 배로 뺌 (비선형의 핵심!)
    };
    float B2[1] = {0.0f};

    // XOR 문제의 4가지 입력 케이스
    float X_cases[4][2] = {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}};

    printf("Input(X1, X2) | Hidden(H1, H2) | Output(Y)\n");
    printf("------------------------------------------\n");

    // 4가지 케이스에 대해 추론(Inference) 반복
    for (int i = 0; i < 4; i++) {
        float H[2] = {0}; // 은닉층 결과 저장용 버퍼
        float Y[1] = {0}; // 최종 출력 저장용 버퍼

        // [순전파(Forward Propagation) 1단계] Input -> Hidden Layer
        nn_dense_layer_f32(H, X_cases[i], W1, B1, 2, 2);

        // [순전파(Forward Propagation) 2단계] Hidden -> Output Layer
        // 앞서 계산된 H를 다시 다음 레이어의 입력으로 사용합니다!
        nn_dense_layer_f32(Y, H, W2, B2, 2, 1);

        // 결과 출력
        printf("  (%1.0f, %1.0f)   |  (%4.1f, %4.1f)  |  %4.1f\n", X_cases[i][0], X_cases[i][1],
               H[0], H[1], Y[0]);
    }
}
