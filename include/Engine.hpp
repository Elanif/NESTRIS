#ifndef ENGINE_H
#define ENGINE_H

#include"Input.hpp"
#include"ActiveInputs.hpp"
#include<SFML/Graphics.hpp>
#include"TileContainer.hpp"
#include"TileRenderer.hpp"
//#include"Renderer.hpp"
#include"RenderLevelSelect.hpp"
#include"RenderPlayField.hpp"
#include"RenderHighScore.hpp"
#include"Audio.hpp"
#include"GameplayContainer.hpp"

class Engine
{
    public:
        enum MenuType {
            LEVELSELECT,
            PLAYFIELD,
            HIGHSCORE,
        };
        Engine(TileContainer* _tilecont, const MenuType& _startingmenu);
        void frame(const ActiveInputs& _inputs, Audio & _audio);
    protected:
    private:

        GameplayContainer gameplay_container;
        TileContainer* tilecont;
        MenuType currentmenu;

        RenderLevelSelect RLS;
        RenderPlayField RPF;
        RenderHighScore RHS;

        //levelselect
        int levelselect = 0;
        bool levelselectreload = 0;

        //game
        unsigned long long framecounter = 0;
        //statistics
        int piececount[7];

        //int updatePlayField(const ActiveInputs& _input);
        int updateStatistics();
        int updateEndingScreen();
};

#endif // ENGINE_H
