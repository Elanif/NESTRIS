#include "ConsoleManager.h"
#include<rlutil.h>
#include<iostream>
#include<SFML/System/Sleep.hpp>
void lowercase_str(std::string& str) { //TODO make it portable
    for(auto& c : str)
    {
       c = std::tolower(c);
    }
}

ConsoleManager::ConsoleManager()
{
    rlutil::cls();

    OutputInfo& fps=add_value("fps","hz");
    fps.set_value<double>(0);
    fps.set_position({0,0});

    OutputInfo& input_delay=add_value("input delay","microseconds");
    input_delay.set_value<sf::Int64>(0);
    input_delay.set_position({0,1});

    OutputInfo& render_delay=add_value("render delay","microseconds");
    render_delay.set_value<sf::Int64>(0);
    render_delay.set_position({0,2});

    OutputInfo& draw_delay=add_value("draw delay","microseconds");
    draw_delay.set_value<sf::Int64>(0);
    draw_delay.set_position({0,3});

    OutputInfo& display_delay=add_value("display delay","microseconds");
    display_delay.set_value<sf::Int64>(0);
    display_delay.set_position({0,4});

    OutputInfo& current_menu=add_value("current menu","");
    current_menu.set_value<int>(0);
    current_menu.set_position({0,5});

    OutputInfo& error=add_value("error","");
    error.set_value<int>(0);
    error.set_position({0,6});
}

ConsoleManager::OutputInfo& ConsoleManager::add_value(std::string info, std::string unit) {
    lowercase_str(info);
    lowercase_str(unit);
    OutputInfo fpsinfo(info,unit);
    CMmap[info]=fpsinfo;
    return CMmap[info];
}


void ConsoleManager::print() {
    if (framecounter++%32==0) {
        for (auto info:CMmap) {
            info.second.print();
        }
    }
}

/*template<>
void ConsoleManager::OutputInfo::set_value<std::__cxx11::basic_string<char> >(const std::__cxx11::basic_string<char>& t) {
}*/

