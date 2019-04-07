#ifndef TILECONTAINER_H
#define TILECONTAINER_H
#include"enums.h"
#include"stddef.h"
#include<SFML/System/NonCopyable.hpp>
#include<functional>

class tiletype {
    public:
    size_t tilenumber;
    nes_uchar palette_color[4];
    tiletype()
        :tilenumber(0)
    {
        palette_color[0]=palette_color[1]=palette_color[2]= palette_color[3]=0x0D;
    }
    tiletype(const nes_uchar& _level, const nes_uchar& _blocktype)
    :tilenumber(87)
    {
        if (_blocktype==0) {
            palette_color[0]=palette_color[1]=palette_color[2]= palette_color[3]=0x0D;
        }
        else {
            tilenumber=891+_blocktype;
            palette_color[0]=colors[_level][0];
            palette_color[1]=colors[_level][1];
            palette_color[2]=colors[_level][2];
            palette_color[3]=colors[_level][3];
        }
    }

    bool operator==(const tiletype& t2) const {
        return (this->tilenumber==t2.tilenumber)&&(this->palette_color[0]==t2.palette_color[0])&&(this->palette_color[1]==t2.palette_color[1])&&(this->palette_color[2]==t2.palette_color[2])&&(this->palette_color[3]==t2.palette_color[3]);
    }

    static unsigned char colors[10][4];
};

namespace std {
    template<>
    struct hash<tiletype>
    {
        std::size_t operator()(const tiletype& t) const noexcept
        {
            size_t colors=64-8;
            return std::hash<sf::Int64>()(t.palette_color[0]+colors*(t.palette_color[1]+colors*(t.palette_color[2]+colors*(t.palette_color[3]+colors*(t.tilenumber)))));
        }
    };
}

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
