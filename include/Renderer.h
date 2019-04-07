#ifndef RENDERER_H
#define RENDERER_H

#include"stddef.h"
#include "SDL.h"

class Renderer
{
    public:
        Renderer();
        Renderer(SDL_Window * _window, const size_t& _frameappearance);
    protected:
        unsigned long long frameappearance;
        SDL_Window * window;
        SDL_Surface * renderSurface;
};
#endif
