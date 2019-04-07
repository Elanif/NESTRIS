#ifndef WINDOW_H
#define WINDOW_H

#include"Input.h"
#include"SDL.h"
#include"Engine.h"
#include<cstdio>

class Window
{
    public:
        Window(const int& width, const int& height);
        virtual ~Window();
    protected:
    private:
        SDL_Window* initWindow(const int& _width, const int& _height);
        Input* initInput();
        Input* inputManager;
        SDL_Window* window;
        void mainloop();
        bool active;
};

#endif
