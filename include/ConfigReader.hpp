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
		enum CaseType {
			Sensitive,
			Lower,
			Upper
		};
        ConfigReader(const std::string& file_location, CaseType const& _m_case);
        bool open(const std::string& file_location);
        void elaborate();
        template <typename T>
        std::vector<T> get(const std::string& name);
        template <typename T>
        std::vector<T> get(char* name);
		template <typename T>
		void append(char* name, T const& t);
		template <typename T>
		void append(std::string const& name, T const& t);
		template <typename T>
		void overwrite(char* name, T const& t);
		template <typename T>
		void overwrite(std::string const& name, T const& t);
		template <typename T>
		void overwrite(char* name, std::vector<T> const& t);
		template <typename T>
		void overwrite(std::string const& name, std::vector<T> const& t);
		bool delete_entry(std::string const& name);
		bool save();

        bool close();
        void reload();
        void print();

		CaseType const m_case = CaseType::Sensitive;
    private:
        std::vector<std::string> line_vector;
        std::unordered_map<std::string, std::vector<std::string> > name_map;

		std::unordered_map<std::string, std::size_t> name_map_map;
		std::vector<std::vector<std::string> > line_vector_map;
		std::vector<bool> line_has_variable;
        std::ifstream file_data;
		std::string file_path;
        bool is_open=false;
        bool is_loaded=false;
};

#endif // CONFIGREADER_H
