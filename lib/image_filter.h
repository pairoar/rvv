#ifndef IMAGE_FILTER_H
#define IMAGE_FILTER_H

#include "dsp_filter.h" // FIR/IIR 필터 함수 선언 포함

void image_conv2d_f32(float *output, const float *input, int width, int height, const float *kernel,
                      int k_size);

#endif // IMAGE_FILTER_H
