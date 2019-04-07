#include "RenderPlayField.h"
#include"random.h"
#include"PieceContainer.h"
#include<cstdio>


RenderPlayField::RenderPlayField(TileContainer * _tilecont, const nes_ushort& _frameappearance, nes_uchar _level):Renderer(_tilecont, _frameappearance), level(_level){
    init_assets();
    piecehandler = PieceContainer(_tilecont, frameappearance);
    matrixhandler = MatrixContainer(_tilecont, frameappearance);
    scorehandler = Score(_tilecont, frameappearance, true);
    levellineshandler = LevelLines(_tilecont, frameappearance, _level);
    //statistics
    //level
    //lines
    nes_uchar _tempgravity[19]={48,43,38,33,28,23,18,13,8,6,5,5,5,4,4,4,3,3,3};
    for (std::size_t i=0; i<19; ++i) gravity[i]=_tempgravity[i];
    for (std::size_t i=19; i<29; ++i) gravity[i]=2;
    for (std::size_t i=29; i<255; ++i) gravity[i]=1;

    glb::lineclearframecounter=0;
    playfield_blink=firstframeis4=tetris=false;

}

void RenderPlayField::update(const ActiveInputs& _input, const nes_ushort& _framecounter) {
    //printf("renderplayfield::update\n");
    piecehandler.inputManager(_input, matrixhandler.getMatrix(), gravity[level]);
    if (piecehandler.dropped) {
        nes_uchar linescleared=matrixhandler.lockpiece(piecehandler.lastdroppedpiece,_framecounter); //for some reason the locked piece doesnt appear at first
        scorehandler.lineclear(level,linescleared);
        scorehandler.softdrop(piecehandler.holddownpoints);
        glb::cm.update<std::string>("system","hold down points= "+std::to_string(piecehandler.holddownpoints));
        piecehandler.lockpiece(piecehandler.lastdroppedpiece.y);
        piecehandler.dropped=false;
        if (linescleared) {
            glb::lineclearframecounter=5;
            if (getframemod4()==0) firstframeis4=true; //normally rendering happens before inputs, since I do it right after input managing I have to account for the frame being already eligible for blinking
            else firstframeis4=false;
            //nes_uchar _spawndelay=20;
            //piecehandler.sleep(_spawndelay);
            //piecehandler.hidecurrentpiece(_spawndelay-1);
        }
        else {
            nes_uchar _spawndelay=10+((piecehandler.lastdroppedpiece.y+2)/5)*2; //TODO fidn true formula
            piecehandler.sleep(_spawndelay);
            piecehandler.hidecurrentpiece(_spawndelay-1);
        }
        if (linescleared>=4) {
            tetris=true;
        }
    }
}

