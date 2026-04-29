#pragma once

// 기존 C 언어 헤더들을 C++ 맹글링(Mangling) 없이 안전하게 가져오기
extern "C" {
#include "hal_math.h"
// (실제 구현하신 헤더 파일명에 맞게 수정해주세요)
extern void dsp_fir_f32(float *output, const float *input, const float *coeffs, int num_samples,
                        int num_taps);
extern void dsp_iir_f32(float *output, const float *input, const float *a_coeffs,
                        const float *b_coeffs, int num_samples, int num_a, int num_b);
extern void nn_dense_layer_f32(float *output, const float *input, const float *weight,
                               const float *bias, int in_features, int out_features);
}

namespace vmath {

// ==========================================
// 1. FIR Filter Class
// ==========================================
class FIRFilter {
  private:
    const float *m_coeffs; // 필터 계수 포인터
    int m_taps;            // 탭 수

  public:
    // 생성자: 필터를 생성할 때 계수를 한 번만 주입받아 상태로 저장합니다.
    FIRFilter(const float *coeffs, int taps) : m_coeffs(coeffs), m_taps(taps) {}

    // 처리 함수: 사용자는 입출력 데이터와 샘플 수만 넘기면 됩니다.
    void process(const float *input, float *output, int num_samples) const {
        dsp_fir_f32(output, input, m_coeffs, num_samples, m_taps);
    }
};

// ==========================================
// 2. AI Dense Layer Class
// ==========================================
class DenseLayer {
  private:
    const float *m_weights;
    const float *m_bias;
    int m_in_features;
    int m_out_features;

  public:
    // 생성자: 딥러닝 모델의 파라미터(가중치, 편향)를 객체 생성 시점에 고정합니다.
    DenseLayer(const float *weights, const float *bias, int in_feat, int out_feat)
        : m_weights(weights), m_bias(bias), m_in_features(in_feat), m_out_features(out_feat) {}

    // 순전파 함수 (Forward Propagation)
    void forward(const float *input, float *output) const {
        nn_dense_layer_f32(output, input, m_weights, m_bias, m_in_features, m_out_features);
    }
};

} // namespace vmath
