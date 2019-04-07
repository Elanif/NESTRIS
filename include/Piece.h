#ifndef PIECE_H
#define PIECE_H
#include"enums.h"
#include<vector>
#include<utility>

class Piece
{
    public:
        Piece();
        Piece(const char& charpiecetype);
        nes_uchar x, y, piecetype, rotation;
        nes_uchar color() const;
        std::vector<std::pair<nes_uchar, nes_uchar> > getPos() const;
        std::vector<std::pair<nes_schar, nes_schar> > nextpiecePos() const;

    static nes_schar rotationmatrix[28][4][2];
    static nes_uchar nextpiecespawn[7][2];
    protected:

    private:
};


#endif // PIECE_H
