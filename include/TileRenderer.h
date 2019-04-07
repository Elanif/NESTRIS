#ifndef TILERENDERER_H
#define TILERENDERER_H
#include"TileContainer.h"
#include<SFML/Graphics.hpp>
#include"enums.h"
#include<vector>
#include<string>
#include<cstdio>
#include<unordered_map>
#include"Sprite.h"

class TileRenderer : public sf::Transformable, public sf::NonCopyable
{

    //bool load(const std::string& tileset, sf::Vector2u tileSize, const TileContainer& tiles, const nes_uchar& width, const nes_uchar& height)
public:
    TileRenderer(const std::size_t& _width, const std::size_t& _height, sf::Vector2u _tilesize, const int& _drawmethod, const std::size_t& _extra_render=0);
    ~TileRenderer();
    bool load(const std::string& tilefile);

    TileContainer* getTileContainer();

    const std::size_t width;
    const std::size_t height;
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
    std::vector<Sprite> spritevector;
    std::unordered_map<Sprite,std::size_t,std::hash<Sprite>,SpriteEqual> spritemap;
    static sf::Uint32 palette[4][16];
    static unsigned char colors[10][4];
    TileContainer tilecont;
    sf::Image finalimageclass;
    sf::Texture temptexclass;
    sf::Sprite tempspriteclass;
    sf::VertexArray verteximage;
    std::size_t extra_render;
    sf::Vector2u tilesize;
    uint8container* quadretti;
    std::unordered_map<tiletype, std::size_t> texturemap;
    sf::Texture tiletexture;
    std::size_t texturesize;
    std::size_t texturenumber;
    void add_frequent_textures();
    std::size_t add_or_find_texture(const tiletype& newtile, sf::Image* prerendering) ;
    void load_palette(const std::string& path);
    FILE*newtextures;
    void renderExtraTiles(std::size_t offset, const decltype(tilecont.previous_tiles)& extra_tiles);
};

#endif // TILERENDERER_H
