#ifndef WINDOW_H
#define WINDOW_H

#include"Input.hpp"
#include<SFML/Graphics.hpp>
#include"Engine.hpp"
#include"ntris.hpp"

class ConfigReader;
class Window : public sf::NonCopyable
{
public:
	enum OPT {
		GENERAL,
		SMALLEST,
		SPAM,
		FULL,
		NOTHING,
		ARRAY,
		ARRAYLOG,
		ARRAYBUCKET
	};
	Window(const std::size_t& width, const std::size_t& height, const OPT& optimized);
private:
	void general_delay_manager(largest_uint target_delay);
	void smallest_delay_manager(largest_uint target_delay);
	void full_thread_delay_manager(largest_uint target_delay);
	void nothing_delay_manager(largest_uint target_delay);
	void array_delay_manager(largest_uint target_delay);
	void array_delay_manager_log(largest_uint target_delay);
	void array_delay_manager_bucket(largest_uint target_delay);
	void spam_delay_manager(largest_uint target_delay);
	void sleep(largest_uint _delay);
	largest_uint sleep_for_how_long(largest_uint _delay);
	Input inputManager;

	bool setFourThirds(ConfigReader& cfg);
	sf::Vector2<long double> setWindowScale(ConfigReader& cfg, std::size_t const& width_pixels, std::size_t const& height_pixels, bool const& four_thirds);
	sf::Vector2i setWindowPosition(ConfigReader& cfg, std::size_t const& window_width, std::size_t const& window_height);
};

#endif
