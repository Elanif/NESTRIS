#include "Renderer.h"

Renderer::Renderer(SDL_Window * _window, const size_t& _frameappearance)
    :window(_window), frameappearance(_frameappearance)
{
    renderSurface=SDL_GetWindowSurface(window);
}
Renderer::Renderer()
{
    window=NULL;
    renderSurface=NULL;
}
