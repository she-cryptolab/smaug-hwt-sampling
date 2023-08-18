#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "fips202.h"
#include "hwt.h"
#include "parameters.h"
#include "rng.h"

#define NTESTS 100000 // HS * NTEST < 2^64

// distribution of cnt for each coefficient in sx[LWE_N]
void sx_degree_dist(uint64_t *deg_dist, char *filename) {
    FILE *fp = fopen(filename, "w");

    fprintf(fp, "%d %d %d\n", HS, NTESTS, DIMENSION);
    for (int i = 0; i < DIMENSION; ++i)
        fprintf(fp, "%ld ", deg_dist[i]);

    fclose(fp);
}

uint64_t sx_degree_cnt_dist(uint64_t *deg_cnt, char *filename) {
    int total = 0;
    FILE *fp = fopen(filename, "w");
    fprintf(fp, "%d %d %d\n", HS, NTESTS, MODULE_RANK);
    for (int i = 0; i < MODULE_RANK; ++i) {
        fprintf(fp, "%ld ", deg_cnt[i]);
        total += deg_cnt[i];
    }
    fclose(fp);
    return total;
}

int main() {
    uint64_t dist_res[DIMENSION] = {0};  // degree distribution
    uint64_t cnt_res[MODULE_RANK] = {0}; // degree count for each vector section

    for (int i = 0; i < NTESTS; ++i) {
        uint8_t seed[CRYPTO_BYTES + PKSEED_BYTES] = {0};
        randombytes(seed, CRYPTO_BYTES);
        shake128(seed, CRYPTO_BYTES + PKSEED_BYTES, seed, CRYPTO_BYTES);

        hwt_bike_degree(dist_res, cnt_res, seed, CRYPTO_BYTES, HS);
    }

    char fname[20] = "";
    printf("** HS = %d, NTESTS = %d\n", HS, NTESTS);
    sprintf(fname, "smaug%d-deg-dist.txt", SMAUG_MODE);
    sx_degree_dist(dist_res, fname);

    sprintf(fname, "smaug%d-deg-cnt.txt", SMAUG_MODE);
    uint64_t total = sx_degree_cnt_dist(cnt_res, fname);
    printf("\n- total cnt = %ld /(%d)", total, HS * NTESTS);

    return 0;
}