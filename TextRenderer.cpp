#include "include\TextRenderer.hpp"

TextRenderer::TextRenderer(sf::RenderTarget* _render_target, sf::Font* _font)
	:render_target(_render_target),
	font(_font)
{

}

void TextRenderer::setFont(sf::Font* _font)
{
	font.reset(_font);
}

void TextRenderer::setRenderTarget(sf::RenderTarget* _render_target)
{
	render_target.reset(_render_target);
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

void TextRenderer::render()
{
	for (auto const& i : text_vector)
		render_target->draw(i);
}
