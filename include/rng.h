#ifndef RNG_h
#define RNG_h

#include <stdio.h>

#define RNG_SUCCESS (0)
#define RNG_FAILURE (1)
#define RNG_BAD_MAXLEN (-1)
#define RNG_BAD_OUTBUF (-2)
#define RNG_BAD_REQ_LEN (-3)

int randombytes(unsigned char *x, size_t xlen);

#endif /* RNG_h */
