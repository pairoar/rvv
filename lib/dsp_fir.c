#include "hal_math.h"
#include "vmath_driver.h"
#include <stddef.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

/**
 * @brief FIR (Finite Impulse Response) 필터 함수
 * @param output       필터링된 결과가 저장될 배열
 * @param input        센서에서 들어온 원본 입력 배열
 * @param coeffs       필터 계수 (Taps). 시간 역순으로 배치되어 있다고 가정.
 * @param num_samples  전체 처리할 샘플(데이터) 개수
 * @param num_taps     필터의 길이 (계수의 개수)
 */
void dsp_fir_f32(float *output, const float *input, const float *coeffs, int num_samples,
                 int num_taps) {
    if (num_samples < num_taps)
        return;

    // 함수 시작 시 가속기 전원 ON 및 독점
    vmath_drv_lock();

    // 1. 초기 지연(Delay Line) 구간 처리
    // 과거 데이터가 충분하지 않은 맨 앞부분은 0으로 채우거나 원본을 바이패스합니다.
    for (int i = 0; i < num_taps - 1; i++) {
        output[i] = input[i]; // 여기서는 원본 바이패스로 처리
    }

    // 2. 본격적인 FIR 필터링 (Sliding Window & Dot Product)
    // 이 루프가 바로 DSP의 핵심입니다!
    for (int i = num_taps - 1; i < num_samples; i++) {
        double dot_result = 0.0;

        // input 배열에서 현재 처리할 윈도우의 '시작 주소'를 넘겨줍니다.
        // 그리고 num_taps 길이만큼 RVV 가속기가 한 방에 내적(Dot Product)을 수행합니다!
        const float *current_window = &input[i - num_taps + 1];

        hal_dot_f32(&dot_result, current_window, coeffs, num_taps);

        // double로 정밀하게 계산된 결과를 float로 다운캐스팅하여 저장
        output[i] = (float)dot_result;
    }

    // 함수 종료 전 가속기 전원 OFF 및 반환
    vmath_drv_unlock();
}
