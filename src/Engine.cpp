#include "Engine.h"
#include<cstdio>
#include"random.h"
Engine::Engine(SDL_Window *_window, size_t _startingmenu)
    :window(_window), currentmenu(_startingmenu)
{
    renderer=new Renderer(_window);
    RLS = new RenderLevelSelect(_window, 0);

    levelselectreload=true;

    RPF = new RenderPlayField(_window, 0);
    framecounter=0;
    printf("Engine init\n");
}

Engine::~Engine()
{
    //dtor
}

void Engine::frame(const ActiveInputs& _inputs) {
    ++framecounter;
    random::prng();
    switch(currentmenu) {
    case LEVELSELECT:
        RLS->renderLevelSelect(levelselectreload);
        levelselectreload=false;
        levelselect=RLS->updateLevelSelect(_inputs);
        if (levelselect>=0) {
            currentmenu=PLAYFIELD;
            RPF->resetPlayField(levelselect);
        }
        break;
    case PLAYFIELD:
        RPF->renderandupdate(_inputs,framecounter);
        break;
    }
}
