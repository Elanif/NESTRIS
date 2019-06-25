#include"random.hpp"
#include<cstdio>
nes_ushort random::randomvalue=0x8988; //starting rng value of the original tetris rom

nes_ushort random::prng(const nes_ushort& value) {
    //printf("rng: %d -> %d\n",value,((((value >> 9) & 1) ^ ((value >> 1) & 1)) << 15) | (value >> 1));
    return ((((value >> 9) & 1) ^ ((value >> 1) & 1)) << 15) | (value >> 1);
}

nes_ushort random::prng() {
    return randomvalue=prng(randomvalue);
}

nes_ushort random::get() {
    return randomvalue;
}
