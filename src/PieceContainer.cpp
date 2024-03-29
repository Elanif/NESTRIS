#include"PieceContainer.hpp"
#include<cstddef>
#include"ntris.hpp"
#include"random.hpp"
#include<cstdio>
#include<sstream>

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
void PieceContainer::inputManager(const ActiveInputs& _inputs, const PFMatrix& pfmatrix, const nes_uchar& _gravity, Audio& _audio) {
    dropped_event=false;
    bool piece_changed=false;
    //96 frames of ARE when the game starts
    if (init_delay>0) {
        init_delay--; //TODO before or after, frame discrepancy?
    }
    //Select hides the next piece
    if (_inputs.getPress(ntris::Select)) hidenextpiece=!hidenextpiece;
    //if sleepcounter>0 the piececontainer wont work for sleepcounter frames
    if (sleepcounter>0) {
        --sleepcounter;
        return;
    }
    //if the lines are being cleared, or the matrix is being shifted down, or ARE>0, the piececontainer sleeps
    if (ntris::lineclearframecounter>0||ntris::updatingmatrix>0||ntris::ARE>0) return; //TODO 1 frame error? updating>1?
    //downcounter is how many frames passed since gravity pushed the piece down
    ++downcounter;
    //MOVE
    if (!_inputs.getHold(ntris::Down)) {
        //resets how many frames down has been held
        holddowncounter=holddownpoints=0;
    }
    Piece temppiece=currentpiece;
    if (_inputs.getPress(ntris::Down)) {
        //holding down stops working if left or right are pressed
        downinterrupted=false;
        //forces the piece to fall down during the beginning of the game
        init_delay=0;
    }
    if (_inputs.getHold(ntris::Down)) {
        //redudant?
        init_delay=0;
        //interrupts holding down
        if (_inputs.getPress(ntris::Right)||_inputs.getPress(ntris::Left)||_inputs.getHold(ntris::Right)||_inputs.getHold(ntris::Left)) downinterrupted=true;
    }
    else {
        if (_inputs.getPress(ntris::Right)) {
            piece_changed=true;
            das=0;
            ++temppiece.x;
        }
        else if (_inputs.getPress(ntris::Left)) {
            piece_changed=true;
            das=0;
            --temppiece.x;
        }
        else if (_inputs.getHold(ntris::Right)) {
            ++das;
            if (das>=16) {
                piece_changed=true;
                ++temppiece.x;
                das=10;
            }
        }
        else if (_inputs.getHold(ntris::Left)) {
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
        if (piece_changed)
            _audio.playPieceMove(); //HAS TO BE PLAYED BEFORE PLAYPIECEROTATE BECAUSE IF A PIECE IS ROTATED AND MOVE IN THE SAME FRAME THIS SOUND DOESNT PLAY
        currentpiece=temppiece;
    }
    //ROTATE
    temppiece=currentpiece;
    piece_changed=false;
    if (_inputs.getPress(ntris::A)) {
        piece_changed=true;
        temppiece.rotation=(temppiece.rotation+1)%4;
    }
    else if (_inputs.getPress(ntris::B)) {
        piece_changed=true;
        temppiece.rotation=(temppiece.rotation-1)%4;
    }
    if (!collision(pfmatrix,temppiece)) {
        if (piece_changed)
            _audio.playPieceRotate(); //HAS TO BE PLAYED AFTER PLAYPIECEMOVE BECAUSE IF A PIECE IS ROTATED AND MOVE IN THE SAME FRAME THIS SOUND TAKES PRIORITY
        currentpiece=temppiece;
    }

    //ifnot holding down or have been holding down
    //DROP
    temppiece=currentpiece;
    if (_inputs.getHold(ntris::Down)&&!downinterrupted) {
        ++holddowncounter;
        if (holddowncounter>=3) {
            downcounter=0;
            ++holddownpoints;
            ++temppiece.y;
            holddowncounter-=2;
            if (collision(pfmatrix,temppiece)) {
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
            dropped_event=true;
            lastdroppedpiece=currentpiece;
        }
        else currentpiece=temppiece;
    }
    if (dropped_event)
        _audio.playPieceDrop();
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
    if (ntris::lineclearframecounter>0||ntris::updatingmatrix>0||ntris::ARE>0) return;
    else if (true){
        std::vector<std::pair<nes_uchar, nes_uchar> > piecepositions = currentpiece.getPos();
        for (std::vector<std::pair<nes_uchar, nes_uchar> >::size_type i=0; i<piecepositions.size(); ++i) {
            std::size_t _xx=piecepositions[i].first;
            std::size_t _yy=piecepositions[i].second;
            if (PFMatrix::visible(_xx,_yy)) {
                tilecont->at(ntris::playfieldx+_xx,ntris::playfieldy+_yy-2)=tiletype(_level,currentpiece.color());
            }
        }
    }
}

//TODO can keep unsigned char, unless we decide to add many pieces
void PieceContainer::spawnPiece() {
    //holddownpoints = 0; //in RenderPlayField for now
    spawned_event=true;
    currentpiece=nextpiece;

    if (current_drought <= 254) {
        if (currentpiece.piecetype != -1 && currentpiece.piecetype != 6) {
            current_drought++;
        }
        else if (currentpiece.piecetype == 6) {
            current_drought = 0;
        }
    }
    if (current_drought > max_drought)
        max_drought = current_drought;

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

nes_uchar PieceContainer::getDrought() const {
    return max_drought;
}
//TODO change in bigger game modes
nes_uchar PieceContainer::spawn_table[7]={0x02,0x07,0x08,0x0A,0x0B,0x0E,0x12};
