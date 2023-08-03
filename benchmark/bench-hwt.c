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

int main() {
    uint8_t seed[CRYPTO_BYTES + PKSEED_BYTES] = {0};
    randombytes(seed, CRYPTO_BYTES);
    shake128(seed, CRYPTO_BYTES + PKSEED_BYTES, seed, CRYPTO_BYTES);

    uint8_t cnt_arr[MODULE_RANK] = {0};
    uint8_t res[DIMENSION] = {0};

    clock_t srt, ed;
    clock_t latency, overhead;

    latency = clock();
    cpucycles();
    latency = clock() - latency;

    /* SMAUG */
    srt = clock();
    for (int i = 0; i < NTESTS; ++i) {
        t[i] = cpucycles();
        hwt(res, cnt_arr, seed, CRYPTO_BYTES, HS);
    }
    ed = clock();
    print_results("SMAUG hwt: ", t, NTESTS);
    printf(" - time elapsed: %.8f ms\n\n",
           (double)(ed - srt) * 1000 / CLOCKS_PER_SEC / NTESTS);

    /* NTRU */
    srt = clock();
    for (int i = 0; i < NTESTS; ++i) {
        t[i] = cpucycles();
        sample_fixed_type(res, cnt_arr, seed, CRYPTO_BYTES, HS);
    }
    ed = clock();
    print_results("NTRU hwt: ", t, NTESTS);
    printf(" - time elapsed: %.8f ms\n\n",
           (double)(ed - srt) * 1000 / CLOCKS_PER_SEC / NTESTS);

    return 0;
}