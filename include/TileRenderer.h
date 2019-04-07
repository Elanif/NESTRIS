#ifndef TILERENDERER_H
#define TILERENDERER_H
#include"TileContainer.h"
#include<SFML/Graphics.hpp>
#include"enums.h"
#include<vector>
#include<string>
#include<cstdio>
#include<unordered_map>

class TileRenderer : public sf::Transformable, public sf::NonCopyable
{
    class sprite {
    public:
        sprite() {
            for (size_t i=0; i<8; ++i)
                for (size_t j=0; j<8; ++j)
                    arr[i][j]=0;
        }
        void print() {
            for (size_t i=0; i<8; ++i) {
                for (size_t j=0; j<8; ++j)
                    printf("%d ",arr[j][i]);
                printf("\n");
            }
        }
        nes_uchar arr[8][8];
    };

    class uint8container {
    public:
        uint8container() {
            for (size_t i=0; i<4*8*8;++i)
                quadretto[i]=0;
        }
        sf::Uint8* getQuadretto() {return quadretto;}
    private:
        sf::Uint8 quadretto[4*8*8];
    };

    //bool load(const std::string& tileset, sf::Vector2u tileSize, const TileContainer& tiles, const nes_uchar& width, const nes_uchar& height)
public:
    TileRenderer(const size_t& _width, const size_t& _height, const int& _drawmethod);
    ~TileRenderer();
    bool load(const std::string& tilefile);

    TileContainer* getTileContainer();

    const size_t width;
    const size_t height;
    void drawmod(sf::RenderTarget& target, sf::RenderStates states);
    void drawimage(sf::RenderTarget& target, sf::RenderStates states);
    void drawsprite(sf::RenderTarget& target, sf::RenderStates states);
    void drawvertex(sf::RenderTarget& target, sf::RenderStates states);
    void drawtexture(sf::RenderTarget& target, sf::RenderStates states);
    enum DRAWMETHOD{
        DRAWIMAGE,
        DRAWSPRITE,
        DRAWVERTEX,
        DRAWTEXTURE,
    };

private:

    int drawmethod;
    std::vector<sprite> spritevector;
    static sf::Uint32 palette[4][16];
    static unsigned char colors[10][4];
    TileContainer tilecont;
    sf::Image finalimageclass;
    sf::Texture temptexclass;
    sf::Sprite tempspriteclass;
    sf::VertexArray verteximage;
    uint8container* quadretti;
    std::unordered_map<tiletype, size_t> texturemap;
    sf::Texture tiletexture;
    size_t texturesize;
    size_t texturenumber;

};

#endif // TILERENDERER_H
