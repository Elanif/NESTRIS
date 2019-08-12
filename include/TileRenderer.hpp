#ifndef TILERENDERER_H
#define TILERENDERER_H
#include"TileContainer.hpp"
#include<SFML/Graphics.hpp>
#include"ntris.hpp"
#include<vector>
#include<string>
#include<fstream>
#include<unordered_map>
#include"Sprite.hpp"
#include<utility>

class TileRenderer : public sf::Transformable, public sf::NonCopyable
{

    //bool load(const std::string& tileset, sf::Vector2u tileSize, const TileContainer& tiles, const nes_uchar& width, const nes_uchar& height)
public:
    TileRenderer(const std::size_t& _width, const std::size_t& _height, std::pair<largest_uint, largest_uint>, const int& _drawmethod, const sf::Vector3<std::size_t>& _extra_render=sf::Vector3<std::size_t>());
    ~TileRenderer();
    bool load(const std::string& tilefile);

    TileContainer* getTileContainer();

    const std::size_t width;
    const std::size_t width_pixels;
    const std::size_t height;
    const std::size_t height_pixels;

    void drawmod(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());
    void drawimage(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());
    void drawsprite(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());
    void drawvertex(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());
    void drawtexture(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());
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
    const sf::Vector3<std::size_t>& extra_render=sf::Vector3<std::size_t>();
	std::pair<std::size_t, std::size_t> tilesize{ 8,8 };
    uint8container* quadretti;
    std::unordered_map<tiletype, std::size_t> texturemap;
    sf::Texture tiletexture;
	std::size_t texturesize{ 0 };
	std::size_t texturenumber{ 0 };
    void add_frequent_textures();
    std::size_t add_or_find_texture(const tiletype& newtile, sf::Image* prerendering) ;
    void load_palette(const std::string& path);
    std::ofstream newtextures;
    void renderExtraTiles(std::size_t offset, const decltype(tilecont.extra_tiles.x)& extra_tiles, const std::size_t max_extra) ;
};

#endif // TILERENDERER_H
