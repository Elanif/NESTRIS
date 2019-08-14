#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Graphics/Font.hpp>
#include<vector>
#include<memory>

class TextRenderer {
public:
	TextRenderer(sf::RenderTarget* _render_target, sf::Font* _font);
	void setFont(sf::Font* _font);
	void setRenderTarget(sf::RenderTarget* _render_target);
	void addText(sf::Text const& text);
	void clear();
	void render();
	std::vector<sf::Text> const& getTexts() const;
private:
	std::shared_ptr<sf::RenderTarget> render_target;
	std::shared_ptr<sf::Font> font;
	std::vector<sf::Text> text_vector;
};