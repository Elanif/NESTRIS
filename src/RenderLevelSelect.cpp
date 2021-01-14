#include"RenderLevelSelect.hpp"
#include<cstdio>
RenderLevelSelect::RenderLevelSelect(TileContainer *_tilecont, const std::size_t& _frameappearance, const std::size_t& _currentlevel):
Renderer(_tilecont,_frameappearance),
currentlevel(_currentlevel)
{
    blink=0;
    lastrenderedlevel=0;
}
RenderLevelSelect::RenderLevelSelect()
{

}
int RenderLevelSelect::updateLevelSelect(const ActiveInputs& _input, Audio & _audio) {
//todo manage priorities
    unsigned int tempblink=blink;
    blink=0;
    if (_input.getPress(ntris::Start)) {
        _audio.playMenuSelectShort();
        if (_input.getHold(ntris::A)&&!_input.getPress(ntris::A)) return currentlevel+10;//A has to be held 1 frame earlier
        return currentlevel;
    }
    else 
    if (_input.getPress(ntris::Left)) {
        _audio.playMenuMove();
        if (currentlevel>0) --currentlevel;
    }
    else if (_input.getPress(ntris::Right)) {
        _audio.playMenuMove();
        if (currentlevel<9) ++currentlevel;
    }
    else if (_input.getPress(ntris::Up)) {
        _audio.playMenuMove();
        if (currentlevel>4) currentlevel-=5;
    }
    else if (_input.getPress(ntris::Down)) {
        _audio.playMenuMove();
        if (currentlevel<5) currentlevel+=5;
    }
    else blink=tempblink;
    return -1; //TODO
}

void RenderLevelSelect::renderLevelSelect(const bool& _reload) {

    if (_reload) reload();
    const std::size_t levelselecttilex=6+(lastrenderedlevel%5)*2;
    const std::size_t levelselecttiley=9+(lastrenderedlevel/5)*2;

	//Yellow blinking square under level selection
    if (blink++%4!=0) {
        for (std::size_t i=0; i<2; ++i) {
            for (std::size_t j=0; j<2; ++j) {
                tilecont->renderExtra(levelselecttilex*8+4+i*8,levelselecttiley*8+4+j*8,tiletype(87,0x27,0x0d,0x0d,0x0d),0.1);
            }
        }
    }
    lastrenderedlevel=currentlevel;

}

