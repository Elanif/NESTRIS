#ifndef LEVELLINES_H
#define LEVELLINES_H
#include"Renderer.h"
#include"TileContainer.h"
#include"enums.h"

class LevelLines : public Renderer
{
    public:
        LevelLines() {};
        LevelLines(TileContainer * _tilecont, const nes_ushort& _frameappearance, const nes_uchar& _level);
        const nes_uchar& getlevel() const;
        void addlines(const nes_uchar& _clearedlines);
        void render();
    private:
        nes_ushort lines;
        nes_uchar level;
        nes_ushort linestolevelup; //TODO
};

#endif // LEVELLINES_H
