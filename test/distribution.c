#include <stdint.h>
#include <stdio.h>

#include "fips202.h"
#include "hwt.h"
#include "parameters.h"
#include "rng.h"

#define NTESTS 100000 // HS * NTEST < 2^64

int main() {

    uint64_t dist_res[DIMENSION] = {0}; // degree distribution

    for (int i = 0; i < NTESTS; ++i) {
        uint8_t seed[CRYPTO_BYTES + PKSEED_BYTES] = {0};
        randombytes(seed, CRYPTO_BYTES);
        shake128(seed, CRYPTO_BYTES + PKSEED_BYTES, seed, CRYPTO_BYTES);

        hwt_bike_degree(dist_res, seed, CRYPTO_BYTES, HS);
    }

    printf("** HS = %d, NTESTS = %d\n", HS, NTESTS);
    // printf("** expected count [i]: %d\n\n", (int)((HS * NTESTS) /
    // DIMENSION));

    for (int i = 0; i < DIMENSION; ++i) {
        printf("%ld  ", dist_res[i]);
    }
}