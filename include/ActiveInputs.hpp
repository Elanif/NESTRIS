#ifndef ACTIVEINPUTS_H
#define ACTIVEINPUTS_H
#include"enums.hpp"
class ActiveInputs {
    public:
        ActiveInputs(const std::size_t& _buttons, bool *_prevactiveinputs, bool *_activeinputs, bool _leftandright=false);
        bool getPress(const std::size_t& _button) const;
        bool getHold(const std::size_t& _button) const;
        bool getLAR() const;

    private:

        std::size_t maxbuttons;
        bool leftandright;
        bool prevactiveinputs[8];
        bool activeinputs[8];
};

#endif
