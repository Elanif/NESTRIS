#ifndef LEVELLINES_H
#define LEVELLINES_H
#include"Renderer.h"
#include"enums.h"

class LevelLines : public Renderer
{
    public:
        LevelLines() {};
        LevelLines(SDL_Window * _window, const nes_ushort& _frameappearance, const nes_uchar& _level);
        const nes_uchar& getlevel() const;
        void addlines(const nes_uchar& _clearedlines);
    private:
        nes_ushort lines;
        nes_uchar level;
        nes_ushort linestolevelup; //TODO
};

#endif // LEVELLINES_H
