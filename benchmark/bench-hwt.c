#include <stdio.h>
#include <time.h>

#include "cpucycles.h"
#include "speed_print.h"

#include "fips202.h"
#include "hwt.h"
#include "parameters.h"
#include "rng.h"

#define NTESTS 10000
uint64_t t[NTESTS] = {0};

void overhead(unsigned long long c1, unsigned long long c2) {
    double diff_c = (double)c2 - c1;
    diff_c = (double)(diff_c / c1);

    printf(" - cycle overhead: %.2f (%d%%)\n\n", diff_c, (int)(diff_c * 100));
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

    /* SMAUG */
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

    /* NTRU */
    srt = clock();
    for (int i = 0; i < NTESTS; ++i) {
        t[i] = cpucycles();
        sample_fixed_type(res, cnt_arr, seed, CRYPTO_BYTES, HS);
    }
    ed = clock();

    unsigned long long res_ntru[2] = {0};
    print_results(res_ntru, "NTRU hwt: ", t, NTESTS);
    printf(" - time elapsed: %.8f ms\n",
           (double)(ed - srt) * 1000 / CLOCKS_PER_SEC / NTESTS);
    overhead(res_smaug[1], res_ntru[1]);

    /* BIKE */
    srt = clock();
    for (int i = 0; i < NTESTS; ++i) {
        t[i] = cpucycles();
        hwt_bike(res, cnt_arr, seed, CRYPTO_BYTES, HS);
    }
    ed = clock();

    unsigned long long res_bike[2] = {0};
    print_results(res_bike, "BIKE hwt: ", t, NTESTS);
    printf(" - time elapsed: %.8f ms\n",
           (double)(ed - srt) * 1000 / CLOCKS_PER_SEC / NTESTS);
    overhead(res_smaug[1], res_bike[1]);

    return 0;
}