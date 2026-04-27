// --- zephyr_app/src/main.c ---

#include "vmath_driver.h"
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/poweroff.h>

// 기존 애플리케이션 계층 함수들 선언
extern void test_mlp_xor(void);
extern void test_dsp_iir(void);
extern void test_image_conv2d(void);

int main(void) {
    // VMath 하드웨어 드라이버 초기화 (뮤텍스 준비 등)
    vmath_drv_init();

    printf("==================================\n");
    printf(" VMath on Zephyr RTOS Booted!\n");
    printf("==================================\n");

    // -------------------------------------------------------------
    // [핵심 해킹 포인트]
    // mstatus 레지스터의 VS(Vector Status, 23~24번 비트)를
    // 01(Initial 상태)로 강제로 덮어써서 벡터 가속기 전원을 켭니다!
    // -------------------------------------------------------------
    // __asm__ volatile("csrs mstatus, %0" ::"r"(1 << 23));

    // Zephyr 커널 부팅 완료 후 진입
    printf("==================================\n");
    printf(" VMath on Zephyr RTOS Booted!\n");
    printf("==================================\n");

    // RTOS 환경 위에서 하드웨어 가속기 테스트 실행
    test_mlp_xor();

    // 약간의 딜레이(Sleep)를 주며 멀티태스킹 흉내내기
    k_msleep(1000);

    test_dsp_iir();
    k_msleep(1000);

    test_image_conv2d();

    printf("\n--- All Tasks Completed Successfully ---\n");

    // 시스템 종료
    // sys_poweroff();
    return 0;
}
