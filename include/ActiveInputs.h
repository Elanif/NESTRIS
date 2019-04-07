#ifndef ACTIVEINPUTS_H
#define ACTIVEINPUTS_H
#include"stddef.h"
class ActiveInputs {
    public:
        ActiveInputs(const size_t& _buttons, bool *_prevactiveinputs, bool *_activeinputs, bool _leftandright=false);
        bool getPress(const size_t& _button) const;
        bool getHold(const size_t& _button) const;
        bool getLAR() const;

    private:
        size_t maxbuttons;
        bool leftandright;
        bool prevactiveinputs[8];
        bool activeinputs[8];
};

#endif
