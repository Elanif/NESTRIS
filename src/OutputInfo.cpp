#include "OutputInfo.h"
#include<chrono>
#include<ctime>
#include<rlutil.h>

void gotoxycm(std::size_t x, std::size_t y) {
    gotoxy(x+1,y+1);
}

OutputInfo::OutputInfo():name(""),value(""),unit(""),last_render_length(0) {}
OutputInfo::~OutputInfo() {}
OutputInfo::OutputInfo(const std::string& _name,const std::string &_unit)
    :name(_name),
     unit(_unit),
     last_render_length(0)
{}

template<typename T>
void OutputInfo::set_value(const T& t)
{
    value=ntris::to_string(t);
}
void OutputInfo::set_value(const char* const& t)
{
    value=ntris::to_string(t);
}
template void OutputInfo::set_value<double>(const double& t);
template void OutputInfo::set_value<int>(const int& t);
template void OutputInfo::set_value<unsigned int>(const unsigned int& t);
template void OutputInfo::set_value<long long>(const long long& t);
template void OutputInfo::set_value<unsigned long long>(const unsigned long long& t);

template <typename Duration>
void print_time(FILE* output, tm t, Duration fraction) {
    using namespace std::chrono;
    std::fprintf(output,"%04u-%02u-%02u %02u:%02u:%02u.%03u ", t.tm_year + 1900,
                t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec,
                static_cast<unsigned>(fraction / milliseconds(1)));
}
sf::Vector2u OutputInfo::print(sf::Vector2u currentposition, int conwidth, FILE*error_log)
{
    std::string outputstring=name+"="+value+unit;
    gotoxycm(currentposition.x,currentposition.y);
    currentposition.x+=outputstring.size();
    std::cout<<outputstring;
    if (name==std::string("error")&&value!=std::string("0")&&value.length()>0) {
        auto clock = std::chrono::system_clock::now();
        std::time_t current_time = std::chrono::system_clock::to_time_t(clock);
        using namespace std::chrono;
        system_clock::time_point now = system_clock::now();
        system_clock::duration tp = now.time_since_epoch();
        tp -= duration_cast<seconds>(tp);
        print_time(error_log,*localtime(&current_time), tp);
        fprintf(error_log,"%s\n",outputstring.c_str());
        fflush(error_log);
    }
    value="";
    return sf::Vector2u(currentposition.x%conwidth,currentposition.y+currentposition.x/conwidth);
}


//OUTPUTINFOLOW
template<typename T>
OutputInfoLow<T>::~OutputInfoLow() {}

template<typename T>
OutputInfoLow<T>::OutputInfoLow(const std::string& _name,const std::string &_unit, const bool& _low)
    :OutputInfo(_name,_unit),
    low(_low)
{}

template<typename T>
void OutputInfoLow<T>::set_value(const T& t) {
    if (t>highest_value) highest_value=t;
    if (t<lowest_value) lowest_value=t;
    value=std::to_string(t);
}

template<typename T>
sf::Vector2u OutputInfoLow<T>::print(sf::Vector2u currentposition, int conwidth) {
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

template class OutputInfoLow<int>;
template class OutputInfoLow<double>;
template class OutputInfoLow<long long>;
template class OutputInfoLow<unsigned long long>;

//OUTPUTINFOERROR
OutputInfoError::~OutputInfoError() {}

OutputInfoError::OutputInfoError(const std::string& _name, const std::size_t& _max_error_number)
    :OutputInfo(_name,""),
    max_error_number(_max_error_number)
{}

template<typename T>
void OutputInfoError::set_value(const T& t) {
    while (current_errors_stored>=max_error_number) {
        error_list.pop_front();
    }
    error_list.push_back(ntris::to_string(t));
}

void OutputInfoError::set_value(const char *t) {
    set_value<std::string>(std::string(t));
}

template void OutputInfoError::set_value<char const*>(char const* const& t) ;
template void OutputInfoError::set_value<std::string>(const std::string& t) ;
template void OutputInfoError::set_value<double>(const double& t) ;
template void OutputInfoError::set_value<int>(const int& t) ;
template void OutputInfoError::set_value<unsigned int>(const unsigned int& t) ;
template void OutputInfoError::set_value<long long>(const long long& t) ;
template void OutputInfoError::set_value<unsigned long long>(const unsigned long long& t) ;

sf::Vector2u OutputInfoError::print(sf::Vector2u currentposition, int conwidth) {
    std::cout<<name<<"=";
    currentposition.x+=name.length()+1;
    if ((int)currentposition.x>conwidth) {
        long long y_shift=currentposition.x%conwidth;
        currentposition.y+=(y_shift>0?y_shift:1);
        currentposition.x-=name.length()+1;
    }
    for (const auto& i:error_list) {
        gotoxycm(currentposition.x,currentposition.y);
        std::cout<<i;
        long long y_shift=(currentposition.x+i.length())%conwidth;
        currentposition.y+=(y_shift>0?y_shift:1);
    }
    return sf::Vector2u(currentposition.x%conwidth,currentposition.y+currentposition.x/conwidth);
}

