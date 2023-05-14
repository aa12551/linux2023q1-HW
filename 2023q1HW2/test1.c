#include <stdint.h>
#include <stdio.h>
static inline uint64_t pow2(uint8_t e) { return ((uint64_t)1) << e; }

uint64_t next_pow2(uint64_t x)
{
    x |= x >> 1;
    x |= x >> 1;
    x |= x >> 1;
    x |= x >> 1;
    x |= x >> 1;
    x |= x >> 1;
    x |= x >> 1;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    return x+1;
}

int main()
{
	printf("%ld\n", next_pow2(2048));
}
