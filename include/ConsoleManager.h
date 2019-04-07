#ifndef CONSOLEMANAGER_H
#define CONSOLEMANAGER_H
#include<vector>
#include<unordered_map>
#include<string>
#include<SFML/System/NonCopyable.hpp>
#include<SFML/System/Vector2.hpp>
#include<utility>
#include"rlutil.h"
/*
template<typename T>
std::string set_value_priv(const T& t){
    return std::to_string(t);
}

typedef std::__cxx11::basic_string<char> string_literal;
template<>
std::string set_value_priv(const string_literal& t){
    return std::string(t);
}
typedef char* string_literal;
template<>
std::string set_value_priv(const string_literal& t){
    return std::string(t);
}*/
class OutputInfo
{
public:
    std::string name;
    std::string value;
    std::string unit;
    std::size_t last_render_length;
    sf::Vector2u outputposition;
    OutputInfo();
    virtual ~OutputInfo();
    OutputInfo(const std::string& _name,const std::string &_unit);
    void set_value(const std::__cxx11::basic_string<char>& t);
    template<typename T>
    void set_value(const T& t)
    {
        value=std::to_string(t);
    }

    void set_position(const sf::Vector2u& _position);
    virtual sf::Vector2u print(sf::Vector2u currentposition, int conwidth, FILE* error_log=NULL);

};

template<>
inline void OutputInfo::set_value<std::string>(const std::string& t) {
    value=t;
}

typedef const char* const_string_literal;
template<>
inline void OutputInfo::set_value<const_string_literal>(const const_string_literal& t) {
    value=std::string(t);
}

typedef char* string_literal;
template<>
inline void OutputInfo::set_value<string_literal>(const string_literal& t) {
    value=std::string(t);
}

class ConsoleManager : public sf::NonCopyable
{
    public:
        ConsoleManager();
        template<class T>
        void update(std::string info, const T& t) {
            if (CMmap.find(info)==CMmap.end()) {
                //error message somewhere
            }
            else {
                CMmap[info].set_value<T>(t);
                if (info==std::string("error")) {
                    print();
                }
            }
        }
        void print();
        ~ConsoleManager();

    private:
        unsigned char framecounter=0;

        OutputInfo& add_value(std::string info, std::string unit);
        OutputInfo& add_value(const OutputInfo& outputinfo);
        std::unordered_map<std::string, OutputInfo> CMmap;
        FILE* error_log;
};
#endif // CONSOLEMANAGER_H
