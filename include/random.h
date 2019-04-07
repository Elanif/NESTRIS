#pragma once
#ifndef RANDOM_H
#define RANDOM_H


class random
{
    public:
        random();
    static unsigned short prng(unsigned short value);
    static unsigned short prng();
    static unsigned short get();
    protected:

    private:
        static unsigned short randomvalue;
};

#endif // RANDOM_H
