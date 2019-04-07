#ifndef CONSOLEMANAGER_H
#define CONSOLEMANAGER_H
#include<vector>
#include<unordered_map>
#include<string>
#include<SFML/System/NonCopyable.hpp>
#include<SFML/System/Vector2.hpp>
#include<utility>
#include"rlutil.h"
#include"OutputInfo.h"
/*
template<typename T>
std::string set_value_priv(const T& t){
    return std::to_string(t);
}

typedef std::__cxx11::basic_string<char> glb::string_literal;
template<>
std::string set_value_priv(const glb::string_literal& t){
    return std::string(t);
}
typedef char* glb::string_literal;
template<>
std::string set_value_priv(const glb::string_literal& t){
    return std::string(t);
}*/


class ConsoleManager : public sf::NonCopyable
{
    public:
        ConsoleManager();
        template<class T>
        void update(std::string info, const T& t) ;
        void update(std::string info, const glb::const_string_literal& t) ;
        template<class T>
        void update_error(const T& t) ;
        void print(bool always_print=false);
        ~ConsoleManager();

    private:
        unsigned char framecounter=0;
        bool error_print=false;
        std::size_t add_value(std::string info, std::string unit);
        std::size_t add_value(const OutputInfo& outputinfo);
        std::vector<OutputInfo> CMvector;
        std::unordered_map<std::string, std::size_t> CMmap;
        FILE* error_log;
};
#endif // CONSOLEMANAGER_H
