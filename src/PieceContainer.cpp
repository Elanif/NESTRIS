#include "PieceContainer.h"
#include<stddef.h>
#include"enums.h"
#include"random.h"
#include"BlockRenderer.h"
#include<cstdio>

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
    ++downcounter;
    //MOVE
    Piece temppiece=currentpiece;
    if (_inputs.getPress(DOWN)) downinterrupted=false;
    if (_inputs.getHold(DOWN)) {
        if (_inputs.getPress(RIGHT)||_inputs.getPress(LEFT)||_inputs.getHold(RIGHT)||_inputs.getHold(LEFT)) downinterrupted=true;
    }
    else {
        holddowncounter=holddownpoints=0;
        if (_inputs.getPress(RIGHT)) {
            das=0;
            ++temppiece.x;
        }
        else if (_inputs.getPress(LEFT)) {
            das=0;
            --temppiece.x;
        }
        else if (_inputs.getHold(RIGHT)) {
            ++das;
            if (das>=16) {
                ++temppiece.x;
                das=10;
            }
        }
        else if (_inputs.getHold(LEFT)) {
            ++das;
            if (das>=16) {
                --temppiece.x;
                das=10;
            }
        }
    }
    if (collision(pfmatrix,temppiece)) das=16;
    else currentpiece=temppiece;
    //ROTATE
    temppiece=currentpiece;
    if (_inputs.getPress(A)) {
        temppiece.rotation=(temppiece.rotation-1)%4;
    }
    else if (_inputs.getPress(B)) {
        temppiece.rotation=(temppiece.rotation-1)%4;
    }
    if (!collision(pfmatrix,temppiece)) currentpiece=temppiece;

    //ifnot holding down or have been holding down
    //DROP
    bool alreadymoveddown=false;
    temppiece=currentpiece;
    if (_inputs.getHold(DOWN)&&!downinterrupted) {
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
    for (nes_uchar x=0; x<4; ++x)
        for (nes_uchar y=0; y<3; ++y)
            BlockRenderer::block(renderSurface, 0, 0, NEXTPIECEX+x*8-8, NEXTPIECEY+y*8);

}

void PieceContainer::rendernextpiece(const nes_uchar& _level) {
    for (size_t i=0; i<4; ++i) {
        nes_uchar _xx=Piece::rotationmatrix[nextpiece.piecetype*4+nextpiece.rotation%4][i][0];
        nes_uchar _yy=Piece::rotationmatrix[nextpiece.piecetype*4+nextpiece.rotation%4][i][1];
        _xx=_xx*8+NEXTPIECEX+8;
        _yy=_yy*8+NEXTPIECEY+8;
        BlockRenderer::block(renderSurface, nextpiece.color, _level, _xx,_yy);
    }

}

void PieceContainer::render(const nes_ushort& _framecounter, const nes_uchar& _level) {
    if (hidecounter>0) {
        printf("hidecounter=%d\n",hidecounter);
        --hidecounter;
        return;
    }
    lastrenderedpos.clear();
    for (size_t i=0; i<4; ++i) {
        nes_uchar _xx=currentpiece.x+Piece::rotationmatrix[currentpiece.piecetype*4+currentpiece.rotation%4][i][0];
        nes_uchar _yy=currentpiece.y+Piece::rotationmatrix[currentpiece.piecetype*4+currentpiece.rotation%4][i][1];
        if (PFMatrix::visible(_xx,_yy)) {
            _xx=_xx*8+PLAYFIELDX;
            _yy=(_yy-2)*8+PLAYFIELDY;
            BlockRenderer::block(renderSurface, currentpiece.color, _level, _xx,_yy);
            lastrenderedpos.push_back(std::make_pair(_xx,_yy));
        }
    }
    for (size_t i=0; i<4; ++i) {
        nes_uchar _xx=currentpiece.x+Piece::rotationmatrix[currentpiece.piecetype*4+currentpiece.rotation%4][i][0];
        nes_uchar _yy=currentpiece.y+Piece::rotationmatrix[currentpiece.piecetype*4+currentpiece.rotation%4][i][1];
        _xx=_xx*8+PLAYFIELDX;
        _yy=(_yy-2)*8+PLAYFIELDY;
        BlockRenderer::block(renderSurface, currentpiece.color, _level, _xx,_yy);
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

void PieceContainer::lockpiece(const nes_uchar& _lockheight) {
    printf("Lockpiece\n");
    nes_uchar _spawndelay=10+((_lockheight+3)/5)*2; //TODO fidn true formula
    spawnPiece(_spawndelay);
    downinterrupted=true; //TODO where to put this
    sleep(_spawndelay);
    hide(_spawndelay-1);
}

