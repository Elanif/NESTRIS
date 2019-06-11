#ifndef RENDERER_H
#define RENDERER_H

#include<cstddef>
#include <SFML/Graphics.hpp>
#include"TileContainer.hpp"
#include"enums.hpp"

class Renderer
{
    public:
		Renderer() {};
        Renderer(TileContainer* _tilegrid);
        Renderer(TileContainer* _tilegrid, const std::size_t& _frameappearance);
        void sleep(const nes_uchar& _sleepcounter);
        void hide(const nes_uchar& _hidecounter);
    protected:
		nes_uchar sleepcounter{ 0 };
		nes_uchar hidecounter{ 0 };
		std::size_t frameappearance{ 0 };
		TileContainer* tilecont{ nullptr };
};
#endif
