#ifndef TILECONTAINER_H
#define TILECONTAINER_H
#include"enums.h"
#include"stddef.h"
#include<SFML/System/NonCopyable.hpp>

class tiletype {
    public:
    size_t tilenumber;
    nes_uchar palette_color[4];
    tiletype()
        :tilenumber(0)
    {
        palette_color[0]=palette_color[1]=palette_color[2]= palette_color[3]=0xF;
    }
    tiletype(const nes_uchar& _level, const nes_uchar& _blocktype) {
        if (_blocktype==0) {
            tilenumber=87;
            palette_color[0]=palette_color[1]=palette_color[2]= palette_color[3]=0xF;
        }
        else {
            tilenumber=733+_blocktype;
            palette_color[0]=colors[_level][0];
            palette_color[1]=colors[_level][1];
            palette_color[2]=colors[_level][2];
            palette_color[3]=colors[_level][3];
        }
    }
    static unsigned char colors[10][4];
};
class TileContainer : sf::NonCopyable //TODO CHANGE WHEN CLONE
{
    private:
    public:
        //TODO COPY ETC
        const tiletype& at(const size_t& x, const size_t& y) const;
        tiletype& at(const size_t& x, const size_t& y);
        const tiletype& atconst(const size_t& x, const size_t& y) const {return at(x,y);}
        void resetupdated() ;
        const bool& updated(const size_t& x, const size_t& y);
        TileContainer(const size_t& _width, const size_t& _height);
        TileContainer();
        ~TileContainer();
        tiletype* tilegrid;
        size_t width, height;
        tiletype oob_error;
        bool* _upd;
};

#endif // TILECONTAINER_H
