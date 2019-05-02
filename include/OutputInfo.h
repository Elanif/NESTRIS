#ifndef OUTPUTINFO_H
#define OUTPUTINFO_H
#include<string>
#include<list>
#include<SFML/System/Vector2.hpp>
#include<SFML/System/Time.hpp>

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
    virtual void set_value(const std::string& t);
    virtual void set_value(const unsigned int& t);
    virtual void set_value(const unsigned long long& t);
    virtual void set_value(const sf::Int64& t);
    virtual void set_value(const int& t);
    virtual void set_value(const double& t);
    virtual void set_value(const long double& t);
    virtual sf::Vector2u print(sf::Vector2u currentposition, unsigned int conwidth);
};

//OUTPUTINFOLOW
class OutputInfoLowDouble: public OutputInfo {
public:
    ~OutputInfoLowDouble();
    OutputInfoLowDouble(const std::string& _name,const std::string &_unit, const bool& _low); //if _low==true stores the lowest only
    void set_value(const double& t);
    void set_value(const long double& t);
    sf::Vector2u print(sf::Vector2u currentposition, unsigned int conwidth);
private:
    bool low;
    std::list<long double> value_list;
};

class OutputInfoLowI64: public OutputInfo {
public:
    ~OutputInfoLowI64();
    OutputInfoLowI64(const std::string& _name,const std::string &_unit, const bool& _low); //if _low==true stores the lowest only
    void set_value(const sf::Int64& t);
    sf::Vector2u print(sf::Vector2u currentposition, unsigned int conwidth);
private:
    bool low;
    std::list<sf::Int64> value_list;
};
//OUTPUTINFOERROR
class OutputInfoError: public OutputInfo {
public:
    ~OutputInfoError();
    OutputInfoError(const std::string& _name, const std::size_t& _max_error_number=1); //if _low==true stores the lowest only
    void set_value(char const* const& t);
    void set_value(std::string const& t);
    virtual sf::Vector2u print(sf::Vector2u currentposition, unsigned int conwidth);
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
