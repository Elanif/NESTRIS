#include<cstdio>
#include"SDL.h"
#undef main
#include"Window.h"
const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 224;


int main(int argc, char** args) {
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        Window finestra=Window(SCREEN_WIDTH,SCREEN_HEIGHT);
    }
}
