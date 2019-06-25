#pragma once
#ifndef LOG_H
#define LOG_H
#include<SFML/System/NonCopyable.hpp>
#include<string>
#include<memory>
#include<vector>
#include<unordered_map>
#include"OutputInfo.hpp"
#include<fstream>
#include"ntris.hpp"

class Log : public sf::NonCopyable {
public:
	static void init();
	template<class T>
	static void update(std::string info, const T& t);
	static void update(std::string info, const char* const& t);
	template<class T>
	static void update_error(const T& t);
	static void update_error(const char* t);
	static bool error_print;
	static void print_error(const char* error_string);
	static void print_error(const std::string& error_string);
	static std::size_t add_value(std::string info, std::string unit);
	static std::size_t add_value(std::unique_ptr<OutputInfo>&& outputinfo);
	static std::vector<std::unique_ptr<OutputInfo> > log_vector;
	static std::unordered_map<std::string, std::size_t> log_map;
	static std::ofstream error_log;
};
#endif // !LOG_H
