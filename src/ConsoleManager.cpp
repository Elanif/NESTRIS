#include "ConsoleManager.hpp"
#include<chrono>
#include<ctime>
#include<iostream>
#include<Window.hpp>
#include<sstream>
#include<limits>
#include"TextFormatter.hpp"

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1ull; // Extra space for '\0'
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
} //https://stackoverflow.com/a/26221725

sf::Font ConsoleManager::info_window_font;
unsigned char ConsoleManager::framecounter=0;
bool ConsoleManager::error_print=false;
std::vector<std::unique_ptr<OutputInfo> > ConsoleManager::CMvector;
std::unordered_map<std::string, std::size_t> ConsoleManager::CMmap;
std::ofstream ConsoleManager::error_log;

template <typename Duration>
std::string print_time(tm t, Duration fraction) {
    using namespace std::chrono;
    return string_format("%04u-%02u-%02u %02u:%02u:%02u.%03u ", t.tm_year + 1900,
                t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec,
                static_cast<unsigned>(fraction / milliseconds(1)));
}

void ConsoleManager::print_error(const std::string& error_string) {
    print_error(error_string.c_str());
}
void ConsoleManager::print_error(const char* error_string) {
    auto clock = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(clock);
    using namespace std::chrono;
    system_clock::time_point now = system_clock::now();
    system_clock::duration tp = now.time_since_epoch();
    tp -= duration_cast<seconds>(tp);
	error_log<<print_time(*localtime(&current_time), tp);
	error_log << error_string << glb::newline;
	error_log.flush();
}

void lowercase_str(std::string& str) { //TODO make it portable with 16bitchar
    for(auto& c : str)
    {
       c = std::tolower(c);
    }
}

template<typename T>
void ConsoleManager::update(std::string info, const T& t) {
    if (CMmap.find(info)==CMmap.end()) {
        print_error(std::string("No OutpufInfo of type ")+info+std::string(" found"));
    }
    else {
        CMvector[CMmap[info]]->set_value(t);
    }
}
template void ConsoleManager::update(std::string info, const std::string& t);
template void ConsoleManager::update(std::string info, char const* const& t);
template void ConsoleManager::update(std::string info, const int& t);
template void ConsoleManager::update(std::string info, const std::size_t& t);
template void ConsoleManager::update(std::string info, const unsigned long long& t);
template void ConsoleManager::update(std::string info, const long long& t);
template void ConsoleManager::update(std::string info, const double& t);
template void ConsoleManager::update(std::string info, const long double& t);

template<typename T>
void ConsoleManager::update_error(const T& t) {
    error_print=true;
    if (CMmap.find("error")==CMmap.end()) {
        print_error("No OutputInfo of type error found");
    }
    else {
        std::string error_string=ntris::to_string(t);
        if (error_string.length()>0 && error_string!="0") {
            print_error(error_string);
        }
        CMvector[CMmap["error"]]->set_value(error_string);
    }
}
void ConsoleManager::update_error(const char* t) {
    update_error<std::string>(std::string(t));
}
template void ConsoleManager::update_error(const std::string& t);
template void ConsoleManager::update_error(const int& t);
template void ConsoleManager::update_error(const std::size_t& t);
template void ConsoleManager::update_error(const unsigned long long& t);
template void ConsoleManager::update_error(const long long& t);
template void ConsoleManager::update_error(const double& t);
template void ConsoleManager::update_error(const long double& t);

sf::RenderWindow ConsoleManager::info_window;

void ConsoleManager::open_info_window() {
	if (!info_window.isOpen()) info_window.create(sf::VideoMode(glb::ntsc_screen_width * glb::window_scale.x, glb::ntsc_screen_height * glb::window_scale.y), "Information");
}

void ConsoleManager::close_info_window() {
	if (info_window.isOpen()) info_window.close();
}

void ConsoleManager::toggle_info_window()
{
	if (info_window.isOpen()) close_info_window();
	else open_info_window();
}

bool ConsoleManager::is_window_open() {
	return info_window.isOpen();
}

