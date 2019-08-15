#pragma once

#include<SFML/Graphics.hpp>
#include"TextRenderer.hpp"

class OptionsMenu {
public:
	OptionsMenu();
	OptionsMenu(sf::Font const* _font);
	std::vector<sf::Text> getOutput(unsigned int const& characterSize);
	bool isOpenable();
	OptionsMenu open();
	void render(sf::RenderTarget const& render_target);
private:
	const std::string title;
	std::shared_ptr<sf::Font const> font;
	TextRenderer text_renderer;
	std::vector<std::unique_ptr<OptionsMenu> > OptionsMenu_vector;
};