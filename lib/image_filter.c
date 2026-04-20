#include "hal_math.h"
#include <stddef.h>

/**
 * @brief 2D 이미지 컨볼루션 함수
 * @param output 출력 이미지 버퍼 (크기: width * height)
 * @param input  입력 이미지 버퍼 (크기: width * height)
 * @param width  이미지 가로 크기
 * @param height 이미지 세로 크기
 * @param kernel 2D 필터 커널 (예: 3x3 커널이면 길이 9의 1차원 배열)
 * @param k_size 커널의 가로/세로 크기 (예: 3)
 */
void image_conv2d_f32(float *output, const float *input, int width, int height, const float *kernel,
                      int k_size) {
    int k_half = k_size / 2;
    int k_len = k_size * k_size;

    // 최대 5x5 커널까지 지원할 수 있도록 임시 버퍼 할당
    float window[25];

    // 결과 배열 초기화 (가장자리는 0으로 둔다고 가정 - Zero Padding)
    for (int i = 0; i < width * height; i++) {
        output[i] = 0.0f;
    }

    // 커널이 삐져나가는 가장자리(테두리)를 제외한 내부 픽셀 순회
    for (int y = k_half; y < height - k_half; y++) {
        for (int x = k_half; x < width - k_half; x++) {

            // 1. 2D 윈도우(예: 3x3) 영역을 읽어서 1D 배열로 펼치기 (Flatten)
            int idx = 0;
            for (int ky = -k_half; ky <= k_half; ky++) {
                for (int kx = -k_half; kx <= k_half; kx++) {
                    window[idx++] = input[(y + ky) * width + (x + kx)];
                }
            }

            // 2. RVV 가속기를 이용한 초고속 내적(Dot Product) 연산!
            // 3x3 커널이면 길이 9짜리 벡터 내적이 한 방에 수행됩니다.
            double dot_result = 0.0;
            hal_dot_f32(&dot_result, window, kernel, k_len);

            // 3. 결과 저장 (중앙 픽셀 위치에 덮어쓰기)
            output[y * width + x] = (float)dot_result;
        }
    }
}