void RenderPlayField::renderimage(bool blink) {
    if (blink) {
        playfield_blink=true;
        tilecont->at(30,23)=tiletype(65,0x0d,0x3c,0x00,0x20);
        tilecont->at(23,23)=tiletype(65,0x0d,0x3c,0x00,0x20);
        tilecont->at(24,17)=tilecont->at(25,17)=tilecont->at(26,17)=tilecont->at(27,17)=tilecont->at(12,25)=tilecont->at(13,25)=tilecont->at(14,25)=tilecont->at(15,25)=tilecont->at(16,25)=tilecont->at(17,25)=tilecont->at(18,25)=tilecont->at(19,25)=tilecont->at(20,25)=tilecont->at(21,25)=tiletype(628,0x0d,0x3c,0x20,0x00);
        tilecont->at(22,5)=tilecont->at(22,6)=tilecont->at(22,7)=tilecont->at(22,8)=tilecont->at(22,9)=tilecont->at(22,10)=tilecont->at(22,11)=tilecont->at(22,12)=tilecont->at(28,12)=tilecont->at(22,13)=tilecont->at(28,13)=tilecont->at(22,14)=tilecont->at(28,14)=tilecont->at(22,15)=tilecont->at(28,15)=tilecont->at(22,16)=tilecont->at(28,16)=tilecont->at(22,17)=tilecont->at(22,18)=tilecont->at(22,19)=tilecont->at(22,20)=tilecont->at(22,21)=tilecont->at(22,22)=tilecont->at(22,23)=tilecont->at(22,24)=tiletype(626,0x0d,0x3c,0x20,0x00);
        tilecont->at(5,5)=tilecont->at(31,14)=tilecont->at(24,25)=tilecont->at(18,27)=tiletype(95,0x0d,0x3c,0x00,0x20);
        tilecont->at(25,22)=tilecont->at(3,26)=tiletype(68,0x0d,0x3c,0x00,0x20);
        tilecont->at(12,4)=tilecont->at(13,4)=tilecont->at(14,4)=tilecont->at(15,4)=tilecont->at(16,4)=tilecont->at(17,4)=tilecont->at(18,4)=tilecont->at(19,4)=tilecont->at(20,4)=tilecont->at(21,4)=tilecont->at(24,11)=tilecont->at(25,11)=tilecont->at(26,11)=tilecont->at(27,11)=tiletype(623,0x0d,0x3c,0x20,0x00);
        tilecont->at(11,4)=tilecont->at(23,11)=tiletype(622,0x0d,0x3c,0x20,0x00);
        tilecont->at(5,0)=tilecont->at(10,1)=tilecont->at(0,2)=tilecont->at(31,2)=tilecont->at(4,5)=tilecont->at(7,5)=tilecont->at(31,6)=tilecont->at(31,8)=tilecont->at(30,11)=tilecont->at(0,13)=tilecont->at(30,17)=tilecont->at(31,20)=tilecont->at(0,21)=tilecont->at(23,22)=tilecont->at(27,22)=tilecont->at(29,23)=tilecont->at(23,25)=tilecont->at(27,25)=tilecont->at(31,25)=tilecont->at(0,26)=tilecont->at(1,26)=tilecont->at(8,26)=tilecont->at(13,26)=tilecont->at(28,26)=tilecont->at(4,27)=tilecont->at(6,27)=tilecont->at(9,27)=tilecont->at(15,27)=tilecont->at(17,27)=tilecont->at(22,27)=tiletype(97,0x0d,0x3c,0x00,0x20);
        tilecont->at(28,17)=tilecont->at(22,25)=tiletype(629,0x0d,0x3c,0x20,0x00);
        tilecont->at(6,5)=tilecont->at(0,17)=tilecont->at(25,25)=tilecont->at(19,27)=tiletype(96,0x0d,0x3c,0x00,0x20);
        tilecont->at(18,0)=tilecont->at(5,1)=tiletype(66,0x0d,0x00,0x00,0x20);
        tilecont->at(23,0)=tilecont->at(6,6)=tilecont->at(0,18)=tilecont->at(25,26)=tiletype(111,0x0d,0x00,0x00,0x20);
        tilecont->at(15,0)=tilecont->at(19,0)=tilecont->at(21,0)=tilecont->at(26,0)=tilecont->at(2,1)=tilecont->at(6,1)=tilecont->at(8,1)=tilecont->at(1,4)=tilecont->at(3,5)=tilecont->at(1,6)=tilecont->at(9,6)=tilecont->at(0,10)=tilecont->at(26,10)=tilecont->at(0,11)=tilecont->at(0,12)=tilecont->at(31,13)=tilecont->at(30,15)=tilecont->at(0,19)=tilecont->at(0,20)=tilecont->at(26,22)=tilecont->at(30,22)=tilecont->at(24,23)=tilecont->at(31,24)=tilecont->at(4,26)=tilecont->at(7,26)=tilecont->at(15,26)=tilecont->at(18,26)=tilecont->at(22,26)=tilecont->at(11,27)=tilecont->at(21,27)=tilecont->at(24,27)=tiletype(129,0x0d,0x3c,0x00,0x20);
        tilecont->at(13,0)=tilecont->at(14,0)=tilecont->at(25,0)=tilecont->at(1,5)=tilecont->at(2,5)=tilecont->at(9,5)=tilecont->at(0,6)=tilecont->at(3,6)=tilecont->at(8,6)=tilecont->at(24,10)=tilecont->at(25,10)=tilecont->at(28,10)=tilecont->at(29,22)=tilecont->at(31,23)=tilecont->at(25,24)=tilecont->at(6,26)=tilecont->at(17,26)=tilecont->at(20,26)=tilecont->at(21,26)=tilecont->at(30,26)=tilecont->at(30,27)=tilecont->at(31,27)=tiletype(127,0x0d,0x3c,0x00,0x20);
        tilecont->at(2,0)=tilecont->at(4,0)=tilecont->at(8,0)=tilecont->at(11,0)=tilecont->at(16,0)=tilecont->at(27,0)=tilecont->at(0,1)=tilecont->at(3,1)=tilecont->at(9,1)=tilecont->at(31,1)=tilecont->at(1,3)=tilecont->at(10,4)=tilecont->at(31,5)=tilecont->at(10,6)=tilecont->at(0,9)=tilecont->at(29,11)=tilecont->at(29,13)=tilecont->at(0,16)=tilecont->at(29,17)=tilecont->at(31,18)=tilecont->at(30,20)=tilecont->at(31,22)=tilecont->at(25,23)=tilecont->at(28,23)=tilecont->at(23,24)=tilecont->at(27,24)=tilecont->at(0,25)=tilecont->at(26,25)=tilecont->at(28,25)=tilecont->at(3,27)=tilecont->at(5,27)=tilecont->at(16,27)=tilecont->at(27,27)=tiletype(128,0x0d,0x3c,0x00,0x20);
        tilecont->at(20,0)=tilecont->at(24,0)=tilecont->at(7,1)=tilecont->at(7,6)=tilecont->at(31,7)=tilecont->at(1,27)=tilecont->at(10,27)=tiletype(157,0x0d,0x3c,0x00,0x20);
        tilecont->at(22,0)=tilecont->at(0,4)=tilecont->at(5,6)=tilecont->at(31,15)=tilecont->at(24,26)=tilecont->at(23,27)=tiletype(110,0x0d,0x3c,0x00,0x20);
        tilecont->at(7,0)=tilecont->at(10,5)=tilecont->at(0,7)=tilecont->at(29,10)=tilecont->at(31,16)=tilecont->at(0,23)=tilecont->at(26,24)=tilecont->at(10,26)=tilecont->at(12,26)=tilecont->at(2,27)=tilecont->at(26,27)=tiletype(143,0x0d,0x3c,0x00,0x20);
        tilecont->at(1,0)=tilecont->at(10,0)=tilecont->at(29,0)=tilecont->at(4,6)=tilecont->at(31,10)=tilecont->at(30,12)=tilecont->at(30,14)=tilecont->at(0,22)=tilecont->at(29,24)=tilecont->at(30,25)=tilecont->at(31,26)=tilecont->at(8,27)=tiletype(158,0x0d,0x3c,0x00,0x20);
        tilecont->at(11,5)=tilecont->at(11,6)=tilecont->at(11,7)=tilecont->at(11,8)=tilecont->at(11,9)=tilecont->at(11,10)=tilecont->at(11,11)=tilecont->at(11,12)=tilecont->at(23,12)=tilecont->at(11,13)=tilecont->at(23,13)=tilecont->at(11,14)=tilecont->at(23,14)=tilecont->at(11,15)=tilecont->at(23,15)=tilecont->at(11,16)=tilecont->at(23,16)=tilecont->at(11,17)=tilecont->at(11,18)=tilecont->at(11,19)=tilecont->at(11,20)=tilecont->at(11,21)=tilecont->at(11,22)=tilecont->at(11,23)=tilecont->at(11,24)=tiletype(625,0x0d,0x3c,0x20,0x00);
        tilecont->at(3,0)=tilecont->at(1,2)=tilecont->at(10,2)=tilecont->at(0,3)=tilecont->at(10,3)=tilecont->at(31,3)=tilecont->at(31,4)=tilecont->at(0,8)=tilecont->at(31,9)=tilecont->at(0,14)=tilecont->at(0,15)=tilecont->at(29,16)=tilecont->at(31,17)=tilecont->at(30,18)=tilecont->at(30,19)=tilecont->at(0,24)=tilecont->at(23,26)=tilecont->at(0,27)=tilecont->at(12,27)=tilecont->at(13,27)=tilecont->at(14,27)=tilecont->at(28,27)=tiletype(112,0x0d,0x3c,0x00,0x20);
        tilecont->at(11,2)=tilecont->at(23,2)=tilecont->at(2,3)=tilecont->at(23,3)=tilecont->at(23,4)=tilecont->at(23,5)=tilecont->at(23,6)=tilecont->at(23,7)=tilecont->at(1,8)=tilecont->at(23,8)=tilecont->at(1,9)=tilecont->at(1,10)=tilecont->at(1,11)=tilecont->at(1,12)=tilecont->at(1,13)=tilecont->at(1,14)=tilecont->at(1,15)=tilecont->at(1,16)=tilecont->at(1,17)=tilecont->at(1,18)=tilecont->at(1,19)=tilecont->at(23,19)=tilecont->at(1,20)=tilecont->at(23,20)=tilecont->at(1,21)=tilecont->at(1,22)=tilecont->at(1,23)=tilecont->at(1,24)=tiletype(633,0x0d,0x3c,0x20,0x00);
        tilecont->at(0,0)=tilecont->at(9,0)=tilecont->at(1,1)=tilecont->at(29,12)=tilecont->at(30,13)=tilecont->at(29,15)=tilecont->at(28,22)=tilecont->at(28,24)=tilecont->at(30,24)=tilecont->at(5,26)=tilecont->at(14,26)=tilecont->at(16,26)=tilecont->at(7,27)=tilecont->at(20,27)=tiletype(142,0x0d,0x3c,0x00,0x20);
        tilecont->at(31,0)=tilecont->at(31,21)=tilecont->at(27,23)=tilecont->at(27,26)=tiletype(67,0x0d,0x3c,0x00,0x20);
        tilecont->at(11,1)=tilecont->at(23,1)=tilecont->at(2,2)=tilecont->at(1,7)=tilecont->at(23,18)=tiletype(630,0x0d,0x3c,0x20,0x00);
        tilecont->at(12,1)=tilecont->at(13,1)=tilecont->at(14,1)=tilecont->at(15,1)=tilecont->at(16,1)=tilecont->at(17,1)=tilecont->at(18,1)=tilecont->at(19,1)=tilecont->at(20,1)=tilecont->at(21,1)=tilecont->at(24,1)=tilecont->at(25,1)=tilecont->at(26,1)=tilecont->at(27,1)=tilecont->at(28,1)=tilecont->at(29,1)=tilecont->at(3,2)=tilecont->at(4,2)=tilecont->at(5,2)=tilecont->at(6,2)=tilecont->at(7,2)=tilecont->at(8,2)=tilecont->at(2,7)=tilecont->at(3,7)=tilecont->at(4,7)=tilecont->at(5,7)=tilecont->at(6,7)=tilecont->at(7,7)=tilecont->at(8,7)=tilecont->at(9,7)=tilecont->at(24,18)=tilecont->at(25,18)=tilecont->at(26,18)=tilecont->at(27,18)=tilecont->at(28,18)=tiletype(631,0x0d,0x3c,0x20,0x00);
        tilecont->at(22,1)=tilecont->at(30,1)=tilecont->at(9,2)=tilecont->at(10,7)=tilecont->at(29,18)=tiletype(632,0x0d,0x3c,0x20,0x00);
        tilecont->at(23,17)=tilecont->at(11,25)=tiletype(627,0x0d,0x3c,0x20,0x00);
        tilecont->at(22,4)=tilecont->at(28,11)=tiletype(624,0x0d,0x3c,0x20,0x00);
        tilecont->at(22,3)=tilecont->at(9,4)=tilecont->at(30,9)=tilecont->at(29,21)=tilecont->at(10,25)=tiletype(637,0x0d,0x3c,0x20,0x00);
        tilecont->at(6,0)=tilecont->at(12,0)=tilecont->at(17,0)=tilecont->at(28,0)=tilecont->at(30,0)=tilecont->at(4,1)=tilecont->at(0,5)=tilecont->at(8,5)=tilecont->at(2,6)=tilecont->at(23,10)=tilecont->at(27,10)=tilecont->at(30,10)=tilecont->at(31,11)=tilecont->at(31,12)=tilecont->at(29,14)=tilecont->at(30,16)=tilecont->at(31,19)=tilecont->at(30,21)=tilecont->at(24,22)=tilecont->at(26,23)=tilecont->at(30,23)=tilecont->at(24,24)=tilecont->at(29,25)=tilecont->at(2,26)=tilecont->at(9,26)=tilecont->at(11,26)=tilecont->at(19,26)=tilecont->at(26,26)=tilecont->at(29,26)=tilecont->at(25,27)=tilecont->at(29,27)=tiletype(126,0x0d,0x3c,0x00,0x20);
        tilecont->at(22,2)=tilecont->at(30,2)=tilecont->at(9,3)=tilecont->at(30,3)=tilecont->at(30,4)=tilecont->at(30,5)=tilecont->at(30,6)=tilecont->at(30,7)=tilecont->at(10,8)=tilecont->at(30,8)=tilecont->at(10,9)=tilecont->at(10,10)=tilecont->at(10,11)=tilecont->at(10,12)=tilecont->at(10,13)=tilecont->at(10,14)=tilecont->at(10,15)=tilecont->at(10,16)=tilecont->at(10,17)=tilecont->at(10,18)=tilecont->at(10,19)=tilecont->at(29,19)=tilecont->at(10,20)=tilecont->at(29,20)=tilecont->at(10,21)=tilecont->at(10,22)=tilecont->at(10,23)=tilecont->at(10,24)=tiletype(634,0x0d,0x3c,0x20,0x00);
        tilecont->at(11,3)=tilecont->at(2,4)=tilecont->at(23,9)=tilecont->at(23,21)=tilecont->at(1,25)=tiletype(635,0x0d,0x3c,0x20,0x00);
        tilecont->at(12,3)=tilecont->at(13,3)=tilecont->at(14,3)=tilecont->at(15,3)=tilecont->at(16,3)=tilecont->at(17,3)=tilecont->at(18,3)=tilecont->at(19,3)=tilecont->at(20,3)=tilecont->at(21,3)=tilecont->at(3,4)=tilecont->at(4,4)=tilecont->at(5,4)=tilecont->at(6,4)=tilecont->at(7,4)=tilecont->at(8,4)=tilecont->at(24,9)=tilecont->at(25,9)=tilecont->at(26,9)=tilecont->at(27,9)=tilecont->at(28,9)=tilecont->at(29,9)=tilecont->at(24,21)=tilecont->at(25,21)=tilecont->at(26,21)=tilecont->at(27,21)=tilecont->at(28,21)=tilecont->at(2,25)=tilecont->at(3,25)=tilecont->at(4,25)=tilecont->at(5,25)=tilecont->at(6,25)=tilecont->at(7,25)=tilecont->at(8,25)=tilecont->at(9,25)=tiletype(636,0x0d,0x3c,0x20,0x00);
    }
    else {
        playfield_blink=false;
        tilecont->at(23,23)=tiletype(65,0x0d,0x31,0x00,0x00);
        tilecont->at(24,17)=tilecont->at(25,17)=tilecont->at(26,17)=tilecont->at(27,17)=tilecont->at(12,25)=tilecont->at(13,25)=tilecont->at(14,25)=tilecont->at(15,25)=tilecont->at(16,25)=tilecont->at(17,25)=tilecont->at(18,25)=tilecont->at(19,25)=tilecont->at(20,25)=tilecont->at(21,25)=tiletype(628,0x0d,0x31,0x00,0x00);
        tilecont->at(22,5)=tilecont->at(22,6)=tilecont->at(22,7)=tilecont->at(22,8)=tilecont->at(22,9)=tilecont->at(22,10)=tilecont->at(22,11)=tilecont->at(22,12)=tilecont->at(28,12)=tilecont->at(22,13)=tilecont->at(28,13)=tilecont->at(22,14)=tilecont->at(28,14)=tilecont->at(22,15)=tilecont->at(28,15)=tilecont->at(22,16)=tilecont->at(28,16)=tilecont->at(22,17)=tilecont->at(22,18)=tilecont->at(22,19)=tilecont->at(22,20)=tilecont->at(22,21)=tilecont->at(22,22)=tilecont->at(22,23)=tilecont->at(22,24)=tiletype(626,0x0d,0x31,0x00,0x00);
        tilecont->at(5,5)=tilecont->at(31,14)=tilecont->at(24,25)=tilecont->at(18,27)=tiletype(95,0x0d,0x31,0x00,0x00);
        tilecont->at(25,22)=tilecont->at(3,26)=tiletype(68,0x0d,0x31,0x00,0x00);
        tilecont->at(12,4)=tilecont->at(13,4)=tilecont->at(14,4)=tilecont->at(15,4)=tilecont->at(16,4)=tilecont->at(17,4)=tilecont->at(18,4)=tilecont->at(19,4)=tilecont->at(20,4)=tilecont->at(21,4)=tilecont->at(24,11)=tilecont->at(25,11)=tilecont->at(26,11)=tilecont->at(27,11)=tiletype(623,0x0d,0x31,0x00,0x00);
        tilecont->at(11,4)=tilecont->at(23,11)=tiletype(622,0x0d,0x31,0x00,0x00);
        tilecont->at(5,0)=tilecont->at(10,1)=tilecont->at(0,2)=tilecont->at(31,2)=tilecont->at(4,5)=tilecont->at(7,5)=tilecont->at(31,6)=tilecont->at(31,8)=tilecont->at(30,11)=tilecont->at(0,13)=tilecont->at(30,17)=tilecont->at(31,20)=tilecont->at(0,21)=tilecont->at(23,22)=tilecont->at(27,22)=tilecont->at(29,23)=tilecont->at(23,25)=tilecont->at(27,25)=tilecont->at(31,25)=tilecont->at(0,26)=tilecont->at(1,26)=tilecont->at(8,26)=tilecont->at(13,26)=tilecont->at(28,26)=tilecont->at(4,27)=tilecont->at(6,27)=tilecont->at(9,27)=tilecont->at(15,27)=tilecont->at(17,27)=tilecont->at(22,27)=tiletype(97,0x0d,0x31,0x00,0x00);
        tilecont->at(28,17)=tilecont->at(22,25)=tiletype(629,0x0d,0x31,0x00,0x00);
        tilecont->at(6,5)=tilecont->at(0,17)=tilecont->at(25,25)=tilecont->at(19,27)=tiletype(96,0x0d,0x31,0x00,0x00);
        tilecont->at(18,0)=tilecont->at(5,1)=tiletype(66,0x0d,0x00,0x00,0x00);
        tilecont->at(23,0)=tilecont->at(6,6)=tilecont->at(0,18)=tilecont->at(25,26)=tiletype(111,0x0d,0x00,0x00,0x00);
        tilecont->at(15,0)=tilecont->at(19,0)=tilecont->at(21,0)=tilecont->at(26,0)=tilecont->at(2,1)=tilecont->at(6,1)=tilecont->at(8,1)=tilecont->at(1,4)=tilecont->at(3,5)=tilecont->at(1,6)=tilecont->at(9,6)=tilecont->at(0,10)=tilecont->at(26,10)=tilecont->at(0,11)=tilecont->at(0,12)=tilecont->at(31,13)=tilecont->at(30,15)=tilecont->at(0,19)=tilecont->at(0,20)=tilecont->at(26,22)=tilecont->at(30,22)=tilecont->at(24,23)=tilecont->at(31,24)=tilecont->at(4,26)=tilecont->at(7,26)=tilecont->at(15,26)=tilecont->at(18,26)=tilecont->at(22,26)=tilecont->at(11,27)=tilecont->at(21,27)=tilecont->at(24,27)=tiletype(129,0x0d,0x31,0x00,0x00);
        tilecont->at(13,0)=tilecont->at(14,0)=tilecont->at(25,0)=tilecont->at(1,5)=tilecont->at(2,5)=tilecont->at(9,5)=tilecont->at(0,6)=tilecont->at(3,6)=tilecont->at(8,6)=tilecont->at(24,10)=tilecont->at(25,10)=tilecont->at(28,10)=tilecont->at(29,22)=tilecont->at(31,23)=tilecont->at(25,24)=tilecont->at(6,26)=tilecont->at(17,26)=tilecont->at(20,26)=tilecont->at(21,26)=tilecont->at(30,26)=tilecont->at(30,27)=tilecont->at(31,27)=tiletype(127,0x0d,0x31,0x00,0x00);
        tilecont->at(2,0)=tilecont->at(4,0)=tilecont->at(8,0)=tilecont->at(11,0)=tilecont->at(16,0)=tilecont->at(27,0)=tilecont->at(0,1)=tilecont->at(3,1)=tilecont->at(9,1)=tilecont->at(31,1)=tilecont->at(1,3)=tilecont->at(10,4)=tilecont->at(31,5)=tilecont->at(10,6)=tilecont->at(0,9)=tilecont->at(29,11)=tilecont->at(29,13)=tilecont->at(0,16)=tilecont->at(29,17)=tilecont->at(31,18)=tilecont->at(30,20)=tilecont->at(31,22)=tilecont->at(25,23)=tilecont->at(28,23)=tilecont->at(23,24)=tilecont->at(27,24)=tilecont->at(0,25)=tilecont->at(26,25)=tilecont->at(28,25)=tilecont->at(3,27)=tilecont->at(5,27)=tilecont->at(16,27)=tilecont->at(27,27)=tiletype(128,0x0d,0x31,0x00,0x00);
        tilecont->at(20,0)=tilecont->at(24,0)=tilecont->at(7,1)=tilecont->at(7,6)=tilecont->at(31,7)=tilecont->at(1,27)=tilecont->at(10,27)=tiletype(157,0x0d,0x31,0x00,0x00);
        tilecont->at(22,0)=tilecont->at(0,4)=tilecont->at(5,6)=tilecont->at(31,15)=tilecont->at(24,26)=tilecont->at(23,27)=tiletype(110,0x0d,0x31,0x00,0x00);
        tilecont->at(7,0)=tilecont->at(10,5)=tilecont->at(0,7)=tilecont->at(29,10)=tilecont->at(31,16)=tilecont->at(0,23)=tilecont->at(26,24)=tilecont->at(10,26)=tilecont->at(12,26)=tilecont->at(2,27)=tilecont->at(26,27)=tiletype(143,0x0d,0x31,0x00,0x00);
        tilecont->at(1,0)=tilecont->at(10,0)=tilecont->at(29,0)=tilecont->at(4,6)=tilecont->at(31,10)=tilecont->at(30,12)=tilecont->at(30,14)=tilecont->at(0,22)=tilecont->at(29,24)=tilecont->at(30,25)=tilecont->at(31,26)=tilecont->at(8,27)=tiletype(158,0x0d,0x31,0x00,0x00);
        tilecont->at(11,5)=tilecont->at(11,6)=tilecont->at(11,7)=tilecont->at(11,8)=tilecont->at(11,9)=tilecont->at(11,10)=tilecont->at(11,11)=tilecont->at(11,12)=tilecont->at(23,12)=tilecont->at(11,13)=tilecont->at(23,13)=tilecont->at(11,14)=tilecont->at(23,14)=tilecont->at(11,15)=tilecont->at(23,15)=tilecont->at(11,16)=tilecont->at(23,16)=tilecont->at(11,17)=tilecont->at(11,18)=tilecont->at(11,19)=tilecont->at(11,20)=tilecont->at(11,21)=tilecont->at(11,22)=tilecont->at(11,23)=tilecont->at(11,24)=tiletype(625,0x0d,0x31,0x00,0x00);
        tilecont->at(3,0)=tilecont->at(1,2)=tilecont->at(10,2)=tilecont->at(0,3)=tilecont->at(10,3)=tilecont->at(31,3)=tilecont->at(31,4)=tilecont->at(0,8)=tilecont->at(31,9)=tilecont->at(0,14)=tilecont->at(0,15)=tilecont->at(29,16)=tilecont->at(31,17)=tilecont->at(30,18)=tilecont->at(30,19)=tilecont->at(0,24)=tilecont->at(23,26)=tilecont->at(0,27)=tilecont->at(12,27)=tilecont->at(13,27)=tilecont->at(14,27)=tilecont->at(28,27)=tiletype(112,0x0d,0x31,0x00,0x00);
        tilecont->at(11,2)=tilecont->at(23,2)=tilecont->at(2,3)=tilecont->at(23,3)=tilecont->at(23,4)=tilecont->at(23,5)=tilecont->at(23,6)=tilecont->at(23,7)=tilecont->at(1,8)=tilecont->at(23,8)=tilecont->at(1,9)=tilecont->at(1,10)=tilecont->at(1,11)=tilecont->at(1,12)=tilecont->at(1,13)=tilecont->at(1,14)=tilecont->at(1,15)=tilecont->at(1,16)=tilecont->at(1,17)=tilecont->at(1,18)=tilecont->at(1,19)=tilecont->at(23,19)=tilecont->at(1,20)=tilecont->at(23,20)=tilecont->at(1,21)=tilecont->at(1,22)=tilecont->at(1,23)=tilecont->at(1,24)=tiletype(633,0x0d,0x31,0x00,0x00);
        tilecont->at(0,0)=tilecont->at(9,0)=tilecont->at(1,1)=tilecont->at(29,12)=tilecont->at(30,13)=tilecont->at(29,15)=tilecont->at(28,22)=tilecont->at(28,24)=tilecont->at(30,24)=tilecont->at(5,26)=tilecont->at(14,26)=tilecont->at(16,26)=tilecont->at(7,27)=tilecont->at(20,27)=tiletype(142,0x0d,0x31,0x00,0x00);
        tilecont->at(31,0)=tilecont->at(31,21)=tilecont->at(27,23)=tilecont->at(27,26)=tiletype(67,0x0d,0x31,0x00,0x00);
        tilecont->at(11,1)=tilecont->at(23,1)=tilecont->at(2,2)=tilecont->at(1,7)=tilecont->at(23,18)=tiletype(630,0x0d,0x31,0x00,0x00);
        tilecont->at(12,1)=tilecont->at(13,1)=tilecont->at(14,1)=tilecont->at(15,1)=tilecont->at(16,1)=tilecont->at(17,1)=tilecont->at(18,1)=tilecont->at(19,1)=tilecont->at(20,1)=tilecont->at(21,1)=tilecont->at(24,1)=tilecont->at(25,1)=tilecont->at(26,1)=tilecont->at(27,1)=tilecont->at(28,1)=tilecont->at(29,1)=tilecont->at(3,2)=tilecont->at(4,2)=tilecont->at(5,2)=tilecont->at(6,2)=tilecont->at(7,2)=tilecont->at(8,2)=tilecont->at(2,7)=tilecont->at(3,7)=tilecont->at(4,7)=tilecont->at(5,7)=tilecont->at(6,7)=tilecont->at(7,7)=tilecont->at(8,7)=tilecont->at(9,7)=tilecont->at(24,18)=tilecont->at(25,18)=tilecont->at(26,18)=tilecont->at(27,18)=tilecont->at(28,18)=tiletype(631,0x0d,0x31,0x00,0x00);
        tilecont->at(22,1)=tilecont->at(30,1)=tilecont->at(9,2)=tilecont->at(10,7)=tilecont->at(29,18)=tiletype(632,0x0d,0x31,0x00,0x00);
        tilecont->at(23,17)=tilecont->at(11,25)=tiletype(627,0x0d,0x31,0x00,0x00);
        tilecont->at(22,4)=tilecont->at(28,11)=tiletype(624,0x0d,0x31,0x00,0x00);
        tilecont->at(22,3)=tilecont->at(9,4)=tilecont->at(30,9)=tilecont->at(29,21)=tilecont->at(10,25)=tiletype(637,0x0d,0x31,0x00,0x00);
        tilecont->at(6,0)=tilecont->at(12,0)=tilecont->at(17,0)=tilecont->at(28,0)=tilecont->at(30,0)=tilecont->at(4,1)=tilecont->at(0,5)=tilecont->at(8,5)=tilecont->at(2,6)=tilecont->at(23,10)=tilecont->at(27,10)=tilecont->at(30,10)=tilecont->at(31,11)=tilecont->at(31,12)=tilecont->at(29,14)=tilecont->at(30,16)=tilecont->at(31,19)=tilecont->at(30,21)=tilecont->at(24,22)=tilecont->at(26,23)=tilecont->at(30,23)=tilecont->at(24,24)=tilecont->at(29,25)=tilecont->at(2,26)=tilecont->at(9,26)=tilecont->at(11,26)=tilecont->at(19,26)=tilecont->at(26,26)=tilecont->at(29,26)=tilecont->at(25,27)=tilecont->at(29,27)=tiletype(126,0x0d,0x31,0x00,0x00);
        tilecont->at(22,2)=tilecont->at(30,2)=tilecont->at(9,3)=tilecont->at(30,3)=tilecont->at(30,4)=tilecont->at(30,5)=tilecont->at(30,6)=tilecont->at(30,7)=tilecont->at(10,8)=tilecont->at(30,8)=tilecont->at(10,9)=tilecont->at(10,10)=tilecont->at(10,11)=tilecont->at(10,12)=tilecont->at(10,13)=tilecont->at(10,14)=tilecont->at(10,15)=tilecont->at(10,16)=tilecont->at(10,17)=tilecont->at(10,18)=tilecont->at(10,19)=tilecont->at(29,19)=tilecont->at(10,20)=tilecont->at(29,20)=tilecont->at(10,21)=tilecont->at(10,22)=tilecont->at(10,23)=tilecont->at(10,24)=tiletype(634,0x0d,0x31,0x00,0x00);
        tilecont->at(11,3)=tilecont->at(2,4)=tilecont->at(23,9)=tilecont->at(23,21)=tilecont->at(1,25)=tiletype(635,0x0d,0x31,0x00,0x00);
        tilecont->at(12,3)=tilecont->at(13,3)=tilecont->at(14,3)=tilecont->at(15,3)=tilecont->at(16,3)=tilecont->at(17,3)=tilecont->at(18,3)=tilecont->at(19,3)=tilecont->at(20,3)=tilecont->at(21,3)=tilecont->at(3,4)=tilecont->at(4,4)=tilecont->at(5,4)=tilecont->at(6,4)=tilecont->at(7,4)=tilecont->at(8,4)=tilecont->at(24,9)=tilecont->at(25,9)=tilecont->at(26,9)=tilecont->at(27,9)=tilecont->at(28,9)=tilecont->at(29,9)=tilecont->at(24,21)=tilecont->at(25,21)=tilecont->at(26,21)=tilecont->at(27,21)=tilecont->at(28,21)=tilecont->at(2,25)=tilecont->at(3,25)=tilecont->at(4,25)=tilecont->at(5,25)=tilecont->at(6,25)=tilecont->at(7,25)=tilecont->at(8,25)=tilecont->at(9,25)=tiletype(636,0x0d,0x31,0x00,0x00);
    }
}

void RenderPlayField::render(const nes_ushort& _framecounter) {
    //renderimage(false); more optimization to be done
    if (glb::lineclearframecounter>0) {
        if (tetris) {
            if (getframemod4()==0&&!firstframeis4){
                renderimage(true);
            }
            else if (playfield_blink) {
                renderimage(false);
            }
        }
    }
    else if (tetris) {
        tetris=false;
        if (playfield_blink) renderimage(false);
    }
    levellineshandler.render();
    matrixhandler.render(level);
    piecehandler.render(_framecounter,level);
    scorehandler.render();
    if (glb::lineclearframecounter>0 && !firstframeis4 && getframemod4()==0) glb::lineclearframecounter--; //TODO pause itneraction
    firstframeis4=false;
}

void RenderPlayField::resetPlayField(const nes_uchar& _level){
    //todo next piece
    level=_level;
    tilecont->reset();
    renderimage(false);
    piecehandler = PieceContainer(tilecont, frameappearance);
    matrixhandler = MatrixContainer(tilecont, frameappearance);
    scorehandler = Score(tilecont, frameappearance, true);
    levellineshandler = LevelLines(tilecont, frameappearance, _level);
}

void RenderPlayField::init_assets() {
}
