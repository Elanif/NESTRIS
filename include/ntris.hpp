#ifndef NTRIS_H
#define NTRIS_H
#include<cstdint>
typedef std::uint_least8_t nes_uchar;
typedef std::int_least8_t nes_schar;
typedef std::uint_least16_t nes_ushort;
typedef std::int_least16_t nes_sshort;
typedef std::uint_fast64_t largest_uint;
#include<cstddef>
#include<SFML/System/Vector2.hpp>
#include<tuple>
#include<string>
#include<memory>

typedef char string_character;

class tiletype;
namespace ntris {
    typedef const char* const_string_literal;
    typedef char* string_literal;
    enum BTN {
        Left=0,
        Up,
        Right,
        Down,
        Select,
        Start,
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

	void incframe();

	nes_schar getframemod4();

    extern nes_uchar lineclearframecounter;
    extern nes_uchar updatingmatrix;
    extern nes_uchar ARE;
    extern nes_uchar real_level; //level that sometimes isn't shown e.g. while lines are being cleared
    extern nes_uchar shown_level; //level that is shown by the game
    constexpr std::size_t playfieldx=12;
    constexpr std::size_t playfieldy=5;
    const sf::Vector2u nextpiece_coords[7]={{204,112},{204,112},{204,112},{208,112},{204,112},{204,112},{208,112}};
    constexpr std::size_t linesx=19;
    constexpr std::size_t linesy=2;
    constexpr std::size_t levelx=24;
    constexpr std::size_t levely=19;
    constexpr std::size_t topscorex=24;
    constexpr std::size_t topscorey=4;
    constexpr std::size_t scorex=24;
    constexpr std::size_t scorey=7;
    constexpr std::size_t nextx=24;
    constexpr std::size_t nexty=12;
    constexpr std::size_t typex=3;
    constexpr std::size_t typey=3;
    const std::pair< std::size_t, std::size_t> tilesize={8,8};
    constexpr int spritemode=1;
    constexpr std::size_t maxcolor=4;
    constexpr std::size_t maxbuttons=8;
    constexpr nes_uchar trnspr=0x1d;
    typedef std::tuple<std::size_t, std::size_t, tiletype> triple;
    constexpr long double color_subcarrier_frequency=21477272.72727272727272727272727272L;
    constexpr long double clock_divisor = 12.L;
    constexpr long double ntsc_fps_odd=color_subcarrier_frequency/clock_divisor/(341*262-1)*3;
    constexpr long double ntsc_fps_even=color_subcarrier_frequency/clock_divisor/(341*262)*3;
    constexpr long double ntsc_fps=2.L/(1.L/ntsc_fps_even+1.L/ntsc_fps_odd);
	constexpr std::size_t ntsc_screen_width = 256;
	constexpr std::size_t ntsc_screen_height = 224;
	constexpr std::size_t ntsc_tiles_x = 32;
	constexpr std::size_t ntsc_tiles_y = 28;
	extern sf::Vector2<long double> window_scale;
	extern sf::Vector2i window_position;
	extern bool four_thirds;
	extern bool fullscreen;
	extern bool shader;
	constexpr float info_window_character_size = 8;
	constexpr largest_uint MIN_DELAY_ERROR = 1000; 
	constexpr std::size_t bucket_size = 200;
	const std::string newline = { "\r\n" }; //could change depending on the os
	constexpr std::size_t max_string_length = 30;

    constexpr nes_uchar block_colors[10][4]={
    {0x0D ,0x30 ,0x21 ,0x12},
    {0x0D ,0x30 ,0x29 ,0x1A},
    {0x0D ,0x30 ,0x24 ,0x14},
    {0x0D ,0x30 ,0x2A ,0x12},
    {0x0D ,0x30 ,0x2B ,0x15},
    {0x0D ,0x30 ,0x22 ,0x2B},
    {0x0D ,0x30 ,0x00 ,0x16},
    {0x0D ,0x30 ,0x05 ,0x13},
    {0x0D ,0x30 ,0x16 ,0x12},
    {0x0D ,0x30 ,0x27 ,0x16}
    };

	std::string lowercase_str(std::string const& str); //TODO make it portable with 16bitchar
	std::string uppercase_str(std::string const& str); //TODO make it portable with 16bitchar

	template<typename ... Args>
	std::string string_format(const std::string& format, Args ... args);

	template<typename T>
	std::string to_string(const T& t) {
		return std::to_string(t);
	}
	//TODO OVERLOAD?
	template<>
	inline std::string to_string<std::string>(const std::string& t) {
		return t;
	}
	template<>
	inline std::string to_string<const char*>(const char* const& t) {
		return std::string(t);
	}
}

#endif // NTRIS_H
