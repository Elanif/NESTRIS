#ifndef WINDOW_H
#define WINDOW_H

#include"Input.hpp"
#include<SFML/Graphics.hpp>
#include"Engine.hpp"
#include"ntris.hpp"
#include"SafeQueue.hpp"
#include"DelayManager.hpp"
#include<memory>
#include<atomic>

class ConfigReader;
class TileRenderer;
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
	void render(sf::RenderWindow& window, TileRenderer& tilerend);
	void toggle_fullscreen(sf::RenderWindow& window);
private:
	std::atomic<bool> close_window;
	std::atomic<bool> hide_cursor;
	Input inputManager;
	std::unique_ptr<DelayManager> delay_manager;
	SafeQueue<sf::Event> event_queue;
};

#endif
