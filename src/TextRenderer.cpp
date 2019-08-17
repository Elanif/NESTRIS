#include "TextRenderer.hpp"

TextRenderer::TextRenderer(sf::Font const& _font)
	:font(_font)
{

}

void TextRenderer::addText(sf::Text const& text)
{
	text_vector.push_back(text);
}

void TextRenderer::clear()
{
	text_vector.clear();
}

std::vector<sf::Text> const& TextRenderer::getTexts() const
{
	return text_vector;
}

void TextRenderer::draw(sf::RenderTarget& render_target)
{
	for (auto const& i : text_vector)
		render_target.draw(i);
	clear();
}