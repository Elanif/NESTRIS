#include "ConsoleManager.h"
#include<rlutil.h>
#include<iostream>
#include<SFML/System/Sleep.hpp>
#include<chrono>
#include<ctime>
#include<sstream>


void gotoxycm(std::size_t x, std::size_t y) {
    gotoxy(x+1,y+1);
}

void lowercase_str(std::string& str) { //TODO make it portable with 16bitchar
    for(auto& c : str)
    {
       c = std::tolower(c);
    }
}

//OUTPUTINFO
OutputInfo::OutputInfo():name(""),value(""),unit(""),last_render_length(0) {}
OutputInfo::~OutputInfo() {}
OutputInfo::OutputInfo(const std::string& _name,const std::string &_unit)
    :name(_name),
     unit(_unit),
     last_render_length(0)
{}
void OutputInfo::set_position(const sf::Vector2u& _position)
{
    outputposition=_position;
}
sf::Vector2u OutputInfo::print(sf::Vector2u currentposition, int conwidth, FILE*error_log)
{
    if (currentposition.x!=0) {
        currentposition.x=0;
        currentposition.y++;
    }
    std::string outputstring=name+"="+value+unit;
    gotoxycm(currentposition.x,currentposition.y);
    currentposition.x+=outputstring.size();
    std::cout<<outputstring;
    if (name==std::string("error")&&value!=std::string("0")&&value.length()>0) {
        auto clock = std::chrono::system_clock::now();
        std::time_t current_time = std::chrono::system_clock::to_time_t(clock);
        std::string time_str=std::ctime(&current_time);
        time_str.resize(time_str.length()-1);
        fprintf(error_log,"%s %s\n",time_str.c_str(),outputstring.c_str());
        fflush(error_log);
    }
    return sf::Vector2u(currentposition.x%conwidth,currentposition.y+currentposition.x/conwidth);
}


//OUTPUTINFOLOW
template<typename T>
class OutputInfoLow: public OutputInfo {
public:
    ~OutputInfoLow() {}
    OutputInfoLow(const std::string& _name,const std::string &_unit, const bool& _low)
        :OutputInfo(_name,_unit),
        low(_low)
    {}
    void set_value(const T& t) {
        if (t>highest_value) highest_value=t;
        if (t<lowest_value) lowest_value=t;
        value=std::to_string(t);
    }

    sf::Vector2u print(sf::Vector2u currentposition, int conwidth) {
        if (currentposition.x!=0) {
            currentposition.x=0;
            currentposition.y++;
        }
        std::string outputstring;
        if (low) {
            outputstring=name+"="+std::to_string(lowest_value)+unit;
            lowest_value=highest_value;
        }
        else {
            outputstring=name+"="+std::to_string(highest_value)+unit;
            lowest_value=lowest_value;
        }
        gotoxycm(currentposition.x,currentposition.y);
        currentposition.x+=outputstring.size();
        std::cout<<outputstring;
        return sf::Vector2u(currentposition.x%conwidth,currentposition.y+currentposition.x/conwidth);
    }
private:
    T lowest_value={};
    T highest_value={};
    bool low;

};

ConsoleManager::ConsoleManager()
{
    error_log=fopen("error_log.txt","a");
    rlutil::cls();
    //std::cout.sync_with_stdio(false); messes up with rlutil
    OutputInfoLow<double> _fps("fps","hz",true);
    OutputInfo& fps=add_value(_fps);
    fps.set_value<double>(0);
    fps.set_position({0,0});

    OutputInfoLow<sf::Int64> _input("input delay","microseconds",false);
    OutputInfo& input_delay=add_value(_input);
    input_delay.set_value<sf::Int64>(0);
    input_delay.set_position({0,1});

    OutputInfoLow<sf::Int64> _render("render delay","microseconds",false);
    OutputInfo& render_delay=add_value(_render);
    render_delay.set_value<sf::Int64>(0);
    render_delay.set_position({0,2});

    OutputInfoLow<sf::Int64> _draw("draw delay","microseconds",false);
    OutputInfo& draw_delay=add_value(_draw);
    draw_delay.set_value<sf::Int64>(0);
    draw_delay.set_position({0,3});

    OutputInfoLow<sf::Int64> _display("display delay","microseconds",false);
    OutputInfo& display_delay=add_value(_display);
    display_delay.set_value<sf::Int64>(0);
    display_delay.set_position({0,4});

    OutputInfo& error=add_value("error","");
    error.set_value<int>(0);
    error.set_position({0,5});

    OutputInfo& system=add_value("system","");
    system.set_value<int>(0);
    system.set_position({0,6});
}

OutputInfo& ConsoleManager::add_value(std::string info, std::string unit) {
    lowercase_str(info);
    lowercase_str(unit);
    OutputInfo fpsinfo(info,unit);
    CMmap[info]=fpsinfo;
    return CMmap[info];
}

OutputInfo& ConsoleManager::add_value(const OutputInfo& outputinfo) {
    CMmap[outputinfo.name]=outputinfo;
    return CMmap[outputinfo.name];
}


void ConsoleManager::print(bool always_print) {
    if (framecounter++%4==0||always_print) {
        //rlutil::cls();
        sf::Vector2u pos={0,0};
        for (auto info:CMmap) {
            pos=info.second.print(pos,rlutil::tcols(),error_log);
        }
    }
}


ConsoleManager::~ConsoleManager() {
    if (error_log)
        fclose(error_log);
}
/*template<>
void ConsoleManager::OutputInfo::set_value<std::__cxx11::basic_string<char> >(const std::__cxx11::basic_string<char>& t) {
}*/

