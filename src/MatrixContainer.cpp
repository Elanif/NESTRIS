#include "MatrixContainer.h"
#include"enums.h"
#include<vector>
#include<utility>
#include<cstdio>
#include"BlockRenderer.h"
#include"SDL_Image.h"

MatrixContainer::MatrixContainer(SDL_Window * _window, const nes_ushort& _frameappearance)
    :Renderer(_window, _frameappearance)
{
    updatingmatrix=0;
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }
    hidecounter=sleepcounter=0;
    blinkscreencounter=linescleared=0;
}

nes_uchar MatrixContainer::getBlock(const nes_uchar& x, const nes_uchar& y) {
    return matrix(x,y);
}

void MatrixContainer::render(const nes_uchar& _level) {
    if (hidecounter>0) {
        --hidecounter;
        return;
    }
    if (blinkscreencounter>0) {    //TODO how does pause interact with the clear animation?
        int _ttest;
        scanf("%d",&_ttest);
        for (size_t i=0; i<linescleared; ++i ){
                printf("blinkscreencounter= %d, linesclearedarray[%d]=%d\n",blinkscreencounter,i,linesclearedarray[i]);
            matrix(blinkscreencounter/4,linesclearedarray[i])=0;
            matrix(9-blinkscreencounter/4,linesclearedarray[i])=0;
        }
        --blinkscreencounter;
        return;
    }
    if (updatingmatrix>0) {
        for (nes_uchar y=2+(5-updatingmatrix)*4; y<2+(5-(updatingmatrix-1))*4; ++y)
            for (nes_uchar x=0; x<10; ++x)
                BlockRenderer::block(renderSurface,newmatrix(x,y),_level,PLAYFIELDX+x*8,PLAYFIELDY+(y-2)*8);
        for (nes_uchar y=2+(5-(updatingmatrix-1))*4; y<22; ++y)
            for (nes_uchar x=0; x<10; ++x)
                BlockRenderer::block(renderSurface,matrix(x,y),_level,PLAYFIELDX+x*8,PLAYFIELDY+(y-2)*8);
        --updatingmatrix;
        if (updatingmatrix==0) matrix=newmatrix;
    }
    else {
        for (nes_uchar x=0; x<10; ++x) { //TODO maybe optimize to render only new stuff around piece
            for (nes_uchar y=2; y<22; ++y) {
                BlockRenderer::block(renderSurface,matrix(x,y),_level,PLAYFIELDX+x*8,PLAYFIELDY+(y-2)*8);
            }
        }
    }
    //TODO
    //if (spawnpiececounter--<=0) renderPiece(piece);//postfix or prefix?
}

bool MatrixContainer::collision(const Piece& _piece) const{
    bool collision = false;
    std::vector<std::pair<nes_uchar, nes_uchar> > piecepositions = _piece.getPos();
    for (std::vector<std::pair<nes_uchar, nes_uchar> >::size_type i=0; i<piecepositions.size(); ++i) {
        size_t _xx=piecepositions[i].first;
        size_t _yy=piecepositions[i].second;
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
        size_t _xx=piecepositions[i].first;
        size_t _yy=piecepositions[i].second;
        matrix(_xx,_yy)=_piece.color;
    }
    char _tempclearedlines=clearlines();
    if (_tempclearedlines) {
        blinkscreencounter=((_framecounter+20)/5)*5-_framecounter+1;
        updatingmatrix=5;

    }
    return _tempclearedlines;
}


nes_uchar MatrixContainer::clearlines() {
    bool whichlines[22];
    //TODO if row 2 is cleared also row 21 is cleared (bug)
    size_t lowestline=0;
    linescleared=0;
    for (size_t row=0; row<22; ++row) {
        bool clearedline=true;
        for (size_t column=0; column<10; ++column) {
            if (matrix(column,row)==0) {
                clearedline=false;
                column=10;
            }
        }
        if(whichlines[row]=clearedline) { //no == intentional
            lowestline=row;
            //blinkscreencounter=19;
            linesclearedarray[linescleared++]=row;
        }
    }
    //TODO clearanimation
    for (size_t i=0; i<10; ++i) {
        for (size_t j=0; j<22; ++j) {
            newmatrix(i,j)=matrix(i,j);
        }
    }
    for (size_t i=lowestline, rowcounter=lowestline; i>=1&&rowcounter>=1; --i,--rowcounter) { //20 because playfield isn't saved over 20
        while(rowcounter>=1&&whichlines[rowcounter]) --rowcounter;
        for (size_t j=0; j<10; ++j) {
            newmatrix(j,i)=newmatrix(j,rowcounter);
        }
    }
    return linescleared;
}

/*void RenderPlayField::resetPlayField(const size_t& _level){
    //todo next piece
    level=_level;
    spawnCount=0;
    for (size_t i=0; i<10; ++i) {
        for (size_t j=0; j< 22;++j) {
            matrix(i,j)=0;
        }
    }
}*/

/*void RenderPlayField::renderPlayField(const unsigned long long& framecounter) {
    //TODO blinking clear delay
    if (blinkscreencounter-->0) {//postfix or prefix?
        for (size_t i=0; i<linescleared; ++i ){
            matrix(blinkscreencounter/4,linesclearedarray[i)]=0;
            matrix(9-blinkscreencounter/4,linesclearedarray[i)]=0;
        }
        if (blinkscreencounter==0) {
            for (size_t i=0; i<10; ++i)
                for (size_t j=0; j<22; ++j)
                    matrix(i,j)=newmatrix(i,j);
        }
    }
    for (size_t x=0; x<10; ++x) { //TODO maybe optimize to render only new stuff around piece
        for (size_t y=3 ; y<22; ++y) {
            if ((blinkscreencounter%4>1)&&matrix(x,y)==0) //TODO wht if it just became 0
                BlockRenderer::block(renderSurface,matrix(x,y),10, x*8,y*8);
            BlockRenderer::block(renderSurface,matrix(x,y),level, x*8,y*8);
        }
    }
    //TODO
    if (spawnpiececounter--<=0) renderPiece(piece);//postfix or prefix?
}*/
