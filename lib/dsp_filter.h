#ifndef DSP_FILTER_H
#define DSP_FILTER_H

#include "hal_math.h"
#include <stddef.h> // size_t 정의를 위해 추가

void dsp_fir_f32(float *output, const float *input, const float *coeffs, int num_samples,
                 int num_taps);

void dsp_iir_f32(float *output, const float *input, const float *a_coeffs, const float *b_coeffs,
                 int num_samples, int num_a, int num_b);

#endif // DSP_FILTER_H
