#ifndef PIECE_H
#define PIECE_H
#include "Renderer.h"
#include<vector>
#include<utility>
#include"ActiveInputs.h"
#include"enums.h"

class Piece
{
    public:
        Piece();
        nes_uchar x, y, piecetype, rotation, color;
        std::vector<std::pair<nes_uchar, nes_uchar> > getPos() const;

    static nes_schar rotationmatrix[28][4][2];
    protected:

    private:
};


class PieceContainer : public Renderer
{
public:
    PieceContainer(){};
    PieceContainer(SDL_Window * _window, const nes_ushort& _frameappearance);
    Piece tryMove(const ActiveInputs& _inputs);
    Piece tryRotate(const ActiveInputs& _inputs);
    Piece tryDrop(const ActiveInputs& _inputs, const nes_uchar& _gravity);
    void doMove(const bool& _collision);
    void doRotate(const bool& _collision);
    void doDrop();
    void lockpiece(const nes_uchar& _lockheight);
    const Piece& getPiece() const;
    void deletepiece() ;
    void render(const nes_ushort& _framecounter, const nes_uchar& _level);
private:

    std::vector<std::pair<nes_uchar, nes_uchar> > lastrenderedpos;
    size_t spawncount;
    nes_uchar spawnpiececounter;
    void spawnPiece(const nes_uchar& _spawndelay);

    bool downinterrupted;
    nes_uchar das, downcounter, holddowncounter;
    Piece currentpiece, nextpiece;

    nes_uchar tempdas, tempholddowncounter, tempdowncounter;
    Piece temppiece;
};
#endif // PIECE_H
