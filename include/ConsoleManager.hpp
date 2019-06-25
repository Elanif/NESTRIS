#ifndef CONSOLEMANAGER_H
#define CONSOLEMANAGER_H
#include<string>
#include<SFML/System/NonCopyable.hpp>
#include<SFML/System/Vector2.hpp>
#include<utility>
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
        static void refresh(bool always_print=false);

    private:
		static sf::RenderWindow info_window;
		static sf::Font info_window_font;
		static TextFormatter<string_character> text_formatter;
		enum MENU {
			HOME,
			SETTINGS
		};
		static std::string text_entered;
		static bool detecting_text_entered = true;
		static bool detecting_hotkey_input = false;
		static void handle_and_render_menu(MENU const& menu);
		static void basic_handler(sf::Event const& event);

		static void handleHOME(sf::Event const& event);
		static void renderHOME();

        static unsigned char framecounter;
        static bool error_print;
};
#endif // CONSOLEMANAGER_H