void ConsoleManager::init()
{
	error_log.open("error_log.txt", std::ios::app);
    //std::cout.sync_with_stdio(false); messes up with rlutil
    std::unique_ptr<OutputInfo> _fps(new OutputInfoLowDouble("fps","hz",true));
    std::unique_ptr<OutputInfo>& fps=CMvector[add_value(std::move(_fps))];
    fps->set_value(0);

    std::unique_ptr<OutputInfo> _input(new OutputInfoLowI64("input delay","microseconds",false));
    std::unique_ptr<OutputInfo>& input_delay=CMvector[add_value(std::move(_input))];
    input_delay->set_value(0);

    std::unique_ptr<OutputInfo> _render(new OutputInfoLowI64("render delay","microseconds",false));
    std::unique_ptr<OutputInfo>& render_delay=CMvector[add_value(std::move(_render))];
    render_delay->set_value(0);

    std::unique_ptr<OutputInfo> _draw(new OutputInfoLowI64("draw delay","microseconds",false));
    std::unique_ptr<OutputInfo>& draw_delay=CMvector[add_value(std::move(_draw))];
    draw_delay->set_value(0);

    std::unique_ptr<OutputInfo> _display(new OutputInfoLowI64("display delay","microseconds",false));
    std::unique_ptr<OutputInfo>& display_delay=CMvector[add_value(std::move(_display))];
    display_delay->set_value(0);

    std::unique_ptr<OutputInfo>& system=CMvector[add_value("system","")];
    system->set_value("");

    std::unique_ptr<OutputInfo> _error(new OutputInfoError("error",8));
    add_value(std::move(_error));

	info_window_font.loadFromFile("Roboto.ttf");
	text_formatter.setCharacterSize(glb::info_window_character_size);
	text_formatter.setFillColor(sf::Color::White);
	text_formatter.setFont(info_window_font);
	text_formatter.calc_font_sizes();
	open_info_window();
}

std::size_t ConsoleManager::add_value(std::string info, std::string unit) {
    lowercase_str(info);
    lowercase_str(unit);
    std::unique_ptr<OutputInfo> fpsinfo=std::make_unique<OutputInfo>(info,unit);
    return add_value(std::move(fpsinfo));
}

std::size_t ConsoleManager::add_value(std::unique_ptr<OutputInfo>&& outputinfo) {
    lowercase_str(outputinfo->name);
    std::size_t CMvector_size=CMvector.size();
    bool push_back_error=false;
    try {
        CMvector.push_back(std::move(outputinfo));
    }
    catch (std::exception const& bad_alloc_exception) {
        push_back_error=true;
		error_log << "OutputInfo pushback exception " << bad_alloc_exception.what() << glb::newline;
		error_log.flush();
    }
    if (!push_back_error && CMvector_size==CMvector.size()) {
        push_back_error=true;
		error_log << "OutputInfo pushback failed"<<glb::newline;
		error_log.flush();
    }
    if (push_back_error) return (std::size_t) -1;
    else {
        CMmap[CMvector[CMvector.size()-1]->name]=CMvector.size()-1;
        return CMvector.size()-1;
    }
}

TextFormatter<string_character> ConsoleManager::text_formatter;

void ConsoleManager::refresh(bool always_print) {
	static nes_uchar counter = 0;
	static std::string text_test = "";
	if (info_window.isOpen()) {
		sf::Event event;
		while (info_window.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				close_info_window();
				return;
				break;
			case sf::Event::TextEntered:
				if (info_window.hasFocus()) {
					// Handle ASCII characters only
					if (event.text.unicode < 128)
					{
						if (event.text.unicode == 8 && text_test.length() > 0) {
							text_test.pop_back();
						}
						else text_test += static_cast<char>(event.text.unicode);
						std::cout << text_test << glb::newline;
					}
				}
			break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::F1: {
						toggle_info_window();
						if (is_window_open())
							info_window.requestFocus();
					}
					break;
				}
			}

		}
		info_window.clear();
		sf::Vector2f pos{ 0,0 };
		bool reset = false;
		if (++counter >= 60) {
			counter = 0;
			reset = true;
		}
		for (auto& info : CMvector) {
			std::string outputstring = info->print(reset);
			text_formatter.setString(outputstring);
			text_formatter.setBoundaries({ 256,224 });
			text_formatter.setPosition(pos);
			sf::Text t= text_formatter.getFormattedText(glb::info_window_character_size * sqrt((double)glb::window_scale.x * glb::window_scale.y));
			info_window.draw(t);
			pos.y += text_formatter.getLastFormattedSize().y;
		}
		info_window.display();
		error_print = false;
	}
}


ConsoleManager::~ConsoleManager() {
	close_info_window();
}
