#include "hwt.h"

#define int32 int32_t

#define int32_MINMAX(a, b)                                                     \
    do {                                                                       \
        int32_t ab = (b) ^ (a);                                                \
        int32_t c = (int32_t)((int64_t)(b) - (int64_t)(a));                    \
        c ^= ab & (c ^ (b));                                                   \
        c >>= 31;                                                              \
        c &= ab;                                                               \
        (a) ^= c;                                                              \
        (b) ^= c;                                                              \
    } while (0)

void crypto_sort_int32(int32 *array, size_t n);
void crypto_sort(void *array, long long n);

void sample_fixed_type(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input,
                       const size_t input_size, const uint16_t hmwt) {
    // Assumes NTRU_SAMPLE_FT_BYTES = ceil(30*(n-1)/8)

    int32_t s[DIMENSION];
    uint8_t u[30 * (DIMENSION / 8)];
    int i;

    shake128(u, 30 * (DIMENSION / 8), input, input_size);

    // Use 30 bits of u per word
    for (i = 0; i < (DIMENSION) / 4; i++) {
        s[4 * i + 0] = (u[15 * i + 0] << 2) + (u[15 * i + 1] << 10) +
                       (u[15 * i + 2] << 18) + (u[15 * i + 3] << 26);
        s[4 * i + 1] = ((u[15 * i + 3] & 0xc0) >> 4) + (u[15 * i + 4] << 4) +
                       (u[15 * i + 5] << 12) + (u[15 * i + 6] << 20) +
                       (u[15 * i + 7] << 28);
        s[4 * i + 2] = ((u[15 * i + 7] & 0xf0) >> 2) + (u[15 * i + 8] << 6) +
                       (u[15 * i + 9] << 14) + (u[15 * i + 10] << 22) +
                       (u[15 * i + 11] << 30);
        s[4 * i + 3] = (u[15 * i + 11] & 0xfc) + (u[15 * i + 12] << 8) +
                       (u[15 * i + 13] << 15) + (u[15 * i + 14] << 24);
    }

    for (i = 0; i < hmwt / 2; i++)
        s[i] |= 1;

    for (i = hmwt / 2; i < hmwt; i++)
        s[i] |= 2;

    // crypto_sort(s, DIMENSION);
    crypto_sort_int32(s, DIMENSION);

    for (i = 0; i < DIMENSION; i++)
        res[i] = ((uint16_t)(s[i] & 3));

    size_t cnt_arr_idx = 0;
    for (i = 0; i < DIMENSION; ++i) {
        cnt_arr_idx = ((i & 0x700) >> 8) & (-(res[i] & 0x01));
        cnt_arr[cnt_arr_idx] += (res[i] & 0x01);
    }
}

/* assume 2 <= n <= 0x40000000 */
void crypto_sort_int32(int32 *array, size_t n) {
    size_t top, p, q, r, i, j;
    int32 *x = array;

    top = 1;
    while (top < n - top)
        top += top;

    for (p = top; p >= 1; p >>= 1) {
        i = 0;
        while (i + 2 * p <= n) {
            for (j = i; j < i + p; ++j) {
                int32_MINMAX(x[j], x[j + p]);
            }
            i += 2 * p;
        }
        for (j = i; j < n - p; ++j) {
            int32_MINMAX(x[j], x[j + p]);
        }

        i = 0;
        j = 0;
        for (q = top; q > p; q >>= 1) {
            if (j != i) {
                for (;;) {
                    if (j == n - q)
                        goto done;
                    int32 a = x[j + p];
                    for (r = q; r > p; r >>= 1) {
                        int32_MINMAX(a, x[j + r]);
                    }
                    x[j + p] = a;
                    ++j;
                    if (j == i + p) {
                        i += 2 * p;
                        break;
                    }
                }
            }
            while (i + p <= n - q) {
                for (j = i; j < i + p; ++j) {
                    int32 a = x[j + p];
                    for (r = q; r > p; r >>= 1) {
                        int32_MINMAX(a, x[j + r]);
                    }
                    x[j + p] = a;
                }
                i += 2 * p;
            }
            /* now i + p > n - q */
            j = i;
            while (j < n - q) {
                int32 a = x[j + p];
                for (r = q; r > p; r >>= 1) {
                    int32_MINMAX(a, x[j + r]);
                }
                x[j + p] = a;
                ++j;
            }

        done:;
        }
    }
}

void crypto_sort(void *array, long long n) {
    long long top, p, q, r, i;
    int32_t *x = array;

    if (n < 2)
        return;
    top = 1;
    while (top < n - top)
        top += top;

    for (p = top; p > 0; p >>= 1) {
        for (i = 0; i < n - p; ++i)
            if (!(i & p))
                int32_MINMAX(x[i], x[i + p]);
        i = 0;
        for (q = top; q > p; q >>= 1) {
            for (; i < n - q; ++i) {
                if (!(i & p)) {
                    int32_t a = x[i + p];
                    for (r = q; r > p; r >>= 1)
                        int32_MINMAX(a, x[i + r]);
                    x[i + p] = a;
                }
            }
        }
    }
}