#include"ActiveInputs.hpp"
#include<cstdio>
#include"ntris.hpp"
ActiveInputs::ActiveInputs(const std::size_t& _buttons, bool *_prevactiveinputs, bool *_activeinputs, bool _leftandright)
    :maxbuttons(_buttons), leftandright(_leftandright)
{
    //todo use std::copy
    //opposing axis handling
    for (std::size_t buttoninit=0; buttoninit<_buttons; ++buttoninit)
        prevactiveinputs[buttoninit]=_prevactiveinputs[buttoninit];
    for (std::size_t buttoninit=0; buttoninit<_buttons; ++buttoninit)
        activeinputs[buttoninit]=_activeinputs[buttoninit];

}

bool ActiveInputs::getHold(const std::size_t& _button) const{ //todo maybe && prevactiveinputs?
    return activeinputs[_button];
}

bool ActiveInputs::getPress(const std::size_t& _button) const{
    return activeinputs[_button]&&!prevactiveinputs[_button];
}

bool ActiveInputs::getLAR() const {
    return leftandright;
}
