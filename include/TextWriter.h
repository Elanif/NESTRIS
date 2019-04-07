#ifndef TEXTWRITER_H
#define TEXTWRITER_H
#include"enums.h"
class TileContainer;

class TextWriter
{
public:
    static void init();
    static void write(const std::string& text, TileContainer *_tilecont, sf::Vector2u _position);
    static void write_hex(unsigned int, TileContainer *_tilecont, sf::Vector2u _position);
    static void write_int(unsigned int, TileContainer *_tilecont, sf::Vector2u _position);
    static std::size_t char_lookup[255];
};

#endif // TEXTWRITER_H
