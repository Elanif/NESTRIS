#include "Engine.h"
#include<cstdio>
#include"random.h"
#include"ConsoleManager.h"
Engine::Engine(TileContainer* _tilecont, const size_t& _startingmenu):
tilecont(_tilecont),
currentmenu(_startingmenu),
RLS(_tilecont,0,0),
RPF(_tilecont, framecounter, 0)
{
    //renderer=new Renderer(_window);

    levelselectreload=true;

    framecounter=0;
    glb::cm.update<std::string>("error",std::string("Engine init"));
}


void Engine::frame(const ActiveInputs& _inputs) {
    ++framecounter;
    incframe();
    random::prng();
    glb::cm.update<int>("current menu",currentmenu);
    switch(currentmenu) {
    case LEVELSELECT:
        RLS.renderLevelSelect(levelselectreload);
        levelselectreload=false;
        levelselect=RLS.updateLevelSelect(_inputs);
        if (levelselect>=0) {
            currentmenu=PLAYFIELD;
            RPF.resetPlayField(levelselect);
        }
        break;
    case PLAYFIELD:

        RPF.update(_inputs, framecounter);
        RPF.render(framecounter);
        break;
    default:
        glb::cm.update<std::string>("error","ERROR default frame case in switch");
        break;
    }
}
