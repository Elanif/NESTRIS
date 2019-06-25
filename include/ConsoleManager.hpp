#ifndef CONSOLEMANAGER_H
#define CONSOLEMANAGER_H
#include<string>
#include<SFML/System/NonCopyable.hpp>
#include<SFML/System/Vector2.hpp>
#include<utility>
#include<stack>
#include<SFML/Graphics.hpp>
#include"TextFormatter.hpp"

class ConsoleManager : public sf::NonCopyable
{
public:
	ConsoleManager() { init(); }
	void init();
	void close_info_window();
	void open_info_window();
	void toggle_info_window();
	bool is_window_open();
	~ConsoleManager();
	void refresh(bool always_print = false);

private:
	sf::RenderWindow info_window{};
	sf::Font info_window_font{};
	TextFormatter<string_character> text_formatter;
	enum MENU {
		HOME,
		SETTINGS
	} current_menu;
	std::stack<MENU> menu_stack;
	std::string text_entered{};
	bool detecting_text_entered{ true };
	bool detecting_hotkey_input{ false };
	void handle_menu(MENU const& menu);
	void render_menu(MENU const& menu);
	void basic_handler(sf::Event const& event);

	void handleHOME(sf::Event const& event);
	void renderHOME();

	unsigned char framecounter{ 0 };
	bool error_print{ false };
};
#endif // CONSOLEMANAGER_H
