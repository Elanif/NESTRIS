#include"Log.hpp"
#include<chrono>
#include<ctime>
#include<iostream>
#include<Window.hpp>
#include<sstream>
#include<limits>

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1ull; // Extra space for '\0'
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
} //https://stackoverflow.com/a/26221725

bool Log::error_print = false;
std::vector<std::unique_ptr<OutputInfo> > Log::log_vector;
std::unordered_map<std::string, std::size_t> Log::log_map;
std::ofstream Log::error_log;

template <typename Duration>
std::string print_time(tm t, Duration fraction) {
	using namespace std::chrono;
	return string_format("%04u-%02u-%02u %02u:%02u:%02u.%03u ", t.tm_year + 1900,
		t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec,
		static_cast<unsigned>(fraction / milliseconds(1)));
}

void Log::print_error(const std::string& error_string) {
	print_error(error_string.c_str());
}
OutputInfo* Log::getOutputInfo(std::string const& name)
{
	if (log_map.find(name) != log_map.end()) {
		return log_vector[log_map[name]].get();
	}
	return nullptr;
}
void Log::print_error(const char* error_string) {
	auto clock = std::chrono::system_clock::now();
	std::time_t current_time = std::chrono::system_clock::to_time_t(clock);
	using namespace std::chrono;
	system_clock::time_point now = system_clock::now();
	system_clock::duration tp = now.time_since_epoch();
	tp -= duration_cast<seconds>(tp);
	error_log << print_time(*localtime(&current_time), tp);
	error_log << error_string << ntris::newline;
	error_log.flush();
}

template<typename T>
void Log::update(std::string info, const T& t) {
	if (log_map.find(info) == log_map.end()) {
		print_error(std::string("No OutputInfo of type ") + info + std::string(" found"));
	}
	else {
		log_vector[log_map[info]]->set_value(t);
	}
}
template void Log::update(std::string info, const std::string& t);
template void Log::update(std::string info, char const* const& t);
template void Log::update(std::string info, const int& t);
template void Log::update(std::string info, const std::size_t& t);
template void Log::update(std::string info, const unsigned long long& t);
template void Log::update(std::string info, const long long& t);
template void Log::update(std::string info, const double& t);
template void Log::update(std::string info, const long double& t);

template<typename T>
void Log::update_error(const T& t) {
	error_print = true;
	if (log_map.find("error") == log_map.end()) {
		print_error("No OutputInfo of type error found");
	}
	else {
		std::string error_string = ntris::to_string(t);
		if (error_string.length() > 0 && error_string != "0") {
			print_error(error_string);
		}
		log_vector[log_map["error"]]->set_value(error_string);
	}
}
void Log::update_error(const char* t) {
	update_error<std::string>(std::string(t));
}
template void Log::update_error(const std::string& t);
template void Log::update_error(const int& t);
template void Log::update_error(const std::size_t& t);
template void Log::update_error(const unsigned long long& t);
template void Log::update_error(const long long& t);
template void Log::update_error(const double& t);
template void Log::update_error(const long double& t);

std::size_t Log::add_value(std::string info, std::string unit) {
	info=ntris::lowercase_str(info);
	info=ntris::lowercase_str(unit);
	std::unique_ptr<OutputInfo> fpsinfo = std::make_unique<OutputInfo>(info, unit);
	return add_value(std::move(fpsinfo));
}

std::size_t Log::add_value(std::unique_ptr<OutputInfo>&& outputinfo) {
	outputinfo->name=ntris::lowercase_str(outputinfo->name);
	std::size_t CMvector_size = log_vector.size();
	bool push_back_error = false;
	try {
		log_vector.push_back(std::move(outputinfo));
	}
	catch (std::exception const& bad_alloc_exception) {
		push_back_error = true;
		error_log << "OutputInfo pushback exception " << bad_alloc_exception.what() << ntris::newline;
		error_log.flush();
	}
	if (!push_back_error && CMvector_size == log_vector.size()) {
		push_back_error = true;
		error_log << "OutputInfo pushback failed" << ntris::newline;
		error_log.flush();
	}
	if (push_back_error) return (std::size_t) - 1;
	else {
		log_map[log_vector[log_vector.size() - 1]->name] = log_vector.size() - 1;
		return log_vector.size() - 1;
	}
}

void Log::init()
{
	error_log.open("error_log.txt", std::ios::app);
	//std::cout.sync_with_stdio(false); messes up with rlutil
	std::unique_ptr<OutputInfo> _fps(new OutputInfoLowDouble("fps", "hz", true));
	std::unique_ptr<OutputInfo>& fps = log_vector[add_value(std::move(_fps))];
	fps->set_value(0);

	std::unique_ptr<OutputInfo> _input(new OutputInfoLowI64("processing delay", "microseconds", false));
	std::unique_ptr<OutputInfo>& input_delay = log_vector[add_value(std::move(_input))];
	input_delay->set_value(0);

	std::unique_ptr<OutputInfo> _render(new OutputInfoLowI64("render delay", "microseconds", false));
	std::unique_ptr<OutputInfo>& render_delay = log_vector[add_value(std::move(_render))];
	render_delay->set_value(0);

	std::unique_ptr<OutputInfo> _draw(new OutputInfoLowI64("draw delay", "microseconds", false));
	std::unique_ptr<OutputInfo>& draw_delay = log_vector[add_value(std::move(_draw))];
	draw_delay->set_value(0);

	std::unique_ptr<OutputInfo> _display(new OutputInfoLowI64("display delay", "microseconds", false));
	std::unique_ptr<OutputInfo>& display_delay = log_vector[add_value(std::move(_display))];
	display_delay->set_value(0);

	std::unique_ptr<OutputInfo> _system(new OutputInfoError("system", 5));
	std::unique_ptr<OutputInfo>& system = log_vector[add_value(std::move(_system))];
	system->set_value("");

	std::unique_ptr<OutputInfo> _error(new OutputInfoError("error", 5));
	add_value(std::move(_error));
}