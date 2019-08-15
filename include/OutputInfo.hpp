#ifndef OUTPUTINFO_H
#define OUTPUTINFO_H
#include<string>
#include<list>
#include<SFML/System/Vector2.hpp>
#include<SFML/System/Time.hpp>
#include"ntris.hpp"
#include<vector>

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
    virtual void set_value(const largest_uint& t);
    virtual void set_value(const sf::Int64& t);
    virtual void set_value(const int& t);
    virtual void set_value(const double& t);
    virtual void set_value(const long double& t);
    virtual std::vector<std::string> print(bool const& clear = true);
	virtual std::string print_complete(bool const& clear = true);
};

//OUTPUTINFOLOW
class OutputInfoLowDouble: public OutputInfo {
public:
    ~OutputInfoLowDouble();
    OutputInfoLowDouble(const std::string& _name,const std::string &_unit, const bool& _low); //if _low==true stores the lowest only
    void set_value(const double& t);
    void set_value(const long double& t);
	std::vector<std::string> print(bool const& clear = true);
    std::string print_complete(bool const& clear = true);
private:
    bool low;
    std::list<long double> value_list;
};

class OutputInfoLowI64: public OutputInfo {
public:
    ~OutputInfoLowI64();
    OutputInfoLowI64(const std::string& _name,const std::string &_unit, const bool& _low); //if _low==true stores the lowest only
    void set_value(const sf::Int64& t);
    std::vector<std::string> print(bool const& clear = true);
    std::string print_complete(bool const& clear = true);
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
    virtual std::vector<std::string> print(bool const& clear = true);
    virtual std::string print_complete(bool const& clear = true);
private:
    std::size_t max_error_number=1;
    std::list<std::string> error_list;
};


#endif // OUTPUTINFO_H
