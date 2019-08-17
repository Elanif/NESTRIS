#include"ConfigReader.hpp"
#include<iostream>
#include<regex>
#include<filesystem>
#include"ntris.hpp"

template<typename T>
std::string to_string(const T& t) {
	return std::to_string(t);
}
//TODO OVERLOAD?
template<>
inline std::string to_string<std::string>(const std::string& t) {
	return t;
}
template<>
inline std::string to_string<const char*>(const char* const& t) {
	return std::string(t);
}

std::string lowercase_str(const std::string& str) { //TODO make it portable with 16bitchar
	std::string result = "";
	for (auto& c : str)
	{
		result += std::tolower(c);
	}
	return result;
}

std::string uppercase_str(const std::string& str) { //TODO make it portable with 16bitchar
	std::string result = "";
	for (auto& c : str)
	{
		result += std::toupper(c);
	}
	return result;
}
template<typename T>
T from_string(char* const& str) {
    return from_string<T>(std::string(str));
}

template<typename T>
T from_string(const std::string& str) {
	return T{str};
}

template<>
std::string from_string<std::string>(const std::string& str) {
    return str;
}

template<>
char from_string<char>(const std::string& str) {
    if (str.length()>0) return str[0];
    else return (char)0;
}

template<>
int from_string<int>(const std::string& str) {
	int result = 0;
	try {
		result = std::stoi(str);
	}
	catch (std::exception e) {
	}
	return result;
}

template<>
std::size_t from_string<std::size_t>(const std::string& str) {
	int result = 0;
	try {
		result = std::stoi(str);
	}
	catch (std::exception e) {
	}
	return result;
}

template<>
double from_string<double>(const std::string& str) {
	double result = 0;
	try {
		result = std::stod(str);
	}
	catch (std::exception e) {
	}
	return result;
}

