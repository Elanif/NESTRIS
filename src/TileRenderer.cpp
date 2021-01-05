#include"TileRenderer.hpp"
#include"Log.hpp"
#include"ntris.hpp"
#include<string>
#include<fstream>
#include<sstream>
#include <iostream>
template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1ull; // Extra space for '\0'
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
} //https://stackoverflow.com/a/26221725

constexpr std::size_t itox(const std::size_t & i, const std::size_t & width, const std::size_t & height) {
	return i % width;
}
constexpr std::size_t itoy(const std::size_t & i, const std::size_t & width, const std::size_t & height) {
	return i / width;
}
constexpr std::size_t itoxrect(const std::size_t & i, const std::size_t & width, const std::size_t & height, const std::size_t & rectwidth, const std::size_t & rectheight) {
	return itox(i, width / rectwidth, height / rectheight) * rectwidth;
}
constexpr std::size_t itoyrect(const std::size_t & i, const std::size_t & width, const std::size_t & height, const std::size_t & rectwidth, const std::size_t & rectheight) {
	//constexpr std::size_t rectx=itox(i,width/rectwidth,height/rectheight)*rectwidth;
	return itoy(i, width / rectwidth, height / rectheight) * rectheight;
}

constexpr std::size_t xytoi(const std::size_t & x, const std::size_t & y, const std::size_t & width, const std::size_t & height) {
	return x + y * width;
}
constexpr std::size_t xytoi(const std::size_t & x, const std::size_t & y, const std::size_t & width, const std::size_t & height, const std::size_t & rectwidth, const std::size_t & rectheight) {
	return x + y * width;
}
TileRenderer::TileRenderer() 
	:tilecont(0, 0, {}),
	width(0),
	height(0),
	drawmethod(0),
	quadretti(NULL),
	tilesize(0,0),
	width_pixels(0),
	height_pixels(0),
	texturenumber(0),
	extra_render({})
{

}

void TileRenderer::create(const std::size_t& _width, const std::size_t& _height, std::pair<std::size_t, std::size_t> _tilesize, const int& _drawmethod, const sf::Vector3<std::size_t>& _extra_render) {
	width = _width;
	height = _height;
	extra_render = _extra_render;
	tilecont.create(width, height, extra_render);
	drawmethod = _drawmethod;
	if (quadretti) delete[] quadretti;
	quadretti = NULL;
	tilesize = _tilesize;
	width_pixels = width * tilesize.first;
	height_pixels = height * tilesize.second;
	texturenumber = 0;
	if (drawmethod == DRAWSPRITE) {
		quadretti = new uint8container[width * height];
		if (!temptexclass.create(width * tilesize.first, height * tilesize.first))
			Log::update_error<std::string>(std::string("Failed to create texture for DRAWSPRITE render mode"));
		tempspriteclass.setTexture(temptexclass, true);
	}
	else if (drawmethod == DRAWIMAGE) {
		finalimageclass.create(width * tilesize.first, height * tilesize.second);
		if (!temptexclass.loadFromImage(finalimageclass))
			Log::update_error<std::string>(std::string("Failed to load texture for DRAWIMAGE render mode"));
		tempspriteclass.setTexture(temptexclass, true);
	}
	else if (drawmethod == DRAWVERTEX) {
		verteximage = sf::VertexArray(sf::Quads, width * height * 4 * tilesize.first * tilesize.second);
		for (std::size_t i = 0; i < width * height * 4 * tilesize.first * tilesize.second; i += 4) {
			verteximage[i].position = sf::Vector2f(itox(i / 4, width * tilesize.first, height * tilesize.second), itoy(i / 4, width * tilesize.first, height * tilesize.second));
			verteximage[i + 1].position = sf::Vector2f(itox(i / 4, width * tilesize.first, height * tilesize.second) + 1, itoy(i / 4, width * tilesize.first, height * tilesize.second));
			verteximage[i + 2].position = sf::Vector2f(itox(i / 4, width * tilesize.first, height * tilesize.second) + 1, itoy(i / 4, width * tilesize.first, height * tilesize.second) + 1);
			verteximage[i + 3].position = sf::Vector2f(itox(i / 4, width * tilesize.first, height * tilesize.second), itoy(i / 4, width * tilesize.first, height * tilesize.second) + 1);
			verteximage[i].color = verteximage[i + 1].color = verteximage[i + 2].color = verteximage[i + 3].color = sf::Color::Black;
		}
	}
	else if (drawmethod == DRAWTEXTURE) { //TODO DYNAMICALLY CHOOSE SIZE && check if size<8
		texturesize = sf::Texture::getMaximumSize() < 512 ? sf::Texture::getMaximumSize() : 512;
		if (!tiletexture.create(texturesize, texturesize))
			Log::update_error<std::string>(std::string("Failed to create texture for DRAWTEXTURE render mode"));
		verteximage = sf::VertexArray(sf::Quads, width * height * 4 + (extra_render.x + extra_render.y + extra_render.z) * 4);
		const std::size_t headline = 4 * (extra_render.x + extra_render.y);
		for (std::size_t i = 0; i < width; ++i) {
			for (std::size_t j = 0; j < height; ++j) {

				sf::Vertex* quad = &verteximage[(i + j * width) * 4 + headline];

				quad[0].position = sf::Vector2f(i * tilesize.first, j * tilesize.second);
				quad[1].position = sf::Vector2f((i + 1) * tilesize.first, j * tilesize.second);
				quad[2].position = sf::Vector2f((i + 1) * tilesize.first, (j + 1) * tilesize.second);
				quad[3].position = sf::Vector2f(i * tilesize.first, (j + 1) * tilesize.second);
			}
		}
	}
}


