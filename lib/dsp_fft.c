#include "dsp_fft.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

// 1. 비트 역전 재배치 (In-place)
static void dsp_fft_bit_reverse_c(dsp_complex_f32_t *data, int n_fft) {
    int i, j = 0;
    for (i = 0; i < n_fft; i++) {
        if (i < j) {
            dsp_complex_f32_t temp = data[i];
            data[i] = data[j];
            data[j] = temp;
        }
        int m = n_fft >> 1;
        while (m >= 1 && j >= m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
}

// 2. RVV 가속 Radix-2 FFT 코어
void dsp_fft_radix2_f32(dsp_complex_f32_t *data, const dsp_complex_f32_t *w_table, int n_fft) {
    // 1단계: 데이터를 짝수/홀수 분할 위치로 재배치
    dsp_fft_bit_reverse_c(data, n_fft);

    // 2단계: Stage별 버터플라이 연산 (1쌍 -> 2쌍 -> 4쌍 -> ...)
    for (int step = 2; step <= n_fft; step <<= 1) {
        int half_step = step / 2;

        // Twiddle Factor를 듬성듬성 읽기 위한 바이트 간격 계산
        // W 테이블은 전체 N_FFT용으로 만들어졌으므로, 현재 step에 맞게 건너뛰어야 함
        int stride_idx = n_fft / step;
        int w_stride_bytes = stride_idx * sizeof(dsp_complex_f32_t);

        // Group별로 순회하며 RVV 가속기에 던짐
        for (int i = 0; i < n_fft; i += step) {
            // [마법의 한 줄] RVV 128비트 엔진이 half_step 묶음을 통째로 썰어버림!
            hal_fft_butterfly_f32(&data[i], &data[i + half_step], w_table, half_step,
                                  w_stride_bytes);
        }
    }
}

// ==============================================================================
// 정밀도를 위해 math.h의 cosf, sinf를 사용한 Twiddle Factor 생성 (1회성 초기화)
// ==============================================================================
void dsp_fft_init_twiddles_c(dsp_complex_f32_t *w_table, int n_fft) {
    int half_n = n_fft / 2;
    for (int k = 0; k < half_n; k++) {
        // W_N^k = exp(-j * 2 * pi * k / N) = cos(theta) - j * sin(theta)
        float theta = -2.0f * M_PI * k / n_fft;
        w_table[k].real = cosf(theta);
        w_table[k].imag = sinf(theta);
    }
}
