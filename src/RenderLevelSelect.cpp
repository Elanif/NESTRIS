#include "RenderLevelSelect.h"
#include<SDL_image.h>
#include<cstdio>
#include"SquareRenderer.h"
RenderLevelSelect::RenderLevelSelect(SDL_Window * _window, const size_t& _currentlevel):Renderer(_window), currentlevel(_currentlevel)
{
    blink=0;
    lastrenderedlevel=0;
}

int RenderLevelSelect::updateLevelSelect(const ActiveInputs& _input) {
//todo manage priorities
    unsigned int tempblink=blink;
    blink=0;
    if (_input.getPress(START)) {
        if (_input.getHold(A)) return currentlevel+10;
        return currentlevel;
    }
    else if (_input.getPress(LEFT)) {
        if (currentlevel>0) --currentlevel;
    }
    else if (_input.getPress(RIGHT)) {
        if (currentlevel<9) ++currentlevel;
    }
    else if (_input.getPress(UP)) {
        if (currentlevel>4) currentlevel-=5;
    }
    else if (_input.getPress(DOWN)) {
        if (currentlevel<5) currentlevel+=5;
    }
    else blink=tempblink;
    return -1; //TODO
}

void RenderLevelSelect::renderLevelSelect(const bool& _reload) {
    if (_reload) reload();
    SquareRenderer::square(renderSurface, 0, 0, 0, 255, 52+16*(lastrenderedlevel%5), 76+16*(lastrenderedlevel/5), 15 ,15);
    if (blink++%4<2) SquareRenderer::square(renderSurface, 252, 80, 16, 255, 52+16*(currentlevel%5), 76+16*(currentlevel/5), 15 ,15);
    lastrenderedlevel=currentlevel;

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }
    SDL_Surface * surfaceLSNumbers = IMG_Load( "levelselectnumbers.png" );
    if( surfaceLSNumbers== NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", "levelselectnumbers.png", SDL_GetError() );
    }
    SDL_Rect numbersquare;
    numbersquare.x=51;
    numbersquare.y=74;
    SDL_BlitSurface(surfaceLSNumbers, NULL, renderSurface, &numbersquare);
}

void RenderLevelSelect::reload() {
    SDL_Surface * surfaceLSBG = SDL_LoadBMP( "levelselectbg.bmp" ); //TODO OPTIMIZE
    if( surfaceLSBG== NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", "levelselectbg.bmp", SDL_GetError() );
    }
    SDL_BlitSurface(surfaceLSBG, NULL, renderSurface, NULL);
}
