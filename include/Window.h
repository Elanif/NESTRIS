#ifndef WINDOW_H
#define WINDOW_H

#include"Input.h"
#include <SFML/Graphics.hpp>
#include"Engine.h"
#include<cstdio>

class Window
{
    public:
        Window(const size_t& width, const size_t& height);
    protected:
    private:
        void initWindow(const size_t& _width, const size_t& _height);
        Input* initInput();
        Input* inputManager;
        sf::RenderWindow;
        void mainloop();
        bool active;
};

#endif
