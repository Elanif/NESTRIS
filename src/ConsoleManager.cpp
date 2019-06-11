#include "ConsoleManager.hpp"
#include<chrono>
#include<ctime>
#include<iostream>
#include<Window.hpp>
#include<sstream>
#include<limits>

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1ull; // Extra space for '\0'
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
} //https://stackoverflow.com/a/26221725

void gotoxycm(std::size_t x, std::size_t y) {
	gotoxy(x + 1, y + 1);
}
void gotoxycm(const sf::Vector2u& _pos) {
	gotoxy(_pos.x + 1, _pos.y + 1);
}

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
	error_log << error_string << "\n\r";
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
	if (!info_window.isOpen()) info_window.create(sf::VideoMode(256, 224), "Information");
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
    rlutil::cls();
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
		error_log << "OutputInfo pushback exception " << bad_alloc_exception.what() << "\n\r";
		error_log.flush();
    }
    if (!push_back_error && CMvector_size==CMvector.size()) {
        push_back_error=true;
		error_log << "OutputInfo pushback failed\n\r";
		error_log.flush();
    }
    if (push_back_error) return (std::size_t) -1;
    else {
        CMmap[CMvector[CMvector.size()-1]->name]=CMvector.size()-1;
        return CMvector.size()-1;
    }
}


void ConsoleManager::refresh(bool always_print) {
	static std::string text_test = "";
	if (info_window.isOpen()) {
		sf::Event event;
		while (info_window.pollEvent(event))
		{
			if (info_window.hasFocus()) {
				// Handle ASCII characters only
				if (event.type == sf::Event::TextEntered && event.text.unicode < 128)
				{
					text_test += static_cast<char>(event.text.unicode);
					std::cout << text_test;
				}
			}
			if (event.type == sf::Event::Closed) {
				close_info_window();
				return;
			}

		}
		info_window.clear();
		for (auto& info : CMvector) {
			std::string outputstring = info->print(true);
			sf::Text outputstring_text;
			outputstring_text.setFont(info_window_font);
			outputstring_text.setString("Hello world");
			outputstring_text.setCharacterSize(8);
			outputstring_text.setFillColor(sf::Color::White);
			info_window.draw(outputstring_text);
		}
		info_window.display();
	}
    if (framecounter++%32==0||always_print||error_print) {
        //rlutil::cls();
        sf::Vector2u pos={0,0};
        for (auto& info:CMvector) {
            if (pos.x>0) {
                pos.x=0;
                pos.y++;
            }
            int conwidth=rlutil::tcols();
            if (conwidth<0) conwidth=std::numeric_limits<int>::max();
            std::string outputstring=info->print(true);
			gotoxycm(pos);
			std::cout << outputstring << "\n\r";
			pos.y++;
        }
        error_print=false;
    }
}


ConsoleManager::~ConsoleManager() {
	close_info_window();
}