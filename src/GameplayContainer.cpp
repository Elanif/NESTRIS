#include "GameplayContainer.hpp"

GameplayContainer::GameplayContainer(TileContainer* _tilecont, unsigned long long _framecounter):
piecehandler(_tilecont, _framecounter),
matrixhandler(_tilecont, _framecounter),
scorehandler(_tilecont, _framecounter, true),
levellineshandler(_tilecont, _framecounter, 0),
statisticshandler(_tilecont, _framecounter, 0)
{
}
