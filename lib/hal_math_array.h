#ifndef HAL_MATH_ARRAY_H
#define HAL_MATH_ARRAY_H
// Array Reduction 가속 함수들
float hal_max_array_f32(const float *arr, int len);
float hal_sum_array_f32(const float *arr, int len);

void hal_dot_f32(double *result, const float *a, const float *b, int length);

#endif // HAL_MATH_ARRAY_H
