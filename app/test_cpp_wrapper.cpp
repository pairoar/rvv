#include "VMath.hpp"
#include <iomanip>
#include <iostream>

extern "C" void test_cpp_wrapper(void) {
    std::cout << "\n--- C++ Wrapper Test Started ---\n";

    // ---------------------------------------------------------
    // [1] FIR 필터 테스트 (C++ 스타일)
    // ---------------------------------------------------------
    const int NUM_SAMPLES = 10;
    float fir_coeffs[5] = {0.2f, 0.2f, 0.2f, 0.2f, 0.2f};

    // 객체 생성 (계수를 품은 독립적인 필터 인스턴스)
    vmath::FIRFilter my_filter(fir_coeffs, 5);

    float noise_data[10] = {10.5, 8.2, 13.0, 7.5, 10.8, 9.1, 12.2, 6.9, 11.5, 10.3};
    float filtered_data[10] = {0};

    // 필터링 수행 (매우 직관적인 호출!)
    my_filter.process(noise_data, filtered_data, NUM_SAMPLES);

    std::cout << "[FIR Filter Results]\n";
    for (int i = 4; i < NUM_SAMPLES; i++) {
        std::cout << "Raw: " << std::setw(5) << noise_data[i] << " | Filtered: " << filtered_data[i]
                  << "\n";
    }

    // ---------------------------------------------------------
    // [2] AI Dense Layer 테스트 (C++ 스타일)
    // ---------------------------------------------------------
    // XOR 문제의 Hidden Layer 객체화
    float W1[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    float B1[2] = {0.0f, -1.0f};

    // 객체 생성: "입력 2개를 받아 2개로 내보내는 Hidden Layer"
    vmath::DenseLayer hidden_layer(W1, B1, 2, 2);

    float X[2] = {1.0f, 1.0f}; // XOR 입력 (1, 1)
    float H[2] = {0};          // 은닉층 결과

    // 순전파 수행
    hidden_layer.forward(X, H);

    std::cout << "\n[AI Dense Layer Results]\n";
    std::cout << "Input(1,1) -> Hidden Layer Output: (" << H[0] << ", " << H[1] << ")\n";
}
