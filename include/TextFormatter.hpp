#pragma once
#ifndef TEXTFORMATTER_HPP
#define TEXTFORMATTER_HPP
#include<SFML/Graphics/Text.hpp>
#include<SFML/Graphics/Font.hpp>
#include"enums.hpp"
#include<string>
#include<vector>
#include<unordered_map>

template <typename CharType>
class TextFormatter{
public:
	TextFormatter() {};
	TextFormatter(float const& character_size, sf::Font const& font) 
		:m_character_size(character_size)
		,m_font(&font)
	{};
	TextFormatter(std::basic_string<CharType> const& string, float const& character_size, sf::Font const& font)
		:TextFormatter(character_size, font)
	{ setString(string); }
	bool fitString(float const& character_size);
	bool fitStringApprox(float const& character_size);

	void setCharacterSize(float const& character_size) { m_character_size = character_size; }
	void setFont(sf::Font const& font) { m_font = &font; }
	void setString(std::basic_string<CharType> const& string) { m_string = string; }
	void setFillColor(sf::Color const& fill_color) { m_fill_color = fill_color; }
	void setPosition(sf::Vector2f const& position) { m_position = position; }
	void setBoundaries(sf::Vector2f const& boundaries) { m_boundaries = boundaries; }
	std::basic_string<CharType> getFormattedString() { return m_formatted_string; }
	sf::Vector2f getLastFormattedSize() const { return last_formatted_size; }
	sf::Text getFormattedText(float const& character_size);

	std::basic_string<CharType> getFormattedTextString() const;
	void calc_font_sizes();
private:

	std::unordered_map<string_character, sf::Vector2f> character_size_map;
	std::unordered_map<std::basic_string<string_character>, float> kerning_map;
	float max_character_height=0;
	bool calculated=0;

	std::basic_string<CharType> m_string = "";
	std::basic_string<CharType> m_formatted_string = "";
	double m_character_size=30;
	sf::Font const * m_font = nullptr;
	sf::Color m_fill_color=sf::Color::White;
	sf::Vector2f m_position = { 0, 0 }; 
	sf::Vector2f m_boundaries = { 0, 0 };//if one dimension has <=0 boundary it doesnt count
	sf::Vector2f last_formatted_size = { 0,0 };
	bool m_elaborated = false;
};



#endif