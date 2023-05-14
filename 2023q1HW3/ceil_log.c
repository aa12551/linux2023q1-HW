#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int ceil_log2(uint32_t x)
{
    uint32_t r, shift;

    x--;
    r = (x > 0xFFFF) << 4;                 
    x >>= r;
    shift = (x > 0xFF) << 3;
    x >>= shift;
    r |= shift;
    shift = (x > 0xF) << 2;
    x >>= shift;
    r |= shift;
    shift = (x > 0x3) << 1;
    x >>= shift;
    return (r | shift | (x >> 1)) + 1;       
}

int main()
{
	int a = 256;
	printf("log2(%d) is %ld\n",a ,ceil_log2(a));  
	return 0;
}
