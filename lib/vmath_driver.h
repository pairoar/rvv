#pragma once
#include <stdbool.h>

// 가속기 상태 코드
typedef enum { VMATH_OK = 0, VMATH_BUSY, VMATH_ERROR } vmath_status_t;

#ifdef __cplusplus
extern "C" {
#endif

// 가속기 초기화 (전원 ON, 리소스 할당)
vmath_status_t vmath_drv_init(void);

// 가속기 사용 권한 획득 (Mutex Lock)
vmath_status_t vmath_drv_lock(void);

// 가속기 사용 권한 반납 (Mutex Unlock)
vmath_status_t vmath_drv_unlock(void);

#ifdef __cplusplus
}
#endif
