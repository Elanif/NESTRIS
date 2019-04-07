#ifndef WINDOW_H
#define WINDOW_H

#include"Input.h"
#include"SDL.h"
#include"Engine.h"
#include<cstdio>

class Window
{
    public:
        Window(const size_t& width, const size_t& height);
        virtual ~Window();
    protected:
    private:
        SDL_Window* initWindow(const size_t& _width, const size_t& _height);
        Input* initInput();
        Input* inputManager;
        SDL_Window* window;
        void mainloop();
        bool active;
};

#endif
