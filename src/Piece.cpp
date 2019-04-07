#include "Piece.h"

Piece::Piece()
{
    x=5;
    y=2;
    piecetype=-1;
    rotation=0;
}

Piece::Piece(const char& charpiecetype)
{
    x=5;
    y=2;
    rotation=0;
    switch(charpiecetype) {
    case 'T':
    case 't':
        piecetype=0;
        break;
    case 'J':
    case 'j':
        piecetype=1;
        break;
    case 'Z':
    case 'z':
        piecetype=2;
        break;
    case 'O':
    case 'o':
        piecetype=3;
        break;
    case 'S':
    case 's':
        piecetype=4;
        break;
    case 'L':
    case 'l':
        piecetype=5;
        break;
    case 'I':
    case 'i':
        piecetype=6;
        break;
    default:
        piecetype=-1;
        break;
    }
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

typedef std::tuple<nes_schar, nes_schar, std::size_t> triple;
std::vector<triple > Piece::nextpiecePos() const { //OUTDATED
    std::vector<triple >  result;
    if (piecetype<0 || piecetype>6) return result;
    switch(piecetype) {
    case 0:
        result.push_back(triple(27,14,743));
        result.push_back(triple(26,15,743));

        result.push_back(triple(25,14,742));
        result.push_back(triple(26,14,742));

        result.push_back(triple(24,14,741));
        result.push_back(triple(25,15,741));
        break;
    case 1:
        result.push_back(triple(27,14,746));
        result.push_back(triple(27,15,746));

        result.push_back(triple(25,14,745));
        result.push_back(triple(26,14,745));

        result.push_back(triple(24,14,744));
        result.push_back(triple(26,15,744));
        break;
    case 2:
        result.push_back(triple(26,14,746));
        result.push_back(triple(27,15,746));

        result.push_back(triple(25,14,745));
        result.push_back(triple(26,15,745));

        result.push_back(triple(24,14,744));
        result.push_back(triple(25,15,744));
        break;
    case 3:
        result.push_back(triple(25,14,678));
        result.push_back(triple(26,14,678));
        result.push_back(triple(25,15,678));
        result.push_back(triple(26,15,678));
        break;
    case 4:
        result.push_back(triple(27,14,746));
        result.push_back(triple(26,15,746));

        result.push_back(triple(26,14,745));
        result.push_back(triple(25,15,745));

        result.push_back(triple(25,14,744));
        result.push_back(triple(24,15,744));
        break;
    case 5:
        result.push_back(triple(27,14,746));
        result.push_back(triple(25,15,746));

        result.push_back(triple(25,14,745));
        result.push_back(triple(26,14,745));

        result.push_back(triple(24,14,744));
        result.push_back(triple(24,15,744));
        break;
    case 6:
        result.push_back(triple(24,15,748));
        result.push_back(triple(25,15,748));
        result.push_back(triple(26,15,748));
        result.push_back(triple(27,15,748));

        result.push_back(triple(24,14,747));
        result.push_back(triple(25,14,747));
        result.push_back(triple(26,14,747));
        result.push_back(triple(27,14,747));
        break;


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