TileRenderer::TileRenderer(const std::size_t & _width, const std::size_t & _height, std::pair<std::size_t, std::size_t> _tilesize, const int& _drawmethod, const sf::Vector3<std::size_t> & _extra_render)
	:tilecont(_width, _height, _extra_render),
	width(_width),
	height(_height),
	drawmethod(_drawmethod),
	quadretti(NULL),
	tilesize(_tilesize),
	width_pixels(_width * _tilesize.first),
	height_pixels(_height * _tilesize.second),
	texturenumber(0),
	extra_render(_extra_render) //TODO EXTRA RENDER FOR MORE STUFF THAN DRAWTEXTURE
{
	if (drawmethod == DRAWSPRITE) {
		quadretti = new uint8container[width * height];
		if (!temptexclass.create(width * tilesize.first, height * tilesize.first))
			Log::update_error<std::string>(std::string("Failed to create texture for DRAWSPRITE render mode"));
		tempspriteclass.setTexture(temptexclass, true);
	}
	else if (drawmethod == DRAWIMAGE) {
		finalimageclass.create(width * tilesize.first, height * tilesize.second);
		if (!temptexclass.loadFromImage(finalimageclass))
			Log::update_error<std::string>(std::string("Failed to load texture for DRAWIMAGE render mode"));
		tempspriteclass.setTexture(temptexclass, true);
	}
	else if (drawmethod == DRAWVERTEX) {
		verteximage = sf::VertexArray(sf::Quads, width * height * 4 * tilesize.first * tilesize.second);
		for (std::size_t i = 0; i < width * height * 4 * tilesize.first * tilesize.second; i += 4) {
			verteximage[i].position = sf::Vector2f(itox(i / 4, width * tilesize.first, height * tilesize.second), itoy(i / 4, width * tilesize.first, height * tilesize.second));
			verteximage[i + 1].position = sf::Vector2f(itox(i / 4, width * tilesize.first, height * tilesize.second) + 1, itoy(i / 4, width * tilesize.first, height * tilesize.second));
			verteximage[i + 2].position = sf::Vector2f(itox(i / 4, width * tilesize.first, height * tilesize.second) + 1, itoy(i / 4, width * tilesize.first, height * tilesize.second) + 1);
			verteximage[i + 3].position = sf::Vector2f(itox(i / 4, width * tilesize.first, height * tilesize.second), itoy(i / 4, width * tilesize.first, height * tilesize.second) + 1);
			verteximage[i].color = verteximage[i + 1].color = verteximage[i + 2].color = verteximage[i + 3].color = sf::Color::Black;
		}
	}
	else if (drawmethod == DRAWTEXTURE) { //TODO DYNAMICALLY CHOOSE SIZE && check if size<8
		texturesize = sf::Texture::getMaximumSize() < 512 ? sf::Texture::getMaximumSize() : 512;
		if (!tiletexture.create(texturesize, texturesize))
			Log::update_error<std::string>(std::string("Failed to create texture for DRAWTEXTURE render mode"));
		verteximage = sf::VertexArray(sf::Quads, width * height * 4 + (extra_render.x + extra_render.y + extra_render.z) * 4);
		const std::size_t headline = 4 * (extra_render.x + extra_render.y);
		for (std::size_t i = 0; i < width; ++i) {
			for (std::size_t j = 0; j < height; ++j) {

				sf::Vertex* quad = &verteximage[(i + j * width) * 4 + headline];

				quad[0].position = sf::Vector2f(i * tilesize.first, j * tilesize.second);
				quad[1].position = sf::Vector2f((i + 1) * tilesize.first, j * tilesize.second);
				quad[2].position = sf::Vector2f((i + 1) * tilesize.first, (j + 1) * tilesize.second);
				quad[3].position = sf::Vector2f(i * tilesize.first, (j + 1) * tilesize.second);
			}
		}
	}
}

