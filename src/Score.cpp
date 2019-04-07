#include "Score.h"
#include"TextWriter.h"
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
        glb::cm.update<std::string>("system","score[0]="s+ntris::to_string(score[0]));
        if (glb::lineclearframecounter>0) {
            for (std::size_t i=0; i<3; i++) {
                TextWriter::write_hex(topscores[0][i],tilecont,{glb::topscorex-i*2-1,glb::topscorey},2);
            }
            for (std::size_t i=0; i<3; i++) {
                TextWriter::write_hex(scoretemp[i],tilecont,{glb::scorex-i*2-1,glb::scorey},2);
            }
        }
        else {
            scoretemp=score;
            for (std::size_t i=0; i<3; i++) {
                TextWriter::write_hex(topscores[0][i],tilecont,{glb::topscorex-i*2-1,glb::topscorey},2);
            }
            for (std::size_t i=0; i<3; i++) {
                TextWriter::write_hex(score[i],tilecont,{glb::scorex-i*2-1,glb::scorey},2);
            }
        }
    }
}


unsigned int Score::getscore() {
    unsigned int result=(score[0]&0x0f)+((score[0]&0xf0)/0x0f)*10;
    result+=(score[1]&0x0f)*100+((score[1]&0xf0)/0x0f)*1000;
    result+=(score[2]&0x0f)*10000+((score[2]&0xf0)/0x0f)*100000;
    return result;
}

void Score::lastdigitcheck() {
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

void Score::softdrop(nes_uchar helddownpoints) { //helddownpoints used in calculations so value
    if (helddownpoints>=2) {
        --helddownpoints; //it should add heldpoints-1 to the score
        score[0]+=helddownpoints;
        //lowbytecheck();
        bytechecklowdigit(0,true);
        //lowbytecheck2();
        bytecheckhighdigit(0,true);
    }
}
void Score::lineclear(const nes_uchar& level, const nes_uchar& linescleared) {
    for (std::size_t i=0; i<=level; ++i) {
        score[0]+=pointsarray[linescleared*2];
        bytecheckhighdigit(0,false); //for some reason it doesn't do &0xf0
        score[1]+=pointsarray[linescleared*2+1];
        bytechecklowdigit(1,true);
        bytecheckhighdigit(1,true);
        bytechecklowdigit(2,true);
        lastdigitcheck();
    }
}
/*void Score::lowbytecheck() {
    unsigned char A=lowbyte&0x0f; //but the score is stored in 3 bytes and each of the numbers in the byte's hex rappresentation is used as a decimal unit
    if (A-0x0A>=0) { //checks if last digit overflows and adds 6 to "fix" it
        A=lowbyte;
        A+=0x06;
    }//example 12_dec+12_hex=1A_hex->1E+6=24, it works but it doesnt detect overflow all the time, example 9_dec+9_hex=[12]_hex, no overflow in the last digit so it thinks 9+9=12
    lowbyte = A;
}

void Score::lowbytecheck2() {
    unsigned char A=lowbyte%0xf0
    if (A-0xA0>=0) {
        A+=0x60;
        ++midbyte;
    }
    lowbyte=A;
}*/

nes_uchar Score::pointsarray[10]= {
    00, 00,
    0x40, 00,
    00, 0x01,
    00, 0x03,
    00, 0x12
};
