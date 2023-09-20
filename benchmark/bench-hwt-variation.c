#include <stdio.h>
#include <time.h>

#include "cpucycles.h"
#include "speed_print.h"

#include "fips202.h"
#include "hwt.h"
#include "parameters.h"
#include "rng.h"

#define NTESTS 100000
uint64_t t[NTESTS] = {0};

void overhead(unsigned long long c1, unsigned long long c2) {
    double diff_c = (double)c2 - c1;
    diff_c = (double)(diff_c / c1);

    printf(" - cycle overhead: %.2f (%d%%)\n\n", diff_c, (int)(diff_c * 100));
}

void xof_bench() {
    uint8_t seed[CRYPTO_BYTES + PKSEED_BYTES] = {0};
    randombytes(seed, CRYPTO_BYTES);
    shake128(seed, CRYPTO_BYTES + PKSEED_BYTES, seed, CRYPTO_BYTES);

    clock_t srt, ed;

    /* XOF 8-times */
    srt = clock();
    for (int i = 0; i < NTESTS; ++i) {
        t[i] = cpucycles();
        xof_32(seed, CRYPTO_BYTES);
    }

    unsigned long long res_0[2] = {0};
    ed = clock();
    print_results(res_0, "xof 8-times: ", t, NTESTS);
    printf(" - time elapsed: %.8f ms\n",
           (double)(ed - srt) * 1000 / CLOCKS_PER_SEC / NTESTS);
    overhead(res_0[1], res_0[1]);

    /* XOF 8-times */
    srt = clock();
    for (int i = 0; i < NTESTS; ++i) {
        t[i] = cpucycles();
        xof_20(seed, CRYPTO_BYTES);
    }

    unsigned long long res_1[2] = {0};
    ed = clock();
    print_results(res_1, "xof optimized: ", t, NTESTS);
    printf(" - time elapsed: %.8f ms\n",
           (double)(ed - srt) * 1000 / CLOCKS_PER_SEC / NTESTS);
    overhead(res_0[1], res_1[1]);
}

int main() {
    uint8_t seed[CRYPTO_BYTES + PKSEED_BYTES] = {0};
    randombytes(seed, CRYPTO_BYTES);
    shake128(seed, CRYPTO_BYTES + PKSEED_BYTES, seed, CRYPTO_BYTES);

    uint8_t cnt_arr[MODULE_RANK] = {0};
    uint8_t res[DIMENSION] = {0};

    clock_t srt, ed;
    clock_t latency;

    latency = clock();
    cpucycles();
    latency = clock() - latency;

    printf("******** SMAUG%d hwt benchmark ********\n", SMAUG_MODE);
    // xof_bench();

    /* SMAUG original */
    srt = clock();
    for (int i = 0; i < NTESTS; ++i) {
        t[i] = cpucycles();
        hwt(res, cnt_arr, seed, CRYPTO_BYTES, HS);
    }

    unsigned long long res_smaug[2] = {0};
    ed = clock();
    print_results(res_smaug, "SMAUG hwt: ", t, NTESTS);
    printf(" - time elapsed: %.8f ms\n",
           (double)(ed - srt) * 1000 / CLOCKS_PER_SEC / NTESTS);
    overhead(res_smaug[1], res_smaug[1]);

    /* 32-bit & block XOF*/
    srt = clock();
    for (int i = 0; i < NTESTS; ++i) {
        t[i] = cpucycles();
        // hwt_bike(res, cnt_arr, seed, CRYPTO_BYTES, HS);
        hwt_32_block(res, cnt_arr, seed, CRYPTO_BYTES, HS);
    }

    unsigned long long res_0[2] = {0};
    ed = clock();
    print_results(res_0, "hwt-32-block: ", t, NTESTS);
    printf(" - time elapsed: %.8f ms\n",
           (double)(ed - srt) * 1000 / CLOCKS_PER_SEC / NTESTS);
    overhead(res_smaug[1], res_0[1]);

    /* 22-bit & block XOF */
    srt = clock();
    for (int i = 0; i < NTESTS; ++i) {
        t[i] = cpucycles();
        hwt_22_block(res, cnt_arr, seed, CRYPTO_BYTES, HS);
    }

    unsigned long long res_1[2] = {0};
    ed = clock();
    print_results(res_1, "hwt-22-block: ", t, NTESTS);
    printf(" - time elapsed: %.8f ms\n",
           (double)(ed - srt) * 1000 / CLOCKS_PER_SEC / NTESTS);
    overhead(res_smaug[1], res_1[1]);

    /* 21-bit & block XOF */
    srt = clock();
    for (int i = 0; i < NTESTS; ++i) {
        t[i] = cpucycles();
        hwt_21_block(res, cnt_arr, seed, CRYPTO_BYTES, HS);
    }

    // unsigned long long res_2[2] = {0};
    // ed = clock();
    // print_results(res_2, "hwt-21-block: ", t, NTESTS);
    // printf(" - time elapsed: %.8f ms\n",
    //        (double)(ed - srt) * 1000 / CLOCKS_PER_SEC / NTESTS);
    // overhead(res_smaug[1], res_2[1]);

    return 0;
}