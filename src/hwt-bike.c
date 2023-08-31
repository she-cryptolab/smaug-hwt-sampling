#include "hwt.h"
#include <stdio.h>

/*************************************************
 * Name:        hwt
 *
 * Description: Hamming weight sampling deterministically to generate sparse
 *              polynomial r(x) from a seed. shake256 is the Extendable-Output
 *              Function from the SHA-3 family.
 *
 * Arguments:   - uint8_t *res: pointer to ouptput polynomial r(x)
 *                (of length LWE), assumed to be already initialized
 *              - uint8_t *input: pointer to input seed (of length input_size)
 *              - size_t input_size: length of input seed
 **************************************************/
void hwt_bike(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input,
              const size_t input_size, const uint16_t hmwt) {

    uint32_t i, pos = 0;
    uint32_t deg = 0;
    uint32_t buf[SHAKE256_RATE * 2] = {0};

    keccak_state state;
    shake256_init(&state);
    shake256_absorb_once(&state, input, input_size);
    shake256_squeezeblocks((uint8_t *)buf, 8, &state);

    for (i = 0; i < DIMENSION; ++i)
        res[i] = 0;

    for (i = DIMENSION - hmwt; i < DIMENSION; ++i) {
        uint64_t rand = (uint64_t)buf[pos] * i;
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

/***************************** (cnt) FILE TEST ********************************/

void hwt_bike_degree(uint64_t *deg_dist, uint64_t *cnt_dist,
                     const uint8_t *input, const size_t input_size,
                     const uint16_t hmwt) {

    uint32_t i, pos = 0;
    uint32_t deg = 0;
    uint32_t buf[SHAKE256_RATE * 2] = {0};
    uint8_t res[DIMENSION] = {0};

    keccak_state state;
    shake256_init(&state);
    shake256_absorb_once(&state, input, input_size);
    shake256_squeezeblocks((uint8_t *)buf, 8, &state);

    for (i = 0; i < DIMENSION; ++i)
        res[i] = 0;

    uint32_t deg_list[DIMENSION] = {0}; // degree i selected -> list[i]=1
    for (i = DIMENSION - hmwt; i < DIMENSION; ++i) {
        uint64_t rand = (uint64_t)buf[pos] * i;
        deg = (uint32_t)(rand >> 32);

        res[i] = res[deg];
        res[deg] =
            ((*((uint8_t *)buf + sizeof(uint32_t) * hmwt + pos)) & 0x02) - 1;

        deg_list[i] = deg_list[deg];
        deg_list[deg] = 1;
        pos++;
    }

    size_t cnt_arr_idx = 0;
    for (i = 0; i < DIMENSION; ++i) {
        deg_dist[i] += deg_list[i];
        cnt_arr_idx = ((i & 0x700) >> 8) & (-(res[i] & 0x01));
        cnt_dist[cnt_arr_idx] += (res[i] & 0x01);
    }
}

/**************************** (val) PRINT TEST ********************************/

void hwt_bike_degree_test(uint64_t *deg_dist, uint64_t *cnt_dist,
                          const uint8_t *input, const size_t input_size,
                          const uint16_t hmwt) {

    uint32_t i, pos = 0;
    uint32_t deg = 0;
    uint32_t buf[SHAKE256_RATE * 2] = {0};
    uint8_t res[DIMENSION] = {0};

    keccak_state state;
    shake256_init(&state);
    shake256_absorb_once(&state, input, input_size);
    shake256_squeezeblocks((uint8_t *)buf, 8, &state);

    for (i = 0; i < DIMENSION; ++i)
        res[i] = 0;

    uint32_t deg_list[DIMENSION] = {0}; // degree i selected -> list[i]=1
    for (i = DIMENSION - hmwt; i < DIMENSION; ++i) {
        uint64_t rand = (uint64_t)buf[pos] * i;
        // printf("%ld ", buf[pos]);
        // printf("%ld ", rand);
        deg = (uint32_t)(rand >> 32);

        if (res[deg] != 0) {
            printf("%ld ", i);
        } else {
            printf("%ld ", deg);
        }

        res[i] = res[deg];
        res[deg] =
            ((*((uint8_t *)buf + sizeof(uint32_t) * hmwt + pos)) & 0x02) - 1;

        // deg_list[i] = deg_list[deg];
        // deg_list[deg] = 1;
        pos++;
    }
    printf("\n");

    // size_t cnt_arr_idx = 0;
    // for (i = 0; i < DIMENSION; ++i) {
    //     deg_dist[i] += deg_list[i];
    //     cnt_arr_idx = ((i & 0x700) >> 8) & (-(res[i] & 0x01));
    //     cnt_dist[cnt_arr_idx] += (res[i] & 0x01);
    // }
}

void hwt_bike_test(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input,
                   const size_t input_size, const uint16_t hmwt) {

    uint32_t i, pos = 0;
    uint32_t deg = 0;
    uint32_t buf[SHAKE256_RATE * 2] = {0};

    keccak_state state;
    shake256_init(&state);
    shake256_absorb_once(&state, input, input_size);
    shake256_squeezeblocks((uint8_t *)buf, 8, &state);

    for (i = 0; i < DIMENSION; ++i)
        res[i] = 0;

    for (i = DIMENSION - hmwt; i < DIMENSION; ++i) {
        uint64_t rand = (uint64_t)buf[pos] * i;
        deg = (uint32_t)(rand >> 32);
        printf("%ld ", deg);

        // res[i] = res[deg];
        // res[deg] =
        //     ((*((uint8_t *)buf + sizeof(uint32_t) * hmwt + pos)) & 0x02) - 1;
        pos++;
    }
    printf("\n");

    // size_t cnt_arr_idx = 0;
    // for (i = 0; i < DIMENSION; ++i) {
    //     cnt_arr_idx = ((i & 0x700) >> 8) & (-(res[i] & 0x01));
    //     cnt_arr[cnt_arr_idx] += (res[i] & 0x01);
    // }
}