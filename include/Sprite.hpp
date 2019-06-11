#ifndef SPRITE_H
#define SPRITE_H
#include"enums.hpp"
#include<SFML/System/Utf.hpp>
#include<SFML/Graphics.hpp>
#include<climits>
#include"ConsoleManager.hpp"

class Sprite {
public:
    Sprite();
    void print();
    nes_uchar arr[8][8];
};

class uint8container {
public:
    sf::Uint8* getQuadretto() {return quadretto;}
private:
	sf::Uint8 quadretto[4 * 8 * 8]{ 0 };
};

struct SpriteEqual {
    std::size_t lookup(unsigned char color, unsigned char* palette_color, std::size_t colors_found) const{
        for (std::size_t i=0; i<colors_found; ++i)
            if (palette_color[i]==color) return i;
        return (std::size_t)-1;
    }
    bool operator()(const Sprite& img1, const Sprite& img2) const{
        std::size_t colors_found=0;
        unsigned char palette_color1[glb::maxcolor];//={0,0,0,0};
        unsigned char palette_color2[glb::maxcolor];//={0,0,0,0};
        for (std::size_t x=0; x<glb::tilesize.x; ++x) {
            for (std::size_t y=0; y<glb::tilesize.y; ++y) {
                std::size_t lookup1=lookup(img1.arr[x][y],palette_color1,colors_found);
                std::size_t lookup2=lookup(img2.arr[x][y],palette_color2,colors_found);
                if (lookup1!=lookup2) return false;
                if (lookup1==(std::size_t)-1) {
                    if (colors_found>=glb::maxcolor) throw "Too many colors in sprite equal"; //ERROR
                    palette_color1[colors_found]=img1.arr[x][y];
                    palette_color2[colors_found]=img2.arr[x][y];
                    ++colors_found;
                }
            }
        }
        return true;
    }
};

template <typename INT>
constexpr INT rol(INT val, unsigned int moves) {
    static_assert(std::is_unsigned<INT>::value,
                  "Rotate Left only makes sense for unsigned types");
    return (val << (moves%(sizeof(INT)*CHAR_BIT))) | (val >> (sizeof(INT)*CHAR_BIT-(moves%(sizeof(INT)*CHAR_BIT))));
}

namespace std
{
    template<>
    struct hash<sf::Image> {
        std::size_t operator()(const sf::Image& t) const noexcept
        {
            sf::Vector2u imagesize=t.getSize();
            unsigned long long temphash=0;
            for (std::size_t x=0; x<imagesize.x; ++x)
                for (std::size_t y=0; y<imagesize.y; ++y)
                    temphash+=t.getPixel(x,y).toInteger();
            return std::hash<unsigned long long>()(temphash);
        }
    };

    template<>
    struct hash<Sprite> {
        std::size_t operator()(const Sprite& t) const noexcept
        {
            struct funcholder {
                static std::size_t lookup(unsigned char color, unsigned char* palette_color, std::size_t colors_found) {
                    for (std::size_t i=0; i<colors_found; ++i)
                        if (palette_color[i]==color) return i;
                    return (std::size_t)-1;
                }
            };
            std::size_t colors_found=0;
            unsigned char palette_color1[glb::maxcolor];
            unsigned long long temphash=0;
            unsigned long long rotations=0;
            for (std::size_t x=0; x<glb::tilesize.x; ++x) {
                for (std::size_t y=0; y<glb::tilesize.y; ++y) {
                    std::size_t lookup1=funcholder::lookup(t.arr[x][y],palette_color1,colors_found);
                    if (lookup1==(std::size_t)-1) {
						ConsoleManager::update_error("Too many colors in sprite hash");
                        palette_color1[colors_found]=t.arr[x][y];
                        ++colors_found;
                    }
                    temphash+=rol<unsigned long long>(lookup1,rotations++);
                }
            }
            return std::hash<unsigned long long>()(temphash);
        }
    };
}

#endif // SPRITE_H
