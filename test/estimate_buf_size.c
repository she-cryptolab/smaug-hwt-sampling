#include <stdint.h>
#include <stdio.h>

#include "fips202.h"
#include "parameters.h"

// estimate buffer length for degree sampling
int main() {

    printf("*** SMAUG MODE %d ***\n", SMAUG_MODE);
    printf("- XOF output bytes: %d\n",
           (int)(SHAKE256_RATE * sizeof(uint32_t)) * 2);

    uint32_t req_deg = (HS * 32) / 8;
    // uint32_t req_coef = (HS * 2) / 8;  // take coeff by (2-bit)
    uint32_t req_coef = HS; // take coeff by (8-bit & 0x02)
    printf("- HS require bytes: %d = %d + %d (deg + coef) \n",
           req_deg + req_coef, req_deg, req_coef);

    req_deg = (HR * 32) / 8;
    // req_coef = (HR * 2) / 8;
    req_coef = HR;
    printf("- HR require bytes: %d = %d + %d (deg + coef) \n",
           req_deg + req_coef, req_deg, req_coef);

    return 0;
}