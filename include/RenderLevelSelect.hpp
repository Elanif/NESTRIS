#ifndef RENDERLEVELSELECT_H
#define RENDERLEVELSELECT_H

#include"ActiveInputs.hpp"
#include<SFML/Graphics.hpp>
#include"TileContainer.hpp"
#include"Renderer.hpp"
#include"Audio.hpp"
#include"GameplayContainer.hpp"

class RenderLevelSelect : Renderer
{
    public:
        RenderLevelSelect(TileContainer* _tilecont, const std::size_t& _frameappearance, const std::size_t& _currentlevel);
        RenderLevelSelect();
        void renderLevelSelect(const bool& _reload, GameplayContainer& _gameplay_container);
        int updateLevelSelect(const ActiveInputs& _input, GameplayContainer& _gameplay_container, Audio & _audio);
        bool exitMenu();
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
        bool exit_menu = false;
};

#endif
