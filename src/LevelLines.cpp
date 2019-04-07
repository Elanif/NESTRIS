#include "LevelLines.h"

LevelLines::LevelLines(TileContainer * _tilecont, const nes_ushort& _frameappearance, const nes_uchar& _level)
    :Renderer(_tilecont, _frameappearance), level(_level), lines(0u)
{
    nes_ushort linestolevelup=(level+1)/16*100;
    if ((level+1)%16>=10) linestolevelup+=100;
    else linestolevelup+=((level+1)%16)*10;
}

void LevelLines::render() {
if (hidecounter>0) {
        --hidecounter;
        return;
    }
    else {
        for (std::size_t i=0; i<2; i++) {
            tilecont->at(glb::linesx-i,glb::linesy)=tiletype(1+(lines[i]&0x0f),0x0d,0x30,0x00,0x00);
        }
    }
}

const nes_uchar& LevelLines::getlevel() const {
    return level;
}

void LevelLines::addlines(const nes_uchar& _clearedlines) {
    lines.addLines(_clearedlines);
    if (lines.reallines()>linestolevelup) ++level;
}
