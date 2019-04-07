#ifndef MATRIXCONTAINER_H
#define MATRIXCONTAINER_H
#include"SDL.h"
#include"Renderer.h"
#include"PieceContainer.h"

class MatrixContainer : public Renderer
{
    public:
        MatrixContainer() {};
        MatrixContainer(SDL_Window * _window, const size_t& _frameappearance);
        bool collision(const Piece& _piece) const;
        char lockpiece(const Piece& _piece);
        char clearlines();
        int getBlock(const char& x, const char& y) const;
    protected:

    private:
        int linesclearedarray[22];
        int newmatrix[10][22];
        int matrix[10][22];
};

#endif // MATRIXCONTAINER_H
