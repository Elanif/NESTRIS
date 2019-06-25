#include"Input.hpp"
#include<cstdio>
#include<SFML/Window/Keyboard.hpp>
#include"ConfigReader.hpp"
#include"Log.hpp"

Input::Input()
    :leftandright(false)
{
    for (std::size_t buttoninit=0; buttoninit<MAXBUTTONS; ++buttoninit) prevactiveinputs[buttoninit]=activeinputs[buttoninit]=false;
    setup();
}

void Input::update(const std::size_t& _buttons) {
    for (std::size_t buttoninit=0; buttoninit<MAXBUTTONS; ++buttoninit) prevactiveinputs[buttoninit]=activeinputs[buttoninit];
    for (std::size_t buttoninit=0; buttoninit<MAXBUTTONS; ++buttoninit) activeinputs[buttoninit]=false;
    for (std::size_t buttoninit=0; buttoninit<MAXBUTTONS; ++buttoninit) {
        for (const auto& _input_union: inputdependancies[buttoninit]) {
            if (isActive(_input_union)) {
                activeinputs[buttoninit]=true;
                break;
            }
        }
    }
    if (activeinputs[ntris::Right]) {
        if (activeinputs[ntris::Left]) {
            activeinputs[ntris::Left]=false;
            leftandright=true;
        }
    }
    if (activeinputs[ntris::Down]&&activeinputs[ntris::Up]); //TODO up and down stop working until both are held up
}

ActiveInputs Input::getInput() {
    this->update(MAXBUTTONS);
    return ActiveInputs(MAXBUTTONS,prevactiveinputs,activeinputs,leftandright);
}

void Input::setup() {
    active_joysticks.push_back(0);
    for (std::size_t i=0; i<sf::Joystick::AxisCount; ++i)
        joystick_axis_deadzone[0][i]=(0.5);

    ntris::BTN btn_arr[]={ntris::Left,ntris::Right,ntris::Down,ntris::Up,ntris::Start,ntris::Select,ntris::A,ntris::B};
    using namespace std::string_literals;
    std::string s_btn_arr[]={"left_button"s,"right_button"s,"down_button"s,"up_button"s,"start_button"s,"select_button"s,"a_button"s,"b_button"s};
    ConfigReader keybinds("settings/keybinds.ini");

    initMap();
    for (std::size_t i=0; i<ntris::maxbuttons; ++i) {
        std::vector<std::string> keybinds_get=keybinds.get<std::string>(s_btn_arr[i]);
        for (const auto& values:keybinds_get)
            inputdependancies[btn_arr[i]].push_back(keybinds_lookup_table[values]);
    }
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
                return sf::Joystick::getAxisPosition(joystick, _input_union.joystick_axis)/100.0*_input_union.axis_sign>0.5;//joystick_axis_deadzone[joystick][_input_union.joystick_axis];
            }
        }
        break;
        default:
            Log::update_error(std::string("Default case in isActive of input_button"));
    }
    return false;
}

