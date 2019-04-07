#include "Piece.h"

Piece::Piece()
{
    x=5;
    y=2;
    piecetype=-1;
    rotation=0;
}

nes_uchar Piece::color() const{

    switch(piecetype) {
    case 0:
        return 1;
        break;
    case 1:
        return 2;
        break;
    case 2:
        return 3;
        break;
    case 3:
        return 1;
        break;
    case 4:
        return 2;
        break;
    case 5:
        return 3;
        break;
    case 6:
        return 1;
        break;
    default:
        return 0;
        break;
    }
}

std::vector<std::pair<nes_uchar, nes_uchar> > Piece::getPos() const {
    std::vector<std::pair<nes_uchar,nes_uchar> > result;
    if (piecetype<0 || piecetype>6) return result;
    for (std::vector<std::pair<nes_uchar,nes_uchar> >::size_type i=0; i< 4; ++i) {
        result.push_back(std::make_pair(rotationmatrix[piecetype*4+rotation%4][i][0]+x,rotationmatrix[piecetype*4+rotation%4][i][1]+y));
    }
    return result;
}
std::vector<std::pair<nes_uchar, nes_uchar> > Piece::nextpiecePos() const {
    std::vector<std::pair<nes_uchar,nes_uchar> > result;
    if (piecetype<0 || piecetype>6) return result;
    for (std::vector<std::pair<nes_uchar,nes_uchar> >::size_type i=0; i< 4; ++i) {
        result.push_back(std::make_pair(rotationmatrix[piecetype*4+rotation%4][i][0]*8+nextpiecespawn[piecetype][0],rotationmatrix[piecetype*4+rotation%4][i][1]*8+nextpiecespawn[piecetype][1]));
    }
    return result;
}

nes_schar Piece::rotationmatrix[28][4][2]={
   { { -1,  0 }, {  0,  0 }, {  1,  0 }, {  0,  1 }, },  // 02: T down (spawn)
   { {  0, -1 }, { -1,  0 }, {  0,  0 }, {  0,  1 }, },  // 03: T left
    { { -1,  0 }, {  0,  0 }, {  1,  0 }, {  0, -1 }, },  // 00: T up
   { {  0, -1 }, {  0,  0 }, {  1,  0 }, {  0,  1 }, },  // 01: T right

   { { -1,  0 }, {  0,  0 }, {  1,  0 }, {  1,  1 }, },  // 07: J down (spawn)
   { {  0, -1 }, {  0,  0 }, { -1,  1 }, {  0,  1 }, },  // 04: J left
   { { -1, -1 }, { -1,  0 }, {  0,  0 }, {  1,  0 }, },  // 05: J up
   { {  0, -1 }, {  1, -1 }, {  0,  0 }, {  0,  1 }, },  // 06: J right

   { { -1,  0 }, {  0,  0 }, {  0,  1 }, {  1,  1 }, },  // 08: Z horizontal (spawn)
   { {  1, -1 }, {  0,  0 }, {  1,  0 }, {  0,  1 }, },  // 09: Z vertical
   { { -1,  0 }, {  0,  0 }, {  0,  1 }, {  1,  1 }, },  // 08: Z horizontal (spawn)
   { {  1, -1 }, {  0,  0 }, {  1,  0 }, {  0,  1 }, },  // 09: Z vertical

   { { -1,  0 }, {  0,  0 }, { -1,  1 }, {  0,  1 }, },  // 0A: O (spawn)
   { { -1,  0 }, {  0,  0 }, { -1,  1 }, {  0,  1 }, },  // 0A: O (spawn)
   { { -1,  0 }, {  0,  0 }, { -1,  1 }, {  0,  1 }, },  // 0A: O (spawn)
   { { -1,  0 }, {  0,  0 }, { -1,  1 }, {  0,  1 }, },  // 0A: O (spawn)

   { {  0,  0 }, {  1,  0 }, { -1,  1 }, {  0,  1 }, },  // 0B: S horizontal (spawn)
   { {  0, -1 }, {  0,  0 }, {  1,  0 }, {  1,  1 }, },  // 0C: S vertical
   { {  0,  0 }, {  1,  0 }, { -1,  1 }, {  0,  1 }, },  // 0B: S horizontal (spawn)
   { {  0, -1 }, {  0,  0 }, {  1,  0 }, {  1,  1 }, },  // 0C: S vertical

   { { -1,  0 }, {  0,  0 }, {  1,  0 }, { -1,  1 }, },  // 0E: L down (spawn)
   { { -1, -1 }, {  0, -1 }, {  0,  0 }, {  0,  1 }, },  // 0F: L left
   { {  1, -1 }, { -1,  0 }, {  0,  0 }, {  1,  0 }, },  // 10: L up
   { {  0, -1 }, {  0,  0 }, {  0,  1 }, {  1,  1 }, },  // 0D: L right

   { { -2,  0 }, { -1,  0 }, {  0,  0 }, {  1,  0 }, },  // 12: I horizontal (spawn)
   { {  0, -2 }, {  0, -1 }, {  0,  0 }, {  0,  1 }, },  // 11: I vertical
   { { -2,  0 }, { -1,  0 }, {  0,  0 }, {  1,  0 }, },  // 12: I horizontal (spawn)
   { {  0, -2 }, {  0, -1 }, {  0,  0 }, {  0,  1 }, },  // 11: I vertical
    };
nes_uchar Piece::nextpiecespawn[7][2]={
    {203,111},
    {203,111},
    {203,111},
    {207,111},
    {203,111},
    {203,111},
    {207,115},
};
