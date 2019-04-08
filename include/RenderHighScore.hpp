#ifndef RENDERHIGHSCORE_H
#define RENDERHIGHSCORE_H

#include"Renderer.h"
#include"TileContainer.h"
#include"Score.h"
#include"LevelLines.h"
#include"Statistics.h"
#include"ActiveInputs.h"
#include"MatrixContainer.h"

class RenderHighScore : public Renderer
{
    public:
        RenderHighScore(TileContainer* _tilecont, const nes_ushort& _frameappearance);
        void update(const ActiveInputs& _input, const nes_ushort& _framecounter) ;
        void render(const nes_ushort& framecounter) ;

        //TODO is there a faster way to do this?
        void resetHighScore(const MatrixContainer& _matrixhandler, const PieceContainer& _piecehandler, const Score& _scorehandler, const LevelLines& _levellineshandler, const Statistics& _statisticshandler);

    private:
        MatrixContainer matrixhandler;

        PieceContainer piecehandler;

        Score scorehandler;

        LevelLines levellineshandler;

        Statistics statisticshandler;
};

#endif // RENDERHIGHSCORE_H
