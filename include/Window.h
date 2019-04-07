#ifndef WINDOW_H
#define WINDOW_H

#include"Input.h"
#include <SFML/Graphics.hpp>
#include"Engine.h"
#include<cstdio>

class Window : public sf::NonCopyable
{
    public:
        Window(const size_t& width, const size_t& height, sf::RenderStates _states, const bool& optimized);
    private:
        Input inputManager;
};

#endif
