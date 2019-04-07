#pragma once
#ifndef RENDERPLAYFIELD_H
#define RENDERPLAYFIELD_H
#include "SDL.h"
#include "Renderer.h"
#include"ActiveInputs.h"

class RenderPlayField : public Renderer
{
    public:
        RenderPlayField(SDL_Window * _window, size_t _level);
        int renderandupdate(const ActiveInputs& _input, const unsigned long long& framecounter) ;
        void resetPlayField(const size_t& _level);
    private:
        int lockpiece();
        int updatePlayField(const ActiveInputs& _input) ;
        void renderPlayField(const unsigned long long& framecounter);
        int** pfmatrix;
        void renderSquare(const Uint8& red, const Uint8& green, const Uint8& blue, const Uint8& alpha, const size_t& x, const size_t& y, const size_t& w, const size_t& h);
        size_t level;
        unsigned int das;
        unsigned int fallcounter;
        unsigned int gravity[255];
        int spawnpiececounter;
        int blinkscreencounter;
        short spawnCount;
        void spawnPiece();
        int clearlines();
        int ** newmatrix;
        size_t linescleared;
        int linesclearedarray[22];


        struct PieceDetails{
            PieceDetails() {
                x=5;
                y=3;
                currentpiece=1;
                rotation=0;
                color=2;
            }
            int x;
            int y;
            int currentpiece;
            int rotation;
            int color;
        } piece, prevpiece;

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
        static char spawnTable[7];
};

#endif // RENDERPLAYFIELD_H