TileContainer * TileRenderer::getTileContainer() {
	return &tilecont;
}

void TileRenderer::load_palette(const std::string & path) {
	sf::Uint32 palette_temp[4][16];
	std::ifstream hex_pal(path.c_str(), std::ios::in | std::ios::binary);
	std::size_t counter = 0;
	sf::Uint8 r;
	sf::Uint8 g;
	sf::Uint8 b; //can't use << operators for binary input because it interprets some chars as whitespaces
	while (hex_pal.read(reinterpret_cast<char*>(&r), 1) && hex_pal.read(reinterpret_cast<char*>(&g), 1) && hex_pal.read(reinterpret_cast<char*>(&b), 1)) {
		sf::Uint32 final_color = 0;
		final_color += r << 24;
		final_color += g << 16;
		final_color += b << 8;
		final_color += 0xff;
		palette_temp[counter / 16][counter % 16] = final_color;
		++counter;
		if (counter >= 16 * 4) break;
	}
	//TODO CARE FOR TRANSPARENCY
	palette_temp[1][0xd] = (palette_temp[1][0xd] >> 8) << 8;
	if (counter >= 64) {
		for (std::size_t i = 0; i < 4; ++i) {
			for (std::size_t j = 0; j < 16; ++j) {
				palette[i][j] = palette_temp[i][j];
			}
		}
	}
	else {
		Log::update_error(path + " palette couldn't be loaded, using default palette" + std::to_string(counter));
	}
}

bool TileRenderer::load(const std::string & tilefile) {
	load_palette("palette/YPbPr.pal");
	std::ifstream spritefile(tilefile.c_str(), std::ios::in);
	if (!spritefile) {
		Log::update_error("Couldn't open sprite file: " + tilefile);
		return false;
	}
	std::size_t spritenumber = spritevector.size();
	while (!spritefile.eof()) {
		std::size_t characters = 0;
		Sprite newsprite;
		for (characters = 0; characters < tilesize.second * 2 && !spritefile.eof(); ++characters) {
			unsigned int hex;
			spritefile >> std::hex >> hex;
			if (characters < tilesize.second) {
				for (std::size_t i = 0; i < tilesize.first; ++i)
					newsprite.arr[tilesize.first - i - 1][characters] = (hex >> i) & 1U;
			}
			else {
				for (std::size_t i = 0; i < tilesize.first; ++i)
					newsprite.arr[tilesize.first - i - 1][characters - tilesize.second] += ((hex >> i) & 1U) << 1;
			}
		}
		if (characters >= tilesize.second * 2) {
			if (ntris::spritemode == 1) {
				if (spritemap.find(newsprite) == spritemap.end()) {
					spritemap[newsprite] = spritenumber;
					spritevector.push_back(newsprite);
					spritenumber++;
				}
			}
			else if (ntris::spritemode == 0) {
				if (spritemap.find(newsprite) == spritemap.end()) {
					spritemap[newsprite] = spritenumber;
				}
				spritevector.push_back(newsprite);
				spritenumber++;
			}
		}
	}
	if (drawmethod == DRAWTEXTURE) add_frequent_textures(); //puts blocks, characters, borders, stuff that you'd see in a normal gameplay
	return true;
}

