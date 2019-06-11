#ifndef RENDERLEVELSELECT_H
#define RENDERLEVELSELECT_H

#include"ActiveInputs.hpp"
#include <SFML/Graphics.hpp>
#include "TileContainer.hpp"
#include"Renderer.hpp"

class RenderLevelSelect : Renderer
{
    public:
        RenderLevelSelect(TileContainer* _tilecont, const std::size_t& _frameappearance, const std::size_t& _currentlevel);
        RenderLevelSelect();
        int update(ActiveInputs* activeinputs);
        void renderLevelSelect(const bool& _reload);
        int updateLevelSelect(const ActiveInputs& _input);
    protected:
    private:
        void reload();
        enum BUTTONS {
            Left,
            Up,
            Right,
            Down,
            Select,
            Start,
            B,
            A
        };
        std::size_t currentlevel;
        unsigned int blink;
        std::size_t lastrenderedlevel;
};

#endif
