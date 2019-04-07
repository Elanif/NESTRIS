#include "ActiveInputs.h"
#include<cstdio>
ActiveInputs::ActiveInputs(const size_t& _buttons, bool *_prevactiveinputs, bool *_activeinputs) :maxbuttons(_buttons)
{
    //memcpy
    for (size_t buttoninit=0; buttoninit<_buttons; ++buttoninit) prevactiveinputs[buttoninit]=_prevactiveinputs[buttoninit];
    for (size_t buttoninit=0; buttoninit<_buttons; ++buttoninit) activeinputs[buttoninit]=_activeinputs[buttoninit];

}

bool ActiveInputs::getHold(const size_t& _button) const{
    return activeinputs[_button];
}

bool ActiveInputs::getPress(const size_t& _button) const{
    return activeinputs[_button]&&!prevactiveinputs[_button];
}
