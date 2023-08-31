#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "fips202.h"
#include "hwt.h"
#include "parameters.h"
#include "rng.h"

#define NSAMPLE 20   // 20
#define NTESTS 10000 // 100000 // HS * NTEST < 2^64

void (*hwt_callback)(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input,
                     const size_t input_size, const uint16_t hmwt);

void (*hwt_deg_callback)(uint64_t *deg_dist, uint64_t *cnt_dist,
                         const uint8_t *input, const size_t input_size,
                         const uint16_t hmwt);

void test_hwt(int original);      // file output for degree
void test_hwt_deg(int original);  // print
void test_hwt_rand(int original); // print
void test_hwt_i(int original);    // print

int main() {
    // printf("** HS = %d, NSAMPLE:%d NTESTS: %d\n", HS, NSAMPLE, NTESTS);

    /* degree(count) dist check */
    test_hwt(1); // SMAUG origin
    test_hwt(0); // SMAUG update

    /* degree dist check */
    // test_hwt_deg(1);
    // test_hwt_deg(0);

    /* rand dist check */
    // test_hwt_rand(1);
    // test_hwt_rand(0);

    /* degree dist for each i check*/
    // test_hwt_i(1);
    // test_hwt_i(0);

    return 0;
}

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

// (cnt) Save degree and cnt_arry distributions to file
void test_hwt(int original) {
    char suf[10] = "";

    if (original) {
        hwt_deg_callback = (void *)hwt_degree;
        strcat(suf, "origin");
    } else {
        hwt_deg_callback = (void *)hwt_bike_degree;
        strcat(suf, "update-2");
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

// (val) Print the degree for each poly (HS for one line)
void test_hwt_deg(int original) {
    if (original) {
        hwt_deg_callback = (void *)hwt_degree_test;
    } else {
        hwt_deg_callback = (void *)hwt_bike_degree_test;
    }

    printf("%d %d\n", HS, NTESTS);

    for (int s = 0; s < NSAMPLE; ++s) {
        for (int i = 0; i < NTESTS; ++i) {
            uint64_t res[DIMENSION] = {0};
            uint64_t cnt_arr[MODULE_RANK] = {0};
            uint8_t seed[CRYPTO_BYTES + PKSEED_BYTES] = {0};
            randombytes(seed, CRYPTO_BYTES);
            shake128(seed, CRYPTO_BYTES + PKSEED_BYTES, seed, CRYPTO_BYTES);

            hwt_deg_callback(res, cnt_arr, seed, CRYPTO_BYTES, HS);
        }
    }
}