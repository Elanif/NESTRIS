#ifndef CONFIGREADER_H
#define CONFIGREADER_H
#include<string>
#include<unordered_map>
#include<vector>
#include<tuple>
#include<fstream>

class ConfigReader
{
    public:
        ConfigReader(const std::string& file_location);
        bool open(const std::string& file_location);
        void elaborate();
        template <typename T>
        std::vector<T> get(const std::string& name);
        template <typename T>
        std::vector<T> get(char* name);
        bool close();
        void reload();
        void print();

    private:
        std::vector<std::string> line_vector;
        std::unordered_map<std::string, std::vector<std::string> > name_map;
        std::ifstream file_data;
        bool is_open=false;
        bool is_loaded=false;
};

#endif // CONFIGREADER_H
