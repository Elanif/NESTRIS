#include"Statistics.hpp"
#include"TextWriter.hpp"

Statistics::Statistics(TileContainer * _tilecont, const nes_ushort& _frameappearance, const nes_uchar& _level)
    :Renderer(_tilecont, _frameappearance), level(_level)
{

}

void Statistics::incrementPiece(const nes_uchar& _piecetype) {
    stats[_piecetype].increase();
}

void Statistics::render(const nes_uchar& _level) {
    level=_level;
    if (hidecounter>0) {
        --hidecounter;
        return;
    }
    else {
        renderBlocks(); //TODO Optimize: render only on level change and reset/unpause
        const std::size_t topstatx=7;
        const std::size_t topstaty=11;
        for (std::size_t i=0; i<7; ++i) {
            TextWriter::write_hex(stats[i].high_byte,tilecont,sf::Vector2u(topstatx-1,topstaty+i*2),1,0x16);
            TextWriter::write_hex(stats[i].low_byte,tilecont, sf::Vector2u(topstatx,topstaty+i*2),2,0x16);
        }
    }
}

void Statistics::renderBlocks() {
    const nes_uchar levelmod10=level%10;
    const nes_uchar c0=glb::block_colors[levelmod10][0];
    const nes_uchar c1=glb::block_colors[levelmod10][1];
    const nes_uchar c2=glb::block_colors[levelmod10][2];
    const nes_uchar c3=glb::block_colors[levelmod10][3];
    tilecont->at(4,21)=tiletype(662,c0,c1,c2,c3);
    tilecont->at(3,19)=tiletype(652,c0,c1,c2,c3);
    tilecont->at(5,18)=tiletype(642,c0,c1,c2,c3);
    tilecont->at(5,21)=tiletype(663,c0,c1,c2,c3);
    tilecont->at(4,12)=tiletype(646,c0,c1,c2,c3);
    tilecont->at(3,12)=tiletype(645,c0,c1,c2,c3);
    tilecont->at(3,14)=tiletype(643,c0,c1,c2,c3);
    tilecont->at(4,19)=tiletype(653,c0,c1,c2,c3);
    tilecont->at(4,11)=tiletype(650,c0,c1,c2,c3);
    tilecont->at(5,12)=tiletype(647,c0,c1,c2,c3);
    tilecont->at(5,11)=tiletype(651,c0,c1,c2,c3);
    tilecont->at(3,10)=tiletype(638,c0,c1,c2,c3);
    tilecont->at(3,13)=tiletype(658,c0,c1,c2,c3);
    tilecont->at(4,10)=tiletype(639,c0,c1,c2,c3);
    tilecont->at(4,17)=tiletype(668,c0,c1,c2,c3);
    tilecont->at(5,19)=tiletype(654,c0,c1,c2,c3);
    tilecont->at(5,10)=tiletype(640,c0,c1,c2,c3);
    tilecont->at(3,15)=tiletype(655,c0,c1,c2,c3);
    tilecont->at(3,11)=tiletype(649,c0,c1,c2,c3);
    tilecont->at(4,13)=tiletype(659,c0,c1,c2,c3);
    tilecont->at(5,13)=tiletype(660,c0,c1,c2,c3);
    tilecont->at(4,14)=tiletype(644,c0,c1,c2,c3);
    tilecont->at(4,18)=tiletype(641,c0,c1,c2,c3);
    tilecont->at(4,16)=tiletype(666,c0,c1,c2,c3);
    //L
    tilecont->at(5,20)=tiletype(647,c0,c1,c3,c2);
    tilecont->at(4,20)=tiletype(646,c0,c1,c3,c2);
    tilecont->at(3,20)=tiletype(645,c0,c1,c3,c2);
    tilecont->at(3,21)=tiletype(661,c0,c1,c2,c3);
    //I
    tilecont->at(5,23)=tiletype(671,c0,c1,c2,c3);
    tilecont->at(4,23)=tiletype(670,c0,c1,c2,c3);
    tilecont->at(3,23)=tiletype(669,c0,c1,c2,c3);

    tilecont->at(4,15)=tiletype(656,c0,c1,c2,c3);
    tilecont->at(5,15)=tiletype(657,c0,c1,c2,c3);
    tilecont->at(3,17)=tiletype(667,c0,c1,c2,c3);
    tilecont->at(3,16)=tiletype(665,c0,c1,c2,c3);

}
