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

}