#include <stdint.h>
#include <stdio.h>

#include "fips202.h"
#include "parameters.h"

#define NUNIT 3

void main() {

    uint32_t i = 0;
    uint32_t buf[SHAKE256_RATE * 2] = {0};
    uint8_t res[DIMENSION] = {0};

    // 48 * 2-bit coeff
    uint32_t rand[NUNIT] = {0x334B82, 0x000000, 0x325C28};
    // [0]: (0 0 1 1 0 0 1) 1 0 1 0 0 1 0 1 1 1 0 0 0 0 0 1 (0)
    // [1]: (0 0 0 0 0 0 0) 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 (0)
    // [2]: (0 0 1 1 0 0 1) 0 0 1 0 1 1 1 0 0 0 0 1 0 1 0 0 (0)

    int encoded_rand[NUNIT * 16] = {
        1,  -1, -1, -1, -1, -1, 1,  1,  1,  -1, 1,  -1, -1, 1,  -1, 1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, 1,  -1, 1,  -1, -1, -1, -1, 1,  1,  1,  -1, 1,  -1, -1};

    printf(">> degree set\n");
    for (i = 0; i < NUNIT * 16; ++i) {
        buf[i] = i;
        printf("[%d]:%06x ", i, buf[i]);
    }
    printf("\n>> coef set\n");
    for (i = 0; i < NUNIT; ++i) {
        buf[HS + i] = rand[i];
        printf("[%d]:%06x ", HS + i, buf[HS + i]);
    }

    printf("\n\n>> **expected\n");
    for (i = 0; i < NUNIT * 16; ++i)
        printf("[%d]:%02x ", i, (uint8_t)encoded_rand[i]);

    uint32_t pos = 0;
    for (i = 0; i < NUNIT * 16; ++i) {
        uint32_t deg = buf[pos];
        res[deg] = ((buf[HS + (pos >> 4)] >> (pos & 0x0f)) & 0x02) - 1;
        // printf("[%d] %d, %d \n", pos, deg, pos >> 4);
        pos++;
    }
    printf("\n\n>> **encoded\n");
    for (i = 0; i < NUNIT * 16; ++i)
        printf("[%d]:%02x ", i, res[i]);
    printf("\n");
}