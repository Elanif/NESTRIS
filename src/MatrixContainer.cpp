#include "MatrixContainer.h"
#include"enums.h"
#include<vector>
#include<utility>
#include<cstdio>

MatrixContainer::MatrixContainer(TileContainer * _tilecont, const nes_ushort& _frameappearance)
    :Renderer(_tilecont, _frameappearance)
{
    hidecounter=sleepcounter=0;
    linescleared=0;
    for (int i=0; i<9;++i) {
        for (int j=10; j<22; ++j) {
            matrix(i,j)=1; //staring wtih a tetris
        }
    }
}

nes_uchar MatrixContainer::getBlock(const nes_uchar& x, const nes_uchar& y) {
    return matrix(x,y);
}

void MatrixContainer::render(const nes_uchar& _level) {
    /*if (hidecounter>0 || glb::lineclearframecounter>0 || glb::updatingmatrix) {
        auto tmphidecounter=hidecounter;
        auto tmplineclearframecounter=glb::lineclearframecounter;
        auto tmpupdatingmatrix=glb::updatingmatrix;
    }*/
    if (hidecounter>0) {
        --hidecounter;
        return;
    }
    if (glb::lineclearframecounter>0) {    //TODO how does pause interact with the clear animation?
        if (getframemod4()==0) {
            for (std::size_t i=0; i<linescleared; ++i ){
                std::size_t x=glb::lineclearframecounter-1;
                std::size_t y=linesclearedarray[i];
                matrix(x,y)=0; //was y-2
                //tilecont->at(x,y)=tiletype(_level,matrix(x,y));

                x=10-glb::lineclearframecounter;
                matrix(x,y)=0; //was y-2
                //tilecont->at(x,y)=tiletype(_level,matrix(x,y));
            }
        }
    }
    else if (glb::updatingmatrix>0) {
        std::size_t update_iter=5-glb::updatingmatrix;
        glb::cm.update_error<std::size_t>(glb::updatingmatrix);
        if (update_iter==0u) {
            for (std::size_t y=0; y<6; ++y) {
                for (std::size_t x=0; x<10; ++x) {
                    matrix(x,y)=0;
                }
            }
        }
        else {
            for (std::size_t y=0; y<4; ++y) {
                for (std::size_t x=0; x<10; ++x) {
                    matrix(x,y+update_iter*4)=newmatrix(x,y+update_iter*4-4);
                }
            }
        }
        /*
        for (nes_uchar y=2+(5-glb::updatingmatrix)*4; y<2+(5-(glb::updatingmatrix-1))*4; ++y)
            for (nes_uchar x=0; x<10; ++x)
                tilecont->at(glb::playfieldx+x,glb::playfieldy+y-2)=tiletype(_level,newmatrix(x,y));
                //BlockRenderer::block(renderSurface,newmatrix(x,y),_level,PLAYFIELDX+x*8,PLAYFIELDY+(y-2)*8);
        for (nes_uchar y=2+(5-(glb::updatingmatrix-1))*4; y<22; ++y)
            for (nes_uchar x=0; x<10; ++x)
                tilecont->at(glb::playfieldx+x,glb::playfieldy+y-2)=tiletype(_level,matrix(x,y));
                //BlockRenderer::block(renderSurface,matrix(x,y),_level,PLAYFIELDX+x*8,PLAYFIELDY+(y-2)*8);
        if (glb::updatingmatrix==1) matrix=newmatrix;
        return;
            */
    }
    for (nes_uchar x=0; x<10; ++x) { //TODO maybe optimize to render only new stuff around piece
        for (nes_uchar y=2; y<22; ++y) {
            tilecont->at(glb::playfieldx+x,glb::playfieldy+y-2)=tiletype(_level,matrix(x,y));
        }
    }
    //TODO
    //if (spawnpiececounter--<=0) renderPiece(piece);//postfix or prefix?
}

bool MatrixContainer::collision(const Piece& _piece) const{
    bool collision = false;
    std::vector<std::pair<nes_uchar, nes_uchar> > piecepositions = _piece.getPos();
    for (std::vector<std::pair<nes_uchar, nes_uchar> >::size_type i=0; i<piecepositions.size(); ++i) {
        std::size_t _xx=piecepositions[i].first;
        std::size_t _yy=piecepositions[i].second;
        if (!PFMatrix::inbounds(_xx,_yy)) {
            collision = true;
            break;
        }
        if (matrix(_xx,_yy)) {
            collision = true;
            break;
        }
    }
    return collision;
}

nes_uchar MatrixContainer::lockpiece(const Piece& _piece, const nes_ushort&  _framecounter) {
    std::vector<std::pair<nes_uchar, nes_uchar> > piecepositions = _piece.getPos();
    for (std::vector<std::pair<nes_uchar, nes_uchar> >::size_type i=0; i<piecepositions.size(); ++i) {
        std::size_t _xx=piecepositions[i].first;
        std::size_t _yy=piecepositions[i].second;
        matrix(_xx,_yy)=_piece.color();
    }
    char _tempclearedlines=clearlines();
    return _tempclearedlines;
}


nes_uchar MatrixContainer::clearlines() {
    bool whichlines[22];
    //TODO if row 2 is cleared also row 21 is cleared (bug)
    std::size_t lowestline=0;
    linescleared=0;
    for (std::size_t row=0; row<22; ++row) {
        bool clearedline=true;
        for (std::size_t column=0; column<10; ++column) {
            if (matrix(column,row)==0) {
                clearedline=false;
                column=10;
            }
        }
        whichlines[row]=clearedline;
        if(whichlines[row]) {
            lowestline=row;
            //blinkscreencounter=19;
            linesclearedarray[linescleared++]=row;
        }
    }
    //TODO clearanimation
    for (std::size_t i=0; i<10; ++i) {
        for (std::size_t j=0; j<22; ++j) {
            newmatrix(i,j)=matrix(i,j);
        }
    }
    for (std::size_t i=lowestline, rowcounter=lowestline; i>=1&&rowcounter>=1; --i,--rowcounter) { //20 because glb::playfield isn't saved over 20
        while(rowcounter>=1&&whichlines[rowcounter]) --rowcounter;
        for (std::size_t j=0; j<10; ++j) {
            newmatrix(j,i)=newmatrix(j,rowcounter);
        }
    }
    return linescleared;
}

void MatrixContainer::reset(){
    for (std::size_t i=0; i<10; ++i) {
        for (std::size_t j=0; j< 22;++j) {
            matrix(i,j)=0;
        }
    }
}
