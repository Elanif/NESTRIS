#pragma once
#ifndef RANDOM_H
#define RANDOM_H
#include"ntris.hpp"
#include"StaticClass.hpp"

class random : StaticClass
{
    public:
    static nes_ushort prng(const nes_ushort& value);
    static nes_ushort prng();
    static nes_ushort get();
    protected:

    private:
        static nes_ushort randomvalue;
};

#endif // RANDOM_H
