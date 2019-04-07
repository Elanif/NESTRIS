#include "Score.h"
#include"TextWriter.h"
Score::Score(TileContainer *_tilecont, const nes_ushort& _frameappearance)
:Renderer(_tilecont, _frameappearance),
maxout(true)
{
    pointsarray[0]=00; pointsarray[1]=00;
    pointsarray[2]=40; pointsarray[3]=00;
    pointsarray[4]=00; pointsarray[5]=01;
    pointsarray[6]=00; pointsarray[7]=03;
    pointsarray[8]=00; pointsarray[9]=12;
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
        for (std::size_t i=0; i<3; i++) {
            TextWriter::write_hex(score[i],tilecont,{glb::topscorex-i*2,glb::topscorey});
            tilecont->at(glb::topscorex-i*2,glb::topscorey)=tiletype(1+(score[i]&0x0f),0x0d,0x30,0x00,0x00);
            tilecont->at(glb::topscorex-i*2-1,glb::topscorey)=tiletype(1+((score[i]>>4)&0x0f),0x0d,0x30,0x00,0x00);
        }
        for (std::size_t i=0; i<3; i++) {
            tilecont->at(glb::scorex-i*2,glb::scorey)=tiletype(1+(score[i]&0x0f),0x0d,0x30,0x00,0x00);
            tilecont->at(glb::scorex-i*2-1,glb::scorey)=tiletype(1+((score[i]>>4)&0x0f),0x0d,0x30,0x00,0x00);
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