template<>
long double from_string<long double>(const std::string& str) {
	double result = 0;
	try {
		result = std::stod(str);
	}
	catch (std::exception e) {
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
	file_path = file_location;
	is_open = open(file_location);
}

ConfigReader::ConfigReader(const std::string& file_location, CaseType const& _m_case) 
	:m_case(_m_case)
{
	file_path = file_location;
	is_open = open(file_location);
}

bool ConfigReader::open(const std::string& file_location) {

	const std::size_t max_tries = 3;
	std::size_t tries = 0;
	do {
		file_data.open(file_location.c_str(), std::ios::in);
		is_open = file_data.is_open();
		is_loaded = file_data.good() && !file_data.fail() && !file_data.bad();
		++tries;
	} while (tries < max_tries && (!is_open || !is_loaded));

	if (!is_open || !is_loaded) {
		{
			std::ofstream file_data(file_location);
		}
		file_data.open(file_location.c_str(), std::ios::in);
		is_open = file_data.is_open();
		is_loaded = file_data.good() && !file_data.fail() && !file_data.bad();
	}

    std::string line="";
    while(!safeGetline(file_data, line).eof()) {
		switch (m_case) {
		case CaseType::Lower:
			line=lowercase_str(line);
			break;
		case CaseType::Upper:
			line=uppercase_str(line);
			break;
		case CaseType::Sensitive:
			break;
		case CaseType::Invariate:
			break;
		}
        read_lines.push_back(line);
    }
    elaborate();
    return is_loaded;
}

void ConfigReader::elaborate() {
	//for (decltype(line_vector.size()) i=0; i<line_vector.size();  ++i) {
	std::regex r_variable_name("^\\s*([^\\s\\=]*)\\s*\\=");
	std::regex r_variable_values("([^,]*),");
	std::regex r_variable_values_end("([^,]+)");
	std::string variable_name_final;
	for (auto& line_string : read_lines) {

		std::smatch matches;
		std::regex_search(line_string, matches, r_variable_name);

		if (!matches.empty()) {
			std::string variable_name = matches[1];

			switch (m_case)
			{
			case CaseType::Invariate:
				variable_name_final = variable_name;
			case CaseType::Lower:
				variable_name_final = lowercase_str(variable_name);
				break;
			case CaseType::Upper:
				variable_name_final = uppercase_str(variable_name);
				break;
			case CaseType::Sensitive:
				variable_name_final = variable_name;
				break;
			default:
				variable_name_final = variable_name;
				break;
			}

			std::size_t line_vector_position;

			if (name_map.find(variable_name_final) == name_map.end()) {
				//name_map[variable_name]=std::vector<std::string>();

				line_vector.push_back(std::vector<std::string>(1,variable_name));

				line_vector_position = line_vector.size() - 1;
			}
			else {
				line_vector_position = name_map[variable_name_final];
			}

			name_map[variable_name] = line_vector_position;

			//std::vector<std::string>& value_vector_ref=name_map[variable_name];

			std::smatch value_matches;

			decltype(matches[0].second) start_it = matches[0].second;
			while (std::regex_search(start_it, line_string.cend(), value_matches, r_variable_values)) {
				line_vector[line_vector_position].push_back(value_matches[1]);

				start_it = value_matches[0].second;
			}
			if (std::regex_search(start_it, line_string.cend(), value_matches, r_variable_values_end))
			{
				line_vector[line_vector_position].push_back(value_matches[1]);
			}
			else {
				//line_vector[line_vector_position].push_back("");
			}

		}
		else {
			line_vector.push_back(std::vector<std::string>(1, line_string));
		}

	}
}

template<typename T>
std::vector<T> ConfigReader::get(char* name) {
	return get<T>(std::string(name));
}
template<typename T>
std::vector<T> ConfigReader::get(const std::string& name) {
	std::vector<T> result;
	if (name.length() <= 0) return result;
	if (!is_loaded) return result;
	std::string cased_name = name;
	switch (m_case) {
	case CaseType::Lower:
		cased_name = lowercase_str(name);
		break;
	case CaseType::Upper:
		cased_name = uppercase_str(name);
		break;
	case CaseType::Invariate:
		cased_name = lowercase_str(name);
		break;
	case CaseType::Sensitive:
		cased_name = name;
		break;
	default:
		break;
	}
	
	if (name_map.find(cased_name) == name_map.end()) return result;

	std::vector<std::string> const& value_vector_ref = line_vector[name_map[cased_name]];
	
	result.resize(value_vector_ref.size()-1);

	for (decltype(value_vector_ref.size()) i = 1; i < value_vector_ref.size(); ++i) {
		std::string cased_string = value_vector_ref[i];
		switch (m_case) {
		case CaseType::Lower:
			cased_string = lowercase_str(cased_string);
			break;
		case CaseType::Upper:
			cased_string = uppercase_str(cased_string);
			break;
		case CaseType::Invariate:
			//result[i - 1] = from_string<T>(lowercase_str(cased_string));
			break;
		case CaseType::Sensitive:
			break;
		}
		result[i - 1] = from_string<T>(cased_string);
	}
	return result;
}

template<typename T>
void ConfigReader::append(char* name, T const& t) {
	return append<T>(std::string(name), t);
}
template<>
void ConfigReader::append(std::string const& name, bool const& t) {
	if (t)
		append(name, std::string("true"));
	else
		append(name, std::string("false"));
}
template<typename T>
void ConfigReader::append(std::string const& name, T const& t) {
	if (!is_loaded) return;
	if (name.length() <= 0) return;
	std::string cased_name = name;

	switch (m_case) {
	case CaseType::Lower:
		cased_name = lowercase_str(name);
		break;
	case CaseType::Upper:
		cased_name = uppercase_str(name);
		break;
	case CaseType::Invariate:
		cased_name = lowercase_str(name);
		break;
	case CaseType::Sensitive:
		cased_name = name;
		break;
	default:
		cased_name = name;
		break;
	}

	if (name_map.find(cased_name) == name_map.end()) {
		line_vector.push_back(std::vector<std::string>(1, cased_name));
		name_map[cased_name] = line_vector.size() - 1;
	}

	line_vector[name_map[cased_name]].push_back(ntris::to_string(t));
}

template<typename T>
void ConfigReader::overwrite(char* name, std::vector<T> const& t) {
	return overwrite<T>(std::string(name), t);
}
template<>
void ConfigReader::overwrite(std::string const& name, std::vector<bool> const& t) {
	std::vector<std::string> string_vector;
	for (const auto& i : t) {
		if (i) string_vector.push_back(std::string("true"));
		else string_vector.push_back(std::string("false"));
	}
	return overwrite<std::string>(std::string(name), string_vector);
}
template<typename T>
void ConfigReader::overwrite(std::string const& name, std::vector<T> const& t) {
	if (!is_loaded) return;
	if (name.length() <= 0) return;
	std::string cased_name = name;
	switch (m_case) {
	case CaseType::Lower:
		cased_name=lowercase_str(cased_name);
		break;
	case CaseType::Upper:
		cased_name=uppercase_str(cased_name);
		break;
	case CaseType::Invariate:
		cased_name=lowercase_str(cased_name);
		break;
	case CaseType::Sensitive:
		break;
	default:
		break;
	}
	if (name_map.find(cased_name) == name_map.end()) {
		line_vector.push_back(std::vector<std::string>(1, cased_name));
		name_map[cased_name] = line_vector.size() - 1;
	}
	std::vector<std::string> t_to_string;
	t_to_string.push_back(cased_name);
	for (const auto& i : t)
		t_to_string.push_back(to_string(i));

	line_vector[name_map[cased_name]] = t_to_string;
}

template<typename T>
void ConfigReader::overwrite(char* name,T const& t) {
	return std::move(overwrite<T>(std::string(name), std::vector<T>(1,t)));
}
template<typename T>
void ConfigReader::overwrite(std::string const& name, T const& t) {
	return std::move(overwrite<T>(name, std::vector<T>(1, t)));
}

void ConfigReader::print() {
	for (const auto& line : line_vector) {
		std::cout << line[0];
		if (line.size() >= 2) {
			std::cout << "=";
			for (std::size_t i = 1; i < line.size() - 1; ++i) {
				std::cout << line[i] << ",";
			}
			std::cout << line[line.size() - 1];
		}
		std::cout << "\n";
	}
}

bool ConfigReader::close() {
    if (!is_open) return true;
    file_data.close();
    is_open=file_data.is_open();
    return !is_open;
}

bool ConfigReader::save() {
	try {
		std::filesystem::path final_p = std::filesystem::current_path() / file_path;
		std::filesystem::path tmp_p = std::filesystem::current_path() / (file_path+std::string(".tmp"));
		std::ofstream file_data_output(tmp_p, std::ios::out);
		for (const auto& line : line_vector) {
			file_data_output << line[0];
			if (line.size() >= 2) {
				file_data_output << "=";
				for (std::size_t i = 1; i < line.size() - 1; ++i) {
					file_data_output << line[i] << ",";
				}
				file_data_output << line[line.size() - 1];
			}
			file_data_output << "\n";
		}

		file_data_output.close();

		if (file_data.is_open()) file_data.close();

		std::filesystem::path bak_p = std::filesystem::current_path() / (file_path+std::string(".bak"));
		std::filesystem::rename(final_p, bak_p);

		std::filesystem::rename(tmp_p, final_p);

		std::filesystem::remove(bak_p);
	}
	catch (std::exception ex) {
		return false;
	}
	return true;
}

template std::vector<double> ConfigReader::get(char* name);
template std::vector<double> ConfigReader::get(const std::string& name);
template std::vector<long double> ConfigReader::get(char* name);
template std::vector<long double> ConfigReader::get(const std::string& name);
//template std::vector<float> ConfigReader::get(char* name);
//template std::vector<float> ConfigReader::get(const std::string& name);
template std::vector<bool> ConfigReader::get(char* name);
template std::vector<bool> ConfigReader::get(const std::string& name);
//template std::vector<long long> ConfigReader::get(char* name);
//template std::vector<long long> ConfigReader::get(const std::string& name);
//template std::vector<unsigned long long> ConfigReader::get(char* name);
//template std::vector<unsigned long long> ConfigReader::get(const std::string& name);
template std::vector<int> ConfigReader::get(char* name);
template std::vector<int> ConfigReader::get(const std::string& name);
template std::vector<std::size_t> ConfigReader::get(char* name);
template std::vector<std::size_t> ConfigReader::get(const std::string& name);
//template std::vector<unsigned int> ConfigReader::get(char* name);
//template std::vector<unsigned int> ConfigReader::get(const std::string& name);
template std::vector<char> ConfigReader::get(char* name);
template std::vector<char> ConfigReader::get(const std::string& name);
template std::vector<std::string> ConfigReader::get(char* name);
template std::vector<std::string> ConfigReader::get(const std::string& name);

template void ConfigReader::append(char* name, double const&);
template void ConfigReader::append(const std::string& name, double const&);
template void ConfigReader::append(char* name, long double const&);
template void ConfigReader::append(const std::string& name, long double const&);
//template void ConfigReader::append(char* name, float const&);
//template void ConfigReader::append(const std::string& name, float const&);
template void ConfigReader::append(char* name, bool const&);
template void ConfigReader::append(const std::string& name, bool const&);
//template ConfigReader::append(char* name, unsigned long const&);
//template ConfigReader::append(const std::string& name, unsigned long);
//template std::vector<unsigned long long> ConfigReader::append(char* name);
//template std::vector<unsigned long long> ConfigReader::append(const std::string& name);
template void ConfigReader::append(char* name, int const&);
template void ConfigReader::append(const std::string& name, int const&);
template void ConfigReader::append(char* name, std::size_t const&);
template void ConfigReader::append(const std::string& name, std::size_t const&);
//template std::vector<unsigned int> ConfigReader::append(char* name);
//template std::vector<unsigned int> ConfigReader::append(const std::string& name);
template void ConfigReader::append(char* name, char const&);
template void ConfigReader::append(const std::string& name, char const&);
template void ConfigReader::append(char* name, std::string const&);
template void ConfigReader::append(const std::string& name, std::string const&);

template void ConfigReader::overwrite(char* name, std::vector<double> const&);
template void ConfigReader::overwrite(const std::string& name, std::vector<double> const&);
template void ConfigReader::overwrite(char* name, std::vector<long double> const&);
template void ConfigReader::overwrite(const std::string& name, std::vector<long double> const&);
//template void ConfigReader::overwrite(char* name, float const&);
//template void ConfigReader::overwrite(const std::string& name, float const&);
template void ConfigReader::overwrite(char* name, std::vector<bool> const&);
template void ConfigReader::overwrite(const std::string& name, std::vector<bool> const&);
//template ConfigReader::overwrite(char* name, unsigned long const&);
//template ConfigReader::overwrite(const std::string& name, unsigned long);
//template std::vector<unsigned long long> ConfigReader::overwrite(char* name);
//template std::vector<unsigned long long> ConfigReader::overwrite(const std::string& name);
template void ConfigReader::overwrite(char* name, std::vector<int> const&);
template void ConfigReader::overwrite(const std::string& name, std::vector<int> const&);
template void ConfigReader::overwrite(char* name, std::vector<std::size_t> const&);
template void ConfigReader::overwrite(const std::string& name, std::vector<std::size_t> const&);
//template std::vector<unsigned int> ConfigReader::overwrite(char* name);
//template std::vector<unsigned int> ConfigReader::overwrite(const std::string& name);
template void ConfigReader::overwrite(char* name, std::vector<char> const&);
template void ConfigReader::overwrite(const std::string& name, std::vector<char> const&);
template void ConfigReader::overwrite(char* name, std::vector<std::string> const&);
template void ConfigReader::overwrite(const std::string& name, std::vector<std::string> const&);
