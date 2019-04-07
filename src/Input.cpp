#include "Input.h"
#include<cstdio>
Input::Input()
    :leftandright(false)
{
    for (size_t buttoninit=0; buttoninit<MAXBUTTONS; ++buttoninit) prevactiveinputs[buttoninit]=activeinputs[buttoninit]=false;
    setup(); //tasti della tastiera
}

void Input::update(const size_t& _buttons) {
    for (size_t buttoninit=0; buttoninit<MAXBUTTONS; ++buttoninit) prevactiveinputs[buttoninit]=activeinputs[buttoninit];
    for (size_t buttoninit=0; buttoninit<MAXBUTTONS; ++buttoninit) activeinputs[buttoninit]=false;
    for (size_t buttoninit=0; buttoninit<MAXBUTTONS; ++buttoninit) {
        if (sf::Keyboard::isKeyPressed(inputdependancies[buttoninit].first)) activeinputs[inputdependancies[buttoninit].second]=true;
    }
    if (activeinputs[glb::RIGHT]) {
        if (activeinputs[glb::LEFT]) {
            activeinputs[glb::LEFT]=false;
            leftandright=true;
        }
    }
    if (activeinputs[glb::DOWN]&&activeinputs[glb::UP]); //up and down stop working until both are held up
}

ActiveInputs Input::getInput() {
    this->update(MAXBUTTONS);
    return ActiveInputs(MAXBUTTONS,prevactiveinputs,activeinputs,leftandright);
}

void Input::setup() { //fix parameters
    inputdependancies.push_back(make_pair(sf::Keyboard::Left,glb::LEFT));
    inputdependancies.push_back(make_pair(sf::Keyboard::Up,glb::UP));
    inputdependancies.push_back(make_pair(sf::Keyboard::Right,glb::RIGHT));
    inputdependancies.push_back(make_pair(sf::Keyboard::Down,glb::DOWN));
    inputdependancies.push_back(make_pair(sf::Keyboard::A,glb::SELECT));
    inputdependancies.push_back(make_pair(sf::Keyboard::S,glb::START));
    inputdependancies.push_back(make_pair(SDL_SCANCODE_Z,B));
    inputdependancies.push_back(make_pair(sf::Keyboard::V,glb::B));
}
