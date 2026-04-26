#ifndef VMATH_DRIVER_H
#define VMATH_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief VMath 하드웨어 드라이버 초기화
 * (부팅 시 main에서 한 번 호출)
 */
void vmath_drv_init(void);

/**
 * @brief 하드웨어 가속기 독점 (Lock)
 * 다른 스레드가 사용 중이면 대기(Block) 상태로 전환됨
 */
void vmath_drv_lock(void);

/**
 * @brief 하드웨어 가속기 사용 완료 및 반환 (Unlock)
 */
void vmath_drv_unlock(void);

#ifdef __cplusplus
}
#endif

#endif // VMATH_DRIVER_H
