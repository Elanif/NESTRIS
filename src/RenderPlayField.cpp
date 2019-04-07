#include "RenderPlayField.h"
#include"random.h"
#include"PieceContainer.h"
#include<cstdio>


RenderPlayField::RenderPlayField(TileContainer * _tilecont, const nes_ushort& _frameappearance, nes_uchar _level):Renderer(_tilecont, _frameappearance), level(_level){
    //piecehandler = PieceContainer(_tilecont, frameappearance);
    matrixhandler = MatrixContainer(_tilecont, frameappearance);
    //scorehandler = Score(_tilecont, frameappearance, true);
    //levellineshandler = LevelLines(_tilecont, frameappearance, _level);
    //statistics
    //level
    //lines
    nes_uchar _tempgravity[19]={48,43,38,33,28,23,18,13,8,6,5,5,5,4,4,4,3,3,3};
    for (size_t i=0; i<19; ++i) gravity[i]=_tempgravity[i];
    for (size_t i=19; i<29; ++i) gravity[i]=2;
    for (size_t i=29; i<255; ++i) gravity[i]=1;

    glb::lineclearframecounter=0;
    firstframeis4=tetris=false;

}

void RenderPlayField::update(const ActiveInputs& _input, const nes_ushort& _framecounter) {
    //printf("renderplayfield::update\n");
    /*piecehandler.inputManager(_input, matrixhandler.getMatrix(), gravity[level]);
    if (piecehandler.dropped) {
        nes_uchar linescleared=matrixhandler.lockpiece(piecehandler.lastdroppedpiece,_framecounter); //for some reason the locked piece doesnt appear at first
        scorehandler.lineclear(level,linescleared);
        scorehandler.softdrop(piecehandler.holddownpoints);
        piecehandler.lockpiece(piecehandler.lastdroppedpiece.y);
        piecehandler.dropped=false;
        if (linescleared) {
            glb::lineclearframecounter=5;
            if (getframemod4()==0) firstframeis4=true;
            else firstframeis4=false;
        }
        else {
            nes_uchar _spawndelay=10+((piecehandler.lastdroppedpiece.y+2)/5)*2; //TODO fidn true formula
            piecehandler.sleep(_spawndelay);
            piecehandler.hidecurrentpiece(_spawndelay-1);
        }
        if (linescleared>=4) {
            tetris=true;
        }
    }*/
}

void RenderPlayField::renderimage(bool blink) {
    if (blink) {
        //playfieldblink
    }
    else {
        //playfieldnoblink
    }
}

void RenderPlayField::render(const nes_ushort& _framecounter) {
    //renderimage(false); more optimization to be done
    if (glb::lineclearframecounter>0) {
        if (tetris&&!firstframeis4) {
            if (getframemod4()==0){
                renderimage(true);
            }
            else if (getframemod4()==1) {
                renderimage(false);
            }
        }
    }
    else if (tetris) {
        tetris=false;
        renderimage(false);
    }
    //levellineshandler.render(_framecounter);
    //piecehandler.deletepiece();
    matrixhandler.render(level);
    //piecehandler.render(_framecounter,levellineshandler.getlevel());
    //scorehandler.render(_framecounter);
    if (glb::lineclearframecounter>0 && !firstframeis4 && getframemod4()==0) glb::lineclearframecounter--;
    if (glb::lineclearframecounter>0 && getframemod4()==0) firstframeis4=false;
}

void RenderPlayField::resetPlayField(const nes_uchar& _level){
    //todo next piece

    renderimage(false);
    level=_level;
    //piecehandler = PieceContainer(tilecont, frameappearance);
    matrixhandler = MatrixContainer(tilecont, frameappearance);
    //scorehandler = Score(tilecont, frameappearance, true);
    //levellineshandler = LevelLines(tilecont, frameappearance, _level);
}
