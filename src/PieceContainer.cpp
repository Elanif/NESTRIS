#include "PieceContainer.h"
#include<stddef.h>
#include"enums.h"
#include"random.h"
#include"BlockRenderer.h"
#include<cstdio>
#include"SquareRenderer.h"

PieceContainer::PieceContainer(SDL_Window * _window, const nes_ushort& _frameappearance)
    :Renderer(_window, _frameappearance), downinterrupted(false), das(0), downcounter(0), spawncount(0)
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
    if (sleepcounter>0) {
        --sleepcounter;
        return;
    }
    if (glb::lineclearframecounter>0) return;
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

void PieceContainer::deletepiece() {
    for (size_t i=0; i<lastrenderedpos.size(); ++i) { //TODO size:type
        BlockRenderer::block(renderSurface, 0, 0, lastrenderedpos[i].first,lastrenderedpos[i].second);
    }
}

void PieceContainer::deletenextpiece() {
    SquareRenderer::square(renderSurface,0,0,0,255,191,103,33,34);

}

void PieceContainer::rendernextpiece(const nes_uchar& _level) {
    std::vector<std::pair<nes_uchar, nes_uchar> > piecepositions = nextpiece.nextpiecePos();
    for (std::vector<std::pair<nes_uchar, nes_uchar> >::size_type i=0; i<piecepositions.size(); ++i) {
        size_t _xx=piecepositions[i].first;
        size_t _yy=piecepositions[i].second;
        BlockRenderer::block(renderSurface, nextpiece.color(), _level, _xx,_yy);
    }

}

void PieceContainer::render(const nes_ushort& _framecounter, const nes_uchar& _level) {
    //printf("piecetype=%d, color=%d, blocks=%d, x=%d, y=%d, x1=%d, y1=%d\n",currentpiece.piecetype,currentpiece.color(),currentpiece.getPos().size(),currentpiece.x,currentpiece.y,currentpiece.getPos()[0].first,currentpiece.getPos()[0].second);
    if (hidecounter>0) {
        printf("hidecounter=%d\n",hidecounter);
        --hidecounter;
        return;
    }
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
                _xx=_xx*8+PLAYFIELDX;
                _yy=(_yy-2)*8+PLAYFIELDY;
                BlockRenderer::block(renderSurface, currentpiece.color(), _level, _xx,_yy);
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

