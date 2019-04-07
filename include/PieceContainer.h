#ifndef PIECE_H
#define PIECE_H
#include "Renderer.h"
#include<vector>
#include<utility>
#include"ActiveInputs.h"

class Piece
{
    public:
        Piece();
        char x, y, piecetype, rotation, color;
        std::vector<std::pair<char,char> > getPos() const;

    static char rotationmatrix[28][4][2];
    protected:

    private:
};


class PieceContainer : public Renderer
{
public:
    PieceContainer(){};
    PieceContainer(SDL_Window * _window, const size_t& _frameappearance);
    Piece tryMove(const ActiveInputs& _inputs);
    Piece tryRotate(const ActiveInputs& _inputs);
    Piece tryDrop(const ActiveInputs& _inputs, const unsigned char& _gravity);
    void doMove(const bool& _collision);
    void doRotate(const bool& _collision);
    void doDrop();
    void lockpiece(const char& _lockheight);
    const Piece& getPiece() const;
    void deletepiece() ;
    void render(const size_t& _framecounter, const char& _level);
private:

    std::vector<std::pair<char, char> > lastrenderedpos;
    size_t spawncount;
    unsigned char spawnpiececounter;
    void spawnPiece(const char& _spawndelay);

    bool downinterrupted;
    unsigned char das, downcounter, holddowncounter;
    Piece currentpiece, nextpiece;

    unsigned char tempdas, tempholddowncounter, tempdowncounter;
    Piece temppiece;
};
#endif // PIECE_H
