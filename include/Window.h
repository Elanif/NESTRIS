#ifndef WINDOW_H
#define WINDOW_H

#include"Input.h"
#include <SFML/Graphics.hpp>
#include"Engine.h"
#include<cstdio>

class Window : public sf::NonCopyable
{
    public:
        Window(const std::size_t& width, const std::size_t& height, sf::Vector2f _scale, const bool& optimized);
    private:
        Input inputManager;
};

#endif
