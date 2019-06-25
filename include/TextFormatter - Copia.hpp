#pragma once
#ifndef TEXTFORMATTER_HPP
#define TEXTFORMATTER_HPP
#include<SFML/Graphics/Text.hpp>
#include<SFML/Graphics/Font.hpp>
#include<SFML/Graphics/Drawable.hpp>
#include<SFML/Graphics/RenderTarget.hpp>
#include"enums.hpp"
#include<string>
#include<vector>
#include<unordered_map>

template <typename CharType>
class TextFormatter : public sf::Drawable {
public:
	TextFormatter() {};
	TextFormatter(std::basic_string<CharType> const& string) { setString(string); }
	bool fitString();
	bool fitStringApprox();

	void setString(std::basic_string<CharType> const& string) { m_string = string; }
	void setCharacterSize(double font_size) { m_character_size = font_size; }
	void setFont(sf::Font const& font) { m_font = &font; }
	void setFillColor(sf::Color const& fill_color) { m_fill_color = fill_color; }
	void setPosition(sf::Vector2f const& position) { m_position = position; }
	void setBoundaries(sf::Vector2f const& boundaries) { m_boundaries = boundaries; }
	void setFitToBoundaries(bool const& fit_to_boundaries) { m_fit_to_boundaries = fit_to_boundaries; }
	std::basic_string<CharType> getFormattedString() { return m_formatted_string; }
	sf::Text getFormattedText() const;
	static void calc_font_sizes(sf::Font const* m_font);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		if (m_elaborated) {
			sf::Text t = getFormattedText();
			target.draw(t, states);
		}
	}
	static std::unordered_map<string_character, sf::Vector2f> character_size_map;
	static float max_character_height;
	static bool calculated;

	std::basic_string<CharType> m_string = "";
	std::basic_string<CharType> m_formatted_string = "";
	double m_character_size=30;
	sf::Font const * m_font=nullptr;
	sf::Color m_fill_color=sf::Color::White;
	sf::Vector2f m_position = { 0, 0 }; 
	sf::Vector2f m_boundaries = { 0, 0 };//if one dimension has <=0 boudary it doesnt count
	bool m_fit_to_boundaries; //changes the font size to make it fit
	bool m_elaborated = false;
};



#endif