#include "TileContainer.h"
#include<cstdio>

unsigned char tiletype::colors[10][4]={
    {0x0D ,0x30 ,0x21 ,0x12},
    {0x0D ,0x30 ,0x29 ,0x1A},
    {0x0D ,0x30 ,0x24 ,0x14},
    {0x0D ,0x30 ,0x2A ,0x12},
    {0x0D ,0x30 ,0x2B ,0x15},
    {0x0D ,0x30 ,0x22 ,0x2B},
    {0x0D ,0x30 ,0x00 ,0x16},
    {0x0D ,0x30 ,0x05 ,0x13},
    {0x0D ,0x30 ,0x16 ,0x12},
    {0x0D ,0x30 ,0x27 ,0x16}
};

const tiletype& TileContainer::at(const size_t& x, const size_t& y) const {
    if (x<0||x>=width||y<0||y>=height) {
        printf("out of bounds in tilecontainer %d %d width height= %d %d\n", x, y,width,height);
        return oob_error;
    }
    else return tilegrid[y*width+x];
}

tiletype& TileContainer::at(const size_t& x, const size_t& y) {
    if (x<0||x>=width||y<0||y>=height) {
        printf("out of bounds in tilecontainer %d %d width height= %d %d\n", x, y,width,height);
        return oob_error;
    }
    else {
        _upd[y*width+x]=true;
        return tilegrid[y*width+x];
    }
}


void TileContainer::resetupdated() {
    for (size_t i=0; i<width*height; ++i) _upd[i]=false;
}
const bool& TileContainer::updated(const size_t& x, const size_t& y) {
    return _upd[y*width+x];
}

TileContainer::TileContainer(const size_t& _width, const size_t& _height)
    :width(_width), height(_height)
{
    if (_width*_height==0) {
        tilegrid=NULL;
        _upd=NULL;
    }
    else {
        tilegrid=new tiletype[width*height];
        _upd=new bool[width*height];
        /*
        for (size_t i=0; i<width; ++i) {
            for (size_t j=0; j<height; ++j) {
                this->at(i,j).tilenumber=0;
                for (size_t k=0; k<4; ++k) {
                    this->at(i,j).palette_color[k]=0;
                }
            }
        }*/
        for (size_t i=0; i<width*height; ++i) {
            tilegrid[i]=tiletype();
            _upd[i]=true;
        }
        printf("tilecontainer init success\n");
        printf("after success %d\n",this->at(0,0).tilenumber);
    }
}

TileContainer::TileContainer()
:tilegrid(NULL), height(0), width(0)
{}

TileContainer::~TileContainer() {
    delete[] tilegrid;
    delete[] _upd;
}
