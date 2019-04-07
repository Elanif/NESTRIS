#ifndef CONSOLEMANAGER_H
#define CONSOLEMANAGER_H
#include<vector>
#include<unordered_map>
#include<string>
#include<SFML/System/NonCopyable.hpp>
#include<SFML/System/Vector2.hpp>
#include<utility>



class ConsoleManager : public sf::NonCopyable
{
    public:
        ConsoleManager();
        template<class T>
        void update(std::string info, T t);
        void print();

    private:
        size_t maxinfo;
        void add_value(std::string info, sf::Vector2u position);
        std::unordered_map<std::string, size_t> CMmap;
        std::vector<sf::Vector2u> CMpos;

    template<class T>
    class OutputInfo{
        public:
        std::string name;
        std::string value;
        T originalvalue;
        sf::Vector2u outputposition;

        Info(const std::string& _name)
        :name(_name)
        {}
        set_value(const T& t){
            originalvalue=t;
            value=std::stoi(t);
        }
        set_position(const sf::Vector2u& _position) {
            outputposition=_position
        }
    };
};

#endif // CONSOLEMANAGER_H
