#include "RenderPlayField.h"
#include"BlockRenderer.h"
#include"SquareRenderer.h"
#include"random.h"
#include"PieceContainer.h"
#include"SDL_Image.h"
#include<cstdio>


RenderPlayField::RenderPlayField(SDL_Window * _window, const nes_ushort& _frameappearance, nes_uchar _level):Renderer(_window, _frameappearance), level(_level){
    piecehandler = PieceContainer(_window, frameappearance);
    matrixhandler = MatrixContainer(_window, frameappearance);
    scorehandler = Score(_window, frameappearance, true);
    levellineshandler = LevelLines(_window, frameappearance, _level);
    //statistics
    //level
    //lines
    nes_uchar _tempgravity[19]={48,43,38,33,28,23,18,13,8,6,5,5,5,4,4,4,3,3,3};
    for (size_t i=0; i<19; ++i) gravity[i]=_tempgravity[i];
    for (size_t i=19; i<29; ++i) gravity[i]=2;
    for (size_t i=29; i<255; ++i) gravity[i]=1;

    blinkscreencounter=1;

}

void RenderPlayField::update(const ActiveInputs& _input, const nes_ushort& _framecounter) {
    //printf("renderplayfield::update\n");
    piecehandler.inputManager(_input, matrixhandler.getMatrix(), gravity[level]);
    if (piecehandler.dropped) {
        nes_uchar linescleared=matrixhandler.lockpiece(piecehandler.lastdroppedpiece,_framecounter); //for some reason the locked piece doesnt appear at first
        scorehandler.lineclear(level,linescleared);
        scorehandler.softdrop(piecehandler.holddownpoints);
        piecehandler.lockpiece(piecehandler.lastdroppedpiece.y);
        piecehandler.dropped=false;
        if (linescleared) {
            piecehandler.sleep(20);
            piecehandler.hidecurrentpiece(19);
        }
        else {
            nes_uchar _spawndelay=10+((piecehandler.lastdroppedpiece.y+3)/5)*2; //TODO fidn true formula
            piecehandler.sleep(_spawndelay);
            piecehandler.hidecurrentpiece(_spawndelay-1);
        }
        if (linescleared>=4) {
            printf("setting RPF blinkscreencounter=%d\n",blinkscreencounter=((_framecounter+20)/5)*5-_framecounter+2); //+1 so the extra frame it renders the normal scren
        }
    }
    /*if piecehandler.dropped
        score add points piecehandler.holddownpoints //prima o dopo animazione?
        matrix.lockpiece(piecehandelr last dropped piece)
        if matrix.clearing lines
            piecehandler sleep
            piecehandler hide?
            */
}

void RenderPlayField::render(const nes_ushort& _framecounter) {

        //in render
        //if %4==3 bilnk
        //if %4==0 no blink
        //--
    if (blinkscreencounter>0) {
        if (blinkscreencounter%4==2) {
            SDL_Surface * surfacePlayField = IMG_Load( "playfieldblink.png" );
            if( surfacePlayField== NULL )
            {
                printf( "Unable to load image %s! SDL Error: %s\n", "playfieldblink.png", SDL_GetError() );
            }
            SDL_BlitSurface(surfacePlayField, NULL, renderSurface, NULL);
        }
        else if (blinkscreencounter%4==1) {
            SDL_Surface * surfacePlayField = IMG_Load( "playfieldnoblink.png" );
            if( surfacePlayField== NULL )
            {
                printf( "Unable to load image %s! SDL Error: %s\n", "playfieldnoblink.png", SDL_GetError() );
            }
            SDL_BlitSurface(surfacePlayField, NULL, renderSurface, NULL);
        }
        --blinkscreencounter;
    }
    //levellineshandler.render(_framecounter);
    piecehandler.deletepiece();
    matrixhandler.render(level);
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






*/
void RenderPlayField::resetPlayField(const nes_uchar& _level){
    //todo next piece
    level=_level;
    piecehandler = PieceContainer(window, frameappearance);
    matrixhandler = MatrixContainer(window, frameappearance);
    scorehandler = Score(window, frameappearance, true);
    levellineshandler = LevelLines(window, frameappearance, _level);
}
