#ifndef ACTIVEINPUTS_H
#define ACTIVEINPUTS_H
#include"SDL.h"
class ActiveInputs {
    public:
        ActiveInputs(const size_t& _buttons, bool *_prevactiveinputs, bool *_activeinputs);
        bool getPress(const size_t& _button) const;
        bool getHold(const size_t& _button) const;

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
    private:
        bool prevactiveinputs[8];
        bool activeinputs[8];
        size_t maxbuttons;
};

#endif
