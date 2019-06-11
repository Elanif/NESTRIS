#pragma once
#ifndef RANDOM_H
#define RANDOM_H
#include"enums.hpp"

class random
{
    public:
        random();
    static nes_ushort prng(const nes_ushort& value);
    static nes_ushort prng();
    static nes_ushort get();
    protected:

    private:
        static nes_ushort randomvalue;
};

#endif // RANDOM_H
