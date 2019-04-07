#include "OutputInfo.h"
#include<rlutil.h>
#include<algorithm>
void gotoxycm(std::size_t x, std::size_t y) {
    gotoxy(x+1,y+1);
}
void gotoxycm(const sf::Vector2u& _pos) {
    gotoxy(_pos.x+1,_pos.y+1);
}

OutputInfo::OutputInfo():name(""),value(""),unit(""),last_render_length(0) {}
OutputInfo::~OutputInfo() {}
OutputInfo::OutputInfo(const std::string& _name,const std::string &_unit)
    :name(_name),
     unit(_unit),
     last_render_length(0)
{}

void OutputInfo::set_value(const char* const& t)
{
    value=ntris::to_string(t);
}
void OutputInfo::set_value(std::string const& t)
{
    value=ntris::to_string(t);
}
void OutputInfo::set_value(const unsigned int& t)
{
    value=ntris::to_string(t);
}
void OutputInfo::set_value(const unsigned long long& t)
{
    value=ntris::to_string(t);
}
void OutputInfo::set_value(const sf::Int64& t)
{
    value=ntris::to_string(t);
}
void OutputInfo::set_value(const int& t)
{
    value=ntris::to_string(t);
}
void OutputInfo::set_value(const double& t)
{
    value=ntris::to_string(t);
}
sf::Vector2u OutputInfo::print(sf::Vector2u currentposition, unsigned int conwidth)
{
    if (currentposition.x>0) {
        currentposition.x=0;
        currentposition.y++;
    }
    gotoxycm(currentposition);
    std::string outputstring=name+"="+value+unit;
    currentposition.x+=outputstring.size();
    std::cout<<outputstring;
    value="";
    return sf::Vector2u(currentposition.x%conwidth,currentposition.y+currentposition.x/conwidth);
}


//OUTPUTINFOLOW
OutputInfoLowDouble::~OutputInfoLowDouble() {}

OutputInfoLowDouble::OutputInfoLowDouble(const std::string& _name,const std::string &_unit, const bool& _low)
    :OutputInfo(_name,_unit),
    low(_low)
{}

void OutputInfoLowDouble::set_value(const double& t) {
    value_list.push_front(t);
}

OutputInfoLowI64::~OutputInfoLowI64() {}

OutputInfoLowI64::OutputInfoLowI64(const std::string& _name,const std::string &_unit, const bool& _low)
    :OutputInfo(_name,_unit),
    low(_low)
{}

void OutputInfoLowI64::set_value(const sf::Int64& t) {
    value_list.push_front(t);
}

sf::Vector2u OutputInfoLowDouble::print(sf::Vector2u currentposition, unsigned int conwidth) {
    if (currentposition.x>0) {
        currentposition.x=0;
        currentposition.y++;
    }
    gotoxycm(currentposition);
    std::string outputstring;
    double print_value={};
    if (value_list.size()>0) {
        if (low) print_value=*std::min_element(value_list.cbegin(), value_list.cend());
        else print_value=*std::max_element(value_list.cbegin(), value_list.cend());
    }
    outputstring=name+"="+ntris::to_string(print_value)+unit;
    gotoxycm(currentposition.x,currentposition.y);
    currentposition.x+=outputstring.size();
    std::cout<<outputstring;
    value_list.clear();
    return sf::Vector2u(currentposition.x%conwidth,currentposition.y+currentposition.x/conwidth);
}

sf::Vector2u OutputInfoLowI64::print(sf::Vector2u currentposition, unsigned int conwidth) {
    if (currentposition.x>0) {
        currentposition.x=0;
        currentposition.y++;
    }
    gotoxycm(currentposition);
    std::string outputstring;
    sf::Int64 print_value={};
    if (value_list.size()>0) {
        if (low) print_value=*std::min_element(value_list.cbegin(), value_list.cend());
        else print_value=*std::max_element(value_list.cbegin(), value_list.cend());
    }
    outputstring=name+"="+ntris::to_string(print_value)+unit;
    gotoxycm(currentposition.x,currentposition.y);
    currentposition.x+=outputstring.size();
    std::cout<<outputstring;
    value_list.clear();
    return sf::Vector2u(currentposition.x%conwidth,currentposition.y+currentposition.x/conwidth);
}

//OUTPUTINFOERROR
OutputInfoError::~OutputInfoError() {}

OutputInfoError::OutputInfoError(const std::string& _name, const std::size_t& _max_error_number)
    :OutputInfo(_name,""),
    max_error_number(_max_error_number)
{}

void OutputInfoError::set_value(char const* const& t) {
    while (error_list.size()>=max_error_number) {
        error_list.pop_front();
    }
    error_list.push_back(ntris::to_string(t));
}

void OutputInfoError::set_value(const std::string& t) {
    while (error_list.size()>=max_error_number) {
        error_list.pop_front();
    }
    error_list.push_back(ntris::to_string(t));
}

sf::Vector2u OutputInfoError::print(sf::Vector2u currentposition, unsigned int conwidth) {
    if (currentposition.x>0) {
        currentposition.x=0;
        currentposition.y++;
    }
    gotoxycm(currentposition);
    std::cout<<name<<"=";
    currentposition.x+=name.length()+1;
    if (currentposition.x>conwidth) {
        long long y_shift=currentposition.x/conwidth;
        currentposition.y+=(y_shift>0?y_shift:1);
        currentposition.x-=name.length()+1;
    }
    for (const auto& i:error_list) {
        gotoxycm(currentposition.x,currentposition.y);
        std::cout<<i;
        unsigned int y_shift=(currentposition.x+i.length())/conwidth;
        currentposition.y+=y_shift>0?y_shift:1;
    }
    return sf::Vector2u(currentposition.x%conwidth,currentposition.y+currentposition.x/conwidth);
}

