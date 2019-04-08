#ifndef PIECECONTAINER_H
#define PIECECONTAINER_H
#include"enums.h"
#include"PFMatrix.h"
#include"Piece.h"
#include "Renderer.h"
#include<vector>
#include<utility>
#include"ActiveInputs.h"
#include"TileContainer.h"


class PieceContainer : public Renderer
{
public:
    PieceContainer(){};
    PieceContainer(TileContainer * _tilecont, const nes_ushort& _frameappearance);

    Piece tryMove(const ActiveInputs& _inputs);
    Piece tryRotate(const ActiveInputs& _inputs);
    Piece tryDrop(const ActiveInputs& _inputs, const nes_uchar& _gravity);
    void inputManager(const ActiveInputs& _inputs, const PFMatrix& pfmatrix, const nes_uchar& _gravity);
    void doMove(const bool& _collision);
    void doRotate(const bool& _collision);
    void doDrop();
    void lockpiece();
    void spawnPiece();

    const Piece& getPiece() const;
    void render(const nes_ushort& _framecounter, const nes_uchar& _level);
    void rendernextpiece(const nes_uchar& _level);

    Piece lastdroppedpiece;
    nes_uchar holddownpoints;

    bool gameOver(const PFMatrix& pfmatrix);
    bool dropped_event=false;
    bool spawned_event=false;

private:

    nes_uchar hidecountercurrentpiece=0;
    bool collision(const PFMatrix& _pfmatrix, const Piece& _piece);

    std::vector<std::pair<nes_uchar, nes_uchar> > lastrenderedpos;
    std::size_t spawncount=0; //TODO check spawncount
    nes_uchar spawnpiececounter;

    bool downinterrupted=false, hidenextpiece; //TODO does select carry after new games?
    bool just_spawned=false;
    nes_uchar das=0, downcounter=0, holddowncounter=0, init_delay=96;
    Piece currentpiece, nextpiece, shown_nextpiece;

    static nes_uchar spawn_table[7];
};
#endif // PIECE_H
