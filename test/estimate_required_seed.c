#include "fips202.h"
#include "parameters.h"

#include <stdint.h>
#include <stdio.h>

#define coeff_bit 2
#define xof_block_byte SHAKE256_RATE // 136 bytes

#define SMAUG1_DIMENSION LWE_N * 2
#define SMAUG1_HS 140
#define SMAUG1_HR 132

#define SMAUG3_DIMENSION LWE_N * 3
#define SMAUG3_HS 198
#define SMAUG3_HR 151

#define SMAUG5_DIMENSION LWE_N * 5
#define SMAUG5_HS 176
#define SMAUG5_HR 160

void required_size(int hwt, int d);
int parse_coeff_in_one_seed(int hwt, int d);
void parse_coeff_in_diff_seed(int req[], int hwt, int d);

int main() {
    required_size(SMAUG1_HS, 20);
    required_size(SMAUG3_HS, 20);
    required_size(SMAUG5_HS, 20);

    required_size(SMAUG1_HS, 32);
    required_size(SMAUG3_HS, 32);
    required_size(SMAUG5_HS, 32);

    return 0;
}

// estimate required unit (32-bit seed) size
// - input : hwt(total required hamming weight), d(seed bit for each degree)
void required_size(int hwt, int d) {
    printf("\n>> sample %d indices with %d-bit seeds\n", hwt, d);
    int req1 = parse_coeff_in_one_seed(hwt, d);
    int req2[2] = {0};
    parse_coeff_in_diff_seed(req2, hwt, d);
}

int req_unit(int unit, int hwt, int d) {
    /* LCM(unit, d) = n_unit*unit */
    int n_unit = 1;
    while ((n_unit * unit) % d > 6) // unit per block
        ++n_unit;
    int n_coeff = (n_unit * unit) / d; // coeff per block

    /* req = ceil(hwt/n_coeff) */
    double req_q = ((double)hwt / n_coeff);
    int req = (int)req_q;
    if ((double)req == req_q) {
        req = req * n_unit;
    } else {
        req = (++req) * n_unit;
    }

    return req;
}

int req_xof_block(int req_byte) {
    int n_block = 0;
    while ((n_block * xof_block_byte) < req_byte)
        ++n_block;

    return n_block;
}

// sample degree & coeff in one seed
// - input  : hwt(total required hamming weight), d(seed bit for each degree)
// - output : req1(total required unit)
int parse_coeff_in_one_seed(int hwt, int d) {
    printf("  >>> sample deg & coeff in one seed\n");
    int unit = 32;
    int unit_byte = unit / 8;

    int req1 = req_unit(unit, hwt, d + coeff_bit);
    printf("   - required uint%d_t =\t%d\t(%d-byte)\n", unit, req1,
           req1 * unit_byte);

    int req2 = req_xof_block(req1 * unit_byte);
    printf("   - required xof block =\t%d\t(%d-byte)\n", req2,
           req2 * xof_block_byte);

    return req1;
}

// sample degree in 32-bit seed, coeff in 16-bit seed
// - input  : hwt(total required hamming weight), d(seed bit for each degree)
// - output : req(total required bytes)
void parse_coeff_in_diff_seed(int req[], int hwt, int d) {
    printf("  >>> sample deg & coeff in other seed\n");
    int coeff_unit = 16;
    int coeff_unit_byte = coeff_unit / 8;
    int deg_unit = 32;
    int deg_unit_byte = deg_unit / 8;

    req[0] = req_unit(coeff_unit, hwt, coeff_bit);
    printf("   - required coeff uint%d_t  =\t%d\t(%d-byte)\n", coeff_unit,
           req[0], req[0] * coeff_unit_byte);

    req[1] = req_unit(deg_unit, hwt, d);
    printf("   - required deg uint%d_t   =\t%d\t(%d-byte)\n", deg_unit, req[1],
           req[1] * deg_unit_byte);

    int req2 = req_xof_block(req[0] * coeff_unit_byte + req[1] * deg_unit_byte);
    printf("   - required xof block  =\t%d\t(%d-byte)\n", req2,
           req2 * xof_block_byte);
}