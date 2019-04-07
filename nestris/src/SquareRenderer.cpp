#include "SquareRenderer.h"

SquareRenderer::SquareRenderer()
{
    //ctor
}

void SquareRenderer::square(SDL_Surface* renderSurface, const Uint8 red, const Uint8 green, const Uint8 blue, const Uint8 alpha, const size_t& x, const size_t& y, const size_t& w, const size_t& h) {
    SDL_Rect orangerect={x,y,w,h};
    SDL_FillRect(renderSurface, &orangerect, SDL_MapRGB(renderSurface->format, red, green, blue));
}
void SquareRenderer::square(SDL_Surface* renderSurface, const int color, const Uint8 alpha, const size_t& x, const size_t& y, const size_t& w, const size_t& h) {
    SDL_Rect orangerect={x,y,w,h};
    SDL_FillRect(renderSurface, &orangerect, SDL_MapRGB(renderSurface->format, color/(16*16*16*16), (color%(16*16*16*16))/(16*16), color%(16*16)));
}
