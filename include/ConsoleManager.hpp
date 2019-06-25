#ifndef CONSOLEMANAGER_H
#define CONSOLEMANAGER_H
#include<vector>
#include<unordered_map>
#include<string>
#include<SFML/System/NonCopyable.hpp>
#include<SFML/System/Vector2.hpp>
#include<utility>
#include<memory>
#include"OutputInfo.hpp"
#include<fstream>
#include<SFML/Graphics.hpp>
#include"TextFormatter.hpp"

class ConsoleManager : public sf::NonCopyable
{
    public:
        static void init();
		static void close_info_window();
		static void open_info_window();
		static void toggle_info_window();
		static bool is_window_open();
		~ConsoleManager();
        template<class T>
        static void update(std::string info, const T& t) ;
        static void update(std::string info, const char* const& t) ;
        template<class T>
        static void update_error(const T& t) ;
        static void update_error(const char* t);
        static void refresh(bool always_print=false);

    private:
		static sf::RenderWindow info_window;
		static sf::Font info_window_font;
		static TextFormatter<string_character> text_formatter;
		enum MENU {
			HOME,
			SETTINGS
		};
		static void render_menu(MENU const& menu);
		static void handle_menu(MENU const& menu);
		static std::string text_entered;

        static unsigned char framecounter;
        static bool error_print;
        static void print_error(const char* error_string);
        static void print_error(const std::string& error_string);
        static std::size_t add_value(std::string info, std::string unit);
        static std::size_t add_value(std::unique_ptr<OutputInfo>&& outputinfo);
        static std::vector<std::unique_ptr<OutputInfo> > CMvector;
        static std::unordered_map<std::string, std::size_t> CMmap;
        static std::ofstream error_log;
};
#endif // CONSOLEMANAGER_H
