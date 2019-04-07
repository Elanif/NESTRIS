#include "MatrixContainer.h"
#include<vector>
#include<utility>
#include<cstdio>

MatrixContainer::MatrixContainer(SDL_Window * _window, const size_t& _frameappearance)
    :Renderer(_window, _frameappearance)
{
    for (size_t i=0; i<10; ++i) {
        for (size_t j=0; j<22; ++j) {
            newmatrix[i][j]=matrix[i][j]=0;
        }
    }
}

int MatrixContainer::getBlock(const char& x, const char& y) const {
    return matrix[x][y];
}

bool MatrixContainer::collision(const Piece& _piece) const {
    bool collision = false;
    std::vector<std::pair<char, char> > piecepositions = _piece.getPos();
    for (size_t i=0; i<piecepositions.size(); ++i) {
        size_t _xx=piecepositions[i].first;
        size_t _yy=piecepositions[i].second;
        if (_xx<0||_xx>9||_yy>21||_yy<0) {
                printf("__xx, __yy %d %d is out of bounts\n",_xx,_yy);
            collision = true;
            break;
        }
        if (matrix[_xx][_yy]) {
            collision = true;
            break;
        }
    }
    return collision;
}

char MatrixContainer::lockpiece(const Piece& _piece) {
    std::vector<std::pair<char, char> > piecepositions = _piece.getPos();
    for (size_t i=0; i<piecepositions.size(); ++i) {
        size_t _xx=piecepositions[i].first;
        size_t _yy=piecepositions[i].second;
        matrix[_xx][_yy]=_piece.color;
    }
    char _tempclearedlines=clearlines();
    if (_tempclearedlines) {
        //animation

    }
    return _tempclearedlines;
}


char MatrixContainer::clearlines() {
    bool whichlines[22];
    size_t lowestline=0;
    char linescleared=0;
    for (size_t row=0; row<22; ++row) {
        bool clearedline=true;
        for (size_t column=0; column<10; ++column) {
            if (matrix[column][row]==0) {
                clearedline=false;
                column=10;
            }
        }
        if(whichlines[row]=clearedline) { //no == intentional
            lowestline=row;
            //blinkscreencounter=19;
            linesclearedarray[linescleared++]=row;
        }
    }
    //TODO clearanimation
    for (size_t i=0; i<10; ++i) {
        for (size_t j=0; j<22; ++j) {
            newmatrix[i][j]=matrix[i][j];
        }
    }
    for (size_t i=lowestline, rowcounter=lowestline; i>=1&&rowcounter>=1; --i,--rowcounter) { //20 because playfield isn't saved over 20
        while(rowcounter>=1&&whichlines[rowcounter]) --rowcounter;
        for (size_t j=0; j<10; ++j) {
            newmatrix[j][i]=newmatrix[j][rowcounter];
        }
    }
    return linescleared;
}

/*void RenderPlayField::resetPlayField(const size_t& _level){
    //todo next piece
    level=_level;
    spawnCount=0;
    for (size_t i=0; i<10; ++i) {
        for (size_t j=0; j< 22;++j) {
            matrix[i][j]=0;
        }
    }
}*/

/*void RenderPlayField::renderPlayField(const unsigned long long& framecounter) {
    //TODO blinking clear delay
    if (blinkscreencounter-->0) {//postfix or prefix?
        for (size_t i=0; i<linescleared; ++i ){
            matrix[blinkscreencounter/4][linesclearedarray[i]]=0;
            matrix[9-blinkscreencounter/4][linesclearedarray[i]]=0;
        }
        if (blinkscreencounter==0) {
            for (size_t i=0; i<10; ++i)
                for (size_t j=0; j<22; ++j)
                    matrix[i][j]=newmatrix[i][j];
        }
    }
    for (size_t x=0; x<10; ++x) { //TODO maybe optimize to render only new stuff around piece
        for (size_t y=3 ; y<22; ++y) {
            if ((blinkscreencounter%4>1)&&matrix[x][y]==0) //TODO wht if it just became 0
                BlockRenderer::block(renderSurface,matrix[x][y],10, x*8,y*8);
            BlockRenderer::block(renderSurface,matrix[x][y],level, x*8,y*8);
        }
    }
    //TODO
    if (spawnpiececounter--<=0) renderPiece(piece);//postfix or prefix?
}*/
