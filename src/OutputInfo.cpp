#include"OutputInfo.hpp"
#include<algorithm>
#include<sstream>

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
void OutputInfo::set_value(const largest_uint& t)
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
void OutputInfo::set_value(const long double& t)
{
    value=ntris::to_string(t);
}
std::string OutputInfo::print(bool const& clear)
{
    std::string outputstring=name+"="+value+unit;
    if (clear) value="";
	return outputstring;
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

void OutputInfoLowDouble::set_value(const long double& t) {
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

std::string OutputInfoLowDouble::print(bool const& clear) {
    std::string outputstring;
    long double print_value={};
    if (value_list.size()>0) {
        if (low) print_value=*std::min_element(value_list.cbegin(), value_list.cend());
        else print_value=*std::max_element(value_list.cbegin(), value_list.cend());
    }
    outputstring=name+"="+ntris::to_string(print_value)+unit;
    if (clear) value_list.clear();
	return outputstring;
}

std::string OutputInfoLowI64::print(bool const& clear) {
    std::string outputstring;
    sf::Int64 print_value={};
    if (value_list.size()>0) {
        if (low) print_value=*std::min_element(value_list.cbegin(), value_list.cend());
        else print_value=*std::max_element(value_list.cbegin(), value_list.cend());
    }
    outputstring=name+"="+ntris::to_string(print_value)+unit;
	if (clear) value_list.clear();
	return outputstring;
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

std::string OutputInfoError::print(bool const& clear) {
	std::stringstream outputstring;
	outputstring << name <<"=";
    for (const auto& i:error_list) {
        outputstring<<"\t\t\t"<< i<<glb::newline;
    }
	if (clear && !error_list.empty()) error_list.pop_front();
	return outputstring.str();
}

