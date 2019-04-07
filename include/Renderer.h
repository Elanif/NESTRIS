#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"

class Renderer
{
    public:
        Renderer(SDL_Window * _window);
        virtual ~Renderer();
    protected:
        SDL_Window * window;
        SDL_Surface * renderSurface;
};
#endif
