#pragma once
#ifndef RENDERPLAYFIELD_H
#define RENDERPLAYFIELD_H
#include "SDL.h"
#include "Renderer.h"
#include"ActiveInputs.h"
#include "Score.h"
#include"PieceContainer.h"
#include"MatrixContainer.h"
#include"LevelLines.h"

class RenderPlayField : public Renderer
{
    public:
        RenderPlayField(SDL_Window * _window, const size_t& _frameappearance, size_t _level);
        void update(const ActiveInputs& _input) ;
        void render(const unsigned long long& framecounter) ;
        void resetPlayField(const size_t& _level);
    private:

        MatrixContainer matrixhandler;
        PieceContainer piecehandler;
        Score scorehandler;
        LevelLines levellineshandler;
        size_t level;
        char gravity[255];
        /*
        int lockpiece();
        int updatePlayField(const ActiveInputs& _input) ;
        void renderPlayField(const unsigned long long& framecounter);
        int** pfmatrix;
        void renderSquare(const Uint8& red, const Uint8& green, const Uint8& blue, const Uint8& alpha, const size_t& x, const size_t& y, const size_t& w, const size_t& h);
        unsigned int das;
        unsigned int fallcounter;
        int spawnpiececounter;
        int blinkscreencounter;
        short spawnCount;
        void spawnPiece();
        int clearlines();
        int ** newmatrix;
        size_t linescleared;
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
