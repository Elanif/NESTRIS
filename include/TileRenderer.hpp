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
	/*constructs the array of tiles 
	drawmethod: how the game is drawn texture is the best one
	width and height: self explanatory
	tilesize: width and height of tiles, 8x8 in the original game
	extra_render: sizes of the e (x y and z) containers for tiles to be drawn on top or under the game
	*/
    TileRenderer(const std::size_t& _width, const std::size_t& _height, std::pair<largest_uint, largest_uint> _tilesize, const int& _drawmethod, const sf::Vector3<std::size_t>& _extra_render=sf::Vector3<std::size_t>());
    ~TileRenderer();

	//loads the sprites of the game, found at the bottom of the nes tetris cartridge
    bool load(const std::string& tilefile);

    TileContainer* getTileContainer();

    const std::size_t width;
    const std::size_t width_pixels; //width*tilesize.x
    const std::size_t height;
    const std::size_t height_pixels; //height*tilesize.y

	//draw method, can't be const because of how textures are updated when drawing
    void drawmod(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());

	//draws the whole image, extremely slow
    void drawimage(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());

    void drawsprite(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());
    void drawvertex(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());

	/*  You need to add a tile to the screen:
		if it's the first time using that particular tile, with those particular colors, it's added to the first empty spot in the texture, and it's added to an unordered map
		updating a texture is very slow, so I put the most frequently used tiles already in the texture when the program is opened
		if the tile was already used, it's found in the unordered map, which then links to the position in the texture
	*/
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

	//the max size of the 3 extra texture vectors, y is on bottom and z on top
    const sf::Vector3<std::size_t>& extra_render=sf::Vector3<std::size_t>();

	//the size of a tile, inconsistent with how I've treated this variable in the solution, since it's also in the ntris namespace
	std::pair<std::size_t, std::size_t> tilesize{ 8,8 };

	//a tile
    uint8container* quadretti;

	//returns which number in the texture is the one you're looking for, from left to right, top to bottom
    std::unordered_map<tiletype, std::size_t> texturemap;

	//the texture tiles are placed in
    sf::Texture tiletexture;

	//size of the texture
	std::size_t texturesize{ 0 };

	//number of tiles in the texture
	std::size_t texturenumber{ 0 };

	//adds the textures that you find in game mode A from level 0 to 137
    void add_frequent_textures();

	//searches for the tile in the texture, if it's not found it's added to the texture and the unordered map
	//if prerendering is passed, it instead creates the starting texture from frequent tiles
    std::size_t add_or_find_texture(const tiletype& newtile, sf::Image* prerendering) ;

	//loads the color palette from a path, there is no universal palette because of how the game could look different on different screens
    void load_palette(const std::string& path);

	//output of new textures to a txt file, once in a while, if I see new textures in there, I'll copy them to the prerendering thing
    std::ofstream newtextures;

	//renders extra tiles, useful for tiles that aren't on the normal grid, like the blinking yellow square during level select
    void renderExtraTiles(std::size_t offset, const decltype(tilecont.extra_tiles.x)& extra_tiles, const std::size_t max_extra) ;
};

#endif // TILERENDERER_H
