#ifndef RENDERHIGHSCORE_H
#define RENDERHIGHSCORE_H

#include"Renderer.hpp"
#include"TileContainer.hpp"
#include"Score.hpp"
#include"LevelLines.hpp"
#include"Statistics.hpp"
#include"ActiveInputs.hpp"
#include"MatrixContainer.hpp"
#include"Audio.hpp"

class RenderHighScore : public Renderer
{
    public:
        RenderHighScore(TileContainer* _tilecont, const nes_ushort& _frameappearance);
        void update(const ActiveInputs& _input, const nes_ushort& _framecounter, Audio& _audio) ;
        void render(const nes_ushort& framecounter) ;
        bool submitted=false;
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
