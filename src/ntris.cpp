#include"ntris.hpp"

namespace ntris {

	void incframe() {
		FrameCounter::inc();
	}

	nes_schar getframemod4() {
		return FrameCounter::mod4();
	}

	nes_schar FrameCounter::framecountlittle = 0;
	nes_schar FrameCounter::framecountbig = 0;

	nes_uchar lineclearframecounter = 0;
	nes_uchar updatingmatrix = 0;
	nes_uchar ARE = 0;
	nes_uchar real_level = 0;
	nes_uchar shown_level = 0;

	bool four_thirds = true;
	bool fullscreen = false;
	bool shader = false;

	sf::Vector2<long double> window_scale{ 3,3 };
	extern sf::Vector2i window_position{ 0,0 };

	std::string lowercase_str(std::string const& str) { //TODO make it portable with 16bitchar
		std::string res;
		for (auto& c : str)
		{
			res += std::tolower(c);
		}
		return res;
	}

	std::string uppercase_str(std::string& str) { //TODO make it portable with 16bitchar
		std::string res;
		for (auto& c : str)
		{
			res += std::toupper(c);
		}
		return res;
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