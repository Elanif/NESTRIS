#include<cstdio>
#include"Window.h"
#include<SFML/Graphics.hpp>

const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 224;


int main(int argc, char** args) {
    sf::Transform stretch;
    stretch.scale(4,4);
    Window finestra(SCREEN_WIDTH,SCREEN_HEIGHT,stretch,true);
}
