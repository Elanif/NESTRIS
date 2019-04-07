#include"enums.h"
#include"ConsoleManager.h"
ConsoleManager glb::cm;
#include<cstdio>
#include"Window.h"
#include<SFML/Graphics.hpp>
const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 224;


int main(int argc, char** args) {
    sf::Vector2f scale(3,3);
    Window finestra(SCREEN_WIDTH,SCREEN_HEIGHT,scale,true);
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
