#ifndef PIECE_H
#define PIECE_H
#include"enums.h"
#include<vector>
#include<utility>

class Piece
{
    public:
        Piece();
        nes_uchar x, y, piecetype, rotation, color;
        std::vector<std::pair<nes_uchar, nes_uchar> > getPos() const;

    static nes_schar rotationmatrix[28][4][2];
    protected:

    private:
};


#endif // PIECE_H
