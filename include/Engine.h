#ifndef ENGINE_H
#define ENGINE_H

#include "Input.h"
#include "ActiveInputs.h"
#include <SFML/Graphics.hpp>
#include "TileContainer.h"
#include "TileRenderer.h"
//#include "Renderer.h"
#include "RenderLevelSelect.h"
#include "RenderPlayField.h"

class Engine
{
    public:
        Engine(TileContainer* _tilecont, const size_t& _startingmenu);
        void frame(const ActiveInputs& _inputs);
    protected:
    private:
        RenderLevelSelect RLS;
        TileContainer* tilecont;
        RenderPlayField RPF;
        enum MenuType {
            LEVELSELECT=10,
            PLAYFIELD=11
        };
        size_t currentmenu;

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
