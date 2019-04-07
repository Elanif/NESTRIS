#ifndef RENDERER_H
#define RENDERER_H

#include"stddef.h"
#include "SDL.h"
#include"enums.h"

class Renderer
{
    public:
        Renderer();
        Renderer(SDL_Window * _window, const nes_ushort& _frameappearance);
    protected:
        nes_ushort frameappearance;
        SDL_Window * window;
        SDL_Surface * renderSurface;
};
#endif
