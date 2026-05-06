#ifndef HAL_TYPES_H
#define HAL_TYPES_H

// hal_types.h (또는 hal_math.h 상단)
typedef enum {
    HAL_OK                          = 0,    // 성공 (항상 0으로 두는 것이 표준)

    // parameters
    HAL_ERR_NULL_PTR                = -1,   // 포인터가 NULL임
    HAL_ERR_INVALID_SIZE            = -2,   // 포인터가 NULL임
    HAL_ERR_INVALID_ARG             = -3,   // 잘못된 인자 (NULL 포인터 등)

    // memory
    HAL_ERR_UNALIGNED               = -10,  // 메모리 정렬 오류

    // system/hw
    HAL_ERR_UNSUPPORTED             = -20,  // 해당 하드웨어에서 미지원 (예: RV32에서 특정 연산)    
    HAL_ERR_HW_FAULT                = -21,  // FPU/Vecot
    HAL_ERR_TIMEOUT                 = -21,  // 가속기 응답 지연 (HW 가속기 연동 시 필요)
    HAL_ERR_OUT_OF_RESOURCE         = -22,


    // operation
    HAL_ERR_DIV_BY_ZERO             = -30,  // 0으로 나누기
    HAL_ERR_VALUES_DO_NOT_MATCH    = -31,  //    

} hal_status_t;


#endif