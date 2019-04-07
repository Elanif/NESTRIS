#pragma once
#ifndef SCORE_H
#define SCORE_H
#include<cstdio>
#include "Renderer.h"
#include"enums.h"
#include"TileContainer.h"

class Score : public Renderer
{
    public:
        Score(){};
        Score(TileContainer *_tilecont, const nes_ushort& _frameappearance);
        Score(TileContainer *_tilecont, const nes_ushort& _frameappearance, const bool& _maxout);
        unsigned int getscore();
        void softdrop(nes_uchar);
        void lineclear(const nes_uchar& level, const nes_uchar& linescleared) ; //TODO var type
        void render();
    protected:

    private:
        TileContainer *_tilecont;
        bool maxout;
        //void lowbytecheck();
        //void lowbytecheck2();
        void bytechecklowdigit(const std::size_t& byte, const bool& andop);
        void bytecheckhighdigit(const std::size_t& byte, const bool& andop);
        void lastdigitcheck();
        nes_uchar score[3];
        //unsigned char lowbyte;
        //unsigned char midbyte;
        //unsigned char highbyte;
        nes_uchar pointsarray[10];

};

#endif // SCORE_H
