#include "image_filter.h"
#include <stdio.h>

void test_image_conv2d(void) {
    printf("\n--- 2D Image Convolution (Edge Detection) Started ---\n");

    const int WIDTH = 6;
    const int HEIGHT = 6;

    // 1. 가상의 6x6 입력 이미지 생성 (세로선 엣지가 존재하는 이미지)
    // 왼쪽 3칸은 0.0 (검은색), 오른쪽 3칸은 10.0 (흰색)
    float image_in[36] = {0.0f,  0.0f,  0.0f,  10.0f, 10.0f, 10.0f, 0.0f,  0.0f,  0.0f,
                          10.0f, 10.0f, 10.0f, 0.0f,  0.0f,  0.0f,  10.0f, 10.0f, 10.0f,
                          0.0f,  0.0f,  0.0f,  10.0f, 10.0f, 10.0f, 0.0f,  0.0f,  0.0f,
                          10.0f, 10.0f, 10.0f, 0.0f,  0.0f,  0.0f,  10.0f, 10.0f, 10.0f};

    float image_out[36] = {0};

    // 2. 3x3 소벨 필터 커널 (Sobel X - 세로 윤곽선 추출용)
    // 왼쪽의 값은 빼고, 오른쪽의 값은 더해서 '변화량'을 극대화합니다.
    float sobel_x_kernel[9] = {-1.0f, 0.0f, 1.0f, -2.0f, 0.0f, 2.0f, -1.0f, 0.0f, 1.0f};

    // 3. 하드웨어 가속 2D 컨볼루션 실행!
    image_conv2d_f32(image_out, image_in, WIDTH, HEIGHT, sobel_x_kernel, 3);

    // 4. 결과 출력
    printf("\n[Input Image (6x6)]\n");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            printf("%4.0f ", image_in[y * WIDTH + x]);
        }
        printf("\n");
    }

    printf("\n[Output Image (Filtered)]\n");
    printf("-> 윤곽선(값이 변하는 경계) 부분에서만 높은 수치가 검출됩니다.\n");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            printf("%4.0f ", image_out[y * WIDTH + x]);
        }
        printf("\n");
    }
}
