#include "hal_math.h"
#include "vmath_driver.h"
#include <stddef.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

/*
    DSP 필터링 함수는 일반적으로 다음과 같은 형태로 구현됩니다:
    - 입력 데이터(input)와 필터 계수(coeffs)를 받아서, 출력(output)에 필터링된 결과를 저장합니다.
    - num_samples는 전체 샘플의 개수, num_taps는 필터의 길이(계수의 개수)를 나타냅니다.
    - FIR 필터의 경우, 각 출력 샘플은 현재 입력 샘플과 과거 num_taps-1개의 입력 샘플에 대한 가중
   합으로 계산됩니다.
    - 이때, hal_dot_f32 함수를 활용하여 RVV 가속기로 내적 연산을 수행하면, 매우 효율적으로 FIR
   필터링을 구현할 수 있습니다.
*/
/**
 * @brief IIR (Infinite Impulse Response) 필터 함수
 * @param output   필터링된 결과 배열
 * @param input    원본 입력 배열
 * @param a_coeffs 피드백 계수 (주의: a0=1.0을 제외한 a1, a2 ... 값들이 역순으로 배치됨)
 * @param b_coeffs 피드포워드 계수 (역순으로 배치됨)
 * @param num_samples 샘플 수
 * @param num_a    a 계수의 개수 (a0 제외)
 * @param num_b    b 계수의 개수
 */
void dsp_iir_f32(float *output, const float *input, const float *a_coeffs, const float *b_coeffs,
                 int num_samples, int num_a, int num_b) {

    // 함수 시작 시 가속기 전원 ON 및 독점
    vmath_drv_lock();

    // 과거 데이터를 참조하기 위해 필요한 최대 지연(Delay) 길이 계산
    // 예: num_b = 3 (x[n], x[n-1], x[n-2]), num_a = 4 (y[n-1] ... y[n-4])
    // -> 시작점은 인덱스 4가 되어야 함.
    int max_delay = MAX(num_b - 1, num_a);

    if (num_samples <= max_delay)
        return; // 참조할 수 있는 과거 샘플 공간이 부족하면 리턴

    // 1. 초기 지연(Delay Line) 구간 처리 (바이패스)
    for (int i = 0; i < max_delay; i++) {
        output[i] = input[i];
    }

    // 2. 본격적인 IIR 필터링
    for (int i = max_delay; i < num_samples; i++) {
        double feedforward_sum = 0.0;
        double feedback_sum = 0.0;

        // [피드포워드 계산] b_coeffs 내적 (x[n - num_b + 1] ~ x[n])
        if (num_b > 0) {
            const float *current_input_window = &input[i - num_b + 1];
            hal_dot_f32(&feedforward_sum, current_input_window, b_coeffs, num_b);
        }

        // [피드백 계산] a_coeffs 내적 (y[n - num_a] ~ y[n - 1])
        if (num_a > 0) {
            const float *current_output_window = &output[i - num_a];
            hal_dot_f32(&feedback_sum, current_output_window, a_coeffs, num_a);
        }

        // 최종 출력 계산 (피드포워드 - 피드백)
        double iir_result = feedforward_sum - feedback_sum;
        output[i] = (float)iir_result;
    }

    // 함수 종료 전 가속기 전원 OFF 및 반환
    vmath_drv_unlock();
}
