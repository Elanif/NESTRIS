#include"OptionsMenu.hpp"
#include"Log.hpp"
#include"TextFormatter.hpp"
#include"OutputInfo.hpp"

class HomeMenu : public OptionsMenu {
public:
	HomeMenu(sf::Font const* _font)
		:text_renderer(_font)
	{
		font.reset(_font);
	};
	std::vector<sf::Text> getOutput(unsigned int const& characterSize) {
		sf::Text text(title, *font, characterSize);
		return std::vector(1, text);
	}
	void render(sf::RenderTarget const& render_target, unsigned int const& character_size) {
		for (auto const& i : menu_vector) {
			std::vector<sf::Text> sub_menu_output = i->getOutput(character_size);
			//render_target.draw()

		}
	}
	bool isOpenable() {
		return true;
	}
private:
	const std::string title = "Home Menu";
	std::shared_ptr<sf::Font const> font;
	TextRenderer text_renderer;
	std::vector<std::unique_ptr<OptionsMenu> > menu_vector;
};

class LogEntry : OptionsMenu {
public:
	LogEntry(sf::Font const* _font, std::string const& _log_name)
		:log_name(_log_name)
	{
		font.reset(_font);
	}
	std::vector<sf::Text> getOutput(unsigned int const& characterSize) {
		std::vector<sf::Text> text_vector;
		OutputInfo* outputinfo_ptr = Log::getOutputInfo(log_name);
		if (!outputinfo_ptr) return text_vector;
		sf::Text title(log_name, *font, characterSize);
		text_vector.push_back(title);
		std::vector<std::string> string_vector = outputinfo_ptr->print();
		for (auto const& i: string_vector)
			text_vector.push_back(sf::Text(i,*font,characterSize));
		//sf::Text value(outputinfo_ptr->)
		return text_vector;
	}
	bool isOpenable() {
		return false;
	}
private:
	const std::string log_name;
	std::shared_ptr<sf::Font const> font;
};