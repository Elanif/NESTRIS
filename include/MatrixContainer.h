#ifndef MATRIXCONTAINER_H
#define MATRIXCONTAINER_H
#include"SDL.h"
#include"enums.h"
#include"Renderer.h"
#include"PieceContainer.h"
#include"PFMatrix.h"
#include"Piece.h"
#include<cstdio>

class MatrixContainer : public Renderer
{
    public:
        MatrixContainer() {};
        MatrixContainer(SDL_Window * _window, const nes_ushort& _frameappearance);
        bool collision(const Piece& _piece) const;
        nes_uchar lockpiece(const Piece& _piece);
        nes_uchar clearlines();
        const PFMatrix& getMatrix() const {return matrix;};
        nes_uchar getBlock(const nes_uchar& x, const nes_uchar& y);
        void render();
    protected:

    private:
        nes_uchar blinkscreencounter;
        nes_uchar linesclearedarray[22];
        PFMatrix newmatrix;
        PFMatrix matrix;
};

#endif // MATRIXCONTAINER_H
