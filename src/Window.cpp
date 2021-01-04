#include"Window.hpp"
#include<cmath>
#include"ConsoleManager.hpp"
#include"Log.hpp"
#include<thread>
#include<string>
#include<limits>
#include<iostream>
#include"ntris.hpp"
#include"ConfigReader.hpp"
#include<thread>
#include"MyClock.hpp"

class ConfigSaver {
public:
	bool _save_on_exit = true;
	void dont_save_on_exit() {
		_save_on_exit = false;
	}
	void save_on_exit() {
		_save_on_exit = true;
	}
	bool getFourThirds() {
		std::vector<bool> four_thirds = cfg.get<bool>("four_thirds");
		if (four_thirds.size() < 1) return true;
		if (four_thirds[0] == true) return true;
		return false;
	}
	bool getFullscreen() {
		std::vector<bool> fullscreen = cfg.get<bool>("fullscreen");
		if (fullscreen.size() < 1) return false;
		if (fullscreen[0] == true) return true;
		return false;
	}
	bool getShader() {
		std::vector<bool> shader = cfg.get<bool>("shader");
		if (shader.size() < 1) return false;
		if (shader[0] == true) return true;
		return false;
	}
	sf::Vector2<long double> setWindowScale(std::size_t const& width_pixels, std::size_t const& height_pixels, bool const& four_thirds) {
		std::vector<double> window_scale = cfg.get<double>("window_scale");
		sf::Vector2<long double> scale{ 1,1 };
		if (window_scale.size() >= 2) {
			scale.x = window_scale[0];
			scale.y = window_scale[1];
		}
		if (four_thirds) {
			scale.y = 3.L / 4.L * scale.x * width_pixels / height_pixels;
		}
		return scale;
	}
	sf::Vector2i setWindowPosition(std::size_t const& window_width, std::size_t const& window_height) {
		std::vector<int> window_position = cfg.get<int>("window_position");
		sf::Vector2i window_pos{ 0,0 };
		if (window_position.size() < 2) {
			window_position.resize(2);
			std::size_t screen_width = sf::VideoMode::getDesktopMode().width;
			std::size_t screen_height = sf::VideoMode::getDesktopMode().height;
			if (window_width <= screen_width) {
				window_pos.x = (screen_width - window_width) / 2;
			}
			if (window_height <= screen_height) {
				window_pos.y = (screen_height - window_height) / 2;
			}
		}
		else {
			window_pos.x = window_position[0];
			window_pos.y = window_position[1];
		}
		return window_pos;
	}
	void saveWindowScale() {
		saveConfig("window_scale", ntris::window_scale);
	}
	void saveFourThirds() {
		cfg.overwrite("four_thirds", std::vector<bool>(1, ntris::four_thirds));
	}
	void saveFullscreen() {
		cfg.overwrite("fullscreen", std::vector<bool>(1, ntris::fullscreen));
	}
	void saveShader() {
		cfg.overwrite("shader", std::vector<bool>(1, ntris::shader));
	}
	void saveWindowPosition() {
		saveConfig("window_position", ntris::window_position);
	}
	ConfigSaver(ConfigReader& _cfg) :cfg(_cfg) {};
	~ConfigSaver() {
		if (_save_on_exit) {
			saveFullscreen();
			saveFourThirds();
			saveWindowScale();
			saveWindowPosition();
			cfg.save();
		}
	}
private:
	ConfigReader& cfg;
	template<typename T>
	void saveConfig(std::string const& name, sf::Vector2<T> vector) {
		cfg.overwrite(name, std::vector<T>{vector.x, vector.y});
	}
};

