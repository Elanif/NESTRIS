#include "RenderPlayField.h"
#include"BlockRenderer.h"
#include"SquareRenderer.h"
#include"random.h"
#include<cstdio>

char RenderPlayField::spawnTable[7]={0x02, 0x07, 0x08, 0x0A, 0x0B, 0x0E, 0x12};

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
    };

RenderPlayField::RenderPlayField(SDL_Window * _window, size_t _level):Renderer(_window), level(_level){

    pfmatrix= new int*[10];
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
    spawnPiece();
    int _tempgravity[19]={48,43,38,33,28,23,18,13,8,6,5,5,5,4,4,4,3,3,3};
    for (size_t i=0; i<19; ++i) gravity[i]=_tempgravity[i];
    for (size_t i=19; i<29; ++i) gravity[i]=2;
    for (size_t i=29; i<255; ++i) gravity[i]=1;
}

bool RenderPlayField::checkcollision(PieceDetails& _piece, PieceDetails& _lastgoodpos) {
    bool collision = false;
    for (size_t i=0; i<4; ++i) {
        size_t _xx=_piece.x+rotationmatrix[_piece.currentpiece*4+_piece.rotation][i][0];
        size_t _yy=_piece.y+rotationmatrix[_piece.currentpiece*4+_piece.rotation][i][1];
        if (_xx<0||_xx>9||_yy>21||_yy<0) {
            collision = true;
            break;
        }
        if (pfmatrix[_xx][_yy]) {
            collision = true;
            break;
        }
    }
    if (collision) _piece=_lastgoodpos;
    else _lastgoodpos=_piece;
    return collision;
}

int RenderPlayField::renderandupdate(const ActiveInputs& _input, const unsigned long long& framecounter) {
    updatePlayField(_input);
    renderPlayField(framecounter);
    return 0;
}

int RenderPlayField::lockpiece() {
    clearlines();
    spawnPiece();
    return 0;
}

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

void RenderPlayField::renderPlayField(const unsigned long long& framecounter) {
    //TODO blinking clear delay
    if (blinkscreencounter-->0) {//postfix or prefix?
        for (size_t i=0; i<linescleared; ++i ){
            pfmatrix[blinkscreencounter/4][linesclearedarray[i]]=0;
            pfmatrix[9-blinkscreencounter/4][linesclearedarray[i]]=0;
        }
        if (blinkscreencounter==0) {
            for (size_t i=0; i<10; ++i)
                for (size_t j=0; j<22; ++j)
                    pfmatrix[i][j]=newmatrix[i][j];
        }
    }
    for (size_t x=0; x<10; ++x) { //TODO maybe optimize to render only new stuff around piece
        for (size_t y=3 ; y<22; ++y) {
            if ((blinkscreencounter%4>1)&&pfmatrix[x][y]==0) //TODO wht if it just became 0
                BlockRenderer::block(renderSurface,pfmatrix[x][y],10, x*8,y*8);
            BlockRenderer::block(renderSurface,pfmatrix[x][y],level, x*8,y*8);
        }
    }
    //TODO
    if (spawnpiececounter--<=0) renderPiece(piece);//postfix or prefix?
}

void RenderPlayField::renderPiece(PieceDetails _piece) {
    for (size_t i=0; i<4; ++i) {
        size_t _xx=_piece.x+rotationmatrix[_piece.currentpiece*4+_piece.rotation][i][0];
        size_t _yy=_piece.y+rotationmatrix[_piece.currentpiece*4+_piece.rotation][i][1];
        BlockRenderer::block(renderSurface, _piece.color, level, _xx*8,_yy*8);
    }
}



void RenderPlayField::spawnPiece() {
    char spawnID=piece.currentpiece;
    ++spawnCount;
    piece.x=5;
    piece.y=3;
    char index=random::prng()>>8;
    index+=spawnCount;
    index&=7;
    char newSpawnID;
    if (index!=7) {
        newSpawnID = index;
        if (newSpawnID == spawnID) {
            random::prng();
            index=random::prng()>>8;
            index&=7;
            index+=spawnCount;
            index%=7;
            newSpawnID = index;
        }
    }
    else {
        random::prng();
        index=random::prng()>>8;
        index&=7;
        index+=spawnCount;
        index%=7;
        newSpawnID = index;
    }
    spawnID = newSpawnID;
    printf("spawnid=%d\n",spawnID);
    piece.currentpiece=spawnID;
    piece.rotation=0;
    prevpiece=piece;
    fallcounter=0;
    spawnpiececounter=15; //10~18
}

int RenderPlayField::clearlines() {
    bool whichlines[22];
    size_t lowestline=0;
    linescleared=0;
    for (size_t row=0; row<22; ++row) {
        bool clearedline=true;
        for (size_t column=0; column<10; ++column) {
            if (pfmatrix[column][row]==0) {
                clearedline=false;
                column=10;
            }
        }
        if(whichlines[row]=clearedline) { //no == intentional
            lowestline=row;
            blinkscreencounter=19;
            linesclearedarray[linescleared++]=row;
        }
    }
    //TODO clearanimation
    if (newmatrix) delete[] newmatrix;
    newmatrix=new int*[10];
    for (size_t i=0; i<10; ++i) {
        newmatrix[i]=new int[22];
        for (size_t j=0; j<22; ++j) {
            newmatrix[i][j]=pfmatrix[i][j];
        }
    }
    for (size_t i=lowestline, rowcounter=lowestline; i>=1&&rowcounter>=1; --i,--rowcounter) { //20 because playfield isn't saved over 20
        while(rowcounter>=1&&whichlines[rowcounter]) --rowcounter;
        for (size_t j=0; j<10; ++j) {
            newmatrix[j][i]=pfmatrix[j][rowcounter];
        }
    }
    return linescleared;
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
}