std::size_t TileRenderer::add_or_find_texture(const tiletype & newtile, sf::Image * prerendering = nullptr) {
	std::size_t whereisthistexture = 0;
	tiletype temptile = newtile;
	//TODO black control
	/*if ((temptile.palette_color[0]&0x0F)>0x0D) temptile.palette_color[0]=temptile.palette_color[0]&0xf0+0x0D;
	if ((temptile.palette_color[1]&0x0F)>0x0D) temptile.palette_color[1]=temptile.palette_color[1]&0xf0+0x0D;
	if ((temptile.palette_color[2]&0x0F)>0x0D) temptile.palette_color[2]=temptile.palette_color[2]&0xf0+0x0D;
	if ((temptile.palette_color[3]&0x0F)>0x0D) temptile.palette_color[3]=temptile.palette_color[3]&0xf0+0x0D;*/

	if (texturemap.find(temptile) == texturemap.end()) { //new texture

		if (itoyrect(texturenumber, texturesize, texturesize, tilesize.first, tilesize.first) > texturesize) {
			Log::update_error("Too many textures");
			throw texturenumber; //TODOBETTER
		}

		Log::update("system", std::string("Creating new texture"));

		if (!prerendering) {
			std::string new_texture_string;
			new_texture_string = string_format("%d %x %x %x %x", temptile.tilenumber, temptile.palette_color[0], temptile.palette_color[1], temptile.palette_color[2], temptile.palette_color[3]);
			newtextures << new_texture_string + ntris::newline;
			if (temptile.tilenumber == 87) {//&& temptile.palette_color[0]==0xd && temptile.palette_color[1]==0x0 && temptile.palette_color[2]==0x0 && temptile.palette_color[3]== 0x0) {

			}
		}

		uint8container newtexture;
		const int primacifra[4] = {
			temptile.palette_color[0] / 16,
			temptile.palette_color[1] / 16,
			temptile.palette_color[2] / 16,
			temptile.palette_color[3] / 16
		};
		const int secondacifra[4] = {
			temptile.palette_color[0] % 16,
			temptile.palette_color[1] % 16,
			temptile.palette_color[2] % 16,
			temptile.palette_color[3] % 16
		};
		const unsigned int color[4] = {
			palette[primacifra[0]][secondacifra[0]],
			palette[primacifra[1]][secondacifra[1]],
			palette[primacifra[2]][secondacifra[2]],
			palette[primacifra[3]][secondacifra[3]]
		};
		sf::Uint8 rgba[4][4];
		for (std::size_t colorcounter = 0; colorcounter < 4; ++colorcounter) {
			int colortemp = color[colorcounter];
			for (std::size_t rgbacounter = 0; rgbacounter < 4; ++rgbacounter) {
				rgba[colorcounter][4 - rgbacounter - 1] = colortemp & 0xff;
				colortemp = colortemp >> 8;
			}
		}
		Sprite* spritetemp = &spritevector[temptile.tilenumber];
		if (!prerendering) {
			sf::Uint8* tempquadretto = newtexture.getQuadretto(); //quadretto means small square in Italian
			for (std::size_t pixelx = 0; pixelx < tilesize.first; ++pixelx) {
				for (std::size_t pixely = 0; pixely < tilesize.second; ++pixely) {
					std::size_t tempi = xytoi(pixelx, pixely, tilesize.first, tilesize.second) * 4;
					nes_uchar tiletypetemp = spritetemp->arr[pixelx][pixely];
					tempquadretto[tempi] = rgba[tiletypetemp][0];
					tempquadretto[tempi + 1] = rgba[tiletypetemp][1];
					tempquadretto[tempi + 2] = rgba[tiletypetemp][2];
					tempquadretto[tempi + 3] = rgba[tiletypetemp][3];
				}
			}
			tiletexture.update(tempquadretto,
				tilesize.first,
				tilesize.second,
				itoxrect(texturenumber, texturesize, texturesize, tilesize.first, tilesize.second),
				itoyrect(texturenumber, texturesize, texturesize, tilesize.first, tilesize.second));
		}
		else { //adds everything to an image then lets the add_frequent_texture function save it as a texture
			for (std::size_t pixelx = 0; pixelx < tilesize.first; ++pixelx) {
				for (std::size_t pixely = 0; pixely < tilesize.second; ++pixely) {
					nes_uchar tiletypetemp = spritetemp->arr[pixelx][pixely];
					prerendering->setPixel(pixelx + itoxrect(texturenumber, texturesize, texturesize, tilesize.first, tilesize.second),
						pixely + itoyrect(texturenumber, texturesize, texturesize, tilesize.first, tilesize.second),
						sf::Color(rgba[tiletypetemp][0],
							rgba[tiletypetemp][1],
							rgba[tiletypetemp][2],
							rgba[tiletypetemp][3]));
				}
			}
		}
		whereisthistexture = texturenumber;
		texturemap[temptile] = whereisthistexture;
		++texturenumber;
	}
	else {
		whereisthistexture = texturemap[temptile];
	}
	return whereisthistexture;
}

