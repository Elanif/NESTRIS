#include "RenderPlayField.h"
#include"BlockRenderer.h"
#include"SquareRenderer.h"
#include"random.h"
#include"PieceContainer.h"
#include<cstdio>

/*char RenderPlayField::spawnTable[7]={0x02, 0x07, 0x08, 0x0A, 0x0B, 0x0E, 0x12};

int RenderPlayField::rotationmatrix[28][4][2]={
   { { -1,  0 }, {  0,  0 }, {  1,  0 }, {  0,  1 }, },  // 02: T down (spawn)
   { {  0, -1 }, { -1,  0 }, {  0,  0 }, {  0,  1 }, },  // 03: T left
    { { -1,  0 }, {  0,  0 }, {  1,  0 }, {  0, -1 }, },  // 00: T up
   { {  0, -1 }, {  0,  0 }, {  1,  0 }, {  0,  1 }, },  // 01: T right

   { { -1,  0 }, {  0,  0 }, {  1,  0 }, {  1,  1 }, },  // 07: J down (spawn)
   { {  0, -1 }, {  0,  0 }, { -1,  1 }, {  0,  1 }, },  // 04: J left
   { { -1, -1 }, { -1,  0 }, {  0,  0 }, {  1,  0 }, },  // 05: J up
   { {  0, -1 }, {  1, -1 }, {  0,  0 }, {  0,  1 }, },  // 06: J right

   { { -1,  0 }, {  0,  0 }, {  0,  1 }, {  1,  1 }, },  // 08: Z horizontal (spawn)
   { {  1, -1 }, {  0,  0 }, {  1,  0 }, {  0,  1 }, },  // 09: Z vertical
   { { -1,  0 }, {  0,  0 }, {  0,  1 }, {  1,  1 }, },  // 08: Z horizontal (spawn)
   { {  1, -1 }, {  0,  0 }, {  1,  0 }, {  0,  1 }, },  // 09: Z vertical

   { { -1,  0 }, {  0,  0 }, { -1,  1 }, {  0,  1 }, },  // 0A: O (spawn)
   { { -1,  0 }, {  0,  0 }, { -1,  1 }, {  0,  1 }, },  // 0A: O (spawn)
   { { -1,  0 }, {  0,  0 }, { -1,  1 }, {  0,  1 }, },  // 0A: O (spawn)
   { { -1,  0 }, {  0,  0 }, { -1,  1 }, {  0,  1 }, },  // 0A: O (spawn)

   { {  0,  0 }, {  1,  0 }, { -1,  1 }, {  0,  1 }, },  // 0B: S horizontal (spawn)
   { {  0, -1 }, {  0,  0 }, {  1,  0 }, {  1,  1 }, },  // 0C: S vertical
   { {  0,  0 }, {  1,  0 }, { -1,  1 }, {  0,  1 }, },  // 0B: S horizontal (spawn)
   { {  0, -1 }, {  0,  0 }, {  1,  0 }, {  1,  1 }, },  // 0C: S vertical

   { { -1,  0 }, {  0,  0 }, {  1,  0 }, { -1,  1 }, },  // 0E: L down (spawn)
   { { -1, -1 }, {  0, -1 }, {  0,  0 }, {  0,  1 }, },  // 0F: L left
   { {  1, -1 }, { -1,  0 }, {  0,  0 }, {  1,  0 }, },  // 10: L up
   { {  0, -1 }, {  0,  0 }, {  0,  1 }, {  1,  1 }, },  // 0D: L right

   { { -2,  0 }, { -1,  0 }, {  0,  0 }, {  1,  0 }, },  // 12: I horizontal (spawn)
   { {  0, -2 }, {  0, -1 }, {  0,  0 }, {  0,  1 }, },  // 11: I vertical
   { { -2,  0 }, { -1,  0 }, {  0,  0 }, {  1,  0 }, },  // 12: I horizontal (spawn)
   { {  0, -2 }, {  0, -1 }, {  0,  0 }, {  0,  1 }, },  // 11: I vertical
    };*/

