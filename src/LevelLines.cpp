#include "LevelLines.h"

LevelLines::LevelLines(SDL_Window * _window, const size_t& _frameappearance, const char& _level)
    :Renderer(_window, _frameappearance), level(_level), lines(0)
{
    unsigned int linestolevelup=(level+1)/16*100;
    if ((level+1)%16>=10) linestolevelup+=100;
    else linestolevelup+=((level+1)%16)*10;
}

const char& LevelLines::getlevel() const {
    return level;
}

void LevelLines::addlines(const char& _clearedlines) {
    lines+=_clearedlines;
    if (lines>linestolevelup) ++level;
}
