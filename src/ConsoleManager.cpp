#include"ConsoleManager.hpp"
#include<iostream>
#include<Window.hpp>
#include"TextFormatter.hpp"
#include"Log.hpp"

sf::Font ConsoleManager::info_window_font;
unsigned char ConsoleManager::framecounter=0;
bool ConsoleManager::error_print=false;

sf::RenderWindow ConsoleManager::info_window;

void ConsoleManager::open_info_window() {
	if (!info_window.isOpen()) info_window.create(sf::VideoMode(glb::ntsc_screen_width * glb::window_scale.x, glb::ntsc_screen_height * glb::window_scale.y), "Information");
}

void ConsoleManager::close_info_window() {
	if (info_window.isOpen()) info_window.close();
}

void ConsoleManager::toggle_info_window()
{
	if (info_window.isOpen()) close_info_window();
	else open_info_window();
}

bool ConsoleManager::is_window_open() {
	return info_window.isOpen();
}

void ConsoleManager::init()
{
	info_window_font.loadFromFile("Roboto.ttf");
	text_formatter.setCharacterSize(glb::info_window_character_size);
	text_formatter.setFillColor(sf::Color::White);
	text_formatter.setFont(info_window_font);
	text_formatter.calc_font_sizes();
	open_info_window();
}

TextFormatter<string_character> ConsoleManager::text_formatter;

void ConsoleManager::refresh(bool always_print) {
	static nes_uchar counter = 0;
	static std::string text_test = "";
	if (info_window.isOpen()) {
		sf::Event event;
		while (info_window.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				close_info_window();
				return;
				break;
			case sf::Event::TextEntered:
				if (info_window.hasFocus()) {
					// Handle ASCII characters only
					if (event.text.unicode < 128)
					{
						if (event.text.unicode == 8 && text_test.length() > 0) {
							text_test.pop_back();
						}
						else text_test += static_cast<char>(event.text.unicode);
						std::cout << text_test << glb::newline;
					}
				}
			break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::F1: {
						toggle_info_window();
						if (is_window_open())
							info_window.requestFocus();
					}
					break;
				}
			}

		}
		info_window.clear();
		sf::Vector2f pos{ 0,0 };
		bool reset = false;
		if (++counter >= 30) {
			counter = 0;
			reset = true;
		}
		for (auto& info : Log::log_vector) {
			std::string outputstring = info->print(reset);
			text_formatter.setString(outputstring);
			text_formatter.setBoundaries({ 256,224 });
			text_formatter.setPosition(pos);
			sf::Text t= text_formatter.getFormattedText(glb::info_window_character_size * sqrt((double)glb::window_scale.x * glb::window_scale.y));
			info_window.draw(t);
			pos.y += text_formatter.getLastFormattedSize().y;
		}
		info_window.display();
		error_print = false;
	}
}

void ConsoleManager::handle_and_render_menu(MENU const& menu)
{
	sf::Event event;

	switch (menu) {
	case MENU::HOME:

		while (info_window.pollEvent(event))
		{
			basic_handler(event);
			handleHOME(event);
		}
		renderHOME();
		break;

	}
}

void ConsoleManager::handleHOME(sf::Event const& event)
{
}

void ConsoleManager::renderHOME()
{
}

void ConsoleManager::basic_handler(sf::Event const& event) {
	switch (event.type) {
	case sf::Event::Closed:
		close_info_window();
		return;
		break;
	case sf::Event::TextEntered:
		if (info_window.hasFocus()) {
			// Handle ASCII characters only
			if (event.text.unicode < 128)
			{
				if (event.text.unicode == 8 && text_entered.length() > 0) {
					text_entered.pop_back();
				}
				else text_entered += static_cast<char>(event.text.unicode);
				std::cout << text_entered << glb::newline;
			}
		}
		break;
	case sf::Event::KeyPressed:
		switch (event.key.code) {
		case sf::Keyboard::F1: {
			toggle_info_window();
			if (is_window_open())
				info_window.requestFocus();
			}
			break;
		}
		break;
	}
}

ConsoleManager::~ConsoleManager() {
	close_info_window();
}
