#include "Engine.h"
#include<cstdio>
#include"random.h"
Engine::Engine(TileContainer* _tilecont, const size_t& _startingmenu)
    :tilecont(_tilecont), currentmenu(_startingmenu)
{
    printf("tilenumber=%d \n",tilecont->at(0,0).tilenumber);
    //renderer=new Renderer(_window);
    RLS = RenderLevelSelect(tilecont, framecounter, 0);

    levelselectreload=true;

    //RPF = RenderPlayField(framecounter, 0);
    framecounter=0;
    printf("Engine init\n");
}


void Engine::frame(const ActiveInputs& _inputs) {
    ++framecounter;
    incframe();
    random::prng();
    printf("engine::frame, currentmenu=%d\n",currentmenu);
    switch(currentmenu) {
    case LEVELSELECT:
        RLS.renderLevelSelect(levelselectreload);
        levelselectreload=false;
        levelselect=RLS.updateLevelSelect(_inputs);
        //if (levelselect>=0) {
        //    currentmenu=PLAYFIELD;
        //    RPF.resetPlayField(levelselect);
        //}
        break;
    case PLAYFIELD:

        //RPF.update(_inputs, framecounter);
        //RPF.render(framecounter);
        break;
    }
}
