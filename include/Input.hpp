#ifndef INPUT_H
#define INPUT_H
#define MAXBUTTONS 8
#include"ActiveInputs.hpp"
#include"enums.hpp"
#include<vector>
#include<array>
#include<utility>
#include<SFML/Window/Event.hpp>
#include<SFML/Window/Keyboard.hpp>
#include<SFML/Window/Joystick.hpp>
#include<unordered_map>

struct input_union {
    union {
        sf::Keyboard::Key keyboard_input;
        int joystick_button;
        sf::Joystick::Axis joystick_axis;
    };
    enum  {
        KEYBOARD_INPUT,
        JOYSTICK_BUTTON,
        JOYSTICK_AXIS
    } tag;
    double axis_sign;
};

class Input
{
    public:
        Input();
        ActiveInputs getInput();
    private:
        void update(const std::size_t& _buttons);
        void setup();
        void initMap();
        sf::Event event;
        bool prevactiveinputs[MAXBUTTONS];
        bool activeinputs[MAXBUTTONS];
        bool leftandright;
        bool isActive(const input_union& _input_union) const;
        std::array<std::vector<input_union>, MAXBUTTONS >inputdependancies;
        std::size_t maxbuttons;
        std::vector<unsigned int> active_joysticks;
        std::array<std::array<float, sf::Joystick::AxisCount>, sf::Joystick::Count > joystick_axis_deadzone;
        std::unordered_map<std::string, input_union> keybinds_lookup_table;
};
#endif

/*
enum  	Key {
  Unknown = -1, A = 0, B, C,
  D, E, F, G,
  H, I, J, K,
  L, M, N, O,
  P, Q, R, S,
  T, U, V, W,
  X, Y, Z, Num0,
  Num1, Num2, Num3, Num4,
  Num5, Num6, Num7, Num8,
  Num9, Escape, LControl, LShift,
  LAlt, LSystem, RControl, RShift,
  RAlt, RSystem, Menu, LBracket,
  RBracket, Semicolon, Comma, Period,
  Quote, Slash, Backslash, Tilde,
  Equal, Hyphen, Space, Enter,
  Backspace, Tab, PageUp, PageDown,
  End, Home, Insert, Delete,
  Add, Subtract, Multiply, Divide,
  Left, Right, Up, Down,
  Numpad0, Numpad1, Numpad2, Numpad3,
  Numpad4, Numpad5, Numpad6, Numpad7,
  Numpad8, Numpad9, F1, F2,
  F3, F4, F5, F6,
  F7, F8, F9, F10,
  F11, F12, F13, F14,
  F15, Pause, KeyCount, Dash = Hyphen,
  BackSpace = Backspace, BackSlash = Backslash, SemiColon = Semicolon, Return = Enter
}
 	Key codes. More...
*/
//
//  /([A-Za-z_]\w*)[\s\,]*/g
//
//  insertion.keyboard_input=sf::Keyboard::$1;\nkeybinds_lookup_table["KEYBOARD_$1"s]=insertion;\n
