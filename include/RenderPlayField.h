#pragma once
#ifndef RENDERPLAYFIELD_H
#define RENDERPLAYFIELD_H
#include"TileContainer.h"
#include "Renderer.h"
#include"ActiveInputs.h"
#include "Score.h"
#include"PieceContainer.h"
#include"MatrixContainer.h"
#include"LevelLines.h"
#include"Statistics.h"
#include"enums.h"

class RenderPlayField : public Renderer
{
    public:
        RenderPlayField(TileContainer* _tilecont, const nes_ushort& _frameappearance, nes_uchar _level);
        void update(const ActiveInputs& _input, const nes_ushort& _framecounter) ;
        void render(const nes_ushort& framecounter) ;
        void resetPlayField(const nes_uchar& _level);
        void renderPlayField(const unsigned long long& framecounter);
    private:
        void renderimage(bool blink);
        void renderBackground(const nes_uchar& _color1, const nes_uchar& _color2);
        MatrixContainer matrixhandler;
        bool tetris;
        PieceContainer piecehandler;

        Score scorehandler;

        LevelLines levellineshandler;

        Statistics statisticshandler;

        nes_uchar level;
        nes_uchar gravity[255];

        bool firstframeis4;
        bool paused;
        bool playfield_blink=false;
        nes_uchar pausecounter;

        void init_assets();
        std::vector<tiletype> renderblink;
        /*
        int lockpiece();
        int updatePlayField(const ActiveInputs& _input) ;
        int** pfmatrix;
        void renderSquare(const Uint8& red, const Uint8& green, const Uint8& blue, const Uint8& alpha, const std::size_t& x, const std::size_t& y, const std::size_t& w, const std::size_t& h);
        unsigned int das;
        unsigned int fallcounter;
        int spawnpiececounter;
        int blinkscreencounter;
        short spawnCount;
        void spawnPiece();
        int clearlines();
        int ** newmatrix;
        std::size_t linescleared;
        int linesclearedarray[22];




        void renderPiece(PieceDetails _piece);
        bool checkcollision(PieceDetails& _piece, PieceDetails& _lastgoodpos);

        enum BUTTONS {
            LEFT,
            UP,
            RIGHT,
            DOWN,
            SELECT,
            START,
            B,
            A
        };
        static int rotationmatrix[28][4][2];
        static char spawnTable[7];*/
};

#endif // RENDERPLAYFIELD_H
