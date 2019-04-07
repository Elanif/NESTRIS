#ifndef RENDERER_H
#define RENDERER_H

#include"stddef.h"
#include "SDL.h"
#include"enums.h"

class Renderer
{
    public:
        Renderer();
        Renderer(SDL_Window * _window);
        Renderer(SDL_Window * _window, const nes_ushort& _frameappearance);
        void sleep(const nes_uchar& _sleepcounter);
        void hide(const nes_uchar& _hidecounter);
    protected:
        nes_uchar sleepcounter;
        nes_uchar hidecounter;
        nes_ushort frameappearance;
        SDL_Window * window;
        SDL_Surface * renderSurface;
};
#endif
