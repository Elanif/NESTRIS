#include"enums.hpp"
#include"ConsoleManager.hpp"
#include<cstdio>
#include"Window.hpp"
#include<SFML/Graphics.hpp>
#include"TextWriter.hpp"
#include<SFML/Window/Joystick.hpp>
#include<cstdlib> 
#include"Sound.hpp"
#include"APU.hpp"
#include<iostream>

int main(int argc, char** args) {
    /*APU::init();
    APU::test();
    sf::sleep(sf::milliseconds(10000));*/
    TextWriter::init();
    Sound::init();
    ConsoleManager::init();
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
    Window finestra(glb::ntsc_tiles_x,glb::ntsc_tiles_y,glb::window_scale,Window::GENERAL);
}

/*
non chiamare clear
aggiornamento dinamico del smallesttimeunit
optzione 1: sf::Image collegato a array di bool di cose da aggiornare -> usare rendertexture?
opzine 2:
unordered_map di tiletype, forse ha senso mettere i blocchi colorati dei livelli all'inizio e creare un'hash comoda per questi
<tiletype tiletodraw, std::size_t index> se tiletodraw è in map si usa la posizione di vertexpos[index] messa in VertexArray[i].Quad
se no si fa update di texture
usare comunque un array di bool
thread diversi per ogni combinazione di input da fare dopo draw con pre render
*/