void TileRenderer::renderExtraTiles(std::size_t offset, const decltype(tilecont.extra_tiles.x)& extra_tiles, const std::size_t max_extra) {
	std::size_t extra_render_counter = 0;
	for (const auto& i : extra_tiles) {
		const ntris::triple& triple_it = std::get<1>(i);
		const std::size_t& x = std::get<0>(triple_it);
		const std::size_t& y = std::get<1>(triple_it);
		const tiletype& t = std::get<2>(triple_it);
		std::size_t whereisthistexture = add_or_find_texture(t);
		const std::size_t tempi = offset + extra_render_counter * 4;
		const sf::Vector2u texturesize = tiletexture.getSize();
		std::size_t tu = whereisthistexture % (texturesize.x / tilesize.first);
		std::size_t tv = whereisthistexture / (texturesize.x / tilesize.first);
		verteximage[tempi].position = sf::Vector2f(x, y);
		verteximage[tempi + 1].position = sf::Vector2f(x + tilesize.first, y);
		verteximage[tempi + 2].position = sf::Vector2f(x + tilesize.first, y + tilesize.second);
		verteximage[tempi + 3].position = sf::Vector2f(x, y + tilesize.second);

		verteximage[tempi].texCoords = sf::Vector2f(tu * tilesize.first, tv * tilesize.first);
		verteximage[tempi + 1].texCoords = sf::Vector2f((tu + 1) * tilesize.first, tv * tilesize.first);
		verteximage[tempi + 2].texCoords = sf::Vector2f((tu + 1) * tilesize.first, (tv + 1) * tilesize.first);
		verteximage[tempi + 3].texCoords = sf::Vector2f(tu * tilesize.first, (tv + 1) * tilesize.first);
		extra_render_counter++;
		if (extra_render_counter > max_extra) break;
	}
}

void TileRenderer::drawtexture(sf::RenderTarget & target, sf::RenderStates states) {

	const std::vector<std::size_t> headline = { 0,4 * extra_render.x,4 * (extra_render.x + extra_render.y),4 * width * height + 4 * (extra_render.x + extra_render.y) };
	for (std::size_t x = 0; x < width; ++x) {
		for (std::size_t y = 0; y < height; ++y) {
			if (tilecont.updated(x, y)) {
				std::size_t whereisthistexture = add_or_find_texture(tilecont.atconst(x, y));
				const std::size_t tempi = xytoi(x, y, width, height) * 4 + headline[2];
				const sf::Vector2u texturesize = tiletexture.getSize();
				std::size_t tu = whereisthistexture % (texturesize.x / tilesize.first);
				std::size_t tv = whereisthistexture / (texturesize.x / tilesize.first);

				verteximage[tempi].texCoords = sf::Vector2f(tu * tilesize.first, tv * tilesize.first);
				verteximage[tempi + 1].texCoords = sf::Vector2f((tu + 1) * tilesize.first, tv * tilesize.first);
				verteximage[tempi + 2].texCoords = sf::Vector2f((tu + 1) * tilesize.first, (tv + 1) * tilesize.first);
				verteximage[tempi + 3].texCoords = sf::Vector2f(tu * tilesize.first, (tv + 1) * tilesize.first);
			}
		}
	}
	renderExtraTiles(headline[0], tilecont.extra_tiles.x, extra_render.x);
	renderExtraTiles(headline[1], tilecont.extra_tiles.y, extra_render.y);
	renderExtraTiles(headline[3], tilecont.extra_tiles.z, extra_render.z);

	//states.transform *= getTransform();
	states.texture = &tiletexture;
	if (shader_active) {
		shader.setUniform("rubyTexture", sf::Shader::CurrentTexture);
		states.shader = &shader;
	}
	target.draw(verteximage, states);

	for (std::size_t i = 0; i < 4 * tilecont.extra_tiles.x.size(); ++i) {
		verteximage[i + headline[0]] = sf::Vector2f(0, 0);
		verteximage[i + headline[0]].texCoords = sf::Vector2f(0, 0);
	}
	tilecont.extra_tiles.x.clear();

	for (std::size_t i = 0; i < 4 * tilecont.extra_tiles.y.size(); ++i) {
		verteximage[i + headline[1]] = sf::Vector2f(0, 0);
		verteximage[i + headline[1]].texCoords = sf::Vector2f(0, 0);
	}
	tilecont.extra_tiles.y.clear();

	for (std::size_t i = 0; i < 4 * tilecont.extra_tiles.z.size(); ++i) {
		verteximage[i + headline[3]] = sf::Vector2f(0, 0);
		verteximage[i + headline[3]].texCoords = sf::Vector2f(0, 0);
	}
	tilecont.extra_tiles.z.clear();

	tilecont.resetupdated();
}

