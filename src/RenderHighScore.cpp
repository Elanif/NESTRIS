#include "RenderHighScore.hpp"
#include"TextWriter.h"
#include<string>

RenderHighScore::RenderHighScore(TileContainer * _tilecont, const nes_ushort& _frameappearance) :
Renderer(_tilecont, _frameappearance)
{

}

void RenderHighScore::update(const ActiveInputs& _input, const nes_ushort& _framecounter) {

}


void RenderHighScore::render(const nes_ushort& _framecounter) {
    TextWriter::write(std::string("GAME OVER"), tilecont, {10,10}, 0x30);

}

void RenderHighScore::resetHighScore(const MatrixContainer& _matrixhandler, const PieceContainer& _piecehandler, const Score& _scorehandler, const LevelLines& _levellineshandler, const Statistics& _statisticshandler) {
    matrixhandler=_matrixhandler;
    piecehandler=_piecehandler;
    scorehandler=_scorehandler;
    levellineshandler=_levellineshandler;
    statisticshandler=_statisticshandler;
}
