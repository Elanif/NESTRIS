#ifndef ENUMS_H
#define ENUMS_H
#ifndef nes_uchar
#define nes_uchar unsigned char
#endif
#ifndef nes_schar
#define nes_schar char
#endif
#ifndef nes_ushort
#define nes_ushort unsigned short
#endif
#ifndef nes_sshort
#define nes_sshort short
#endif
#ifndef PLAYFIELDX
#define PLAYFIELDX 96
#endif // PLAYFIELDX
#ifndef PLAYFIELDY
#define PLAYFIELDY 40
#endif // PLAYFIELDY
#ifndef NEXTPIECEX
#define NEXTPIECEX 195
#endif // NEXTPIECEX
#ifndef NEXTPIECEY
#define NEXTPIECEY 111
#endif // NEXTPIECEY
#ifndef getframemod4
#define getframemod4 glb::FrameCounter::mod4
#endif // getframemod
#ifndef incframe
#define incframe glb::FrameCounter::inc
#endif // incframe
#include <cstddef>
#include"ConsoleManager.h"

namespace glb {
    enum BTN {
        LEFT,
        UP,
        RIGHT,
        DOWN,
        SELECT,
        START,
        B,
        A
    };

    class FrameCounter {
        static nes_schar framecountlittle;
        static nes_schar framecountbig;
        public:
        static void inc() {
            if (framecountlittle==127) framecountlittle=-128;
            else ++framecountlittle;
            if (framecountbig==127) framecountbig=-128;
            else ++framecountbig;
        }

        static nes_schar mod4() {
            return framecountlittle%4;
        }
    };
    extern ConsoleManager cm;
    extern nes_uchar lineclearframecounter;
    extern nes_uchar updatingmatrix;
    constexpr std::size_t playfieldx=12;
    constexpr std::size_t playfieldy=5;
    constexpr std::size_t nextpiecex=24;//FIX
    constexpr std::size_t nextpiecey=13;//FIX
    constexpr std::size_t linesx=19;
    constexpr std::size_t linesy=2;
    constexpr std::size_t levelx=26;
    constexpr std::size_t levely=5;//FIX
    constexpr std::size_t scorex=10;
    constexpr std::size_t scorey=10;
    const sf::Vector2u tilesize={8,8};
    constexpr int spritemode=1;
    constexpr std::size_t maxcolor=4;
}

#endif // ENUMS_H
