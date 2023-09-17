#include "hwt.h"
#include <stdio.h>

#define b_off 2
#define r_off 3

#define b_offset 5
#define r_offset 7

#if SMAUG_MODE == 1
#define HS_SEED 100 // size of 32-bit units
#define HS_XOF 3    // number of xof calls

#define HS_SEED_32 154
#define HS_XOF_32 5

#elif SMAUG_MODE == 3
#define HS_SEED 145
#define HS_XOF 5
#define HS_SEED_32 224
#define HS_XOF_32 7

#elif SMAUG_MODE == 5
#define HS_SEED 130
#define HS_XOF 4
#define HS_SEED_32 196
#define HS_XOF_32 6
#endif

/*************************** packing variation *******************************/
void hwt_32_block(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input,
                  const size_t input_size, const uint16_t hmwt) {

    uint32_t i, pos = 0;
    uint32_t deg = 0;
    uint32_t buf[SHAKE256_RATE * 2] = {0};

    keccak_state state;
    shake256_init(&state);
    shake256_absorb_once(&state, input, input_size);
    shake256_squeezeblocks((uint8_t *)buf, HS_XOF_32, &state);

    for (i = 0; i < DIMENSION; ++i)
        res[i] = 0;

    for (i = DIMENSION - hmwt; i < DIMENSION; ++i) {
        uint64_t rand = (uint64_t)buf[pos] * (i + 1);
        deg = (uint32_t)(rand >> 32);

        res[i] = res[deg];
        res[deg] =
            ((*((uint8_t *)buf + sizeof(uint32_t) * hmwt + pos)) & 0x02) - 1;

        pos++;
    }

    size_t cnt_arr_idx = 0;
    for (i = 0; i < DIMENSION; ++i) {
        cnt_arr_idx = ((i & 0x700) >> 8) & (-(res[i] & 0x01));
        cnt_arr[cnt_arr_idx] += (res[i] & 0x01);
    }
}

void hwt_22_block(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input,
                  const size_t input_size, const uint16_t hmwt) {

    uint32_t i = 0, b_idx = 0, r_idx = 0;
    uint32_t buf[SHAKE256_RATE * 2] = {0};
    uint32_t rand[SHAKE256_RATE * 3] = {0};

    keccak_state state;
    shake256_init(&state);
    shake256_absorb_once(&state, input, input_size);
    shake256_squeezeblocks((uint8_t *)buf, HS_XOF, &state);

    for (i = 0; i < DIMENSION; ++i)
        res[i] = 0;

    while (r_idx < hmwt) {
        for (i = 0; i < b_offset; ++i)
            rand[r_idx + i] = buf[b_idx + i] & 0x3fffff;

        rand[r_idx + 5] = ((buf[b_idx] >> 2) & 0x300000) |
                          ((buf[b_idx + 1] >> 12) & 0xffc00) |
                          ((buf[b_idx + 2] >> 22) & 0x3ff);
        rand[r_idx + 6] = ((buf[b_idx] >> 4) & 0x300000) |
                          ((buf[b_idx + 3] >> 12) & 0xffc00) |
                          ((buf[b_idx + 4] >> 22) & 0x3ff);

        r_idx += r_offset;
        b_idx += b_offset;
    }

    uint32_t pos = 0, deg = 0;
    for (i = DIMENSION - hmwt; i < DIMENSION; ++i) {
        deg = (rand[pos] & 0xfffff) * (i + 1);
        deg = (uint32_t)(deg >> 20);

        res[i] = res[deg];
        res[deg] = ((rand[pos] >> 20) & 0x02) - 1;
        pos++;
    }

    size_t cnt_arr_idx = 0;
    for (i = 0; i < DIMENSION; ++i) {
        cnt_arr_idx = ((i & 0x700) >> 8) & (-(res[i] & 0x01));
        cnt_arr[cnt_arr_idx] += (res[i] & 0x01);
    }
}

void hwt_21_block(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input,
                  const size_t input_size, const uint16_t hmwt) {

    uint32_t i = 0, b_idx = 0, r_idx = 0;
    uint32_t buf[SHAKE256_RATE * 2] = {0};
    uint32_t rand[SHAKE256_RATE * 3] = {0};

    keccak_state state;
    shake256_init(&state);
    shake256_absorb_once(&state, input, input_size);
    shake256_squeezeblocks((uint8_t *)buf, HS_XOF, &state);

    for (i = 0; i < DIMENSION; ++i)
        res[i] = 0;

    while (r_idx < hmwt) {
        rand[r_idx] = buf[b_idx] & 0x1fffff;
        rand[r_idx + 1] = buf[b_idx + 1] & 0x1fffff;
        rand[r_idx + 2] =
            ((buf[b_idx] >> 11) & 0x1ffc00) | ((buf[b_idx + 1] >> 21) & 0x3ff);

        r_idx += r_off;
        b_idx += b_off;
    }

    uint32_t pos = 0, deg = 0;
    for (i = DIMENSION - hmwt; i < DIMENSION; ++i) {
        deg = (rand[pos] & 0xfffff) * (i + 1);
        deg = (uint32_t)(deg >> 20);

        res[i] = res[deg];
        res[deg] = ((rand[pos] >> 19) & 0x02) - 1;
        pos++;
    }

    size_t cnt_arr_idx = 0;
    for (i = 0; i < DIMENSION; ++i) {
        cnt_arr_idx = ((i & 0x700) >> 8) & (-(res[i] & 0x01));
        cnt_arr[cnt_arr_idx] += (res[i] & 0x01);
    }
}

/***************************** (cnt) FILE TEST ********************************/
