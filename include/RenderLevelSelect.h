#ifndef RENDERLEVELSELECT_H
#define RENDERLEVELSELECT_H

#include"ActiveInputs.h"
#include <SFML/Graphics.hpp>
#include "TileContainer.h"
#include"Renderer.h"

class RenderLevelSelect : Renderer
{
    public:
        RenderLevelSelect(TileContainer* _tilecont, const size_t& _frameappearance, const size_t& _currentlevel);
        RenderLevelSelect();
        int update(ActiveInputs* activeinputs);
        void renderLevelSelect(const bool& _reload);
        int updateLevelSelect(const ActiveInputs& _input);
    protected:
    private:
        void reload();
        enum BUTTONS {
            LEFT,
            UP,
            RIGHT,
            DOWN,
            SELECT,
            START,
            B,
            A
        };
        size_t currentlevel;
        unsigned int blink;
        size_t lastrenderedlevel;
};

#endif