void Window::render(TileRenderer& tilerend) {
	ConsoleManager cm;
	Engine _engine = Engine(tilerend.getTileContainer(), Engine::LEVELSELECT);

	constexpr largest_uint partspersecond = MyClock::getPartsPerSecond();
	largest_uint timeperframe_odd = (long double)(partspersecond) / ntris::ntsc_fps_odd;
	largest_uint timeperframe_even = (long double)(partspersecond) / ntris::ntsc_fps_even;
	bool odd_frame = false;
	largest_uint timeperframe = timeperframe_odd;

	MyClock elapsedtime;
	std::size_t counter = 0;

	while (!close_window.load()&&isWindowOpen()) {
		odd_frame = !odd_frame; //Would be more efficient to have 2 cycles, but is it needed?
		if (odd_frame) timeperframe = timeperframe_odd;
		else timeperframe = timeperframe_even;
		if (elapsedtime.elapsedTime() >= timeperframe) {

			if (elapsedtime.elapsedTime() > 0) Log::update<long double>("fps", partspersecond / (long double)elapsedtime.elapsedTime());

			elapsedtime.restart();
			sf::Int64 delaycalc = 0;
			
			ActiveInputs ai = inputManager.getInput();
			_engine.frame(ai);
			if (!hide_cursor.load() && ai.getHideMouse()) {
				hide_cursor.store(true);
			}
			else if (hide_cursor.load() && !ai.getHideMouse()) {
				hide_cursor.store(false);
			}

			Log::update<sf::Int64>("processing delay", elapsedtime.elapsedTime() - delaycalc);
			delaycalc = elapsedtime.elapsedTime();
			//std::cout <<"processing delay"<< elapsedtime.elapsedTime() - delaycalc << ntris::newline;
			{
				std::lock_guard<std::mutex> fullscreen_lock(window_mutex);
				window.clear();//there are ways to avoid this but it only adds 15 microseconds

				tilerend.drawmod(window);

				Log::update<sf::Int64>("draw delay", elapsedtime.elapsedTime() - delaycalc);
				//std::cout << "draw delay"<<elapsedtime.elapsedTime() - delaycalc << ntris::newline;
				delaycalc = elapsedtime.elapsedTime();

				window.display();
			}
			Log::update<sf::Int64>("display delay", elapsedtime.elapsedTime() - delaycalc);
			//std::cout << "display delay"<<elapsedtime.elapsedTime() - delaycalc << ntris::newline;
			delaycalc = elapsedtime.elapsedTime();

			while (event_queue.size() > 0)
			{
				sf::Event event = event_queue.pop();
				switch (event.type) {
				case sf::Event::Closed:
					close_window.store(true);
				break;
				case sf::Event::KeyPressed: {
					bool ctrl = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
					std::lock_guard<std::mutex> fullscreen_lock(window_mutex);
					if (window.hasFocus()) {
						if (event.key.code == sf::Keyboard::F1) {
							cm.toggle_info_window();
							if (!cm.is_window_open())
								window.requestFocus();
						}
						if ((event.key.code == sf::Keyboard::F && ctrl)/* || Input::getInput()*/) {
							toggle_fullscreen.store(true);
						}
					}
				}
				break;
				case sf::Event::Resized:
					if (!fullscreen.load()) {
						std::lock_guard<std::mutex> fullscreen_lock(window_mutex);
						window_size_x.store(window.getSize().x);
						window_size_y.store(window.getSize().y);
					}
				break;
				}
			}
			cm.refresh();
		}
		else {
			delay_manager->delay(timeperframe - elapsedtime.elapsedTime());
		}
	}
	if (!fullscreen.load()) {
		std::lock_guard<std::mutex> fullscreen_lock(window_mutex);
		ntris::window_position = window.getPosition();
	}
	Log::update<std::string>("system", std::string("Window terminating"));
	cm.refresh(true);
	cm.close_info_window();
}

bool Window::isWindowOpen()
{
	std::lock_guard<std::mutex> fullscreen_lock(window_mutex);
	return window.isOpen();
}