void Input::initMap() {
    using namespace std::string_literals;
    input_union insertion;
    insertion.tag=input_union::KEYBOARD_INPUT;
    insertion.keyboard_input=sf::Keyboard::A;
    keybinds_lookup_table["KEYBOARD_A"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::B;
    keybinds_lookup_table["KEYBOARD_B"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::C;
    keybinds_lookup_table["KEYBOARD_C"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::D;
    keybinds_lookup_table["KEYBOARD_D"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::E;
    keybinds_lookup_table["KEYBOARD_E"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::F;
    keybinds_lookup_table["KEYBOARD_F"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::G;
    keybinds_lookup_table["KEYBOARD_G"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::H;
    keybinds_lookup_table["KEYBOARD_H"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::I;
    keybinds_lookup_table["KEYBOARD_I"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::J;
    keybinds_lookup_table["KEYBOARD_J"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::K;
    keybinds_lookup_table["KEYBOARD_K"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::L;
    keybinds_lookup_table["KEYBOARD_L"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::M;
    keybinds_lookup_table["KEYBOARD_M"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::N;
    keybinds_lookup_table["KEYBOARD_N"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::O;
    keybinds_lookup_table["KEYBOARD_O"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::P;
    keybinds_lookup_table["KEYBOARD_P"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Q;
    keybinds_lookup_table["KEYBOARD_Q"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::R;
    keybinds_lookup_table["KEYBOARD_R"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::S;
    keybinds_lookup_table["KEYBOARD_S"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::T;
    keybinds_lookup_table["KEYBOARD_T"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::U;
    keybinds_lookup_table["KEYBOARD_U"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::V;
    keybinds_lookup_table["KEYBOARD_V"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::W;
    keybinds_lookup_table["KEYBOARD_W"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::X;
    keybinds_lookup_table["KEYBOARD_X"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Y;
    keybinds_lookup_table["KEYBOARD_Y"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Z;
    keybinds_lookup_table["KEYBOARD_Z"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Num0;
    keybinds_lookup_table["KEYBOARD_Num0"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Num1;
    keybinds_lookup_table["KEYBOARD_Num1"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Num2;
    keybinds_lookup_table["KEYBOARD_Num2"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Num3;
    keybinds_lookup_table["KEYBOARD_Num3"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Num4;
    keybinds_lookup_table["KEYBOARD_Num4"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Num5;
    keybinds_lookup_table["KEYBOARD_Num5"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Num6;
    keybinds_lookup_table["KEYBOARD_Num6"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Num7;
    keybinds_lookup_table["KEYBOARD_Num7"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Num8;
    keybinds_lookup_table["KEYBOARD_Num8"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Num9;
    keybinds_lookup_table["KEYBOARD_Num9"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Escape;
    keybinds_lookup_table["KEYBOARD_Escape"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::LControl;
    keybinds_lookup_table["KEYBOARD_LControl"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::LShift;
    keybinds_lookup_table["KEYBOARD_LShift"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::LAlt;
    keybinds_lookup_table["KEYBOARD_LAlt"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::LSystem;
    keybinds_lookup_table["KEYBOARD_LSystem"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::RControl;
    keybinds_lookup_table["KEYBOARD_RControl"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::RShift;
    keybinds_lookup_table["KEYBOARD_RShift"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::RAlt;
    keybinds_lookup_table["KEYBOARD_RAlt"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::RSystem;
    keybinds_lookup_table["KEYBOARD_RSystem"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Menu;
    keybinds_lookup_table["KEYBOARD_Menu"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::LBracket;
    keybinds_lookup_table["KEYBOARD_LBracket"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::RBracket;
    keybinds_lookup_table["KEYBOARD_RBracket"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Semicolon;
    keybinds_lookup_table["KEYBOARD_Semicolon"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Comma;
    keybinds_lookup_table["KEYBOARD_Comma"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Period;
    keybinds_lookup_table["KEYBOARD_Period"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Quote;
    keybinds_lookup_table["KEYBOARD_Quote"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Slash;
    keybinds_lookup_table["KEYBOARD_Slash"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Backslash;
    keybinds_lookup_table["KEYBOARD_Backslash"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Tilde;
    keybinds_lookup_table["KEYBOARD_Tilde"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Equal;
    keybinds_lookup_table["KEYBOARD_Equal"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Hyphen;
    keybinds_lookup_table["KEYBOARD_Hyphen"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Space;
    keybinds_lookup_table["KEYBOARD_Space"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Enter;
    keybinds_lookup_table["KEYBOARD_Enter"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Backspace;
    keybinds_lookup_table["KEYBOARD_Backspace"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Tab;
    keybinds_lookup_table["KEYBOARD_Tab"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::PageUp;
    keybinds_lookup_table["KEYBOARD_PageUp"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::PageDown;
    keybinds_lookup_table["KEYBOARD_PageDown"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::End;
    keybinds_lookup_table["KEYBOARD_End"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Home;
    keybinds_lookup_table["KEYBOARD_Home"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Insert;
    keybinds_lookup_table["KEYBOARD_Insert"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Delete;
    keybinds_lookup_table["KEYBOARD_Delete"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Add;
    keybinds_lookup_table["KEYBOARD_Add"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Subtract;
    keybinds_lookup_table["KEYBOARD_Subtract"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Multiply;
    keybinds_lookup_table["KEYBOARD_Multiply"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Divide;
    keybinds_lookup_table["KEYBOARD_Divide"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Left;
    keybinds_lookup_table["KEYBOARD_Left"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Right;
    keybinds_lookup_table["KEYBOARD_Right"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Up;
    keybinds_lookup_table["KEYBOARD_Up"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Down;
    keybinds_lookup_table["KEYBOARD_Down"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Numpad0;
    keybinds_lookup_table["KEYBOARD_Numpad0"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Numpad1;
    keybinds_lookup_table["KEYBOARD_Numpad1"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Numpad2;
    keybinds_lookup_table["KEYBOARD_Numpad2"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Numpad3;
    keybinds_lookup_table["KEYBOARD_Numpad3"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Numpad4;
    keybinds_lookup_table["KEYBOARD_Numpad4"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Numpad5;
    keybinds_lookup_table["KEYBOARD_Numpad5"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Numpad6;
    keybinds_lookup_table["KEYBOARD_Numpad6"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Numpad7;
    keybinds_lookup_table["KEYBOARD_Numpad7"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Numpad8;
    keybinds_lookup_table["KEYBOARD_Numpad8"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Numpad9;
    keybinds_lookup_table["KEYBOARD_Numpad9"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::F1;
    keybinds_lookup_table["KEYBOARD_F1"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::F2;
    keybinds_lookup_table["KEYBOARD_F2"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::F3;
    keybinds_lookup_table["KEYBOARD_F3"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::F4;
    keybinds_lookup_table["KEYBOARD_F4"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::F5;
    keybinds_lookup_table["KEYBOARD_F5"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::F6;
    keybinds_lookup_table["KEYBOARD_F6"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::F7;
    keybinds_lookup_table["KEYBOARD_F7"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::F8;
    keybinds_lookup_table["KEYBOARD_F8"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::F9;
    keybinds_lookup_table["KEYBOARD_F9"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::F10;
    keybinds_lookup_table["KEYBOARD_F10"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::F11;
    keybinds_lookup_table["KEYBOARD_F11"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::F12;
    keybinds_lookup_table["KEYBOARD_F12"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::F13;
    keybinds_lookup_table["KEYBOARD_F13"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::F14;
    keybinds_lookup_table["KEYBOARD_F14"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::F15;
    keybinds_lookup_table["KEYBOARD_F15"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::Pause;
    keybinds_lookup_table["KEYBOARD_Pause"s]=insertion;
    insertion.keyboard_input=sf::Keyboard::KeyCount;
    keybinds_lookup_table["KEYBOARD_KeyCount"s]=insertion;

    insertion.tag=input_union::JOYSTICK_AXIS;
    insertion.joystick_axis=sf::Joystick::X;
    insertion.axis_sign=1;
    keybinds_lookup_table["JOYSTICK_X+"s]=insertion;
    insertion.joystick_axis=sf::Joystick::Y;
    keybinds_lookup_table["JOYSTICK_Y+"s]=insertion;
    insertion.joystick_axis=sf::Joystick::Z;
    keybinds_lookup_table["JOYSTICK_Z+"s]=insertion;
    insertion.joystick_axis=sf::Joystick::R;
    keybinds_lookup_table["JOYSTICK_R+"s]=insertion;
    insertion.joystick_axis=sf::Joystick::U;
    keybinds_lookup_table["JOYSTICK_U+"s]=insertion;
    insertion.joystick_axis=sf::Joystick::V;
    keybinds_lookup_table["JOYSTICK_V+"s]=insertion;
    insertion.joystick_axis=sf::Joystick::PovX;
    keybinds_lookup_table["JOYSTICK_PovX+"s]=insertion;
    insertion.joystick_axis=sf::Joystick::PovY;
    keybinds_lookup_table["JOYSTICK_PovY+"s]=insertion;

    insertion.axis_sign=-1;
    keybinds_lookup_table["JOYSTICK_X-"s]=insertion;
    insertion.joystick_axis=sf::Joystick::Y;
    keybinds_lookup_table["JOYSTICK_Y-"s]=insertion;
    insertion.joystick_axis=sf::Joystick::Z;
    keybinds_lookup_table["JOYSTICK_Z-"s]=insertion;
    insertion.joystick_axis=sf::Joystick::R;
    keybinds_lookup_table["JOYSTICK_R-"s]=insertion;
    insertion.joystick_axis=sf::Joystick::U;
    keybinds_lookup_table["JOYSTICK_U-"s]=insertion;
    insertion.joystick_axis=sf::Joystick::V;
    keybinds_lookup_table["JOYSTICK_V-"s]=insertion;
    insertion.joystick_axis=sf::Joystick::PovX;
    keybinds_lookup_table["JOYSTICK_PovX-"s]=insertion;
    insertion.joystick_axis=sf::Joystick::PovY;
    keybinds_lookup_table["JOYSTICK_PovY-"s]=insertion;

    insertion.tag=input_union::JOYSTICK_BUTTON;
    std::string joystick_button="JOYSTICK_"s;
    for (int i=0; i<sf::Joystick::ButtonCount; i++) {
        insertion.joystick_button=i;
        keybinds_lookup_table[joystick_button+std::to_string(i)]=insertion;
    }

}
