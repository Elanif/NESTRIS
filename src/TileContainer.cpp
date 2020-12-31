#include"TileContainer.hpp"
#include<cstdio>
#include"Log.hpp"
#include<string>
#include<sstream>

//lookup colors of tetriminoes from level 0 to 9
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
        Log::update_error(errorstream.str());
        return oob_error;
    }
    else return tilegrid[y*width+x];
}

tiletype& TileContainer::at(const std::size_t& x, const std::size_t& y) {
    if (x<0||x>=width||y<0||y>=height) {
        std::stringstream errorstream;
        errorstream<<"out of bounds in tilecontainer"<<x<<"/"<<width<<" "<<y<<"/"<<height;
        Log::update_error(errorstream.str());
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
        if (extra_tiles.y.size()>=extra_render.y) {
            std::stringstream errorstream;
            errorstream<<"Too many extra previous tiles, pixelx,pixely="<<pixelx<<","<<pixely<<" tiletype="<<t.tilenumber;
            Log::update_error(errorstream.str());
        }
        else {
            extra_tiles.y.insert(std::make_pair(priority,ntris::triple(pixelx,pixely,t)));
        }
        for (const auto& i:t.palette_color) { //this was intended for transparency purposes
            if (i==ntris::trnspr &&false) {
                if (extra_tiles.x.size()>=extra_render.x) {
                    std::stringstream errorstream;
                    errorstream<<"Too many extra trnspr tiles, pixelx,pixely="<<pixelx<<","<<pixely<<" tiletype="<<t.tilenumber;
                    Log::update_error(errorstream.str());
                }
                else {
                    extra_tiles.x.insert(std::make_pair(priority,ntris::triple(pixelx,pixely,tiletype(87,0x0d,0x0d,0x0d,0x0d))));
                }
                break;
            }
        }

    }
    if (priority>=0.5) {
        if (extra_tiles.z.size()>=extra_render.z) {
            std::stringstream errorstream;
            errorstream<<"Too many extra following tiles, pixelx,pixely="<<pixelx<<","<<pixely<<" tiletype="<<t.tilenumber;
            Log::update_error(errorstream.str());
        }
        else{
            extra_tiles.z.insert(std::make_pair(priority,ntris::triple(pixelx,pixely,t)));
        }
    }

}
TileContainer::TileContainer(const std::size_t& _width, const std::size_t& _height, const sf::Vector3<std::size_t>& _extra_render)
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
    }
}

void TileContainer::reset() {
    if (tilegrid==NULL) return;
    for (std::size_t i=0; i<width*height; ++i) {
        tilegrid[i].tilenumber=87;
        tilegrid[i].palette_color[0]=0x0d;
        _upd[i]=true;
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
