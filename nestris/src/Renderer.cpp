#include "Renderer.h"

Renderer::Renderer(SDL_Window * _window, const nes_ushort& _frameappearance)
    :window(_window), frameappearance(_frameappearance)
{
    renderSurface=SDL_GetWindowSurface(window);
}
Renderer::Renderer()
{
    window=NULL;
    renderSurface=NULL;
}

 void Renderer::hide(const nes_uchar& _hidecounter) {
     hidecounter=_hidecounter;
 }
 void Renderer::sleep(const nes_uchar& _sleepcounter) {
     sleepcounter=_sleepcounter;
 }
