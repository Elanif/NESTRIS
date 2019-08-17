#include"ntris.hpp"
#include<cstdio>
#include"Window.hpp"
#include<SFML/Graphics.hpp>
#include"TextWriter.hpp"
#include<SFML/Window/Joystick.hpp>
#include<cstdlib> 
#include<iostream>
#include"Log.hpp"

int WinMain(int argc, char** args) {
	//std::cout.sync_with_stdio(false);
	//std::cin.tie(nullptr);
    /*APU::init();
    APU::test();
    sf::sleep(sf::milliseconds(10000));*/
    TextWriter::init();
	Log::init();
    while(false) { //joystick testing
        sf::Joystick::update();
        std::cout<<"X"<<sf::Joystick::getAxisPosition(0,sf::Joystick::X)<<" ";
        std::cout<<"Y"<<sf::Joystick::getAxisPosition(0,sf::Joystick::Y)<<" ";
        std::cout<<"Z"<<sf::Joystick::getAxisPosition(0,sf::Joystick::Z)<<" ";
        std::cout<<"R"<<sf::Joystick::getAxisPosition(0,sf::Joystick::R)<<" ";
        std::cout<<"U"<<sf::Joystick::getAxisPosition(0,sf::Joystick::U)<<" ";
        std::cout<<"V"<<sf::Joystick::getAxisPosition(0,sf::Joystick::V)<<" ";
        std::cout<<"PovX"<<sf::Joystick::getAxisPosition(0,sf::Joystick::PovX)<<" ";
        std::cout<<"PovY"<<sf::Joystick::getAxisPosition(0,sf::Joystick::PovY)<<" \n";
        for (int i=0; i<sf::Joystick::ButtonCount; ++i) {
            for (int joy=0; joy<sf::Joystick::Count; ++joy)
            if (sf::Joystick::isButtonPressed(joy,i)) std::cout<<joy<<","<<i<<" ";
        }
        sf::sleep(sf::milliseconds(1000));
        system("CLS");
    }
	//TODO load configs like fourthirds (bool)
    Window finestra(ntris::ntsc_screen_width,ntris::ntsc_screen_height,Window::GENERAL);
}

/*
I could make a different thread for each input option to save about 100-300 microseconds
*/
