#include "RenderLevelSelect.h"
#include<cstdio>
RenderLevelSelect::RenderLevelSelect(TileContainer *_tilecont, const size_t& _frameappearance, const size_t& _currentlevel):
Renderer(_tilecont,_frameappearance),
currentlevel(_currentlevel)
{
    blink=0;
    lastrenderedlevel=0;
}
RenderLevelSelect::RenderLevelSelect()
{

}
int RenderLevelSelect::updateLevelSelect(const ActiveInputs& _input) {
//todo manage priorities
    unsigned int tempblink=blink;
    blink=0;
    if (_input.getPress(glb::START)) {
        if (_input.getHold(glb::A)&&!_input.getPress(glb::A)) return currentlevel+10;
        return currentlevel;
    }
    else if (_input.getPress(glb::LEFT)) {
        if (currentlevel>0) --currentlevel;
    }
    else if (_input.getPress(glb::RIGHT)) {
        if (currentlevel<9) ++currentlevel;
    }
    else if (_input.getPress(glb::UP)) {
        if (currentlevel>4) currentlevel-=5;
    }
    else if (_input.getPress(glb::DOWN)) {
        if (currentlevel<5) currentlevel+=5;
    }
    else blink=tempblink;
    return -1; //TODO
}

void RenderLevelSelect::renderLevelSelect(const bool& _reload) {
    printf("levelselect=%d\n",currentlevel);
    tiletype test;
    //levelselect
    test.palette_color[0]=0x0D;
    test.palette_color[1]=0x2A;
    test.palette_color[2]=0x2A;
    test.palette_color[3]=0x2A;
    //topleft corner
    test.tilenumber=41;
    tilecont->at(6,9)=test;
    //topright
    test.tilenumber=41+2;
    tilecont->at(6+10,9)=test;
    //bottomleft
    test.tilenumber=41+2+3;
    tilecont->at(6,9+4)=test;
    //bottomright
    test.tilenumber=41+2+3+2;
    tilecont->at(6+10,9+4)=test;
    //horizontal
    test.tilenumber=42;
    for (size_t x=0; x<5; ++x) {
            tilecont->at(7+x*2,9)=test;
    }

    test.tilenumber=47;
    for (size_t x=0; x<5; ++x) {
        for (size_t y=0; y<2; ++y) {
            tilecont->at(7+x*2,11+y*2)=test;
        }
    }
    //T
    test.tilenumber=81;
    for (size_t x=0; x<4; ++x) {
            tilecont->at(8+x*2,9)=test;
    }


    if (_reload) reload();
    for (size_t x=0; x<3; ++x) {
        for (size_t y=0; y<3; ++y) {
            tilecont->at(6+x+(lastrenderedlevel%5)*2,9+y+(lastrenderedlevel/5)*2).palette_color[0]=0x0D;
        }
    }
    if (blink++%4<2) {
        for (size_t x=0; x<3; ++x) {
            for (size_t y=0; y<3; ++y) {
                tilecont->at(6+x+(currentlevel%5)*2,9+y+(currentlevel/5)*2).palette_color[0]=0x15;
            }
        }
    }
    lastrenderedlevel=currentlevel;

    /*int imgFlags = IMG_INIT_PNG;
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
    SDL_BlitSurface(surfaceLSNumbers, NULL, renderSurface, &numbersquare);*/
}

void RenderLevelSelect::reload() {
    /*SDL_Surface * surfaceLSBG = SDL_LoadBMP( "levelselectbg.bmp" ); //TODO OPTIMIZE
    if( surfaceLSBG== NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", "levelselectbg.bmp", SDL_GetError() );
    }
    SDL_BlitSurface(surfaceLSBG, NULL, renderSurface, NULL);*/
}
