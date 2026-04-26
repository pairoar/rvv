#include "vmath_driver.h"
#include <stdio.h>
#include <zephyr/kernel.h>

K_MUTEX_DEFINE(vmath_hw_mutex);
static int irq_key;

// [핵심] RVV 1.0 스펙에 맞게 VS 비트 위치를 9번으로 수정!
#define MSTATUS_VS_INITIAL (1 << 9)
#define MSTATUS_VS_MASK (3 << 9)

void vmath_drv_init(void) {
    printf("[VMath Driver] Hardware Accelerator Initialized (Mutex Ready).\n");
}

void vmath_drv_lock(void) {
    k_mutex_lock(&vmath_hw_mutex, K_FOREVER);

    // 1. RTOS 타이머 인터럽트 차단
    irq_key = irq_lock();

    // 2. 벡터 가속기 전원 강제 ON (RVV 1.0 스펙: 9번 비트)
    __asm__ volatile("csrs mstatus, %0" ::"r"(MSTATUS_VS_INITIAL));

    // 3. 디버깅 방어막
    uint32_t check;
    __asm__ volatile("csrr %0, mstatus" : "=r"(check));
    if ((check & MSTATUS_VS_MASK) == 0) {
        printf("\n🚨 [CRITICAL] Hardware rejected mstatus.VS! Check QEMU Vector support.\n\n");
    }
}

void vmath_drv_unlock(void) {
    // 1. 벡터 가속기 전원 OFF
    __asm__ volatile("csrc mstatus, %0" ::"r"(MSTATUS_VS_MASK));

    // 2. 인터럽트 차단 해제
    irq_unlock(irq_key);

    k_mutex_unlock(&vmath_hw_mutex);
}
