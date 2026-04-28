#ifndef VMATH_TYPES_H
#define VMATH_TYPES_H

// 라이브러리 동작 상태 (에러 코드) 정의
typedef enum {
    VMATH_SUCCESS             =  0,  // 연산 성공
    
    // 파라미터 관련 에러 (-1 ~ -9)
    VMATH_ERR_NULL_PTR        = -1,  // 포인터가 NULL임
    VMATH_ERR_INVALID_SIZE    = -2,  // 배열 길이(N)가 0이거나 유효하지 않음
    VMATH_ERR_INVALID_ARG     = -3,  // 기타 잘못된 인자 값 (예: 지원하지 않는 행렬 차원)
    
    // 메모리 관련 에러 (-10 ~ -19)
    VMATH_ERR_UNALIGNED_PTR   = -10, // 포인터가 벡터 연산에 필요한 정렬(Alignment)을 만족하지 않음
    
    // 시스템/하드웨어 관련 에러 (-20 ~ )
    VMATH_ERR_UNSUPPORTED     = -20, // 현재 타겟 하드웨어에서 지원하지 않는 연산임
    VMATH_ERR_HW_FAULT        = -21,  // 하드웨어 유닛(FPU/Vector) 상태 이상

    // 값 비교
    VMATH_ERR_VALUES_DO_NOT_MATCH = -30,    // Error. Values ​​do not match.
} vmath_status_t;

#endif // VMATH_TYPES_H
