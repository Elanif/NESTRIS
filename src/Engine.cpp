#include"Engine.hpp"
#include<cstdio>
#include"random.hpp"
#include"Log.hpp"
#include<string>

Engine::Engine(TileContainer* _tilecont, const MenuType& _startingmenu) :
tilecont(_tilecont),
currentmenu(_startingmenu),
RLS(_tilecont, 0, 0),
RPF(_tilecont, framecounter, 0),
RHS(_tilecont, framecounter),
gameplay_container(_tilecont, framecounter)
{
    levelselectreload=true;

    framecounter=0;
    Log::update<std::string>("system",std::string("Engine init"));
}


void Engine::frame(const ActiveInputs& _inputs, Audio & _audio) {
    ++framecounter;
    ntris::incframe();
    random::prng();
    switch(currentmenu) {

    case LEVELSELECT:
        RLS.renderLevelSelect(levelselectreload);
        levelselectreload=false;
        levelselect=RLS.updateLevelSelect(_inputs, gameplay_container, _audio);
        if (levelselect>=0) {
            currentmenu=PLAYFIELD;
            //should these be moved into resetPlayingField?
            tilecont->reset();
            gameplay_container.piecehandler = PieceContainer(tilecont, framecounter);
            gameplay_container.matrixhandler = MatrixContainer(tilecont, framecounter);
            gameplay_container.levellineshandler = LevelLines(tilecont, framecounter, levelselect);
            gameplay_container.statisticshandler = Statistics(tilecont, framecounter, levelselect);
            RPF.resetPlayField(levelselect);
        }
        break;

    case PLAYFIELD:

        RPF.update(_inputs, framecounter,gameplay_container, _audio);
        RPF.render(framecounter, gameplay_container, _audio);
        if (RPF.gameOver()) {
            _audio.playTopOut();
            //RHS.resetHighScore(RPF.matrixhandler, RPF.piecehandler, RPF.scorehandler, RPF.levellineshandler, RPF.statisticshandler);
            currentmenu=HIGHSCORE;
        }
        break;

    case HIGHSCORE:
        RHS.update(_inputs, framecounter, gameplay_container, _audio);
        RHS.render(framecounter, gameplay_container);
        if (RHS.submitted) {
            gameplay_container.scorehandler.storeScore();
            currentmenu=LEVELSELECT;
            levelselectreload=true;
            RHS.submitted=false;
        }
        break;

    default:
        Log::update_error("ERROR default frame case in engine::frame switch");
        break;
    }
}
