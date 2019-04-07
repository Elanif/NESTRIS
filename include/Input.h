#ifndef INPUT_H
#define INPUT_H
#include<SDL.h>
#include "ActiveInputs.h"

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

class Input
{
    public:
        Input();
        virtual ~Input();
        bool windowClosed() const;
        ActiveInputs getInput();
    private:
        void update(const Uint8* _keyboardstate, int*_inputdependancies, const size_t& _keys, const size_t& _buttons);
        void setup(const size_t& test);
        SDL_Event* inputevent;
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
        bool* prevactiveinputs;
        bool* activeinputs;
        int* inputdependancies;
        int maxbuttons;
};
#endif
