#pragma once
#include"Score.hpp"
#include"LevelLines.hpp"
#include"Statistics.hpp"
#include"ActiveInputs.hpp"
#include"MatrixContainer.hpp"
#include"TileContainer.hpp"

class GameplayContainer
{
public:
    GameplayContainer(TileContainer* _tilecont, unsigned long long _framecounter);
    MatrixContainer matrixhandler;

    PieceContainer piecehandler;

    Score scorehandler;

    LevelLines levellineshandler;

    Statistics statisticshandler;
};

