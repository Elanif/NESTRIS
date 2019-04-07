#include "Input.h"
#include<cstdio>
#include<SFML/Window/Keyboard.hpp>
Input::Input()
    :leftandright(false)
{
    for (std::size_t buttoninit=0; buttoninit<MAXBUTTONS; ++buttoninit) prevactiveinputs[buttoninit]=activeinputs[buttoninit]=false;
    setup(); //tasti della tastiera
}

void Input::update(const std::size_t& _buttons) {
    for (std::size_t buttoninit=0; buttoninit<MAXBUTTONS; ++buttoninit) prevactiveinputs[buttoninit]=activeinputs[buttoninit];
    for (std::size_t buttoninit=0; buttoninit<MAXBUTTONS; ++buttoninit) activeinputs[buttoninit]=false;
    for (std::size_t buttoninit=0; buttoninit<MAXBUTTONS; ++buttoninit) {
        for (const auto& _input_union: inputdependancies[buttoninit]) {
            if (isActive(_input_union)) activeinputs[buttoninit]=true;
        }
    }
    if (activeinputs[glb::Right]) {
        if (activeinputs[glb::Left]) {
            activeinputs[glb::Left]=false;
            leftandright=true;
        }
    }
    if (activeinputs[glb::Down]&&activeinputs[glb::Up]); //up and down stop working until both are held up
}

ActiveInputs Input::getInput() {
    this->update(MAXBUTTONS);
    return ActiveInputs(MAXBUTTONS,prevactiveinputs,activeinputs,leftandright);
}

void Input::setup() { //fix parameters
    active_joysticks.push_back(0);
    for (std::size_t i=0; i<sf::Joystick::AxisCount; ++i)
        joystick_axis_deadzone[0][i]=(0.5);
    input_union insertion;
    insertion.tag=input_union::KEYBOARD_INPUT;
    insertion.keyboard_input=sf::Keyboard::Left;
    inputdependancies[glb::Left].push_back(insertion); //CAREFUL FOR glb::BTN ranges
    insertion.keyboard_input=sf::Keyboard::Right;
    inputdependancies[glb::Right].push_back(insertion); //CAREFUL FOR glb::BTN ranges
    insertion.keyboard_input=sf::Keyboard::Down;
    inputdependancies[glb::Down].push_back(insertion); //CAREFUL FOR glb::BTN ranges
    insertion.keyboard_input=sf::Keyboard::Up;
    inputdependancies[glb::Up].push_back(insertion); //CAREFUL FOR glb::BTN ranges
    insertion.keyboard_input=sf::Keyboard::S;
    inputdependancies[glb::Start].push_back(insertion); //CAREFUL FOR glb::BTN ranges
    insertion.keyboard_input=sf::Keyboard::A;
    inputdependancies[glb::Select].push_back(insertion); //CAREFUL FOR glb::BTN ranges
    insertion.keyboard_input=sf::Keyboard::C;
    inputdependancies[glb::A].push_back(insertion); //CAREFUL FOR glb::BTN ranges
    insertion.keyboard_input=sf::Keyboard::V;
    inputdependancies[glb::B].push_back(insertion); //CAREFUL FOR glb::BTN ranges
}

bool Input::isActive(const input_union& _input_union) const {
    switch(_input_union.tag) {
        case input_union::KEYBOARD_INPUT: {
            return sf::Keyboard::isKeyPressed(_input_union.keyboard_input);
        }
        break;
        case input_union::JOYSTICK_BUTTON: {
            for (const auto& joystick : active_joysticks) {
                return sf::Joystick::isButtonPressed(joystick, _input_union.joystick_button);
            }
        }
        break;
        case input_union::JOYSTICK_AXIS: {
            for (const auto& joystick : active_joysticks) {
                return sf::Joystick::getAxisPosition(joystick, _input_union.joystick_axis)>0.5;//joystick_axis_deadzone[joystick][_input_union.joystick_axis];
            }
        }
        break;
        default:
            glb::cm.update_error(std::string("Default cas in isActive of input_button"));
    }
    return false;
}