bool TileRenderer::set_shader(std::string const& path, sf::Shader::Type const& shader_type)
{
	shader_active = false;
	if (!sf::Shader::isAvailable()) {
		Log::update_error("This GPU doesn't support shaders");
		return false;
	}
	{
		std::ifstream test(path,std::ifstream::in);
		if (!test.is_open()) {
			Log::update_error("Couldn't open shader file");
			return false;
		}
	}
	if (!shader.loadFromFile(path, shader_type)) {
		Log::update_error("Couldn't load shader");
		return false;
	}
	shader_active = true;
	return true;
}

std::size_t TileRenderer::getWidth() const
{
	return width;
}
std::size_t TileRenderer::getHeight() const
{
	return height;
}
std::size_t TileRenderer::getWidthPixels() const
{
	return width_pixels;
}
std::size_t TileRenderer::getHeightPixels() const
{
	return height_pixels;
}

void TileRenderer::drawvertex(sf::RenderTarget & target, sf::RenderStates states) {
	for (std::size_t x = 0; x < width; ++x) {
		for (std::size_t y = 0; y < height; ++y) {
			if (tilecont.updated(x, y)) {
				const int primacifra[4] = { //could be made into a routine to clear some of this code
					tilecont.atconst(x,y).palette_color[0] / 16,
					tilecont.atconst(x,y).palette_color[1] / 16,
					tilecont.atconst(x,y).palette_color[2] / 16,
					tilecont.atconst(x,y).palette_color[3] / 16
				};
				const int secondacifra[4] = {
					tilecont.atconst(x,y).palette_color[0] % 16,
					tilecont.atconst(x,y).palette_color[1] % 16,
					tilecont.atconst(x,y).palette_color[2] % 16,
					tilecont.atconst(x,y).palette_color[3] % 16
				};
				const unsigned int color[4] = {
					palette[primacifra[0]][secondacifra[0]],
					palette[primacifra[1]][secondacifra[1]],
					palette[primacifra[2]][secondacifra[2]],
					palette[primacifra[3]][secondacifra[3]]
				};
				const sf::Color sfcolor[4] = {
					sf::Color(color[0]),
					sf::Color(color[1]),
					sf::Color(color[2]),
					sf::Color(color[3])
				};
				Sprite* spritetemp = &spritevector[tilecont.atconst(x, y).tilenumber];
				for (std::size_t pixelx = 0; pixelx < tilesize.first; ++pixelx) {
					for (std::size_t pixely = 0; pixely < tilesize.second; ++pixely) {
						nes_uchar tiletypetemp = spritetemp->arr[pixelx][pixely];
						const std::size_t tempi = xytoi(x * tilesize.first + pixelx, y * tilesize.second + pixely, width * tilesize.first, height * tilesize.second) * 4;
						verteximage[tempi].color = sfcolor[tiletypetemp];
						verteximage[tempi + 1].color = sfcolor[tiletypetemp];
						verteximage[tempi + 2].color = sfcolor[tiletypetemp];
						verteximage[tempi + 3].color = sfcolor[tiletypetemp];
					}
				}
			}
		}
	}
	target.draw(verteximage, states);
	tilecont.resetupdated();
}

