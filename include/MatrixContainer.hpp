#ifndef MATRIXCONTAINER_H
#define MATRIXCONTAINER_H
#include"ntris.hpp"
#include"Renderer.hpp"
#include"PieceContainer.hpp"
#include"PFMatrix.hpp"
#include"Piece.hpp"
#include<cstdio>

class MatrixContainer : public Renderer
{
    public:
        MatrixContainer(TileContainer * _tilecont, const nes_ushort& _frameappearance);
        bool collision(const Piece& _piece) const;
        nes_uchar lockpiece(const Piece& _piece, const nes_ushort&  _framecounter);
        nes_uchar clearlines();
        const PFMatrix& getMatrix() const {return matrix;};
        nes_uchar getBlock(const nes_uchar& x, const nes_uchar& y);
        void render(const nes_uchar& _level);
        void reset();
    protected:

    private:
		nes_uchar linesclearedarray[22]{ 0 };
		nes_uchar linescleared{ 0 };
        PFMatrix newmatrix;
        PFMatrix matrix;
};

#endif // MATRIXCONTAINER_H
