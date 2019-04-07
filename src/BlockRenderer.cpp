#include "BlockRenderer.h"
#include<cstdio>
#include "SquareRenderer.h"
unsigned char BlockRenderer::colors[10][4]={
    {0x0F ,0x30 ,0x21 ,0x12},
    {0x0F ,0x30 ,0x29 ,0x1A},
    {0x0F ,0x30 ,0x24 ,0x14},
    {0x0F ,0x30 ,0x2A ,0x12},
    {0x0F ,0x30 ,0x2B ,0x15},
    {0x0F ,0x30 ,0x22 ,0x2B},
    {0x0F ,0x30 ,0x00 ,0x16},
    {0x0F ,0x30 ,0x05 ,0x13},
    {0x0F ,0x30 ,0x16 ,0x12},
    {0x0F ,0x30 ,0x27 ,0x16}
};

unsigned int BlockRenderer::palette[4][16]= {
    {0x7C7C7C ,0x0000FC ,0x0000BC ,0x4428BC ,0x940084 ,0xA80020 ,0xA81000 ,0x881400 ,0x503000 ,0x007800 ,0x006800 ,0x005800 ,0x004058 ,0x000000 ,0x000000 ,0x000000},
    {0xBCBCBC ,0x0078F8 ,0x0058F8 ,0x6844FC ,0xD800CC ,0xE40058 ,0xF83800 ,0xE45C10 ,0xAC7C00 ,0x00B800 ,0x00A800 ,0x00A844 ,0x008888 ,0x000000 ,0x000000 ,0x000000},
    {0xF8F8F8 ,0x3CBCFC ,0x6888FC ,0x9878F8 ,0xF878F8 ,0xF85898 ,0xF87858 ,0xFCA044 ,0xF8B800 ,0xB8F818 ,0x58D854 ,0x58F898 ,0x00E8D8 ,0x787878 ,0x000000 ,0x000000},
    {0xFCFCFC ,0xA4E4FC ,0xB8B8F8 ,0xD8B8F8 ,0xF8B8F8 ,0xF8A4C0 ,0xF0D0B0 ,0xFCE0A8 ,0xF8D878 ,0xD8F878 ,0xB8F8B8 ,0xB8F8D8 ,0x00FCFC ,0xF8D8F8 ,0x000000 ,0x000000}
};

BlockRenderer::BlockRenderer()
{

}

void BlockRenderer::block(SDL_Surface* renderSurface, const size_t& blocktype, const size_t& level, const size_t& x, const size_t& y) {
    unsigned char c;
    int primacifra;
    int secondacifra;

    c=colors[level%10][0];
    primacifra=c/16;
    secondacifra=c%16;
    SquareRenderer::square(renderSurface, palette[primacifra][secondacifra], 255, x, y, 8 ,8); //black common
    switch(blocktype) {
    case 0:
        break;
    case 1:
        c=colors[level%10][3];
        primacifra=c/16;
        secondacifra=c%16;
        SquareRenderer::square(renderSurface, palette[primacifra][secondacifra], 255, x, y, 7 ,7);

        c=colors[level%10][1]; //white
        primacifra=c/16;
        secondacifra=c%16;
        SquareRenderer::square(renderSurface, palette[primacifra][secondacifra], 255, x, y, 1 , 1);
        SquareRenderer::square(renderSurface, palette[primacifra][secondacifra], 255, x+1, y+1  , 5 , 5);
        break;
    case 2:
        c=colors[level%10][2];
        primacifra=c/16;
        secondacifra=c%16;
        SquareRenderer::square(renderSurface, palette[primacifra][secondacifra], 255, x, y, 7 ,7);

        c=colors[level%10][1];//white
        primacifra=c/16;
        secondacifra=c%16;
        SquareRenderer::square(renderSurface, palette[primacifra][secondacifra], 255, x, y, 1 , 1);
        SquareRenderer::square(renderSurface, palette[primacifra][secondacifra], 255, x+1, y+1  , 1 , 1);
        SquareRenderer::square(renderSurface, palette[primacifra][secondacifra], 255, x+1, y+2  , 1 , 1);
        SquareRenderer::square(renderSurface, palette[primacifra][secondacifra], 255, x+2, y+1  , 1 , 1);
        break;
    case 3:
        c=colors[level%10][3];
        primacifra=c/16;
        secondacifra=c%16;
        SquareRenderer::square(renderSurface, palette[primacifra][secondacifra], 255, x, y, 7 ,7);

        c=colors[level%10][1];//white
        primacifra=c/16;
        secondacifra=c%16;
        SquareRenderer::square(renderSurface, palette[primacifra][secondacifra], 255, x, y, 1 , 1);
        SquareRenderer::square(renderSurface, palette[primacifra][secondacifra], 255, x+1, y+1  , 1 , 1);
        SquareRenderer::square(renderSurface, palette[primacifra][secondacifra], 255, x+1, y+2  , 1 , 1);
        SquareRenderer::square(renderSurface, palette[primacifra][secondacifra], 255, x+2, y+1  , 1 , 1);
        break;
    case 10:
        SquareRenderer::square(renderSurface, 0xffffff, 255, x, y, 8, 8);
        break;
    default:
        printf("Invalid block type %d\n", blocktype);
        break;
    }
}

