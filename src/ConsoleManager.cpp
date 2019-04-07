#include "ConsoleManager.h"
#include<iostream>
#include<SFML/System/Sleep.hpp>
#include<sstream>

void lowercase_str(std::string& str) { //TODO make it portable with 16bitchar
    for(auto& c : str)
    {
       c = std::tolower(c);
    }
}

template<typename T>
void ConsoleManager::update(std::string info, const T& t) {
    if (CMmap.find(info)==CMmap.end()) {
        //error message somewhere
    }
    else {
        CMvector[CMmap[info]]->set_value<T>(t);
    }
}
void ConsoleManager::update(std::string info, const char* const& t) {
    if (CMmap.find(info)==CMmap.end()) {
        //error message somewhere
    }
    else {
        CMvector[CMmap[info]]->set_value(t);
    }
}
template void ConsoleManager::update(std::string info, const std::string& t);
template void ConsoleManager::update(std::string info, const int& t);
template void ConsoleManager::update(std::string info, const std::size_t& t);
template void ConsoleManager::update(std::string info, const unsigned long long& t);
template void ConsoleManager::update(std::string info, const long long& t);
template void ConsoleManager::update(std::string info, const double& t);

template<typename T>
void ConsoleManager::update_error(const T& t) {
    error_print=true;
    if (CMmap.find("error")==CMmap.end()) {
        fprintf(error_log, "No OutputInfo of type error found\n");
        fflush(error_log);
    }
    else {
            //TODO ERROR REPORTING
         /*if (to_string(t).length()>0) {
            auto clock = std::chrono::system_clock::now();
            std::time_t current_time = std::chrono::system_clock::to_time_t(clock);
            using namespace std::chrono;
            system_clock::time_point now = system_clock::now();
            system_clock::duration tp = now.time_since_epoch();
            tp -= duration_cast<seconds>(tp);
            print_time(error_log,*localtime(&current_time), tp);
            fprintf(error_log,"%s\n",outputstring.c_str());
            fflush(error_log);
        }*/
        CMvector[CMmap["error"]]->set_value<T>(t);
    }
}
void ConsoleManager::update_error(const char* t) {
    update_error<std::string>(std::string(t));
}
template void ConsoleManager::update_error(const std::string& t);
template void ConsoleManager::update_error(const int& t);
template void ConsoleManager::update_error(const std::size_t& t);
template void ConsoleManager::update_error(const unsigned long long& t);
template void ConsoleManager::update_error(const long long& t);
template void ConsoleManager::update_error(const double& t);


ConsoleManager::ConsoleManager()
{
    error_log=fopen("error_log.txt","a");
    rlutil::cls();
    //std::cout.sync_with_stdio(false); messes up with rlutil
    std::unique_ptr<OutputInfo> _fps(new OutputInfoLow<double>("fps","hz",true));
    std::unique_ptr<OutputInfo>& fps=CMvector[add_value(_fps)];
    fps->set_value<double>(0);

    std::unique_ptr<OutputInfo> _input(new OutputInfoLow<sf::Int64>("input delay","microseconds",false));
    std::unique_ptr<OutputInfo>& input_delay=CMvector[add_value(_input)];
    input_delay->set_value<sf::Int64>(0);

    std::unique_ptr<OutputInfo> _render(new OutputInfoLow<sf::Int64>("render delay","microseconds",false));
    std::unique_ptr<OutputInfo>& render_delay=CMvector[add_value(_render)];
    render_delay->set_value<sf::Int64>(0);

    std::unique_ptr<OutputInfo> _draw(new OutputInfoLow<sf::Int64>("draw delay","microseconds",false));
    std::unique_ptr<OutputInfo>& draw_delay=CMvector[add_value(_draw)];
    draw_delay->set_value<sf::Int64>(0);

    std::unique_ptr<OutputInfo> _display(new OutputInfoLow<sf::Int64> ("display delay","microseconds",false));
    std::unique_ptr<OutputInfo>& display_delay=CMvector[add_value(_display)];
    display_delay->set_value<sf::Int64>(0);

    std::unique_ptr<OutputInfo>& system=CMvector[add_value("system","")];
    system->set_value<int>(0);

    std::unique_ptr<OutputInfo> _error(new OutputInfoError("error",3));
    add_value(_error);
    //OutputInfoError error("error", 3);
    //add_value(error);

}

std::size_t ConsoleManager::add_value(std::string info, std::string unit) {
    lowercase_str(info);
    lowercase_str(unit);
    std::unique_ptr<OutputInfo> fpsinfo=std::make_unique<OutputInfo>(info,unit);
    return add_value(fpsinfo);
}

std::size_t ConsoleManager::add_value(std::unique_ptr<OutputInfo>& outputinfo) {
    lowercase_str(outputinfo->name);
    std::size_t CMvector_size=CMvector.size();
    bool push_back_error=false;
    try {
        CMvector.push_back(std::move(outputinfo));
    }
    catch (std::exception const& bad_alloc_exception) {
        push_back_error=true;
        fprintf(error_log, "OutputInfo pushback exception: %s\n",bad_alloc_exception.what());
        fflush(error_log);
    }
    if (!push_back_error && CMvector_size==CMvector.size()) {
        push_back_error=true;
        fprintf(error_log, "Outputinfo pushback failed\n");
        fflush(error_log);
    }
    if (push_back_error) return (std::size_t) -1;
    else {
        CMmap[CMvector[CMvector.size()-1]->name]=CMvector.size()-1;
        return CMvector.size()-1;
    }
}


void ConsoleManager::print(bool always_print) {
    if (framecounter++%32==0||always_print||error_print) {
        //rlutil::cls();
        sf::Vector2u pos={0,0};
        for (auto& info:CMvector) {
            if (pos.x>0) {
                pos.x=0;
                pos.y++;
            }
            /*if (info.name=="error") {
                pos=static_cast<OutputInfoError&>(info).print(pos,rlutil::tcols());
            }
            else*/ pos=info->print(pos,rlutil::tcols());

        }
        error_print=false;
    }
}


ConsoleManager::~ConsoleManager() {
    if (error_log)
        fclose(error_log);
}
