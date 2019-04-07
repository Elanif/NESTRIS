#include "random.h"
#include<cstdio>
unsigned short random::randomvalue=0x8988;

unsigned short random::prng(unsigned short value) {
    //printf("rng: %d -> %d\n",value,((((value >> 9) & 1) ^ ((value >> 1) & 1)) << 15) | (value >> 1));
    return ((((value >> 9) & 1) ^ ((value >> 1) & 1)) << 15) | (value >> 1);
}

unsigned short random::prng() {
    return randomvalue=prng(randomvalue);
}

unsigned short random::get() {
    return randomvalue;
}
