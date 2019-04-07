#include "Score.h"

#ifdef c11date
Score::Score():Score(true){
};
#else
Score::Score():maxout(true){
    score[0]=score[1]=score[2]=0;
    pointsarray[0]=00; pointsarray[1]=00;
    pointsarray[2]=40; pointsarray[3]=00;
    pointsarray[4]=00; pointsarray[5]=01;
    pointsarray[6]=00; pointsarray[7]=03;
    pointsarray[8]=00; pointsarray[9]=12;
}
#endif

Score::Score(bool _maxout):maxout(_maxout){
    score[0]=score[1]=score[2]=0;
    pointsarray[0]=00; pointsarray[1]=00;
    pointsarray[2]=40; pointsarray[3]=00;
    pointsarray[4]=00; pointsarray[5]=01;
    pointsarray[6]=00; pointsarray[7]=03;
    pointsarray[8]=00; pointsarray[9]=12;
}

void Score::sofdrop(unsigned char helddownpoints) {
    if (helddownpoints>=2) {
        unsigned char A;
        --helddownpoints; //it should add heldpoints-1 to the score
        score[0]+=helddownpoints;
        //lowbytecheck();
        bytechecklowdigit(0,true);
        //lowbytecheck2();
        bytecheckhighdigit(0,true);
    }
}
void Score::lineclear(unsigned char level, unsigned char linescleared) {
    for (size_t i=0; i<=level; ++i) {
        score[0]+=pointsarray[linescleared*2];
        bytecheckhighdigit(0,false); //for some reason it doesn't do &0xf0
        score[1]+=pointsarray[linescleared*2+1];
        bytechecklowdigit(1,true);
        bytecheckhighdigit(1,true);
        bytechecklowdigit(2,true);
        lastdigitcheck();
    }
}

unsigned int getscore() {
    unsigned int result=score[0]&0x0f+score[0]&0xf0/0x0f*10;
    result+=score[1]&0x0f*100+score[1]&0xf0/0x0f*1000;
    result+=score[2]&0x0f*10000+score[2]&0xf0/0x0f*100000;
    return result;
}

void Score::lastdigitcheck() {
    if (maxout) {
        unsigned char A=score[2]&0xF0;
        if (A-0xA0>=0) score[0]=score[1]=score[2]=0x99;
    }
}

void Score::bytechecklowdigit(size_t byte, bool andop) {
    unsigned char A=score[byte];
    if (andop) A=A&0x0f;
    if (A-0x0A>=0) score[byte]+=0x06;
}

void Score::bytecheckhighdigit(size_t byte, bool andop) {
    unsigned char A=score[byte];
    if (andop) A=A&0xf0;
    if (A-0xA0>=0) {
        score[byte]+=0x60;
        ++score[byte+1];
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
