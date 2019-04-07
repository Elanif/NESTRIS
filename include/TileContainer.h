#ifndef TILECONTAINER_H
#define TILECONTAINER_H
#include"enums.h"
#include"stddef.h"

class tiletype {
    public:
    size_t tilenumber;
    nes_uchar palette_color[4];
    tiletype()
        :tilenumber(0)
    {
        palette_color[0]=palette_color[1]=palette_color[2]= palette_color[3]=0;
    }
};

class TileContainer
{
    private:
    public:
        //TODO COPY ETC
        const tiletype& at(const size_t& x, const size_t& y) const;
        tiletype& at(const size_t& x, const size_t& y);
        TileContainer(const size_t& _width, const size_t& _height);
        TileContainer();
        ~TileContainer();
        tiletype* tilegrid;
        size_t width, height;
        tiletype oob_error;
};

#endif // TILECONTAINER_H
