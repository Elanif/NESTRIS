#include"Score.hpp"
#include"TextWriter.hpp"
#include<sstream>
#include<iomanip>

Score::Score(TileContainer *_tilecont, const nes_ushort& _frameappearance)
:Renderer(_tilecont, _frameappearance),
maxout(true)
{
    nes_uchar scorearray[3]={0x00,0x00,0x01};
    topscores[0]=ScoreContainer(scorearray);
    topscores[1]=ScoreContainer(5000u);
    topscores[2]=ScoreContainer(1000u);
}

Score::Score(TileContainer *_tilecont, const nes_ushort& _frameappearance, const bool& _maxout)
:Score(_tilecont,_frameappearance)
{
    maxout=false;
}

void Score::render() {
    if (hidecounter>0) {
        --hidecounter;
        return;
    }
    else {
        using namespace std::string_literals;
        if (ntris::lineclearframecounter>0) {
            TextWriter::write(topscores[0].getScoreString(),tilecont,{ntris::topscorex,ntris::topscorey}); //these 2 lines
            TextWriter::write(scoretemp.getScoreString(),tilecont,{ntris::scorex,ntris::scorey}); //are useless right now
        }
        else {
            scoretemp=score;
            TextWriter::write(topscores[0].getScoreString(),tilecont,{ntris::topscorex,ntris::topscorey});
            TextWriter::write(score.getScoreString(),tilecont,{ntris::scorex,ntris::scorey});
        }
    }
}


ScoreContainer Score::getScore() {
    return score;
    /*
    unsigned int result=(score[0]&0x0f)+((score[0]&0xf0)/0x0f)*10;
    result+=(score[1]&0x0f)*100+((score[1]&0xf0)/0x0f)*1000;
    result+=(score[2]&0x0f)*10000+((score[2]&0xf0)/0x0f)*100000;
    return result;*/
}

std::string ScoreContainer::getScoreString() {
    std::stringstream _tmpstream;
    for (std::size_t i=3; i-->0;)
        _tmpstream<<std::setfill('0')<<std::setw(2)<<std::hex<<(unsigned int)score[i];
    return _tmpstream.str();
}

void Score::lastdigitcheck() { //TODO make ScoreContainer specific
    if (maxout) {
        nes_uchar A=score[2]&0xF0;
        if (A-0xA0>=0) score[0]=score[1]=score[2]=0x99;
    }
}

void Score::bytechecklowdigit(const std::size_t& byte, const bool& andop) {
    nes_uchar A=score[byte];
    if (andop) A=A&0x0f;
    if (A-0x0A>=0) score[byte]+=0x06;
}

void Score::bytecheckhighdigit(const std::size_t& byte, const bool& andop) {
    nes_uchar A=score[byte];
    if (andop) A=A&0xf0;
    if (A-0xA0>=0) {
        score[byte]+=0x60;
        ++score[byte+1];
    }
}

void Score::softdrop(nes_uchar helddownpoints) {
    if (helddownpoints>=2) {
        --helddownpoints;
        score[0]+=helddownpoints;
        bytechecklowdigit(0,true);
        bytecheckhighdigit(0,true);
    }
}
void Score::lineclear(const nes_uchar& level, const nes_uchar& linescleared) {
    for (std::size_t i=0; i<=level; ++i) {
        score[0]+=pointsarray[linescleared*2];
        bytecheckhighdigit(0,false); //for some reason it doesn't do &0xf0 in the original assembly code
        score[1]+=pointsarray[linescleared*2+1];
        bytechecklowdigit(1,true);
        bytecheckhighdigit(1,true);
        bytechecklowdigit(2,true);
        lastdigitcheck();
    }
}

nes_uchar Score::pointsarray[10]= {
    00, 00,
    0x40, 00,
    00, 0x01,
    00, 0x03,
    00, 0x12
};
