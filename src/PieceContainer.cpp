#include "PieceContainer.h"
#include<stddef.h>
#include"enums.h"
#include"ConsoleManager.h"
#include"random.h"
#include<cstdio>
#include<sstream>
#include"Sound.hpp"

PieceContainer::PieceContainer(TileContainer * _tilecont, const nes_ushort& _frameappearance)
:Renderer(_tilecont, _frameappearance),
downinterrupted(false),
das(0),
downcounter(0),
spawncount(0),
hidenextpiece(false)
{
    spawnPiece();
    spawnPiece();
    hidecounter=sleepcounter=0;
}
bool PieceContainer::collision(const PFMatrix& _pfmatrix, const Piece& _piece) {
    bool collision = false;
    std::vector<std::pair<nes_uchar, nes_uchar> > piecepositions = _piece.getPos();
    for (std::vector<std::pair<nes_uchar, nes_uchar> >::size_type i=0; i<piecepositions.size(); ++i) {
        std::size_t _xx=piecepositions[i].first;
        std::size_t _yy=piecepositions[i].second;
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

//TODO change nes_uchar in slower game modes
void PieceContainer::inputManager(const ActiveInputs& _inputs, const PFMatrix& pfmatrix, const nes_uchar& _gravity) {
    dropped_event=false;
    bool piece_changed=false;
    if (init_delay>0) {
        init_delay--; //TODO before or after, frame discrepancy?
    }
    if (_inputs.getPress(glb::Select)) hidenextpiece=!hidenextpiece;
    if (sleepcounter>0) {
        --sleepcounter;
        return;
    }
    if (glb::lineclearframecounter>0||glb::updatingmatrix>0||glb::ARE>0) return; //TODO 1 frame error? updating>1?
    ++downcounter;
    //MOVE
    if (!_inputs.getHold(glb::Down)) {
        holddowncounter=holddownpoints=0;
    }
    Piece temppiece=currentpiece;
    if (_inputs.getPress(glb::Down)) {
        downinterrupted=false;
        init_delay=0;
    }
    if (_inputs.getHold(glb::Down)) {
        init_delay=0;
        if (_inputs.getPress(glb::Right)||_inputs.getPress(glb::Left)||_inputs.getHold(glb::Right)||_inputs.getHold(glb::Left)) downinterrupted=true;
    }
    else {
        if (_inputs.getPress(glb::Right)) {
            piece_changed=true;
            das=0;
            ++temppiece.x;
        }
        else if (_inputs.getPress(glb::Left)) {
            piece_changed=true;
            das=0;
            --temppiece.x;
        }
        else if (_inputs.getHold(glb::Right)) {
            ++das;
            if (das>=16) {
                piece_changed=true;
                ++temppiece.x;
                das=10;
            }
        }
        else if (_inputs.getHold(glb::Left)) {
            ++das;
            if (das>=16) {
                piece_changed=true;
                --temppiece.x;
                das=10;
            }
        }
    }
    if (collision(pfmatrix,temppiece)) das=16;
    else {
        if (piece_changed) Sound::play(Sound::move_piece);
        currentpiece=temppiece;
    }
    //ROTglb::ATE
    temppiece=currentpiece;
    piece_changed=false;
    if (_inputs.getPress(glb::A)) {
        piece_changed=true;
        temppiece.rotation=(temppiece.rotation+1)%4;
    }
    else if (_inputs.getPress(glb::B)) {
        piece_changed=true;
        temppiece.rotation=(temppiece.rotation-1)%4;
    }
    if (!collision(pfmatrix,temppiece)) {
        currentpiece=temppiece;
        if (piece_changed) Sound::play(Sound::rotate_piece);
    }

    //ifnot holding down or have been holding down
    //DROP
    temppiece=currentpiece;
    if (_inputs.getHold(glb::Down)&&!downinterrupted) {
        ++holddowncounter;
        if (holddowncounter>=3) {
            downcounter=0;
            ++holddownpoints;
            ++temppiece.y;
            holddowncounter-=2;
            if (collision(pfmatrix,temppiece)) {
                Sound::play(Sound::drop_piece);
                dropped_event=true;
                lastdroppedpiece=currentpiece;
            }
            else currentpiece=temppiece;
        }
    }
    if (downcounter>=_gravity &&init_delay==0) { //TODO does it wait 96+gravity[frame] for the first piece or only 96?
        ++temppiece.y;
        downcounter=0;
        if (collision(pfmatrix,temppiece)) {
            Sound::play(Sound::drop_piece);
            dropped_event=true;
            lastdroppedpiece=currentpiece;
        }
        else currentpiece=temppiece;
    }
    //printf("das=%d\n",das);
}

const Piece& PieceContainer::getPiece() const{
    return currentpiece;
}

//TODO change both nes_ushort and nes_uchar with bigger game modes
void PieceContainer::rendernextpiece(const nes_uchar& _level) {
    if (!hidenextpiece) {
        std::vector<std::pair<std::size_t, std::size_t> >  nextpiecepos=nextpiece.nextpiecePos();
        for (const auto& i:nextpiecepos) {
            tilecont->renderExtra(i.first,i.second,tiletype(_level,nextpiece.color()),0.6);
        }
    }
}

//TODO change both nes_ushort and nes_uchar with bigger game modes
void PieceContainer::render(const nes_ushort& _framecounter, const nes_uchar& _level) { //TODO the first piece renders a little bit late
    if (hidecounter>0) {
        --hidecounter;
        return;
    }
    rendernextpiece(_level);
    if (glb::lineclearframecounter>0||glb::updatingmatrix>0||glb::ARE>0) return;
    else if (true){
        std::vector<std::pair<nes_uchar, nes_uchar> > piecepositions = currentpiece.getPos();
        for (std::vector<std::pair<nes_uchar, nes_uchar> >::size_type i=0; i<piecepositions.size(); ++i) {
            std::size_t _xx=piecepositions[i].first;
            std::size_t _yy=piecepositions[i].second;
            if (PFMatrix::visible(_xx,_yy)) {
                tilecont->at(glb::playfieldx+_xx,glb::playfieldy+_yy-2)=tiletype(_level,currentpiece.color());
            }
        }
    }
}

//TODO can keep unsigned char, unless we decide to add many pieces
void PieceContainer::spawnPiece() {
    spawned_event=true;
    currentpiece=nextpiece;
    nes_uchar spawnID=spawn_table[nextpiece.piecetype]; //creates a piece next to nextpiece
    ++spawncount;
    nes_uchar index=random::prng()>>8;
    index+=spawncount;
    index&=7;
    nes_uchar newSpawnID;
    if (index!=7) {
        newSpawnID = spawn_table[index];
        if (newSpawnID == spawnID) {
            random::prng();
            index=random::prng()>>8;
            index&=7;
            index+=spawnID;
            index%=7;
            newSpawnID = spawn_table[index];
        }
    }
    else {
        random::prng();
        index=random::prng()>>8;
        index&=7;
        index+=spawnID;
        index%=7;
        newSpawnID = spawn_table[index];
    }
    spawnID = newSpawnID;
    nes_uchar realID=0;
    for (std::size_t i=0; i<7;++i) {
        if (spawn_table[i]==spawnID) {
            realID=i;
            break;
        }
    }
    nextpiece.piecetype=realID;
    downcounter=holddowncounter=0;
}
bool PieceContainer::gameOver(const PFMatrix& pfmatrix) {
    if (collision(pfmatrix, currentpiece))return true;
    return false;
}

void PieceContainer::lockpiece() {
    //printf("Lockpiece\n");
    //spawnPiece();  //do it later
    currentpiece=Piece();
    downinterrupted=true; //TODO where to put this
}

//TODO change in bigger game modes
nes_uchar PieceContainer::spawn_table[7]={0x02,0x07,0x08,0x0A,0x0B,0x0E,0x12};
