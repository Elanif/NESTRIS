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

class Engine
{
    public:
        enum MenuType {
            LEVELSELECT,
            PLAYFIELD,
            HIGHSCORE,
        };
        Engine(TileContainer* _tilecont, const MenuType& _startingmenu);
        void frame(const ActiveInputs& _inputs);
    protected:
    private:
        TileContainer* tilecont;
        MenuType currentmenu;

        RenderLevelSelect RLS;
        RenderPlayField RPF;
        RenderHighScore RHS;

        //levelselect
        int levelselect;
        bool levelselectreload;

        //game
        unsigned long long framecounter;
        //statistics
        int piececount[7];

        //int updatePlayField(const ActiveInputs& _input);
        int updateStatistics();
        int updateEndingScreen();
};

#endif // ENGINE_H
