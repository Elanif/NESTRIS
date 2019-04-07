#pragma once
#ifndef SCORE_H
#define SCORE_H
#include<cstdio>
#include "Renderer.h"

class Score : public Renderer
{
    public:
        Score(){};
        Score(SDL_Window * _window, const size_t& _frameappearance);
        Score(SDL_Window * _window, const size_t& _frameappearance, const bool& _maxout);
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
        void lastdigitcheck();
        unsigned char score[3];
        //unsigned char lowbyte;
        //unsigned char midbyte;
        //unsigned char highbyte;
        unsigned char pointsarray[10];

};

#endif // SCORE_H
