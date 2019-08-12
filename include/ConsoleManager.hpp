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
	bool refresh(bool always_print = false); //returns true if info_window is open

private:
	float getCharacterSize();
	sf::RenderWindow info_window{};
	sf::Font info_window_font{};
	TextFormatter<string_character> text_formatter;
	enum MENU {
		HOME,
		SETTINGS
	} current_menu = MENU::HOME;
	std::stack<MENU> menu_stack;
	bool is_menu_unlocked(MENU const& from_menu, std::string const& to_menu); // returns false if there is no menu with that name
	std::string text_entered{};
	bool detecting_text_entered{ true };
	bool detecting_hotkey_input{ false };
	void handle_menu(MENU const& menu);
	sf::Vector2f render_menu(MENU const& menu);
	bool basic_handler(sf::Event const& event); //returns true if it handled an event, false otherwise

	void handleHOME(sf::Event const& event);
	void renderHOME(sf::Vector2f pos);

	unsigned char framecounter{ 0 };
	bool error_print{ false };
	
	std::unordered_map<std::string, MENU> text_to_menu_map;
	
	std::unordered_map<MENU, std::unordered_map<MENU, bool> > unlocked_tree_map;

	void init_unlocked_tree_map();

};
#endif // CONSOLEMANAGER_H
