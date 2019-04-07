#ifndef CONSOLEMANAGER_H
#define CONSOLEMANAGER_H
#include<vector>
#include<unordered_map>
#include<string>
#include<SFML/System/NonCopyable.hpp>
#include<SFML/System/Vector2.hpp>
#include<utility>
#include<memory>
#include"rlutil.h"
#include"OutputInfo.h"

class ConsoleManager : public sf::NonCopyable
{
    public:
        ConsoleManager();
        template<class T>
        void update(std::string info, const T& t) ;
        void update(std::string info, const char* const& t) ;
        template<class T>
        void update_error(const T& t) ;
        void update_error(const char* t);
        void print(bool always_print=false);
        ~ConsoleManager();

    private:
        unsigned char framecounter=0;
        bool error_print=false;
        std::size_t add_value(std::string info, std::string unit);
        std::size_t add_value(std::unique_ptr<OutputInfo>&& outputinfo);
        std::vector<std::unique_ptr<OutputInfo> > CMvector;
        std::unordered_map<std::string, std::size_t> CMmap;
        FILE* error_log;
};
#endif // CONSOLEMANAGER_H
