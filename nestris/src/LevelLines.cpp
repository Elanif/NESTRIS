#include "LevelLines.h"

LevelLines::LevelLines(SDL_Window * _window, const nes_ushort& _frameappearance, const nes_uchar& _level)
    :Renderer(_window, _frameappearance), level(_level), lines(0)
{
    nes_ushort linestolevelup=(level+1)/16*100;
    if ((level+1)%16>=10) linestolevelup+=100;
    else linestolevelup+=((level+1)%16)*10;
}

const nes_uchar& LevelLines::getlevel() const {
    return level;
}

void LevelLines::addlines(const nes_uchar& _clearedlines) {
    lines+=_clearedlines;
    if (lines>linestolevelup) ++level;
}
