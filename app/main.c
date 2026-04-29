#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"

int main(void);

extern void test_nn_dense(void);
extern void test_dsp_fir(void);
extern void test_dsp_iir(void);
extern void test_image_conv2d(void);
extern void test_mlp_xor(void);
extern int test_do_all_test(void);

int main(void) {
    printf("\n==================================\n");
    printf("VMath Simulator Version: %s\n", PROJECT_VERSION_STRING);
    printf("==================================\n");

    // test_nn_dense();
    // test_dsp_fir();
    // test_dsp_iir();
    // test_image_conv2d();
    test_mlp_xor();
    //test_do_all_test();

    return 0;
}