void RenderLevelSelect::reload() {
    tilecont->reset();
    //green stuff in the middle
    tilecont->at(15,10)=tiletype(5,0x1d,0x16,0x00,0x00);
    tilecont->at(11,10)=tiletype(3,0x1d,0x16,0x00,0x00);
    tilecont->at(16,13)=tiletype(48,0x1d,0x00,0x2a,0x00);
    tilecont->at(7,11)=tilecont->at(9,11)=tilecont->at(11,11)=tilecont->at(13,11)=tilecont->at(15,11)=tilecont->at(7,13)=tilecont->at(9,13)=tilecont->at(11,13)=tilecont->at(13,13)=tilecont->at(15,13)=tiletype(47,0x1d,0x00,0x2a,0x00);
    tilecont->at(8,11)=tilecont->at(10,11)=tilecont->at(12,11)=tilecont->at(14,11)=tiletype(78,0x1d,0x00,0x2a,0x00);
    tilecont->at(13,10)=tiletype(4,0x1d,0x16,0x00,0x00);
    tilecont->at(7,10)=tiletype(1,0x1d,0x16,0x00,0x00);
    tilecont->at(9,10)=tiletype(2,0x1d,0x16,0x00,0x00);
    tilecont->at(8,9)=tilecont->at(10,9)=tilecont->at(12,9)=tilecont->at(14,9)=tiletype(81,0x1d,0x00,0x2a,0x00);
    tilecont->at(6,10)=tilecont->at(8,10)=tilecont->at(10,10)=tilecont->at(12,10)=tilecont->at(14,10)=tilecont->at(6,12)=tilecont->at(8,12)=tilecont->at(10,12)=tilecont->at(12,12)=tilecont->at(14,12)=tiletype(44,0x1d,0x00,0x2a,0x00);
    tilecont->at(6,11)=tiletype(77,0x1d,0x00,0x2a,0x00);
    tilecont->at(6,9)=tiletype(41,0x1d,0x00,0x2a,0x00);
    tilecont->at(7,9)=tilecont->at(9,9)=tilecont->at(11,9)=tilecont->at(13,9)=tilecont->at(15,9)=tiletype(42,0x1d,0x00,0x2a,0x00);
    tilecont->at(16,9)=tiletype(43,0x1d,0x00,0x2a,0x00);
    tilecont->at(16,10)=tilecont->at(16,12)=tiletype(45,0x1d,0x00,0x2a,0x00);
    tilecont->at(16,11)=tiletype(79,0x1d,0x00,0x2a,0x00);
    tilecont->at(7,12)=tiletype(6,0x1d,0x16,0x00,0x00);
    tilecont->at(15,12)=tiletype(10,0x1d,0x16,0x00,0x00);
    tilecont->at(9,12)=tiletype(7,0x1d,0x16,0x00,0x00);
    tilecont->at(11,12)=tiletype(8,0x1d,0x16,0x00,0x00);
    tilecont->at(13,12)=tiletype(9,0x1d,0x16,0x00,0x00);
    tilecont->at(6,13)=tiletype(46,0x1d,0x00,0x2a,0x00);
    tilecont->at(8,13)=tilecont->at(10,13)=tilecont->at(12,13)=tilecont->at(14,13)=tiletype(82,0x1d,0x00,0x2a,0x00);

    //everything else
    tilecont->at(5,24)=tiletype(46,0x0d,0x00,0x38,0x00);
    tilecont->at(1,24)=tiletype(65,0x0d,0x30,0x00,0x00);
    tilecont->at(26,24)=tiletype(48,0x0d,0x00,0x38,0x00);
    tilecont->at(6,18)=tilecont->at(7,18)=tilecont->at(8,18)=tilecont->at(9,18)=tilecont->at(10,18)=tilecont->at(11,18)=tilecont->at(12,18)=tilecont->at(13,18)=tilecont->at(14,18)=tilecont->at(15,18)=tilecont->at(16,18)=tilecont->at(17,18)=tilecont->at(18,18)=tilecont->at(19,18)=tilecont->at(20,18)=tilecont->at(21,18)=tilecont->at(22,18)=tilecont->at(23,18)=tilecont->at(24,18)=tilecont->at(25,18)=tilecont->at(6,24)=tilecont->at(7,24)=tilecont->at(8,24)=tilecont->at(9,24)=tilecont->at(10,24)=tilecont->at(11,24)=tilecont->at(12,24)=tilecont->at(13,24)=tilecont->at(14,24)=tilecont->at(15,24)=tilecont->at(16,24)=tilecont->at(17,24)=tilecont->at(18,24)=tilecont->at(19,24)=tilecont->at(20,24)=tilecont->at(21,24)=tilecont->at(22,24)=tilecont->at(23,24)=tilecont->at(24,24)=tilecont->at(25,24)=tiletype(47,0x0d,0x00,0x38,0x00);
    tilecont->at(18,17)=tiletype(25,0x0d,0x30,0x00,0x00);
    tilecont->at(17,17)=tiletype(13,0x0d,0x30,0x00,0x00);
    tilecont->at(16,17)=tiletype(29,0x0d,0x30,0x00,0x00);
    tilecont->at(19,17)=tiletype(28,0x0d,0x30,0x00,0x00);
    tilecont->at(31,1)=tilecont->at(30,5)=tilecont->at(1,8)=tilecont->at(30,16)=tilecont->at(1,21)=tilecont->at(31,25)=tilecont->at(17,27)=tiletype(96,0x0d,0x30,0x00,0x00);
    tilecont->at(11,2)=tilecont->at(3,3)=tilecont->at(3,4)=tilecont->at(3,5)=tilecont->at(3,6)=tilecont->at(3,7)=tilecont->at(3,8)=tilecont->at(3,9)=tilecont->at(3,10)=tilecont->at(3,11)=tilecont->at(3,12)=tilecont->at(3,13)=tilecont->at(3,14)=tilecont->at(3,15)=tilecont->at(3,16)=tilecont->at(3,17)=tilecont->at(3,18)=tilecont->at(3,19)=tilecont->at(3,20)=tilecont->at(3,21)=tilecont->at(3,22)=tilecont->at(3,23)=tilecont->at(3,24)=tilecont->at(3,25)=tiletype(60,0x0d,0x00,0x16,0x00);
    tilecont->at(8,7)=tiletype(72,0x0d,0x00,0x16,0x00);
    tilecont->at(30,1)=tilecont->at(29,5)=tilecont->at(0,8)=tilecont->at(29,16)=tilecont->at(0,21)=tilecont->at(30,25)=tilecont->at(16,27)=tiletype(95,0x0d,0x30,0x00,0x00);
    tilecont->at(26,18)=tiletype(79,0x0d,0x00,0x38,0x00);
    tilecont->at(20,3)=tilecont->at(28,26)=tiletype(64,0x0d,0x30,0x16,0x00);
    tilecont->at(11,7)=tilecont->at(24,17)=tiletype(32,0x0d,0x30,0x00,0x00);
    tilecont->at(29,1)=tilecont->at(1,2)=tilecont->at(31,4)=tilecont->at(2,7)=tilecont->at(31,7)=tilecont->at(31,9)=tilecont->at(2,10)=tilecont->at(2,12)=tilecont->at(31,15)=tilecont->at(2,17)=tilecont->at(31,18)=tilecont->at(31,20)=tilecont->at(7,27)=tilecont->at(26,27)=tilecont->at(28,27)=tiletype(143,0x0d,0x30,0x00,0x00);
    tilecont->at(6,16)=tilecont->at(7,16)=tilecont->at(8,16)=tilecont->at(9,16)=tilecont->at(10,16)=tilecont->at(11,16)=tilecont->at(12,16)=tilecont->at(13,16)=tilecont->at(14,16)=tilecont->at(15,16)=tilecont->at(16,16)=tilecont->at(17,16)=tilecont->at(18,16)=tilecont->at(19,16)=tilecont->at(20,16)=tilecont->at(21,16)=tilecont->at(22,16)=tilecont->at(23,16)=tilecont->at(24,16)=tilecont->at(25,16)=tiletype(42,0x0d,0x00,0x38,0x00);
    tilecont->at(26,16)=tiletype(43,0x0d,0x00,0x38,0x00);
    tilecont->at(20,1)=tilecont->at(28,2)=tiletype(59,0x0d,0x30,0x16,0x00);
    tilecont->at(2,2)=tilecont->at(0,3)=tilecont->at(29,8)=tilecont->at(30,8)=tilecont->at(30,10)=tilecont->at(0,11)=tilecont->at(1,11)=tilecont->at(29,12)=tilecont->at(1,13)=tilecont->at(0,15)=tilecont->at(0,17)=tilecont->at(1,18)=tilecont->at(29,19)=tilecont->at(30,19)=tilecont->at(2,20)=tilecont->at(30,21)=tilecont->at(0,23)=tilecont->at(1,23)=tilecont->at(29,23)=tilecont->at(2,25)=tilecont->at(29,25)=tilecont->at(0,27)=tilecont->at(4,27)=tilecont->at(21,27)=tilecont->at(31,27)=tiletype(97,0x0d,0x30,0x00,0x00);
    tilecont->at(8,8)=tiletype(74,0x0d,0x00,0x16,0x00);
    tilecont->at(11,1)=tilecont->at(3,2)=tiletype(57,0x0d,0x30,0x16,0x00);
    tilecont->at(14,6)=tiletype(71,0x0d,0x00,0x16,0x00);
    tilecont->at(5,18)=tiletype(77,0x0d,0x00,0x38,0x00);
    tilecont->at(5,17)=tilecont->at(5,19)=tilecont->at(5,20)=tilecont->at(5,21)=tilecont->at(5,22)=tilecont->at(5,23)=tiletype(44,0x0d,0x00,0x38,0x00);
    tilecont->at(4,0)=tilecont->at(7,0)=tilecont->at(10,0)=tilecont->at(1,3)=tilecont->at(2,3)=tilecont->at(0,4)=tilecont->at(2,4)=tilecont->at(31,5)=tilecont->at(2,8)=tilecont->at(29,9)=tilecont->at(29,10)=tilecont->at(30,11)=tilecont->at(0,12)=tilecont->at(0,13)=tilecont->at(1,14)=tilecont->at(31,16)=tilecont->at(2,18)=tilecont->at(1,19)=tilecont->at(29,20)=tilecont->at(2,21)=tilecont->at(29,21)=tilecont->at(2,22)=tilecont->at(30,22)=tilecont->at(0,24)=tilecont->at(0,25)=tilecont->at(29,26)=tiletype(112,0x0d,0x30,0x00,0x00);
    tilecont->at(19,0)=tiletype(66,0x0d,0x00,0x00,0x00);
    tilecont->at(1,0)=tilecont->at(3,0)=tilecont->at(31,2)=tilecont->at(30,6)=tilecont->at(1,9)=tilecont->at(30,17)=tilecont->at(1,22)=tilecont->at(31,26)=tiletype(111,0x0d,0x00,0x00,0x00);
    tilecont->at(8,0)=tilecont->at(21,0)=tilecont->at(26,0)=tilecont->at(29,3)=tilecont->at(0,6)=tilecont->at(29,14)=tilecont->at(1,26)=tilecont->at(2,27)=tilecont->at(12,27)=tilecont->at(14,27)=tilecont->at(18,27)=tiletype(142,0x0d,0x30,0x00,0x00);
    tilecont->at(6,0)=tilecont->at(0,18)=tiletype(67,0x0d,0x30,0x00,0x00);
    tilecont->at(15,2)=tiletype(30,0x0d,0x30,0x00,0x00);
    tilecont->at(14,2)=tiletype(37,0x0d,0x30,0x00,0x00);
    tilecont->at(31,0)=tilecont->at(6,1)=tilecont->at(7,1)=tilecont->at(8,1)=tilecont->at(21,1)=tilecont->at(26,1)=tilecont->at(29,2)=tilecont->at(1,4)=tilecont->at(29,4)=tilecont->at(2,5)=tilecont->at(31,6)=tilecont->at(0,7)=tilecont->at(31,8)=tilecont->at(2,9)=tilecont->at(31,10)=tilecont->at(2,11)=tilecont->at(29,11)=tilecont->at(2,13)=tilecont->at(0,14)=tilecont->at(29,15)=tilecont->at(31,17)=tilecont->at(0,19)=tilecont->at(2,19)=tilecont->at(31,19)=tilecont->at(31,21)=tilecont->at(29,22)=tilecont->at(2,23)=tilecont->at(1,25)=tilecont->at(0,26)=tilecont->at(1,27)=tiletype(128,0x0d,0x30,0x00,0x00);
    tilecont->at(0,0)=tilecont->at(2,0)=tilecont->at(30,2)=tilecont->at(29,6)=tilecont->at(0,9)=tilecont->at(29,17)=tilecont->at(0,22)=tilecont->at(30,26)=tiletype(110,0x0d,0x30,0x00,0x00);
    tilecont->at(11,0)=tilecont->at(14,0)=tilecont->at(16,0)=tilecont->at(28,0)=tilecont->at(4,1)=tilecont->at(0,5)=tilecont->at(30,9)=tilecont->at(1,12)=tilecont->at(30,12)=tilecont->at(29,13)=tilecont->at(1,15)=tilecont->at(0,16)=tilecont->at(30,20)=tilecont->at(30,23)=tilecont->at(29,24)=tilecont->at(29,27)=tiletype(157,0x0d,0x30,0x00,0x00);
    tilecont->at(11,3)=tilecont->at(3,26)=tiletype(62,0x0d,0x30,0x16,0x00);
    tilecont->at(2,24)=tiletype(113,0x0d,0x30,0x00,0x00);
    tilecont->at(12,0)=tilecont->at(22,0)=tilecont->at(29,0)=tilecont->at(1,1)=tilecont->at(2,1)=tilecont->at(23,1)=tilecont->at(24,1)=tilecont->at(28,1)=tilecont->at(30,3)=tilecont->at(1,6)=tilecont->at(30,7)=tilecont->at(1,10)=tilecont->at(30,13)=tilecont->at(30,14)=tilecont->at(1,16)=tilecont->at(30,18)=tilecont->at(30,24)=tilecont->at(6,27)=tilecont->at(9,27)=tilecont->at(10,27)=tilecont->at(19,27)=tiletype(127,0x0d,0x30,0x00,0x00);
    tilecont->at(13,0)=tilecont->at(15,0)=tilecont->at(17,0)=tilecont->at(20,0)=tilecont->at(23,0)=tilecont->at(30,0)=tilecont->at(3,1)=tilecont->at(5,1)=tilecont->at(25,1)=tilecont->at(31,3)=tilecont->at(1,5)=tilecont->at(2,6)=tilecont->at(31,12)=tilecont->at(31,13)=tilecont->at(31,14)=tilecont->at(2,15)=tilecont->at(2,16)=tilecont->at(31,23)=tilecont->at(31,24)=tilecont->at(3,27)=tilecont->at(11,27)=tilecont->at(13,27)=tilecont->at(15,27)=tilecont->at(20,27)=tilecont->at(24,27)=tilecont->at(30,27)=tiletype(129,0x0d,0x30,0x00,0x00);
    tilecont->at(9,0)=tilecont->at(25,0)=tilecont->at(27,0)=tilecont->at(10,1)=tilecont->at(1,20)=tilecont->at(2,26)=tiletype(158,0x0d,0x30,0x00,0x00);
    tilecont->at(18,2)=tilecont->at(10,7)=tilecont->at(12,7)=tilecont->at(13,17)=tilecont->at(20,17)=tiletype(15,0x0d,0x30,0x00,0x00);
    tilecont->at(26,17)=tilecont->at(26,19)=tilecont->at(26,20)=tilecont->at(26,21)=tilecont->at(26,22)=tilecont->at(26,23)=tiletype(45,0x0d,0x00,0x38,0x00);
    tilecont->at(20,2)=tilecont->at(28,3)=tilecont->at(28,4)=tilecont->at(28,5)=tilecont->at(28,6)=tilecont->at(28,7)=tilecont->at(28,8)=tilecont->at(28,9)=tilecont->at(28,10)=tilecont->at(28,11)=tilecont->at(28,12)=tilecont->at(28,13)=tilecont->at(28,14)=tilecont->at(28,15)=tilecont->at(28,16)=tilecont->at(28,17)=tilecont->at(28,18)=tilecont->at(28,19)=tilecont->at(28,20)=tilecont->at(28,21)=tilecont->at(28,22)=tilecont->at(28,23)=tilecont->at(28,24)=tilecont->at(28,25)=tiletype(61,0x0d,0x00,0x16,0x00);
    tilecont->at(13,2)=tilecont->at(11,17)=tiletype(11,0x0d,0x30,0x00,0x00);
    tilecont->at(12,3)=tilecont->at(13,3)=tilecont->at(14,3)=tilecont->at(15,3)=tilecont->at(16,3)=tilecont->at(17,3)=tilecont->at(18,3)=tilecont->at(19,3)=tilecont->at(4,26)=tilecont->at(5,26)=tilecont->at(6,26)=tilecont->at(7,26)=tilecont->at(8,26)=tilecont->at(9,26)=tilecont->at(10,26)=tilecont->at(11,26)=tilecont->at(12,26)=tilecont->at(13,26)=tilecont->at(14,26)=tilecont->at(15,26)=tilecont->at(16,26)=tilecont->at(17,26)=tilecont->at(18,26)=tilecont->at(19,26)=tilecont->at(20,26)=tilecont->at(21,26)=tilecont->at(22,26)=tilecont->at(23,26)=tilecont->at(24,26)=tilecont->at(25,26)=tilecont->at(26,26)=tilecont->at(27,26)=tiletype(63,0x0d,0x00,0x16,0x00);
    tilecont->at(8,6)=tiletype(69,0x0d,0x00,0x16,0x00);
    tilecont->at(16,2)=tiletype(35,0x0d,0x30,0x00,0x00);
    tilecont->at(9,6)=tilecont->at(10,6)=tilecont->at(11,6)=tilecont->at(12,6)=tilecont->at(13,6)=tiletype(70,0x0d,0x00,0x16,0x00);
    tilecont->at(23,27)=tiletype(68,0x0d,0x30,0x00,0x00);
    tilecont->at(14,7)=tiletype(73,0x0d,0x00,0x16,0x00);
    tilecont->at(9,8)=tilecont->at(10,8)=tilecont->at(11,8)=tilecont->at(12,8)=tilecont->at(13,8)=tiletype(75,0x0d,0x00,0x16,0x00);
    tilecont->at(5,0)=tilecont->at(18,0)=tilecont->at(24,0)=tilecont->at(0,1)=tilecont->at(9,1)=tilecont->at(22,1)=tilecont->at(27,1)=tilecont->at(0,2)=tilecont->at(30,4)=tilecont->at(1,7)=tilecont->at(29,7)=tilecont->at(0,10)=tilecont->at(31,11)=tilecont->at(2,14)=tilecont->at(30,15)=tilecont->at(1,17)=tilecont->at(29,18)=tilecont->at(0,20)=tilecont->at(31,22)=tilecont->at(5,27)=tilecont->at(8,27)=tilecont->at(22,27)=tilecont->at(25,27)=tilecont->at(27,27)=tiletype(126,0x0d,0x30,0x00,0x00);
    tilecont->at(14,8)=tiletype(76,0x0d,0x00,0x16,0x00);
    tilecont->at(12,1)=tilecont->at(13,1)=tilecont->at(14,1)=tilecont->at(15,1)=tilecont->at(16,1)=tilecont->at(17,1)=tilecont->at(18,1)=tilecont->at(19,1)=tilecont->at(4,2)=tilecont->at(5,2)=tilecont->at(6,2)=tilecont->at(7,2)=tilecont->at(8,2)=tilecont->at(9,2)=tilecont->at(10,2)=tilecont->at(21,2)=tilecont->at(22,2)=tilecont->at(23,2)=tilecont->at(24,2)=tilecont->at(25,2)=tilecont->at(26,2)=tilecont->at(27,2)=tiletype(58,0x0d,0x00,0x16,0x00);
    tilecont->at(9,7)=tilecont->at(13,7)=tilecont->at(23,17)=tiletype(22,0x0d,0x30,0x00,0x00);
    tilecont->at(5,16)=tiletype(41,0x0d,0x00,0x38,0x00);
    tilecont->at(17,2)=tiletype(26,0x0d,0x30,0x00,0x00);
    tilecont->at(10,17)=tiletype(24,0x0d,0x30,0x00,0x00);
    tilecont->at(12,17)=tiletype(23,0x0d,0x30,0x00,0x00);
}