void TileRenderer::drawsprite(sf::RenderTarget & target, sf::RenderStates states) {
	for (std::size_t x = 0; x < width; ++x) {
		for (std::size_t y = 0; y < height; ++y) {
			if (tilecont.updated(x, y)) {
				const int primacifra[4] = {
					tilecont.atconst(x,y).palette_color[0] / 16,
					tilecont.atconst(x,y).palette_color[1] / 16,
					tilecont.atconst(x,y).palette_color[2] / 16,
					tilecont.atconst(x,y).palette_color[3] / 16
				};
				const int secondacifra[4] = {
					tilecont.atconst(x,y).palette_color[0] % 16,
					tilecont.atconst(x,y).palette_color[1] % 16,
					tilecont.atconst(x,y).palette_color[2] % 16,
					tilecont.atconst(x,y).palette_color[3] % 16
				};
				const unsigned int color[4] = {
					palette[primacifra[0]][secondacifra[0]],
					palette[primacifra[1]][secondacifra[1]],
					palette[primacifra[2]][secondacifra[2]],
					palette[primacifra[3]][secondacifra[3]]
				};
				sf::Uint8 rgba[4][4];
				for (std::size_t colorcounter = 0; colorcounter < 4; ++colorcounter) {
					int colortemp = color[colorcounter];
					for (std::size_t rgbacounter = 0; rgbacounter < 4; ++rgbacounter) {
						rgba[colorcounter][4 - rgbacounter - 1] = colortemp & 0xff;
						colortemp = colortemp >> 8;
					}
				}
				Sprite* spritetemp = &spritevector[tilecont.atconst(x, y).tilenumber];
				sf::Uint8* tempquadretto = quadretti[xytoi(x, y, width, height)].getQuadretto();
				for (std::size_t pixelx = 0; pixelx < tilesize.first; ++pixelx) {
					for (std::size_t pixely = 0; pixely < tilesize.second; ++pixely) {
						std::size_t tempi = xytoi(pixelx, pixely, tilesize.first, tilesize.second) * 4;
						nes_uchar tiletypetemp = spritetemp->arr[pixelx][pixely];
						tempquadretto[tempi] = rgba[tiletypetemp][0];
						tempquadretto[tempi + 1] = rgba[tiletypetemp][1];
						tempquadretto[tempi + 2] = rgba[tiletypetemp][2];
						tempquadretto[tempi + 3] = rgba[tiletypetemp][3];
					}
				}
				temptexclass.update(tempquadretto, tilesize.first, tilesize.second, x * tilesize.first, y * tilesize.second);
			}
		}
	}
	target.draw(tempspriteclass, states);
	tilecont.resetupdated();
}

void TileRenderer::drawimage(sf::RenderTarget & target, sf::RenderStates states) {
	for (std::size_t x = 0; x < width; ++x) {
		for (std::size_t y = 0; y < height; ++y) {
			if (tilecont.updated(x, y)) {
				const int primacifra[4] = {
					tilecont.atconst(x,y).palette_color[0] / 16,
					tilecont.atconst(x,y).palette_color[1] / 16,
					tilecont.atconst(x,y).palette_color[2] / 16,
					tilecont.atconst(x,y).palette_color[3] / 16
				};
				const int secondacifra[4] = {
					tilecont.atconst(x,y).palette_color[0] % 16,
					tilecont.atconst(x,y).palette_color[1] % 16,
					tilecont.atconst(x,y).palette_color[2] % 16,
					tilecont.atconst(x,y).palette_color[3] % 16
				};
				const unsigned int color[4] = {
					palette[primacifra[0]][secondacifra[0]],
					palette[primacifra[1]][secondacifra[1]],
					palette[primacifra[2]][secondacifra[2]],
					palette[primacifra[3]][secondacifra[3]]
				};
				const sf::Color sfcolor[4] = {
					sf::Color(color[0]),
					sf::Color(color[1]),
					sf::Color(color[2]),
					sf::Color(color[3])
				};
				Sprite* spritetemp = &spritevector[tilecont.atconst(x, y).tilenumber];
				for (std::size_t pixelx = 0; pixelx < tilesize.first; ++pixelx) {
					for (std::size_t pixely = 0; pixely < tilesize.second; ++pixely) {
						nes_uchar tiletypetemp = spritetemp->arr[pixelx][pixely];
						finalimageclass.setPixel(x * tilesize.first + pixelx, y * tilesize.second + pixely, sfcolor[tiletypetemp]); //sf::Color(color[Spritevector[tilecont(x,y).tilenumber].arr[pixelx][pixely]]
					}
				}
			}
		}
	}
	temptexclass.update(finalimageclass);
	target.draw(tempspriteclass, states);
	tilecont.resetupdated();
}

