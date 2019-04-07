#include "TextWriter.h"
#include<string>
#include<sstream>
#include"TileContainer.h"
#include<SFML/System/Vector2.hpp>

std::size_t TextWriter::char_lookup[255]={};

void TextWriter::init() {
    char_lookup[(unsigned char)' ']=87;
    constexpr std::size_t number_offset=1;
    for (std::size_t i=0; i<10; ++i) {
        TextWriter::char_lookup[(unsigned char)('0'+i)]=number_offset+i;
    }
    constexpr std::size_t letter_offset=11;
    for (std::size_t i=0; i<26; ++i) {
        char_lookup[(unsigned char)('a'+i)]=letter_offset+i;
    }
    char_lookup[(unsigned char)'-']=37;
    char_lookup[(unsigned char)',']=38;
    char_lookup[(unsigned char)'\'']=39;
}
void TextWriter::write(const std::string& text, TileContainer *_tilecont, sf::Vector2u _position) {
    for (std::size_t i=0; i<text.length(); ++i) {
        const char ith_char=std::tolower(text[i]);
        if (_position.x<_tilecont->width && _position.y<_tilecont->height) {
            _tilecont->at(_position.x,_position.y) = tiletype(char_lookup[(unsigned char)ith_char], 0x0d,0x30,0,0);
            _position.x++;
        }
        else break;
    }
}
void TextWriter::write_hex(unsigned int _hex_number, TileContainer *_tilecont, sf::Vector2u _position) {
    std::stringstream _tmpstream;
    _tmpstream<<std::hex<<_hex_number;
    TextWriter::write(_tmpstream.str(),_tilecont,_position);
}
void TextWriter::write_int(unsigned int, TileContainer *_tilecont, sf::Vector2u _position) {}
