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
        // printf("i = %d, rand = %10x, deg = %d\n", i, rand, deg);

        res[i] = res[deg];
        res[deg] =
            ((*((uint8_t *)buf + sizeof(uint32_t) * hmwt + pos)) & 0x02) - 1;
        // printf("pos: %d, coef: %02x, res: %d\n", pos,
        //        (*((uint8_t *)buf + sizeof(uint32_t) * hmwt + pos)),
        //        res[deg]);
        pos++;
    }

    size_t cnt_arr_idx = 0;
    for (i = 0; i < DIMENSION; ++i) {
        cnt_arr_idx = ((i & 0x700) >> 8) & (-(res[i] & 0x01));
        cnt_arr[cnt_arr_idx] += (res[i] & 0x01);
    }
}