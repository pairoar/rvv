#ifndef NN_LAYER_H
#define NN_LAYER_H

#include "hal_math.h"
#include "vmath_driver.h"
#include <stdio.h>

/**************************************************
 * 1. Dense Layer (완전 연결 계층) 구현
 **************************************************/
/** * @brief 신경망의 완전 연결 계층 (Dense Layer) 추론 함수
 * @param output 결과가 저장될 버퍼 (크기: out_features)
 * @param input  입력 특징 벡터 (크기: in_features)
 * @param weight 가중치 행렬 (크기: in_features * out_features)
 * @param bias   편향 벡터 (크기: out_features)
 */
void nn_dense_layer_f32(float *output, const float *input, const float *weight, const float *bias,
                        int in_features, int out_features);

void nn_maxpool_2d_f32(float *output, const float *input, int width, int height);
void nn_softmax_f32(float *io_buffer, int length);
// NN Layers

#endif // NN_LAYER_H
