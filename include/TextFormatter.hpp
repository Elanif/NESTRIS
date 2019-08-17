#pragma once
#ifndef TextFormatter_HPP
#define TextFormatter_HPP
#include<SFML/Graphics/Text.hpp>
#include<SFML/Graphics/Font.hpp>
#include"ntris.hpp"
#include<string>
#include<vector>

template <typename CharType>
class TextFormatter{
public:
	TextFormatter(sf::Font const& font) 
		:m_font(font)
	{};
	sf::Text getFormattedText(std::string const& m_string, unsigned int const& character_size, sf::Vector2f const& boundaries, bool const& bold = false);
	sf::Vector2f getLastFormattedSize() const;

private:
	sf::Vector2f last_formatted_size;
	sf::Font const& m_font;
};

#endif