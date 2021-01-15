#pragma once
#include"Renderer.hpp"
#include"ActiveInputs.hpp"
#include"GameplayContainer.hpp"
#include"Audio.hpp"
class RenderGameModeSelect : public Renderer
{
public:
    RenderGameModeSelect(TileContainer* _tilecont, const std::size_t& _frameappearance);
    RenderGameModeSelect();
    void renderGameModeSelect(const bool& _reload, GameplayContainer& _gameplay_container);
    void updateGameModeSelect(const ActiveInputs& _input, GameplayContainer& _gameplay_container, Audio& _audio);
    bool getSelectedEvent();
protected:
private:
    void reload();
    bool selected_event = false;

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
    unsigned int blink=0;
};