void TileRenderer::add_frequent_textures() {
	std::ifstream previous_textures("texturesprite/Pre-rendered textures.txt", std::ios::in);
	if (!previous_textures) {
		Log::update_error("Couldn't open previous textures");
		return;
	}

	sf::Image texture_image;
	texture_image.create(texturesize, texturesize);
	std::size_t tilenumber;
	unsigned int c1, c2, c3, c4; //using char would read 1 char at a time e.g. 2a read as 2
	while ((previous_textures >> std::dec >> tilenumber >> std::hex >> c1 >> c2 >> c3 >> c4 >> std::dec)) {
		add_or_find_texture(tiletype(tilenumber, c1, c2, c3, c4), &texture_image);
	}
	previous_textures.close();
	tiletype block{ 0,0 };
	add_or_find_texture(block, &texture_image);
	for (std::size_t level = 0; level < 10; ++level) {
		block = tiletype(level, 1);
		add_or_find_texture(block, &texture_image);
		block = tiletype(level, 2);
		add_or_find_texture(block, &texture_image);
		block = tiletype(level, 3);
		add_or_find_texture(block, &texture_image);
	}
	tiletexture.loadFromImage(texture_image);
	newtextures.open("texturesprite/Pre-rendered textures.txt", std::ios::app);
}

void TileRenderer::drawmod(sf::RenderTarget & target, sf::RenderStates states)
{
	switch (drawmethod) {
	case DRAWIMAGE:
		drawimage(target, states);
		break;
	case DRAWSPRITE:
		drawsprite(target, states);
		break;
	case DRAWTEXTURE:
		drawtexture(target, states);
		break;
	case DRAWVERTEX:
		drawvertex(target, states);
		break;
	default:
		Log::update_error("Warning, default drawmod");
		drawtexture(target, states);
		break;
	}
}

TileRenderer::~TileRenderer() {
	if (quadretti) delete[]quadretti;
}


unsigned char TileRenderer::colors[10][4] = {
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
/*
sf::Uint32 TileRenderer::palette[4][16]= { //RGB PALLETE
	{0x6d6d6dff,0x2492ff,0xdbff,0x6d49dbff,0x92006dff,0xb6006dff,0xb62400ff,0x924900ff,0x6d4900ff,0x244900ff,0x6d24ff,0x9200ff,0x4949ff,0xff,0xff,0xff},
	{0xb6b6b6ff,0x6ddbff,0x49ffff,0x9200ffff,0xb600ffff,0xff0092ff,0xff0000ff,0xdb6d00ff,0x926d00ff,0x249200ff,0x9200ff,0xb66dff,0x9292ff,0x242424ff,0xff,0xff},
	{0xffffffff,0x6db6ffff,0x9292ffff,0xdb6dffff,0xff00ffff,0xff6dffff,0xff9200ff,0xffb600ff,0xdbdb00ff,0x6ddb00ff,0xff00ff,0x49ffdbff,0xffffff,0x494949ff,0xff,0xff},
	{0xffffffff,0xb6dbffff,0xdbb6ffff,0xffb6ffff,0xff92ffff,0xffb6b6ff,0xffdb92ff,0xffff49ff,0xffff6dff,0xb6ff49ff,0x92ff6dff,0x49ffdbff,0x92dbffff,0x929292ff,0xff,0xff}
};*/

sf::Uint32 TileRenderer::palette[4][16] = { //YPrBr palette //with ntris::trnspr transparent
	{0x7C7C7CFF ,0x0000FCFF ,0x0000BCFF ,0x4428BCFF ,0x940084FF ,0xA80020FF ,0xA81000FF ,0x881400FF ,0x503000FF ,0x007800FF ,0x006800FF ,0x005800FF ,0x004058FF ,0x000000FF ,0x000000FF ,0x000000FF},
	{0xBCBCBCFF ,0x0078F8FF ,0x0058F8FF ,0x6844FCFF ,0xD800CCFF ,0xE40058FF ,0xF83800FF ,0xE45C10FF ,0xAC7C00FF ,0x00B800FF ,0x00A800FF ,0x00A844FF ,0x008888FF ,0x00000000 ,0x000000FF ,0x000000FF},
	{0xF8F8F8FF ,0x3CBCFCFF ,0x6888FCFF ,0x9878F8FF ,0xF878F8FF ,0xF85898FF ,0xF87858FF ,0xFCA044FF ,0xF8B800FF ,0xB8F818FF ,0x58D854FF ,0x58F898FF ,0x00E8D8FF ,0x787878FF ,0x000000FF ,0x000000FF},
	{0xFCFCFCFF ,0xA4E4FCFF ,0xB8B8F8FF ,0xD8B8F8FF ,0xF8B8F8FF ,0xF8A4C0FF ,0xF0D0B0FF ,0xFCE0A8FF ,0xF8D878FF ,0xD8F878FF ,0xB8F8B8FF ,0xB8F8D8FF ,0x00FCFCFF ,0xF8D8F8FF ,0x000000FF ,0x000000FF}
};