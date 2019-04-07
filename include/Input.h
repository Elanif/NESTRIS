#ifndef INPUT_H
#define INPUT_H
#define MAXBUTTONS 8
#include<SDL.h>
#include "ActiveInputs.h"
#include"enums.h"
#include<vector>
#include<utility>

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
        vector<pair<int, int> >inputdependancies;
        size_t maxbuttons;
};
#endif
