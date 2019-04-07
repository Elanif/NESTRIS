#ifndef TILERENDERER_H
#define TILERENDERER_H
#include"TileContainer.h"
#include<SFML/Graphics.hpp>
#include"enums.h"
#include<vector>
#include<string>
#include<cstdio>

class TileRenderer : public sf::Drawable, public sf::Transformable
{
    class sprite {
    public:
        sprite() {
            for (size_t i=0; i<8; ++i)
                for (size_t j=0; j<8; ++j)
                    arr[i][j]=0;
        }
        void print() {
            for (size_t i=0; i<8; ++i) {
                for (size_t j=0; j<8; ++j)
                    printf("%d ",arr[j][i]);
                printf("\n");
            }
        }
        nes_uchar arr[8][8];
    };

    //bool load(const std::string& tileset, sf::Vector2u tileSize, const TileContainer& tiles, const nes_uchar& width, const nes_uchar& height)
public:
    TileRenderer(const size_t& _width, const size_t& _height)
        :tilecont(_width,_height), width(_width), height(_height)
    {

        finalimageclass.create(width*8,height*8);
    }
    bool load(const std::string& tilefile)
    {
        this->width=width;
        this->height=height;
        //tilecont=TileContainer(width,height);
        FILE * spritefile=fopen(tilefile.c_str(),"r");
        if (!spritefile) return false;
        while (!feof(spritefile)) {
            size_t characters=0;
            sprite newsprite;
            for (characters=0; characters<16&&!feof(spritefile); ++characters) {
                int hex;
                fscanf(spritefile,"%x",&hex);
                if (characters<8) {
                    for (size_t i=0; i<8; ++i)
                    newsprite.arr[8-i-1][characters]=(hex >> i) & 1U;
                }
                else {
                    for (size_t i=0; i<8; ++i)
                    newsprite.arr[8-i-1][characters-8]+=((hex >> i) & 1U)<<1;
                }
            }
            if (characters>=16) {
                spritevector.push_back(newsprite);
                /*if (spritevector.size()<20) {
                    newsprite.print();
                    printf("\n");
                }*/
            }
        }
        spritevector[57].print();
        // load the tileset texture
        //size_t tilenumber;
        //load hex value from tileset and get length tilenumber
        //m_tilesets.push_back();
        return true;
    }
    TileContainer* getTileContainer() {
        return &tilecont;
    }
    size_t width;
    size_t height;
    void drawmod(sf::RenderTarget& target, sf::RenderStates states)
    {
        for (size_t x=0; x<width; ++x) {
            for (size_t y=0; y<height; ++y) {
                const int primacifra[4]={
                    tilecont.at(x,y).palette_color[0]/16,
                    tilecont.at(x,y).palette_color[1]/16,
                    tilecont.at(x,y).palette_color[2]/16,
                    tilecont.at(x,y).palette_color[3]/16
                };
                const int secondacifra[4]={
                    tilecont.at(x,y).palette_color[0]%16,
                    tilecont.at(x,y).palette_color[1]%16,
                    tilecont.at(x,y).palette_color[2]%16,
                    tilecont.at(x,y).palette_color[3]%16
                };
                const unsigned int color[4]={
                    palette[primacifra[0]][secondacifra[0]],
                    palette[primacifra[1]][secondacifra[1]],
                    palette[primacifra[2]][secondacifra[2]],
                    palette[primacifra[3]][secondacifra[3]]
                };
                sprite spritetemp=spritevector[tilecont.at(x,y).tilenumber];
                for (size_t pixelx=0; pixelx<8; ++pixelx) {
                    for (size_t pixely=0; pixely<8; ++pixely) {
                            //printf("tilenumber=%d \n",tilecont.at(x,y).tilenumber);
                        nes_uchar tiletypetemp=spritetemp.arr[pixelx][pixely];
                        sf::Color colortemp=sf::Color(color[tiletypetemp]);
                        if (colortemp.a!=0)
                        finalimageclass.setPixel(x*8+pixelx,y*8+pixely,colortemp); //sf::Color(color[spritevector[tilecont(x,y).tilenumber].arr[pixelx][pixely]]
                    }
                }
            }
        }
        temptexclass.loadFromImage(finalimageclass);
        tempspriteclass.setTexture(temptexclass, true);
        target.draw(tempspriteclass,states);
    }

private:



    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        sf::Image finalimage;
        finalimage.create(width*8,height*8);
        for (size_t x=0; x<width; ++x) {
            for (size_t y=0; y<height; ++y) {
                const int primacifra[4]={
                    tilecont.at(x,y).palette_color[0]/16,
                    tilecont.at(x,y).palette_color[1]/16,
                    tilecont.at(x,y).palette_color[2]/16,
                    tilecont.at(x,y).palette_color[3]/16
                };
                const int secondacifra[4]={
                    tilecont.at(x,y).palette_color[0]%16,
                    tilecont.at(x,y).palette_color[1]%16,
                    tilecont.at(x,y).palette_color[2]%16,
                    tilecont.at(x,y).palette_color[3]%16
                };
                const unsigned int color[4]={
                    palette[primacifra[0]][secondacifra[0]],
                    palette[primacifra[1]][secondacifra[1]],
                    palette[primacifra[2]][secondacifra[2]],
                    palette[primacifra[3]][secondacifra[3]]
                };
                for (size_t pixelx=0; pixelx<8; ++pixelx) {
                    for (size_t pixely=0; pixely<8; ++pixely) {
                            //printf("tilenumber=%d \n",tilecont.at(x,y).tilenumber);
                        sprite spritetemp=spritevector[tilecont.at(x,y).tilenumber];
                        nes_uchar tiletypetemp=spritetemp.arr[pixelx][pixely];
                        sf::Color colortemp=sf::Color(color[tiletypetemp]);
                        //if (colortemp.a!=0)
                        finalimage.setPixel(x*8+pixelx,y*8+pixely,colortemp); //sf::Color(color[spritevector[tilecont(x,y).tilenumber].arr[pixelx][pixely]]
                    }
                }
            }
        }
        sf::Texture temptex;
        temptex.loadFromImage(finalimage);
        sf::Sprite tempsprite;
        tempsprite.setTexture(temptex, true);
        target.draw(tempsprite, states);
    }

    std::vector<sprite> spritevector;
    static sf::Uint32 palette[4][16];
    static unsigned char colors[10][4];
    TileContainer tilecont;
    sf::Image finalimageclass;
    sf::Texture temptexclass;
    sf::Sprite tempspriteclass;
};

#endif // TILERENDERER_H
