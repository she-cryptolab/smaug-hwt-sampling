#ifndef SMAUG_HWT_H
#define SMAUG_HWT_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "fips202.h"
#include "parameters.h"

void hwt(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input,
         const size_t input_size, const uint16_t hmwt);
void hwt_degree(uint64_t *deg_dist, uint64_t *cnt_dist, const uint8_t *input,
                const size_t input_size, const uint16_t hmwt);

void sample_fixed_type(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input,
                       const size_t input_size, const uint16_t hmwt);

void hwt_bike(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input,
              const size_t input_size, const uint16_t hmwt);
void hwt_bike_degree(uint64_t *deg_dist, uint64_t *cnt_dist,
                     const uint8_t *input, const size_t input_size,
                     const uint16_t hmwt);

/**** PRINT TEST *****/
void hwt_test(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input,
              const size_t input_size, const uint16_t hmwt);
void hwt_bike_test(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input,
                   const size_t input_size, const uint16_t hmwt);

void hwt_degree_test(uint64_t *deg_dist, uint64_t *cnt_dist,
                     const uint8_t *input, const size_t input_size,
                     const uint16_t hmwt);
void hwt_bike_degree_test(uint64_t *deg_dist, uint64_t *cnt_dist,
                          const uint8_t *input, const size_t input_size,
                          const uint16_t hmwt);

#endif // SMAUG_HWT_H