#include "ConsoleManager.h"
#include<rlutil.h>
#include<iostream>
void lowercase_str(std::string& str) { //TODO make it portable
    for(auto& c : str)
    {
       c = std::tolower(c);
    }
}

ConsoleManager::ConsoleManager()
:maxinfo(0)
{
    add_value("fps",{0,0});
    add_value("draw delay",{0,1});
}

void ConsoleManager::add_value(std::string info) {
    lowercase_str(info);
    CMmap[info]=maxinfo++;
    CMpos.push_back(position);
}

template<class T>
void ConsoleManager::update(std::string info, T t) {
    Info<T> tempinfo(info,t);
    if (CMmap.find(info)==CMmap.end()) {
        //error message somewhere
    }
    else {
        sf::Vector2u printposition=CMpos[CMmap[info]];
        std::cout<<tempinfo.value;
    }
}

void ConsoleManager::print() {
    fflush(stdout);
    for (auto info:CMmap) {

        //gotoxy(printposition.x,printposition.y);
    }
}
