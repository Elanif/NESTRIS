#ifndef OUTPUTINFO_H
#define OUTPUTINFO_H
#include<string>
#include<list>
#include<SFML/System/Vector2.hpp>

class OutputInfo
{
public:
    std::string name;
    std::string value;
    std::string unit;
    std::size_t last_render_length;
    OutputInfo();
    virtual ~OutputInfo();
    OutputInfo(const std::string& _name,const std::string &_unit);
    virtual void set_value(const char* const& t);
    virtual sf::Vector2u print(sf::Vector2u currentposition, int conwidth);

};

template<>
inline void OutputInfo::set_value<std::string>(const std::string& t) {
    value=t;
}

template<>
inline void OutputInfo::set_value<const char*>(const char* const& t) {
    value=std::string(t);
}

template<>
inline void OutputInfo::set_value<char*>(char* const& t) {
    value=std::string(t);
}

//OUTPUTINFOLOW
template<typename T>
class OutputInfoLow: public OutputInfo {
public:
    ~OutputInfoLow();
    OutputInfoLow(const std::string& _name,const std::string &_unit, const bool& _low); //if _low==true stores the lowest only
    void set_value(const T& t);
    //virtual void set_value(const char* t);
    sf::Vector2u print(sf::Vector2u currentposition, int conwidth);
private:
    T lowest_value={};
    T highest_value={};
    bool low;

};

class OutputInfoError: public OutputInfo {
public:
    ~OutputInfoError();
    OutputInfoError(const std::string& _name, const std::size_t& _max_error_number=1); //if _low==true stores the lowest only
    virtual void set_value(const char* t);
    virtual sf::Vector2u print(sf::Vector2u currentposition, int conwidth);
private:
    std::size_t max_error_number=1;
    std::list<std::string> error_list;
};

namespace ntris {
    template<typename T>
    std::string to_string(const T& t) {
        return std::to_string(t);
    }
    //TODO OVERLOAD?
    template<>
    inline std::string to_string<std::string>(const std::string& t) {
        return t;
    }
    template<>
    inline std::string to_string<const char*>(const char* const& t) {
        return std::string(t);
    }
}

#endif // OUTPUTINFO_H
