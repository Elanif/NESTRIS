#include"Window.h"

Window::Window(const size_t& _width, const size_t& _height)
{
    initWindow(_width,_height);
    inputManager=initInput();
    active=true;
    Engine _engine= Engine(window,10); //TODO change 10
    while (window.isOpen()) {
        _engine.frame(inputManager->getInput());
        //SFML DELAY 1000/60.098
        //SFML update window
    }
}

Input* Window::initInput() {
    return new Input();
}

void Window::initWindow(const size_t& _width, const size_t& _height) {
    window=sf::RenderWindow(sf::VideoMode(_width, _height), "Nestris");
}