RenderPlayField::RenderPlayField(SDL_Window * _window, const size_t& _frameappearance, size_t _level):Renderer(_window, _frameappearance), level(_level){
    piecehandler = PieceContainer(_window, frameappearance);
    matrixhandler = MatrixContainer(_window, frameappearance);
    scorehandler = Score(_window, frameappearance, true);
    levellineshandler = LevelLines(_window, frameappearance, _level);
    //statistics
    //level
    //lines
    int _tempgravity[19]={48,43,38,33,28,23,18,13,8,6,5,5,5,4,4,4,3,3,3};
    for (size_t i=0; i<19; ++i) gravity[i]=_tempgravity[i];
    for (size_t i=19; i<29; ++i) gravity[i]=2;
    for (size_t i=29; i<255; ++i) gravity[i]=1;

    /*pfmatrix= new int*[10];
    for (size_t i=0; i<10; ++i) {
        pfmatrix[i]=new int[22];
        for (size_t j=0; j< 22;++j) {
            pfmatrix[i][j]=0;
        }
    }
    das=0;
    spawnCount=0;
    fallcounter=0;
    prevpiece=piece;
    blinkscreencounter=spawnpiececounter=0;
    spawnPiece();;*/
}

void RenderPlayField::update(const ActiveInputs& _input) {
    //printf("renderplayfield::update\n");
    piecehandler.doMove(matrixhandler.collision(piecehandler.tryMove(_input)));

    piecehandler.doRotate(matrixhandler.collision(piecehandler.tryRotate(_input)));

    if (matrixhandler.collision(piecehandler.tryDrop(_input,gravity[level]))) {
            printf("drop down collision\n");
        char _lockheight=piecehandler.getPiece().y;
        matrixhandler.lockpiece(piecehandler.getPiece());
        piecehandler.lockpiece(_lockheight);//TODO
    }
    else piecehandler.doDrop();
}

void RenderPlayField::render(const unsigned long long& _framecounter) {
    //levellineshandler.render(_framecounter);
    piecehandler.deletepiece();
    //matrixhandler.render(_framecounter);
    piecehandler.render(_framecounter,levellineshandler.getlevel());
    //scorehandler.render(_framecounter);
}
/*


int RenderPlayField::updatePlayField(const ActiveInputs& _input) {
    if (spawnpiececounter>0||blinkscreencounter>0) return 0;
    prevpiece=piece;
    PieceDetails _lastgoodpos=piece;
    if (_input.getPress(START)) {
        //pause
    }

    if (_input.getPress(LEFT)&&!_input.getPress(RIGHT)) {
        --piece.x;
        das=0;
        if (checkcollision(piece,_lastgoodpos)) das=16;
    }
    else if (_input.getHold(LEFT)&&!_input.getHold(RIGHT)) { //does holding both buttons inhibit das
        ++das;
        if (das>=16) {
            das=10;
            --piece.x;
        }
        if (checkcollision(piece,_lastgoodpos)) das=16;
    }
    if (_input.getPress(RIGHT)&&!_input.getPress(LEFT)) { //left priority over right? do nothing if both?
        ++piece.x;
        das=0;
        if (checkcollision(piece,_lastgoodpos)) das=16;
    }
    else if (_input.getHold(RIGHT)&&!_input.getHold(LEFT)) {//does holding both buttons inhibit das
        ++das;
        if (das>=16) {
            das=10;
            ++piece.x;
        }
        if (checkcollision(piece,_lastgoodpos)) das=16;
    }
    else _lastgoodpos=piece;
    if (_input.getPress(UP)) {
    }
    else if (_input.getPress(DOWN)) {
    }
    else if (_input.getPress(A)&&!_input.getPress(B)) {
        piece.rotation=(piece.rotation+1)%4;
        checkcollision(piece,_lastgoodpos);
    }
    else if (_input.getPress(B)&&!_input.getPress(A)) {
        piece.rotation=(piece.rotation+3)%4;
        checkcollision(piece,_lastgoodpos);
    }
    ++fallcounter;
    if (fallcounter>=gravity[level]) {
        ++piece.y;
        fallcounter=0;
        if (checkcollision(piece,_lastgoodpos)) {
            for (size_t i=0; i<4; ++i) {
                size_t _xx=piece.x+rotationmatrix[piece.currentpiece*4+piece.rotation][i][0];
                size_t _yy=piece.y+rotationmatrix[piece.currentpiece*4+piece.rotation][i][1];
                pfmatrix[_xx][_yy]=piece.color;
                //TODO delete over 20
            }
            lockpiece();
        }
    }
    return 0;
}







void RenderPlayField::resetPlayField(const size_t& _level){
    //todo next piece
    level=_level;
    spawnCount=0;
    for (size_t i=0; i<10; ++i) {
        for (size_t j=0; j< 22;++j) {
            pfmatrix[i][j]=0;
        }
    }
}*/
