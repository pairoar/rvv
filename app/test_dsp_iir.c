#include "dsp_filter.h"
#include <stdio.h>

void test_dsp_iir(void) {
    printf("\n--- DSP IIR Filter (Biquad Low-pass) Started ---\n");

    const int NUM_SAMPLES = 20;

    // 1. IIR Biquad 필터 계수 설정 (Cutoff: 50Hz, Q: 0.707 @ 1000Hz)
    // [중요] dsp_iir_f32 구조에 맞게 과거 데이터부터 현재 데이터 순으로 '역순' 배치합니다.

    // b_coeffs (Feedforward): {b2, b1, b0}
    float b_coeffs[3] = {0.0201f, 0.0402f, 0.0201f};

    // a_coeffs (Feedback): {a2, a1} (a0 = 1.0은 수식상 이항되어 생략됨)
    float a_coeffs[2] = {0.6414f, -1.5609f};

    float raw_sensor_data[20];

    // IIR 필터는 이전 '출력값'을 피드백하므로, 반드시 결과 배열을 0으로 싹 초기화해야 안전합니다.
    float filtered_data[20] = {0};

    // 2. 가상의 노이즈 데이터 생성 (FIR 테스트와 동일한 데이터)
    float ideal_value = 10.0f;
    float noise[20] = {0.5f, -1.2f, 3.0f, -2.5f, 0.8f, -0.1f, 2.2f, -3.1f, 1.5f, -0.9f,
                       0.3f, -1.8f, 2.7f, -2.1f, 0.4f, -0.6f, 1.9f, -2.8f, 1.1f, -0.2f};

    for (int i = 0; i < NUM_SAMPLES; i++) {
        raw_sensor_data[i] = ideal_value + noise[i];
    }

    // 3. RVV 가속기를 이용한 초고속 IIR 필터링 수행!
    // num_a = 2, num_b = 3
    dsp_iir_f32(filtered_data, raw_sensor_data, a_coeffs, b_coeffs, NUM_SAMPLES, 2, 3);

    // 4. 결과 비교 출력
    printf("[Raw Input vs IIR Filtered Output]\n");
    for (int i = 0; i < NUM_SAMPLES; i++) {
        // max_delay 구간(인덱스 0, 1)은 원본이 바이패스 되므로 2부터 출력하여 IIR의 위력을
        // 확인합니다.
        if (i >= 2) {
            printf("Sample %2d | Raw: %5.2f | Filtered: %5.2f\n", i, raw_sensor_data[i],
                   filtered_data[i]);
        }
    }
}
