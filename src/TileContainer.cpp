#include "TileContainer.h"
#include<cstdio>


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
    else return tilegrid[y*width+x];
}

TileContainer::TileContainer(const size_t& _width, const size_t& _height)
    :width(_width), height(_height)
{
    if (_width*_height==0) tilegrid=NULL;
    else {
        tilegrid=new tiletype[width*height];
        for (size_t i=0; i<width; ++i) {
            for (size_t j=0; j<height; ++j) {
                this->at(i,j).tilenumber=0;
                for (size_t k=0; k<4; ++k) {
                    this->at(i,j).palette_color[k]=0;
                }
            }
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
}
