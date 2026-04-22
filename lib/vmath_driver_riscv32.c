#include "vmath_driver.h"
#include <stdio.h>
#include <pthread.h> // Host OS의 스레드 라이브러리 사용


static bool g_is_initialized = false;

vmath_status_t vmath_drv_init(void) {
    if (!g_is_initialized) {
        printf("[Virtual Driver] 가상 RVV 가속기 부팅 및 초기화 완료.\n");
        g_is_initialized = true;
    }
    return VMATH_OK;
}

vmath_status_t vmath_drv_lock(void) {
    // 다른 스레드가 가속기를 쓰고 있다면 여기서 대기(Block)합니다.

    // printf("[Virtual Driver] 가속기 점유 (Lock)\n"); // 디버깅용
    return VMATH_OK;
}

void vmath_drv_unlock(void) {
    // printf("[Virtual Driver] 가속기 반납 (Unlock)\n"); // 디버깅용

}
