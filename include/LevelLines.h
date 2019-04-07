#ifndef LEVELLINES_H
#define LEVELLINES_H
#include"Renderer.h"
#include"TileContainer.h"
#include"enums.h"

class LinesContainer {
public:
    LinesContainer() {
        lines[0]=lines[1]=lines[2]=0;
    }
    explicit LinesContainer(nes_uchar _lines[]) {
        lines[0]=_lines[0];
        lines[1]=_lines[1];
    }
    explicit LinesContainer(unsigned int _lines) {
        lines[0]=(_lines&0x0f)+((_lines/10)%10)*0x0f;
        _lines/=100;
        lines[1]=(_lines&0x0f)+((_lines/10)%10)*0x0f;
        _lines/=100;
    }
    nes_uchar lines[2];
    unsigned int reallines() const {
        unsigned int result=(lines[0]&0x0f)+((lines[0]&0xf0)/0x0f)*10;
        result+=(lines[1]&0x0f)*100+((lines[1]&0xf0)/0x0f)*1000;
        return result;
    }
    bool operator>(const LinesContainer& _sc) const {
        return this->reallines()>_sc.reallines();
    }
    const nes_uchar& operator[](const std::size_t& i) const {
        return lines[i];
    }
    nes_uchar& operator[](const std::size_t& i) {
        return lines[i];
    }
    LinesContainer& addLines(const nes_uchar& _added_lines) { //TODO breaks for more than 6 lines added
        lines[0]+=_added_lines;
        if (lines[0]&0x0f>0x09) {
            lines[0]=lines[0]&0xf0;
            lines[0]+=0x10;
        }
        if (lines[0]&0xf0>0x90) {
            lines[0]&=0x0f;
            lines[1]++;
        }
        return *this;
    }
};

class LevelLines : public Renderer
{
    public:
        LevelLines() {};
        LevelLines(TileContainer * _tilecont, const nes_ushort& _frameappearance, const nes_uchar& _level);
        const nes_uchar& getlevel() const;
        void addlines(const nes_uchar& _clearedlines);
        void render();
    private:
        LinesContainer lines;
        nes_uchar level;
        nes_ushort linestolevelup; //TODO
};

#endif // LEVELLINES_H
