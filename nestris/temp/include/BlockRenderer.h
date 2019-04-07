#ifndef BLOCKRENDERER_H
#define BLOCKRENDERER_H
#include "SDL.h"

class BlockRenderer
{
    public:
        BlockRenderer(SDL_Window* _window, SDL_Renderer *renderer);
        renderBlock(const size_t blocktype, const size_t level);
    protected:

    private:
};

#endif // BLOCKRENDERER_H
