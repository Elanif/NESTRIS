#include "PieceContainer.h"
#include<stddef.h>
#include"enums.h"
#include"random.h"
#include<cstdio>

PieceContainer::PieceContainer(TileContainer * _tilecont, const nes_ushort& _frameappearance)
:Renderer(_tilecont, _frameappearance),
downinterrupted(false),
das(0),
downcounter(0),
spawncount(0),
hidenextpiece(false)
{
    spawnPiece(0);
    spawnPiece(0);
    hidecounter=sleepcounter=0;
}
bool collision(const PFMatrix& _pfmatrix, const Piece& _piece) {
    bool collision = false;
    std::vector<std::pair<nes_uchar, nes_uchar> > piecepositions = _piece.getPos();
    for (std::vector<std::pair<nes_uchar, nes_uchar> >::size_type i=0; i<piecepositions.size(); ++i) {
        size_t _xx=piecepositions[i].first;
        size_t _yy=piecepositions[i].second;
        if (!PFMatrix::inbounds(_xx,_yy)) {
            collision = true;
            break;
        }
        if (_pfmatrix(_xx,_yy)) {
            collision = true;
            break;
        }
    }
    return collision;
}

void PieceContainer::inputManager(const ActiveInputs& _inputs, const PFMatrix& pfmatrix, const nes_uchar& _gravity) {
    dropped=false;
    if (_inputs.getPress(glb::SELECT)) hidenextpiece=!hidenextpiece;
    if (sleepcounter>0) {
        --sleepcounter;
        return;
    }
    if (glb::lineclearframecounter>0||glb::updatingmatrix>0) return; //TODO 1 frame error? updating>1?
    ++downcounter;
    //MOVE
    Piece temppiece=currentpiece;
    if (_inputs.getPress(glb::DOWN)) downinterrupted=false;
    if (_inputs.getHold(glb::DOWN)) {
        if (_inputs.getPress(glb::RIGHT)||_inputs.getPress(glb::LEFT)||_inputs.getHold(glb::RIGHT)||_inputs.getHold(glb::LEFT)) downinterrupted=true;
    }
    else {
        holddowncounter=holddownpoints=0;
        if (_inputs.getPress(glb::RIGHT)) {
            das=0;
            ++temppiece.x;
        }
        else if (_inputs.getPress(glb::LEFT)) {
            das=0;
            --temppiece.x;
        }
        else if (_inputs.getHold(glb::RIGHT)) {
            ++das;
            if (das>=16) {
                ++temppiece.x;
                das=10;
            }
        }
        else if (_inputs.getHold(glb::LEFT)) {
            ++das;
            if (das>=16) {
                --temppiece.x;
                das=10;
            }
        }
    }
    if (collision(pfmatrix,temppiece)) das=16;
    else currentpiece=temppiece;
    //ROTglb::ATE
    temppiece=currentpiece;
    if (_inputs.getPress(glb::A)) {
        temppiece.rotation=(temppiece.rotation-1)%4;
    }
    else if (_inputs.getPress(glb::B)) {
        temppiece.rotation=(temppiece.rotation-1)%4;
    }
    if (!collision(pfmatrix,temppiece)) currentpiece=temppiece;

    //ifnot holding down or have been holding down
    //DROP
    bool alreadymoveddown=false;
    temppiece=currentpiece;
    if (_inputs.getHold(glb::DOWN)&&!downinterrupted) {
        ++holddowncounter;
        ++holddownpoints;
        if (holddowncounter>=3) {
            alreadymoveddown=true;
            ++temppiece.y;
            holddowncounter-=2;
            if (collision(pfmatrix,temppiece)) {
                dropped=true;
                lastdroppedpiece=currentpiece;
            }
            else currentpiece=temppiece;
        }
    }
    if (downcounter>=_gravity &&!alreadymoveddown) {
        ++temppiece.y;
        downcounter=0;
        if (collision(pfmatrix,temppiece)) {
            dropped=true;
            lastdroppedpiece=currentpiece;
        }
        else currentpiece=temppiece;
    }
    printf("das=%d\n",das);
}

const Piece& PieceContainer::getPiece() const{
    return currentpiece;
}

void PieceContainer::deletenextpiece() {

}

void PieceContainer::rendernextpiece(const nes_uchar& _level) {
    for (size_t x=glb::nextpiecex; x<glb::nextpiecex+4;++x)
        for (size_t y=glb::nextpiecey; y<glb::nextpiecey+4;++y)
            tilecont->at(x,y)=tiletype(0,0);
    if (!hidenextpiece) {
        std::vector<std::pair<nes_uchar, nes_uchar> > piecepositions = nextpiece.nextpiecePos();
        for (std::vector<std::pair<nes_uchar, nes_uchar> >::size_type i=0; i<piecepositions.size(); ++i) { //todo with correct stuff
            size_t _xx=piecepositions[i].first;
            size_t _yy=piecepositions[i].second;
            tilecont->at(glb::nextpiecex+_xx,glb::nextpiecey+_yy)=tiletype(_level,nextpiece.color());
        }
    }
}

void PieceContainer::render(const nes_ushort& _framecounter, const nes_uchar& _level) {
    //printf("piecetype=%d, color=%d, blocks=%d, x=%d, y=%d, x1=%d, y1=%d\n",currentpiece.piecetype,currentpiece.color(),currentpiece.getPos().size(),currentpiece.x,currentpiece.y,currentpiece.getPos()[0].first,currentpiece.getPos()[0].second);
    if (hidecounter>0) {
        printf("hidecounter=%d\n",hidecounter);
        --hidecounter;
        return;
    }
    if (glb::lineclearframecounter>0) return;
    lastrenderedpos.clear();
    if (hidecountercurrentpiece>0) {
        --hidecountercurrentpiece;
    }
    else {
        std::vector<std::pair<nes_uchar, nes_uchar> > piecepositions = currentpiece.getPos();
        for (std::vector<std::pair<nes_uchar, nes_uchar> >::size_type i=0; i<piecepositions.size(); ++i) {
            size_t _xx=piecepositions[i].first;
            size_t _yy=piecepositions[i].second;
            if (PFMatrix::visible(_xx,_yy)) {
                tilecont->at(glb::playfieldx+_xx,glb::playfieldy+_yy-2)=tiletype(_level,currentpiece.color());
                //printf("currentcolor=%d, nextcolor=%d, level=%d\n",currentpiece.color(),nextpiece.color(),_level);
                lastrenderedpos.push_back(std::make_pair(_xx,_yy));
            }
        }
    }
    deletenextpiece();
    rendernextpiece(_level);
}


void PieceContainer::spawnPiece(const nes_uchar& _spawndelay) { //TODO its not using the spawnID table which causes weird weights
    currentpiece=nextpiece;
    nes_uchar spawnID=nextpiece.piecetype; //creates a piece next to nextpiece
    ++spawncount;
    nes_uchar index=random::prng()>>8;
    index+=spawncount;
    index&=7;
    nes_uchar newSpawnID;
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
    downcounter=holddowncounter=0;
}
void PieceContainer::hidecurrentpiece(const nes_uchar& _hidecurrent) {
    hidecountercurrentpiece=_hidecurrent;
}
void PieceContainer::lockpiece(const nes_uchar& _lockheight) {
    printf("Lockpiece\n");
    nes_uchar _spawndelay=10+((_lockheight+3)/5)*2; //TODO fidn true formula
    spawnPiece(_spawndelay);
    downinterrupted=true; //TODO where to put this
}