Window::Window(const std::size_t& _width, const std::size_t& _height, const OPT& optimized)
{
	close_window.store(false);
	hide_cursor.store(false);
	toggle_fullscreen.store(false);
	fullscreen.store(false);
	window_size_x.store(1);
	window_size_y.store(1);
	switch (optimized) {
	case GENERAL:
		delay_manager = std::make_unique<GeneralDelayManager>();
		break;
	case SMALLEST:
		delay_manager = std::make_unique<SmallestDelayManager>();
		break;
	case SPAM:
		delay_manager = std::make_unique<SpamDelayManager>();
		break;
	case FULL:
		delay_manager = std::make_unique<FullThreadDelayManager>();
		break;
	case NOTHING:
		delay_manager = std::make_unique<NothingDelayManager>();
		break;
	case ARRAY:
		delay_manager = std::make_unique<ArrayDelayManager>();
		break;
	case ARRAYLOG:
		delay_manager = std::make_unique<ArrayLogDelayManager>();
		break;
	case ARRAYBUCKET:
		delay_manager = std::make_unique<BucketArrayDelayManager>();
		break;
	default:
		delay_manager = std::make_unique<GeneralDelayManager>();
	}
	ConfigReader cfg(std::string("settings/config.txt"));
	ConfigSaver config_saver(cfg);

	std::pair<largest_uint, largest_uint>  tilesize = { ntris::tilesize.first, ntris::tilesize.second };
	const sf::Vector3<std::size_t> extra_render(16, 16, 64);
	TileRenderer tilerend(ntris::ntsc_tiles_x, ntris::ntsc_tiles_y, tilesize, TileRenderer::DRAWTEXTURE, extra_render);

	tilerend.load("texturesprite/sprites.txt");

	ntris::shader = config_saver.getShader();
	if (ntris::shader)
		tilerend.set_shader("shaders/crt.glsl", sf::Shader::Fragment);

	ntris::four_thirds = config_saver.getFourThirds();
	ntris::fullscreen = config_saver.getFullscreen();
	fullscreen.store(ntris::fullscreen);
	ntris::window_scale = config_saver.setWindowScale(tilerend.width_pixels, tilerend.height_pixels, ntris::four_thirds);

	std::size_t window_width = tilerend.width_pixels * ntris::window_scale.x;
	std::size_t window_height = tilerend.height_pixels * ntris::window_scale.y;

	window.create(sf::VideoMode(tilerend.width_pixels, tilerend.height_pixels), "NESTRIS");

	ntris::window_position = config_saver.setWindowPosition(window_width, window_height);

	window.setPosition(ntris::window_position);

	window.setSize(sf::Vector2u(window_width, window_height));

	window_view = window.getView();
	//tilerend.load("texturesprite/sprites.txtupdated");

	window.setActive(false);
	fullscreen.store(false);

	std::thread render_thread(&Window::render, this, std::ref(tilerend));

	sf::Event event;
	bool is_mouse_hidden = false;
	while (isWindowOpen()&&!close_window.load()) {
		if (window.waitEvent(event)) {
			if (!is_mouse_hidden && hide_cursor.load()) {
				window.setMouseCursorVisible(false);
				is_mouse_hidden = true;
			}
			else if (is_mouse_hidden && event.type==sf::Event::MouseMoved) {
				window.setMouseCursorVisible(true);
				is_mouse_hidden = false;
			}
			event_queue.push(event);
		}
		if (toggle_fullscreen.load()) {
			toggle_fullscreen.store(false);
			toggle_fullscreen_func();
		}
	}
	if (render_thread.joinable())
		render_thread.join();
	window.setActive(true);
	window.close();
}


void Window::toggle_fullscreen_func() {
	std::lock_guard fullscreen_lock(window_mutex);

	if (fullscreen.load()) {
		ntris::fullscreen = false;
		fullscreen.store(false);
		window.setActive();
		window.clear();
		window.close();
		window.create(sf::VideoMode(window_size_x.load(), window_size_y.load()), "NESTRIS");
		window.setView(window_view);
		window.setActive(false);
	}
	else {
		ntris::fullscreen = true;
		fullscreen.store(true);
		window.setActive();
		window_view = window.getView();
		window.clear();
		window.close();
		window.create(sf::VideoMode::getFullscreenModes()[0], "NESTRIS", sf::Style::Fullscreen);
		sf::View fullscreen_view(sf::FloatRect(0,0,256, 224));
		sf::Vector2f size = { window_view.getViewport().width,window_view.getViewport().height };
		sf::Vector2f fullscreen_size = {(float) sf::VideoMode::getFullscreenModes()[0].width, (float)sf::VideoMode::getFullscreenModes()[0].height };
		sf::Vector2f window_position = {(float) ntris::window_position.x+5, (float) ntris::window_position.y +30}; //Idk how to get the title bar height
		fullscreen_view.setViewport(sf::FloatRect(window_position.x/fullscreen_size.x, window_position.y / fullscreen_size.y,window_size_x.load()/fullscreen_size.x, window_size_y.load() / fullscreen_size.y));
		window.setView(fullscreen_view);
		window.setActive(false);
	}
}
		