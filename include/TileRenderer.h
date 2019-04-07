#ifndef TILERENDERER_H
#define TILERENDERER_H
#include"TileContainer.h"
#include<SFML/Graphics.hpp>
#include"enums.h"
#include<vector>
#include<string>
#include<cstdio>
#include<unordered_map>

unsigned char TileRenderer::colors[10][4]={
    {0x0D ,0x30 ,0x21 ,0x12},
    {0x0D ,0x30 ,0x29 ,0x1A},
    {0x0D ,0x30 ,0x24 ,0x14},
    {0x0D ,0x30 ,0x2A ,0x12},
    {0x0D ,0x30 ,0x2B ,0x15},
    {0x0D ,0x30 ,0x22 ,0x2B},
    {0x0D ,0x30 ,0x00 ,0x16},
    {0x0D ,0x30 ,0x05 ,0x13},
    {0x0D ,0x30 ,0x16 ,0x12},
    {0x0D ,0x30 ,0x27 ,0x16}
};


sf::Uint32 TileRenderer::palette[4][16]= { //are these the right values?
    {0x7C7C7CFF ,0x0000FCFF ,0x0000BCFF ,0x4428BCFF ,0x940084FF ,0xA80020FF ,0xA81000FF ,0x881400FF ,0x503000FF ,0x007800FF ,0x006800FF ,0x005800FF ,0x004058FF ,0x000000FF ,0x000000FF ,0x000000FF},
    {0xBCBCBCFF ,0x0078F8FF ,0x0058F8FF ,0x6844FCFF ,0xD800CCFF ,0xE40058FF ,0xF83800FF ,0xE45C10FF ,0xAC7C00FF ,0x00B800FF ,0x00A800FF ,0x00A844FF ,0x008888FF ,0x000000ff ,0x000000FF ,0x000000FF},
    {0xF8F8F8FF ,0x3CBCFCFF ,0x6888FCFF ,0x9878F8FF ,0xF878F8FF ,0xF85898FF ,0xF87858FF ,0xFCA044FF ,0xF8B800FF ,0xB8F818FF ,0x58D854FF ,0x58F898FF ,0x00E8D8FF ,0x787878FF ,0x000000FF ,0x000000FF},
    {0xFCFCFCFF ,0xA4E4FCFF ,0xB8B8F8FF ,0xD8B8F8FF ,0xF8B8F8FF ,0xF8A4C0FF ,0xF0D0B0FF ,0xFCE0A8FF ,0xF8D878FF ,0xD8F878FF ,0xB8F8B8FF ,0xB8F8D8FF ,0x00FCFCFF ,0xF8D8F8FF ,0x000000FF ,0x000000FF}
};

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
    TileRenderer(const size_t& _width, const size_t& _height, sf::Vector2u tilesize, const int& _drawmethod);
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
    sf::Vector2u tilesize;
    uint8container* quadretti;
    std::unordered_map<tiletype, size_t> texturemap;
    sf::Texture tiletexture;
    size_t texturesize;
    size_t texturenumber;
    void add_frequent_textures();
    size_t add_or_find_texture(const tiletype& newtile, const bool& prerendering=false) ;
    FILE*newtextures;

};

#endif // TILERENDERER_H
