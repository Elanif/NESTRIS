#ifndef MATRIXCONTAINER_H
#define MATRIXCONTAINER_H
#include"SDL.h"
#include"Renderer.h"
#include"PieceContainer.h"
#include"enums.h"

class MatrixContainer : public Renderer
{
    public:
        MatrixContainer() {};
        MatrixContainer(SDL_Window * _window, const nes_ushort& _frameappearance);
        bool collision(const Piece& _piece) const;
        nes_uchar lockpiece(const Piece& _piece);
        nes_uchar clearlines();
        nes_uchar getBlock(const nes_uchar& x, const nes_uchar& y) const;
    protected:

    private:
        nes_uchar linesclearedarray[22];
        nes_uchar newmatrix[10][22];
        nes_uchar matrix[10][22];
};

#endif // MATRIXCONTAINER_H
