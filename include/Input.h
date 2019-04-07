#ifndef INPUT_H
#define INPUT_H
#define MAXBUTTONS 8
#include "ActiveInputs.h"
#include"enums.h"
#include<vector>
#include<utility>
#include<SFML/Window/Event.hpp>

class Input
{
    public:
        Input();
        ActiveInputs getInput();
    private:
        void update(const size_t& _buttons);
        void setup();
        sf::Event event;
        bool prevactiveinputs[MAXBUTTONS];
        bool activeinputs[MAXBUTTONS];
        bool leftandright;
        std::vector<std::pair<sf::Keyboard::Key, glb::BTN> >inputdependancies;
        size_t maxbuttons;
};
#endif
