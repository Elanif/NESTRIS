#pragma once
#ifndef SCORE_H
#define SCORE_H
#include<cstdio>

class Score
{
    public:
        Score();
        Score(bool _maxout);
        unsigned int getscore();
        void sofdrop(unsigned char);
        void lineclear(unsigned char level, unsigned char linescleared); //TODO var type
    protected:

    private:
        bool maxout;
        //void lowbytecheck();
        //void lowbytecheck2();
        void bytechecklowdigit(size_t byte, bool andop);
        void bytecheckhighdigit(size_t byte, bool andop);
        unsigned char score[3];
        //unsigned char lowbyte;
        //unsigned char midbyte;
        //unsigned char highbyte;
        unsigned char pointsarray[10];

};

#endif // SCORE_H
