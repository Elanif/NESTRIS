#include "ConfigReader.hpp"
#include<functional>
#include<iostream>
#include<regex>

template<typename T>
T from_string(char* const& str) {
    return std::move(from_string<T>(std::string(str)));
}

template<typename T>
T from_string(const std::string& str) {
    return T(str);
}

template<>
std::string from_string<std::string>(const std::string& str) {
    return str;
}

template<>
int from_string<int>(const std::string& str) {
    int result=0;
    try {
        result=std::stoi(str);
    }
    catch(std::exception e) {
    }
    return result;
}

template<>
double from_string<double>(const std::string& str) {
    double result=0;
    try {
        result=std::stod(str);
    }
    catch(std::exception e) {
    }
    return result;
}

template<>
bool from_string<bool>(const std::string& str) {
    bool result=false;
    try {
        std::string _temp="";
        for (const auto& i:str) _temp+=(char)std::tolower(i);
        result=(_temp=="true");
    }
    catch(std::exception e) {
    }
    return result;
}

//https://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
std::istream& safeGetline(std::istream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}

ConfigReader::ConfigReader(const std::string& file_location) {
    is_open=open(file_location);
}

void ConfigReader::elaborate() {
    //for (decltype(line_vector.size()) i=0; i<line_vector.size();  ++i) {
    std::regex r_variable_name("^\\s*([^\\s\\=]*)\\s*\\=");
    std::regex r_variable_values("([^,]*),");
    std::regex r_variable_values_end("([^,]*)");
    for (auto& line_string: line_vector) {

        std::smatch matches;
        std::regex_search(line_string, matches, r_variable_name);

        if (!matches.empty()) {

            std::string variable_name=matches[1];
            if (name_map.find(variable_name)==name_map.end()) {
                name_map[variable_name]=std::vector<std::string>();
            }
            std::vector<std::string>& value_vector_ref=name_map[variable_name];

            std::smatch value_matches;

            decltype(matches[0].second) start_it=matches[0].second;
            while (std::regex_search(start_it,line_string.cend(),value_matches,r_variable_values)) {

                value_vector_ref.push_back(value_matches[1]);

                start_it=value_matches[0].second;
            }
            if (std::regex_search(start_it,line_string.cend(),value_matches,r_variable_values_end))
            {
                value_vector_ref.push_back(value_matches[1]);
            }

        }

    }
}

bool ConfigReader::open(const std::string& file_location) {
    file_data.open(file_location.c_str(), std::ios::in);
    is_open=file_data.is_open();
    is_loaded=file_data.good()&&!file_data.fail()&&!file_data.bad();

    std::string line="";
    while(!safeGetline(file_data, line).eof()) {
        line_vector.push_back(line);
    }
    elaborate();
    return is_loaded;
}

template<typename T>
std::vector<T> ConfigReader::get(char* name) {
    return std::move(get<T>(std::string(name)));
}
template<typename T>
std::vector<T> ConfigReader::get(const std::string& name) {
    std::vector<T> result;

    if (!is_loaded) return std::move(result);
    if (name_map.find(name)==name_map.end()) return std::move(result);

    std::vector<std::string> const& value_vector_ref=name_map[name];
    result.resize(value_vector_ref.size());

    for (decltype(value_vector_ref.size()) i=0; i<value_vector_ref.size(); ++i) {
        result[i]=from_string<T>(value_vector_ref[i]);
    }
    return std::move(result);
}

void ConfigReader::print() {
    for (const auto& i: name_map) {
        std::cout<<i.first<<"=";
        for (const auto& v: i.second) {
            std::cout<<v<<",";
        }
        std::cout<<"\n";
    }
}

bool ConfigReader::close() {
    if (!is_open) return true;
    file_data.close();
    is_open=file_data.is_open();
    return !is_open;
}

template std::vector<double> ConfigReader::get(char* name);
template std::vector<double> ConfigReader::get(const std::string& name);
template std::vector<bool> ConfigReader::get(char* name);
template std::vector<bool> ConfigReader::get(const std::string& name);
template std::vector<int> ConfigReader::get(char* name);
template std::vector<int> ConfigReader::get(const std::string& name);
template std::vector<std::string> ConfigReader::get(char* name);
template std::vector<std::string> ConfigReader::get(const std::string& name);
