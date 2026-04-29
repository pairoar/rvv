#ifndef DSP_FFT_H
#define DSP_FFT_H

typedef struct {
    float real;
    float imag;
} dsp_complex_f32_t;

// C API 선언
void hal_fft_init_twiddles_f32(dsp_complex_f32_t *w_table, int n_fft);
void hal_fft_butterfly_f32(dsp_complex_f32_t *top, dsp_complex_f32_t *bot,
                           const dsp_complex_f32_t *w_table, int count, int w_stride_bytes);

void dsp_fft_radix2_f32(dsp_complex_f32_t *data, const dsp_complex_f32_t *w_table, int n_fft);

void dsp_fft_init_twiddles_c(dsp_complex_f32_t *w_table, int n_fft);

#endif
