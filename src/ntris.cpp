#include"ntris.hpp"

namespace ntris {

	nes_schar FrameCounter::framecountlittle = 0;
	nes_schar FrameCounter::framecountbig = 0;

	nes_uchar lineclearframecounter = 0;
	nes_uchar updatingmatrix = 0;
	nes_uchar ARE = 0;
	nes_uchar real_level = 0;
	nes_uchar shown_level = 0;

	sf::Vector2f window_scale = { 1,1 };

	void lowercase_str(std::string& str) { //TODO make it portable with 16bitchar
		for (auto& c : str)
		{
			c = std::tolower(c);
		}
	}

	void uppercase_str(std::string& str) { //TODO make it portable with 16bitchar
		for (auto& c : str)
		{
			c = std::toupper(c);
		}
	}

	template<typename ... Args>
	inline std::string string_format(const std::string& format, Args ... args)
	{
		size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1ull; // Extra space for '\0'
		std::unique_ptr<char[]> buf(new char[size]);
		snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
	} //https://stackoverflow.com/a/26221725
}