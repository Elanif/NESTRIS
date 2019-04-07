#include "ConsoleManager.h"
#include<iostream>
#include<SFML/System/Sleep.hpp>
#include<sstream>

void lowercase_str(std::string& str) { //TODO make it portable with 16bitchar
    for(auto& c : str)
    {
       c = std::tolower(c);
    }
}

template<typename T>
void ConsoleManager::update(std::string info, const T& t) {
    if (CMmap.find(info)==CMmap.end()) {
        //error message somewhere
    }
    else {
        CMvector[CMmap[info]].set_value<T>(t);
    }
}
void ConsoleManager::update(std::string info, const glb::const_string_literal& t) {
    if (CMmap.find(info)==CMmap.end()) {
        //error message somewhere
    }
    else {
        CMvector[CMmap[info]].set_value(t);
    }
}
template void ConsoleManager::update(std::string info, const std::string& t);
template void ConsoleManager::update(std::string info, const int& t);
template void ConsoleManager::update(std::string info, const std::size_t& t);
template void ConsoleManager::update(std::string info, const unsigned long long& t);
template void ConsoleManager::update(std::string info, const long long& t);
template void ConsoleManager::update(std::string info, const double& t);

template<typename T>
void ConsoleManager::update_error(const T& t) {
    error_print=true;
    if (CMmap.find("error")==CMmap.end()) {
        fprintf(error_log, "No OutputInfo of type error found\n");
        fflush(error_log);
    }
    else {
        //TODO error reporting
        CMvector[CMmap["error"]].set_value<T>(t);
    }
}

template void ConsoleManager::update_error(const std::string& t);
template void ConsoleManager::update_error(const glb::const_string_literal& t);
template void ConsoleManager::update_error(const int& t);
template void ConsoleManager::update_error(const std::size_t& t);
template void ConsoleManager::update_error(const unsigned long long& t);
template void ConsoleManager::update_error(const long long& t);
template void ConsoleManager::update_error(const double& t);


ConsoleManager::ConsoleManager()
{
    error_log=fopen("error_log.txt","a");
    rlutil::cls();
    //std::cout.sync_with_stdio(false); messes up with rlutil
    OutputInfoLow<double> _fps("fps","hz",true);
    OutputInfo& fps=CMvector[add_value(_fps)];
    fps.set_value<double>(0);

    OutputInfoLow<sf::Int64> _input("input delay","microseconds",false);
    OutputInfo& input_delay=CMvector[add_value(_input)];
    input_delay.set_value<sf::Int64>(0);

    OutputInfoLow<sf::Int64> _render("render delay","microseconds",false);
    OutputInfo& render_delay=CMvector[add_value(_render)];
    render_delay.set_value<sf::Int64>(0);

    OutputInfoLow<sf::Int64> _draw("draw delay","microseconds",false);
    OutputInfo& draw_delay=CMvector[add_value(_draw)];
    draw_delay.set_value<sf::Int64>(0);

    OutputInfoLow<sf::Int64> _display("display delay","microseconds",false);
    OutputInfo& display_delay=CMvector[add_value(_display)];
    display_delay.set_value<sf::Int64>(0);

    OutputInfo& system=CMvector[add_value("system","")];
    system.set_value<int>(0);

    OutputInfoError error("error", 3);
    add_value(error);

}

std::size_t ConsoleManager::add_value(std::string info, std::string unit) {
    lowercase_str(info);
    lowercase_str(unit);
    OutputInfo fpsinfo(info,unit);
    return add_value(fpsinfo);
}

std::size_t ConsoleManager::add_value(const OutputInfo& outputinfo) {
    OutputInfo tmp_outputinfo=outputinfo;
    lowercase_str(tmp_outputinfo.name);
    std::size_t CMvector_size=CMvector.size();
    bool push_back_error=false;
    try {
        CMvector.push_back(tmp_outputinfo);
    }
    catch (std::exception const& bad_alloc_exception) {
        push_back_error=true;
        fprintf(error_log, "OutputInfo pushback exception: %s\n",bad_alloc_exception.what());
        fflush(error_log);
    }
    if (!push_back_error && CMvector_size==CMvector.size()) {
        push_back_error=true;
        fprintf(error_log, "Outputinfo pushback failed\n");
        fflush(error_log);
    }
    if (push_back_error) return (std::size_t) -1;
    else {
        CMmap[outputinfo.name]=CMvector.size()-1;
        return CMvector.size()-1;
    }
}


void ConsoleManager::print(bool always_print) {
    if (framecounter++%4==0||always_print||error_print) {
        //rlutil::cls();
        sf::Vector2u pos={0,0};
        for (auto info:CMvector) {
            if (pos.x>0) {
                pos.x=0;
                pos.y++;
            }
            pos=info.print(pos,rlutil::tcols(),error_log);
        }
        error_print=false;
    }
}


ConsoleManager::~ConsoleManager() {
    if (error_log)
        fclose(error_log);
}
