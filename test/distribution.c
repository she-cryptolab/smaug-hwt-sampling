#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "fips202.h"
#include "hwt.h"
#include "parameters.h"
#include "rng.h"

#define NSAMPLE 20
#define NTESTS 100000 // HS * NTEST < 2^64

void (*hwt_deg_callback)(uint64_t *deg_dist, uint64_t *cnt_dist,
                         const uint8_t *input, const size_t input_size,
                         const uint16_t hmwt);

// distribution of cnt for each coefficient in sx[LWE_N]
void sx_degree_dist(uint64_t *deg_dist, FILE *file) {
    for (int i = 0; i < DIMENSION; ++i)
        fprintf(file, "%ld ", deg_dist[i]);
    fprintf(file, "\n");
}

void sx_degree_cnt_dist(uint64_t *deg_cnt, FILE *file) {
    for (int i = 0; i < MODULE_RANK; ++i)
        fprintf(file, "%ld ", deg_cnt[i]);
    fprintf(file, "\n");
}

void test_hwt(int original) {
    char suf[10] = "";

    if (original) {
        hwt_deg_callback = (void *)hwt_degree;
        strcat(suf, "origin");
    } else {
        hwt_deg_callback = (void *)hwt_bike_degree;
        strcat(suf, "update");
    }

    char fname[30] = "";
    sprintf(fname, "smaug%d-deg-dist-%s.txt", SMAUG_MODE, suf);
    FILE *fp_dist = fopen(fname, "w");
    fprintf(fp_dist, "%d %d %d\n", HS, NTESTS, DIMENSION);

    sprintf(fname, "smaug%d-deg-cnt-%s.txt", SMAUG_MODE, suf);
    FILE *fp_cnt = fopen(fname, "w");
    fprintf(fp_cnt, "%d %d %d\n", HS, NTESTS, MODULE_RANK);

    for (int s = 0; s < NSAMPLE; ++s) {
        uint64_t dist_res[DIMENSION] = {0};  // degree distribution
        uint64_t cnt_res[MODULE_RANK] = {0}; // degree count for each section

        for (int i = 0; i < NTESTS; ++i) {
            uint8_t seed[CRYPTO_BYTES + PKSEED_BYTES] = {0};
            randombytes(seed, CRYPTO_BYTES);
            shake128(seed, CRYPTO_BYTES + PKSEED_BYTES, seed, CRYPTO_BYTES);

            hwt_deg_callback(dist_res, cnt_res, seed, CRYPTO_BYTES, HS);
        }

        sx_degree_dist(dist_res, fp_dist);
        sx_degree_cnt_dist(cnt_res, fp_cnt);
    }

    fclose(fp_dist);
    fclose(fp_cnt);
}

int main() {
    printf("** HS = %d, NSAMPLE:%d NTESTS: %d\n", HS, NSAMPLE, NTESTS);
    test_hwt(1); // SMAUG origin
    test_hwt(0); // SMAUG update

    return 0;
}