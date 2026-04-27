#include "vmath_driver.h"
#include <zephyr/kernel.h>

K_MUTEX_DEFINE(g_vmath_mutex); // Zephyr 전용 Mutex

vmath_status_t vmath_drv_init(void) { return VMATH_OK; }

vmath_status_t vmath_drv_lock(void) {
    k_mutex_lock(&g_vmath_mutex, K_FOREVER);
    // [Zephyr 전용 해킹] 여기서 벡터 유닛 전원 ON (csrs mstatus)
    return VMATH_OK;
}

vmath_status_t vmath_drv_unlock(void) {
    k_mutex_unlock(&g_vmath_mutex);
    // [Zephyr 전용 해킹] 여기서 벡터 유닛 전원 ON (csrs mstatus)
    return VMATH_OK;
}
