#ifndef PIECECONTAINER_H
#define PIECECONTAINER_H
#include"enums.h"
#include"PFMatrix.h"
#include"Piece.h"
#include "Renderer.h"
#include<vector>
#include<utility>
#include"ActiveInputs.h"


class PieceContainer : public Renderer
{
public:
    PieceContainer(){};
    PieceContainer(SDL_Window * _window, const nes_ushort& _frameappearance);
    Piece tryMove(const ActiveInputs& _inputs);
    Piece tryRotate(const ActiveInputs& _inputs);
    Piece tryDrop(const ActiveInputs& _inputs, const nes_uchar& _gravity);
    void inputManager(const ActiveInputs& _inputs, const PFMatrix& pfmatrix, const nes_uchar& _gravity);
    void doMove(const bool& _collision);
    void doRotate(const bool& _collision);
    void doDrop();
    void lockpiece(const nes_uchar& _lockheight);
    const Piece& getPiece() const;
    void deletepiece() ;
    void render(const nes_ushort& _framecounter, const nes_uchar& _level);
    void deletenextpiece();
    void rendernextpiece(const nes_uchar& _level);
    bool dropped;
    Piece lastdroppedpiece;
    nes_uchar holddownpoints;
private:

    std::vector<std::pair<nes_uchar, nes_uchar> > lastrenderedpos;
    size_t spawncount;
    nes_uchar spawnpiececounter;
    void spawnPiece(const nes_uchar& _spawndelay);

    bool downinterrupted;
    nes_uchar das, downcounter, holddowncounter;
    Piece currentpiece, nextpiece;
};
#endif // PIECE_H
