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
#include "stddef.h"

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
    extern nes_uchar lineclearframecounter;
    extern nes_uchar updatingmatrix;
    const size_t playfieldx=12;
    const size_t playfieldy=5;
    const size_t nextpiecex=24;
    const size_t nextpiecey=13;
    const size_t linesx=19;
    const size_t linesy=2;
    const size_t levelx=26;

}

#endif // ENUMS_H
