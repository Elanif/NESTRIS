#include "ConsoleManager.hpp"
#include<chrono>
#include<ctime>
#include<iostream>
#include<SFML/System/Sleep.hpp>
#include<sstream>
#include<limits>

unsigned char ConsoleManager::framecounter=0;
bool ConsoleManager::error_print=false;
std::vector<std::unique_ptr<OutputInfo> > ConsoleManager::CMvector;
std::unordered_map<std::string, std::size_t> ConsoleManager::CMmap;
FILE* ConsoleManager::error_log;

template <typename Duration>
void print_time(FILE* output, tm t, Duration fraction) {
    using namespace std::chrono;
    std::fprintf(output,"%04u-%02u-%02u %02u:%02u:%02u.%03u ", t.tm_year + 1900,
                t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec,
                static_cast<unsigned>(fraction / milliseconds(1)));
}

void ConsoleManager::print_error(const std::string& error_string) {
    print_error(error_string.c_str());
}
void ConsoleManager::print_error(const char* error_string) {
    auto clock = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(clock);
    using namespace std::chrono;
    system_clock::time_point now = system_clock::now();
    system_clock::duration tp = now.time_since_epoch();
    tp -= duration_cast<seconds>(tp);
    print_time(error_log,*localtime(&current_time), tp);
    fprintf(error_log,"%s\n",error_string);
    fflush(error_log);
}

void lowercase_str(std::string& str) { //TODO make it portable with 16bitchar
    for(auto& c : str)
    {
       c = std::tolower(c);
    }
}

template<typename T>
void ConsoleManager::update(std::string info, const T& t) {
    if (CMmap.find(info)==CMmap.end()) {
        print_error(std::string("No OutpufInfo of type ")+info+std::string(" found"));
    }
    else {
        CMvector[CMmap[info]]->set_value(t);
    }
}
template void ConsoleManager::update(std::string info, const std::string& t);
template void ConsoleManager::update(std::string info, char const* const& t);
template void ConsoleManager::update(std::string info, const int& t);
template void ConsoleManager::update(std::string info, const std::size_t& t);
template void ConsoleManager::update(std::string info, const unsigned long long& t);
template void ConsoleManager::update(std::string info, const long long& t);
template void ConsoleManager::update(std::string info, const double& t);
template void ConsoleManager::update(std::string info, const long double& t);

template<typename T>
void ConsoleManager::update_error(const T& t) {
    error_print=true;
    if (CMmap.find("error")==CMmap.end()) {
        print_error("No OutputInfo of type error found");
    }
    else {
        std::string error_string=ntris::to_string(t);
        if (error_string.length()>0 && error_string!="0") {
            print_error(error_string);
        }
        CMvector[CMmap["error"]]->set_value(error_string);
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
template void ConsoleManager::update_error(const long double& t);


void ConsoleManager::init()
{
    error_log=fopen("error_log.txt","a");
    rlutil::cls();
    //std::cout.sync_with_stdio(false); messes up with rlutil
    std::unique_ptr<OutputInfo> _fps(new OutputInfoLowDouble("fps","hz",true));
    std::unique_ptr<OutputInfo>& fps=CMvector[add_value(std::move(_fps))];
    fps->set_value(0);

    std::unique_ptr<OutputInfo> _input(new OutputInfoLowI64("input delay","microseconds",false));
    std::unique_ptr<OutputInfo>& input_delay=CMvector[add_value(std::move(_input))];
    input_delay->set_value(0);

    std::unique_ptr<OutputInfo> _render(new OutputInfoLowI64("render delay","microseconds",false));
    std::unique_ptr<OutputInfo>& render_delay=CMvector[add_value(std::move(_render))];
    render_delay->set_value(0);

    std::unique_ptr<OutputInfo> _draw(new OutputInfoLowI64("draw delay","microseconds",false));
    std::unique_ptr<OutputInfo>& draw_delay=CMvector[add_value(std::move(_draw))];
    draw_delay->set_value(0);

    std::unique_ptr<OutputInfo> _display(new OutputInfoLowI64("display delay","microseconds",false));
    std::unique_ptr<OutputInfo>& display_delay=CMvector[add_value(std::move(_display))];
    display_delay->set_value(0);

    std::unique_ptr<OutputInfo>& system=CMvector[add_value("system","")];
    system->set_value("");

    std::unique_ptr<OutputInfo> _error(new OutputInfoError("error",8));
    add_value(std::move(_error));
    //OutputInfoError error("error", 3);
    //add_value(error);

}

std::size_t ConsoleManager::add_value(std::string info, std::string unit) {
    lowercase_str(info);
    lowercase_str(unit);
    std::unique_ptr<OutputInfo> fpsinfo=std::make_unique<OutputInfo>(info,unit);
    return add_value(std::move(fpsinfo));
}

std::size_t ConsoleManager::add_value(std::unique_ptr<OutputInfo>&& outputinfo) {
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
            int conwidth=rlutil::tcols();
            if (conwidth<0) conwidth=std::numeric_limits<int>::max();
            pos=info->print(pos,conwidth);

        }
        error_print=false;
    }
}


ConsoleManager::~ConsoleManager() {
    if (error_log)
        fclose(error_log);
}
