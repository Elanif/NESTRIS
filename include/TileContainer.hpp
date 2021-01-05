#ifndef TILECONTAINER_H
#define TILECONTAINER_H
#include"ntris.hpp"
#include<cstddef>
#include<SFML/System/NonCopyable.hpp>
#include<functional>
#include<map>
#include<SFML/System/Vector3.hpp>

class tiletype {
    public:
    std::size_t tilenumber;
    nes_uchar palette_color[4]; // a color is 0xAB where A=0 to 3 and B=0 to F: https://meatfighter.com/nintendotetrisai/9.png
	// transparent
    tiletype()
        :tilenumber(0)
    {
        palette_color[0]=palette_color[1]=palette_color[2]= palette_color[3]=0x0D;
    }
	// constructor for tetrimino blocks
    tiletype(const nes_uchar& _level, const nes_uchar& _blocktype)
    :tilenumber(87)
    {
        const nes_uchar modlevel=_level%10; //TODO after level 138 in the original game it will access out of bounds memory, resulting in crazy colors
        switch(_blocktype) {
        case 1:
            tilenumber=678;
            palette_color[2]=colors[modlevel][2];
            palette_color[3]=colors[modlevel][3];
            break;
        case 2:
            tilenumber=679;
            palette_color[2]=colors[modlevel][3];
            palette_color[3]=colors[modlevel][2];
            break;
        case 3:
            tilenumber=679;
            palette_color[2]=colors[modlevel][2];
            palette_color[3]=colors[modlevel][3];
            break;
        default:
            tilenumber=0;
            break;
        }
        if (_blocktype==0) {
            palette_color[0]=palette_color[1]=palette_color[2]= palette_color[3]=0x0D;
        }
        else {
            palette_color[0]=colors[modlevel][0];
            palette_color[1]=colors[modlevel][1];
        }
    }
    tiletype(const std::size_t& _tilenumber, const nes_uchar& c1, const nes_uchar& c2, const nes_uchar& c3, const nes_uchar& c4)
        :tilenumber(_tilenumber),
        palette_color{c1,c2,c3,c4}
    {}
    bool operator==(const tiletype& t2) const { //equal operator
        return (this->tilenumber==t2.tilenumber)&&(this->palette_color[0]==t2.palette_color[0])&&(this->palette_color[1]==t2.palette_color[1])&&(this->palette_color[2]==t2.palette_color[2])&&(this->palette_color[3]==t2.palette_color[3]);
    }

    static unsigned char colors[10][4]; //the colors for levels 0 to 9
};

//hash for tiles, used in the unordered map for lookup of textures
namespace std {
    template<>
    struct hash<tiletype>
    {
        std::size_t operator()(const tiletype& t) const noexcept
        {
            std::size_t colors=64;
            //TODO remove trailing colors
            return std::hash<sf::Int64>()(t.palette_color[0]+colors*(t.palette_color[1]+colors*(t.palette_color[2]+colors*(t.palette_color[3]+colors*(t.tilenumber)))));
        }
    };
}

//the container of the array of tiles that's displayed on screen
class TileContainer : sf::NonCopyable
{
    private:
		//sizes of the 3 extra render vectors (x y and z)
        sf::Vector3<std::size_t> extra_render=sf::Vector3<std::size_t>();
		//very ugly dynamic array of tiles
		tiletype* tilegrid;
		//tilecontainer width and height
		std::size_t width, height;
		//tiletype functions return when it's out of bounds
		tiletype oob_error;
		//very ugly dynamic array of the tiles that were updated
		bool* _upd;
    public:
        //TODO make private some of these
		//does what you expect
        void reset();

		std::size_t getWidth() const { return width; };
		std::size_t getHeight() const { return height; };

		//vector of tiles that can be displayed on top of the original game tiles
        sf::Vector3<std::multimap<double, ntris::triple> > extra_tiles={{},{},{}};

		//returns a const reference to the tile at (x,y)
        const tiletype& at(const std::size_t& x, const std::size_t& y) const;

		//returns a  reference to the tile at (x,y)
        tiletype& at(const std::size_t& x, const std::size_t& y);

		//returns a const reference to the tile at (x,y)
        const tiletype& atconst(const std::size_t& x, const std::size_t& y) const {return at(x,y);}

		//renders an extra tile t at (pixelx,pixely), priority<0.5 -> it goes into extra_tiles.y, priority>=0.5 it goes into extratiles.z
		//higher priority tiles will be on top
		//extra_tiles.y is rendered before the tile array, and extra_tiles.z is rendered on top of the array, so if priority<0.5 the tile will almost always be invisible
        void renderExtra(const std::size_t pixelx, const std::size_t pixely, const tiletype& t, const double& priority) ;

		//returns if the tile at (x,y) was changed in the last frame, only tiles that were changed are drawn again
        const bool& updated(const std::size_t& x, const std::size_t& y);

		//sets everything to false
		void resetupdated();
		//constructor with width and height in tiles, and a 3d vector that contains the sizes of the extra_tiles
        TileContainer(const std::size_t& _width, const std::size_t& _height, const sf::Vector3<std::size_t>& _extra_render=sf::Vector3<std::size_t>());

		void create(const std::size_t& _width, const std::size_t& _height, const sf::Vector3<std::size_t>& _extra_render);

        TileContainer();
        ~TileContainer();
};

#endif // TILECONTAINER_H
