#ifndef TILECONTAINER_H
#define TILECONTAINER_H
#include"enums.h"
#include"stddef.h"
#include<SFML/System/NonCopyable.hpp>
#include<functional>
#include<map>
#include<SFML/System/Vector3.hpp>

class tiletype {
    public:
    std::size_t tilenumber;
    nes_uchar palette_color[4];
    tiletype()
        :tilenumber(0)
    {
        palette_color[0]=palette_color[1]=palette_color[2]= palette_color[3]=0x0D;
    }
    tiletype(const nes_uchar& _level, const nes_uchar& _blocktype)
    :tilenumber(87)
    {
        switch(_blocktype) {
        case 1:
            tilenumber=678;
            palette_color[2]=colors[_level][2];
            palette_color[3]=colors[_level][3];
            break;
        case 2:
            tilenumber=679;
            palette_color[2]=colors[_level][3];
            palette_color[3]=colors[_level][2];
            break;
        case 3:
            tilenumber=679;
            palette_color[2]=colors[_level][2];
            palette_color[3]=colors[_level][3];
            break;
        default:
            tilenumber=0;
            break;
        }
        if (_blocktype==0) {
            palette_color[0]=palette_color[1]=palette_color[2]= palette_color[3]=0x0D;
        }
        else {
            palette_color[0]=colors[_level][0];
            palette_color[1]=colors[_level][1];
        }
    }
    tiletype(const std::size_t& _tilenumber, const nes_uchar& c1, const nes_uchar& c2, const nes_uchar& c3, const nes_uchar& c4)
        :tilenumber(_tilenumber),
        palette_color{c1,c2,c3,c4}
    {}
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
            std::size_t colors=64-8;
            //TODO remove trailing colors
            return std::hash<sf::Int64>()(t.palette_color[0]+colors*(t.palette_color[1]+colors*(t.palette_color[2]+colors*(t.palette_color[3]+colors*(t.tilenumber)))));
        }
    };
}

class TileContainer : sf::NonCopyable
{
    private:
        const sf::Vector3<std::size_t> extra_render=sf::Vector3<std::size_t>();
    public:
        //TODO make private
        void reset();
        sf::Vector3<std::multimap<double, glb::triple> > extra_tiles={{},{},{}};
        const tiletype& at(const std::size_t& x, const std::size_t& y) const;
        tiletype& at(const std::size_t& x, const std::size_t& y);
        const tiletype& atconst(const std::size_t& x, const std::size_t& y) const {return at(x,y);}
        void renderExtra(const std::size_t pixelx, const std::size_t pixely, const tiletype& t, const double& priority) ;
        void resetupdated() ;
        const bool& updated(const std::size_t& x, const std::size_t& y);
        TileContainer(const std::size_t& _width, const std::size_t& _height, const sf::Vector3<std::size_t>& _extra_render=sf::Vector3<std::size_t>());
        TileContainer();
        ~TileContainer();
        tiletype* tilegrid;
        std::size_t width, height;
        tiletype oob_error;
        bool* _upd;
};

#endif // TILECONTAINER_H
