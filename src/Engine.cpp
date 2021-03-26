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
RGMS(_tilecont, framecounter),
gameplay_container(_tilecont, framecounter)
{
    framecounter=0;
    Log::update<std::string>("system",std::string("Engine init"));
    gameplay_container.music_theme = ntris::default_music_theme;
}


void Engine::frame(const ActiveInputs& _inputs, Audio & _audio) {
    ++framecounter;
    ntris::incframe();
    random::prng();
    switch(currentmenu) {

    case GAMEMODESELECT:
        RGMS.renderGameModeSelect(game_mode_select_reload, gameplay_container);
        game_mode_select_reload = false;
        RGMS.updateGameModeSelect(_inputs, gameplay_container, _audio);
        if (RGMS.getSelectedEvent()) {
            currentmenu = LEVELSELECT;
            levelselectreload = true;
        }
        break;

    case LEVELSELECT:
        _audio.stopMusic();
        RLS.renderLevelSelect(levelselectreload, gameplay_container);
        levelselectreload=false;
        levelselect=RLS.updateLevelSelect(_inputs, gameplay_container, _audio);
        if (levelselect>=0) {
            currentmenu=PLAYFIELD;
            RPF.resetPlayField(levelselect, gameplay_container, framecounter);
        }
        else if (RLS.exitMenu()) { 
            currentmenu = GAMEMODESELECT;
            game_mode_select_reload = true;
        }
        break;

    case PLAYFIELD:
        _audio.playMusic(gameplay_container.music_theme,false);
        RPF.update(_inputs, framecounter,gameplay_container, _audio);
        RPF.render(framecounter, gameplay_container, _audio);
        if (RPF.gameOver()) {
            _audio.playTopOut();
            //RHS.resetHighScore(RPF.matrixhandler, RPF.piecehandler, RPF.scorehandler, RPF.levellineshandler, RPF.statisticshandler);
            currentmenu=HIGHSCORE;
        }
        break;

    case HIGHSCORE:
        _audio.stopMusic();
        RHS.update(_inputs, framecounter, gameplay_container, _audio);
        RHS.render(framecounter, gameplay_container);
        if (RHS.submitted) {
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
