#include "dsp_filter.h"
#include <stdio.h>
#include <stdlib.h>

void test_dsp_fir(void) {
    printf("\n--- DSP FIR Filter (Noise Removal) Started ---\n");

    const int NUM_SAMPLES = 20;
    const int NUM_TAPS = 5;

    float raw_sensor_data[20];
    float filtered_data[20];

    // FIR 필터 계수 (Coefficients)
    // 여기서는 가장 간단한 형태인 '5-Tap 이동 평균(Moving Average) 필터'를 사용합니다.
    // 모든 계수에 0.2 (1/5)를 주어 특정 주파수 이상의 튀는 값을 깎아냅니다.
    float fir_coeffs[5] = {0.2f, 0.2f, 0.2f, 0.2f, 0.2f};

    // 1. 가상의 노이즈 데이터 생성
    // 이상적인 값(10.0)에 위아래로 심한 노이즈가 낀 상황을 가정합니다.
    float ideal_value = 10.0f;
    float noise[20] = {0.5, -1.2, 3.0, -2.5, 0.8, -0.1, 2.2, -3.1, 1.5, -0.9,
                       0.3, -1.8, 2.7, -2.1, 0.4, -0.6, 1.9, -2.8, 1.1, -0.2};

    printf("[Raw Input vs Filtered Output]\n");
    for (int i = 0; i < NUM_SAMPLES; i++) {
        raw_sensor_data[i] = ideal_value + noise[i];
    }

    // 2. RVV 가속기를 이용한 초고속 FIR 필터링 수행!
    // 내부적으로 hal_dot_f32가 반복 호출됩니다.
    dsp_fir_f32(filtered_data, raw_sensor_data, fir_coeffs, NUM_SAMPLES, NUM_TAPS);

    // 3. 결과 비교 출력
    for (int i = 0; i < NUM_SAMPLES; i++) {
        // 필터가 적용되기 시작하는 지점(TAPS-1) 이후부터 시각적으로 비교하기 좋게 출력
        if (i >= NUM_TAPS - 1) {
            printf("Sample %2d | Raw: %5.2f | Filtered: %5.2f\n", i, raw_sensor_data[i],
                   filtered_data[i]);
        }
    }
}
