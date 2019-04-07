#include "TileContainer.h"
#include<cstdio>
#include"ConsoleManager.h"
#include<string>
#include<sstream>
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

const tiletype& TileContainer::at(const std::size_t& x, const std::size_t& y) const {
    if (x<0||x>=width||y<0||y>=height) {
        std::stringstream errorstream;
        errorstream<<"out of bounds in tilecontainer"<<x<<"/"<<width<<" "<<y<<"/"<<height;
        glb::cm.update("error",errorstream.str());
        return oob_error;
    }
    else return tilegrid[y*width+x];
}

tiletype& TileContainer::at(const std::size_t& x, const std::size_t& y) {
    if (x<0||x>=width||y<0||y>=height) {
        std::stringstream errorstream;
        errorstream<<"out of bounds in tilecontainer"<<x<<"/"<<width<<" "<<y<<"/"<<height;
        glb::cm.update("error",errorstream.str());
        return oob_error;
    }
    else {
        _upd[y*width+x]=true;
        return tilegrid[y*width+x];
    }
}


void TileContainer::resetupdated() {
    for (std::size_t i=0; i<width*height; ++i) _upd[i]=false;
}
const bool& TileContainer::updated(const std::size_t& x, const std::size_t& y) {
    return _upd[y*width+x];
}

void TileContainer::renderExtra(const std::size_t pixelx, const std::size_t pixely, const tiletype& t, const double& priority) {
    if (priority<0.5) {
        if (previous_tiles.size()>=extra_render) {
            std::stringstream errorstream;
            errorstream<<"Too many extra previous tiles, pixelx,pixely="<<pixelx<<","<<pixely<<" tiletype="<<t.tilenumber;
            glb::cm.update<std::string>("error",errorstream.str());
        }
        else {
            previous_tiles.insert(std::make_pair(priority,glb::triple(pixelx,pixely,t)));
        }
    }
    if (priority>=0.5) {
        if (following_tiles.size()>=extra_render) {
            std::stringstream errorstream;
            errorstream<<"Too many extra following tiles, pixelx,pixely="<<pixelx<<","<<pixely<<" tiletype="<<t.tilenumber;
            glb::cm.update<std::string>("error",errorstream.str());
        }
        else{
            following_tiles.insert(std::make_pair(priority,glb::triple(pixelx,pixely,t)));
        }
    }

}

TileContainer::TileContainer(const std::size_t& _width, const std::size_t& _height, const std::size_t& _extra_render)
    :width(_width),
    height(_height),
    extra_render(_extra_render)
{
    if (_width*_height==0) {
        tilegrid=NULL;
        _upd=NULL;
    }
    else {
        tilegrid=new tiletype[width*height];
        _upd=new bool[width*height];
        for (std::size_t i=0; i<width*height; ++i) {
            tilegrid[i]=tiletype();
            _upd[i]=true;
        }
        printf("tilecontainer init success\n");
        printf("after success %d\n",this->at(0,0).tilenumber);
    }
}

void TileContainer::reset() {
    if (tilegrid==NULL) return;
    for (std::size_t i=0; i<width*height; ++i) {
        tilegrid[i].tilenumber=87;
        tilegrid[i].palette_color[0]=0x0d;
    }
}

TileContainer::TileContainer()
:tilegrid(NULL), height(0), width(0)
{}

TileContainer::~TileContainer() {
    if (width*height>0) {
        delete[] tilegrid;
        delete[] _upd;
    }
}
