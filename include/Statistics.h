#ifndef STATISTICS_H
#define STATISTICS_H
#include"Renderer.h"
#include"enums.h"

class StatisticsContainer {
    public:
    nes_uchar low_byte=0;
    nes_uchar high_byte=0;
    void increase() {
        low_byte++;
        if (low_byte%16u>9u) {
            low_byte+=6u;
        }
        if (low_byte/16u>9u) {
            low_byte=0;
            high_byte++;
        }
    }
};

class Statistics : public Renderer
{
    public:
        Statistics() {};
        Statistics(TileContainer * _tilecont, const nes_ushort& _frameappearance, const nes_uchar& _level);
        void incrementPiece(const nes_uchar& _piecetype);
        void render(const nes_uchar& _level);

    protected:
        nes_uchar level=0;
        void renderBlocks();
        StatisticsContainer stats[7];
    private:
};

#endif // STATISTICS_H
