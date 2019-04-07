#pragma once
#ifndef SQUARERENDERER_H
#define SQUARERENDERER_H

class SquareRenderer
{
    public:
        SquareRenderer();
        static void square(SDL_Surface* renderSurface, const Uint8 red, const Uint8 green, const Uint8 blue, const Uint8 alpha, const size_t& x, const size_t& y, const size_t& w, const size_t& h);
        static void square(SDL_Surface* renderSurface, const int color, const Uint8 alpha, const size_t& x, const size_t& y, const size_t& w, const size_t& h);

    protected:

    private:
};

#endif // SQUARERENDERER_H
