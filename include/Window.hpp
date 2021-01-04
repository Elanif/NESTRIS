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
	void render(TileRenderer& tilerend);
private:
	bool shader = false;
	std::atomic<unsigned int> window_size_x;
	std::atomic<unsigned int> window_size_y;
	std::atomic<bool> close_window;
	std::atomic<bool> hide_cursor;
	std::atomic<bool> toggle_fullscreen;
	std::atomic<bool> fullscreen;
	Input inputManager;
	std::unique_ptr<DelayManager> delay_manager;
	SafeQueue<sf::Event> event_queue;
	std::mutex window_mutex;
	bool isWindowOpen();
	sf::RenderWindow window;
	void toggle_fullscreen_func();
	sf::View window_view;

};

#endif
