#include "Renderer.h"

Renderer::Renderer(SDL_Window *_window)
    :window(_window)
{
    renderSurface=SDL_GetWindowSurface(window);
}

Renderer::~Renderer()
{
    //delete renderSurface; //?
}

