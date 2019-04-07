#ifndef RENDERLEVELSELECT_H
#define RENDERLEVELSELECT_H

#include"ActiveInputs.h"
#include "Renderer.h"

class RenderLevelSelect: public Renderer
{
    public:
        RenderLevelSelect(SDL_Window * _window, const size_t& _currentlevel);
        int update(ActiveInputs* activeinputs);
        void renderLevelSelect(const bool& _reload);
        int updateLevelSelect(const ActiveInputs& _input);
    protected:
    private:
        void renderSquare(const Uint8 red, const Uint8 green, const Uint8 blue, const Uint8 alpha, const size_t& x, const size_t& y, const size_t& w, const size_t& h);
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
