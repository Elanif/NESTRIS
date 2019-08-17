#pragma once

#include<SFML/Graphics.hpp>
#include"TextRenderer.hpp"
#include"TextFormatter.hpp"
#include<string>
#include<vector>
#include<memory>
#include"Log.hpp"
#include"TextFormatter.hpp"
#include"OutputInfo.hpp"


class OptionsMenu {
public:
	OptionsMenu(sf::Font const& _font)
		:font(_font)
		,text_renderer(_font)
	{};
	virtual std::vector<std::string> getOutput() { return std::vector<std::string>(); };
	virtual std::string getName() { return std::string(); };
	virtual bool isOpenable() { return false; };
	virtual OptionsMenu open() { return OptionsMenu(font); };
	virtual void render(sf::RenderTarget& render_target) {};
protected:
	sf::Font const& font;
	TextRenderer text_renderer;
	std::vector<OptionsMenu*> OptionsMenu_vector;
private:
	const std::string title;
};

class LogEntry : public OptionsMenu {
public:
	LogEntry(sf::Font const& _font, std::string const& _log_name)
		:OptionsMenu(_font)
		,log_name(_log_name)
	{
	}
	std::vector<std::string> getOutput() {
		std::vector<std::string> result;
		OutputInfo* outputinfo_ptr = Log::getOutputInfo(log_name);
		if (!outputinfo_ptr) return result;
		return outputinfo_ptr->print();
	}
	std::string getName() {
		return log_name;
	}
	bool isOpenable() {
		return false;
	}
private:
	const std::string log_name;
};

class HomeMenu : public OptionsMenu {
public:
	HomeMenu(sf::Font const& _font)
		:OptionsMenu(_font)
	{

		menu_vector.push_back(new LogEntry(font,"fps"));
		menu_vector.push_back(new LogEntry(font,"processing delay"));
		menu_vector.push_back(new LogEntry(font,"draw delay"));
		menu_vector.push_back(new LogEntry(font,"display delay"));
	};
	std::vector<std::string> getOutput() {
		return std::vector<std::string>();
	}
	std::string getName() {
		return std::string();
	}
	void render(sf::RenderTarget& render_target, unsigned int const& character_size) {
		TextFormatter<char> text_formatter(font);
		const sf::Vector2f left_menu_boundaries{ 300,1000 };
		const sf::Vector2f right_menu_boundaries{ 700,1000 };
		sf::Vector2f menu_entry_pos{ 0,0 };
		for (auto const& i : menu_vector) {
			sf::Vector2f menu_info_pos{ left_menu_boundaries.x,menu_entry_pos.y };

			//left
			std::string sub_menu_name = i->getName();
			sf::Text sub_menu_name_text = text_formatter.getFormattedText(sub_menu_name, character_size, left_menu_boundaries);
			sub_menu_name_text.setFillColor(sf::Color::White);
			sub_menu_name_text.setPosition(menu_entry_pos);
			render_target.draw(sub_menu_name_text);
			menu_entry_pos.y += text_formatter.getLastFormattedSize().y;

			//right
			std::vector<std::string> sub_menu_values = i->getOutput();
			sf::Text sub_menu_value_text = text_formatter.getFormattedText(sub_menu_values[0], character_size, right_menu_boundaries);
			sub_menu_value_text.setFillColor(sf::Color::White);
			sub_menu_value_text.setPosition(menu_info_pos);
			render_target.draw(sub_menu_value_text);
			menu_info_pos.x += text_formatter.getLastFormattedSize().x;
		}
	}
	bool isOpenable() {
		return true;
	}
private:
	const std::string title = "Home Menu";
	std::vector<OptionsMenu*> menu_vector;
};