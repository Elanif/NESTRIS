#ifndef RENDERER_H
#define RENDERER_H

#include"stddef.h"
#include <SFML/Graphics.hpp>
#include"TileContainer.h"
#include"enums.h"

class Renderer
{
    public:
        Renderer();
        Renderer(TileContainer* _tilegrid);
        Renderer(TileContainer* _tilegrid, const std::size_t& _frameappearance);
        void sleep(const nes_uchar& _sleepcounter);
        void hide(const nes_uchar& _hidecounter);
    protected:
        nes_uchar sleepcounter;
        nes_uchar hidecounter;
        std::size_t frameappearance;
        TileContainer* tilecont;
};
#endif
