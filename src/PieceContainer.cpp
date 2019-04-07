#include "PieceContainer.h"
#include<stddef.h>
#include"enums.h"
#include"random.h"
#include"BlockRenderer.h"
#include<cstdio>

PieceContainer::PieceContainer(SDL_Window * _window, const size_t& _frameappearance)
    :Renderer(_window, _frameappearance), downinterrupted(false), das(0), downcounter(0), spawncount(0)
{
    spawnPiece(0);
    spawnPiece(0);
}

Piece PieceContainer::tryDrop(const ActiveInputs& _inputs, const unsigned char& _gravity){
    //TODO holding down to be checked here?
    printf("gravity=%d\n",_gravity);
    temppiece=currentpiece;
    tempdowncounter=++downcounter;
    tempholddowncounter=holddowncounter;
    if (!downinterrupted) {
        if (_inputs.getHold(DOWN)) {
            ++tempholddowncounter;
            if (tempholddowncounter>=3) {
                ++temppiece.y;
                tempholddowncounter-=2;
            }
            else if (tempdowncounter>=_gravity) {
                ++temppiece.y;
                tempdowncounter=0;
            }
            //if holddowncounter>=3 > move down, holddowncounte-=2;
            //else if downcounter>=gravity > move down, downcounter=0;
        }
    }
    return temppiece;
}

Piece PieceContainer::tryMove(const ActiveInputs& _inputs){
    //TODO holding down to be checked here?
    temppiece=currentpiece;
    tempdas=das;
    if (_inputs.getHold(DOWN)) return temppiece;
    if (_inputs.getPress(RIGHT)) {
    printf("pressed %s\n","RIGHT");
        tempdas=0;
        temppiece.x++;
        return temppiece;
    }
    if (_inputs.getPress(LEFT)) {
    printf("pressed %s\n","LEFT");
        tempdas=0;
        temppiece.x--;
        return temppiece;
    }
    if (_inputs.getHold(RIGHT)) {
        tempdas=das+1;
        if (tempdas>=16) {
            temppiece.x++;
            tempdas=10;
        }
        return temppiece;
    }
    if (_inputs.getHold(LEFT)) {
        tempdas=das+1;
        if (tempdas>=16) {
            temppiece.x--;
            tempdas=10;
        }
        return temppiece;
    }
    return temppiece;
}
Piece PieceContainer::tryRotate(const ActiveInputs& _inputs){
    //TODO holding down to be checked here?
    temppiece=currentpiece;
    if (_inputs.getPress(B)) {
        temppiece.rotation=(temppiece.rotation-1)%4;
        return temppiece;
    }
    if (_inputs.getPress(A)) {
        temppiece.rotation=(temppiece.rotation-1)%4;
        return temppiece;
    }
    return temppiece;
}

void PieceContainer::doDrop() {
    printf("dropping\n");
    currentpiece=temppiece;
    downcounter=tempdowncounter;
    holddowncounter=tempholddowncounter;
    printf("downcounter=%d, holddowncounter=%d\n",downcounter,holddowncounter);
}

void PieceContainer::doRotate(const bool& _collision) {
    if (!_collision) currentpiece=temppiece;
}

void PieceContainer::doMove(const bool& _collision) {
    if (_collision) {
            printf("wall charge\n");
        das=16;
    }
    else {
        printf("moving\n");
        currentpiece=temppiece;
        das=tempdas;
    }
}


const Piece& PieceContainer::getPiece() const{
    return currentpiece;
}

void PieceContainer::deletepiece() {
    for (size_t i=0; i<lastrenderedpos.size(); ++i) {
        BlockRenderer::block(renderSurface, 0, 0, lastrenderedpos[i].first*8,lastrenderedpos[i].second*8);
    }
}

void PieceContainer::render(const size_t& _framecounter, const char& _level) {
    printf(" x=%d, y=%d, type=%d, nexttype=%d\n",currentpiece.x,currentpiece.y,currentpiece.piecetype,nextpiece.piecetype);
    lastrenderedpos.clear();
    for (size_t i=0; i<4; ++i) {
        size_t _xx=currentpiece.x+Piece::rotationmatrix[currentpiece.piecetype*4+currentpiece.rotation][i][0];
        size_t _yy=currentpiece.y+Piece::rotationmatrix[currentpiece.piecetype*4+currentpiece.rotation][i][1];
        if (_xx>0&&_xx<10&&_yy>0&&_yy<20) {
            BlockRenderer::block(renderSurface, currentpiece.color, _level, _xx*8,_yy*8);
            lastrenderedpos.push_back(std::make_pair(currentpiece.x+Piece::rotationmatrix[currentpiece.piecetype*4+currentpiece.rotation][i][0],currentpiece.y+Piece::rotationmatrix[currentpiece.piecetype*4+currentpiece.rotation][i][1]));
        }
    }
}


void PieceContainer::spawnPiece(const char& _spawndelay) {
    currentpiece=nextpiece;
    char spawnID=nextpiece.piecetype; //creates a piece next to nextpiece
    ++spawncount;
    char index=random::prng()>>8;
    index+=spawncount;
    index&=7;
    char newSpawnID;
    if (index!=7) {
        newSpawnID = index;
        if (newSpawnID == spawnID) {
            random::prng();
            index=random::prng()>>8;
            index&=7;
            index+=spawncount;
            index%=7;
            newSpawnID = index;
        }
    }
    else {
        random::prng();
        index=random::prng()>>8;
        index&=7;
        index+=spawncount;
        index%=7;
        newSpawnID = index;
    }
    spawnID = newSpawnID;
    //printf("spawnid=%d\n",spawnID);
    nextpiece.piecetype=spawnID;
    nextpiece.piecetype=0;
    das=downcounter=holddowncounter=0;
    spawnpiececounter=_spawndelay; //10~18 TODO
}

void PieceContainer::lockpiece(const char& _lockheight) {
    char _spawndelay=22-_lockheight/4; //TODO
    //spawnPiece();
}

Piece::Piece()
{
    x=5;
    y=3;
    piecetype=-1;
    rotation=0;
    color=2; //TODO
}

std::vector<std::pair<char,char> > Piece::getPos() const {
    std::vector<std::pair<char,char> > result;
    if (piecetype<0 || piecetype>6) return result;
    for (size_t i=0; i< 4; ++i) {
        result.push_back(std::make_pair(rotationmatrix[piecetype*4+rotation][i][0]+x,rotationmatrix[piecetype*4+rotation][i][1]+y));
    }
    return result;
}

char Piece::rotationmatrix[28][4][2]={
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
