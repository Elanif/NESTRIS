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

void Window::render(sf::RenderWindow& window, TileRenderer& tilerend) {
	close_window.store(false);
	hide_cursor.store(false);
	ConsoleManager cm;
	Engine _engine = Engine(tilerend.getTileContainer(), Engine::LEVELSELECT);

	constexpr largest_uint partspersecond = MyClock::getPartsPerSecond();
	largest_uint timeperframe_odd = (long double)(partspersecond) / ntris::ntsc_fps_odd;
	largest_uint timeperframe_even = (long double)(partspersecond) / ntris::ntsc_fps_even;
	bool odd_frame = false;
	largest_uint timeperframe = timeperframe_odd;

	MyClock elapsedtime;
	std::size_t counter = 0;

	while (!close_window.load()&&window.isOpen()) {
		odd_frame = !odd_frame; //Compiler should create 2 different cycles
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
			window.clear();//adds 15microseconds
			
			tilerend.drawmod(window);

			Log::update<sf::Int64>("draw delay", elapsedtime.elapsedTime() - delaycalc);
			//std::cout << "draw delay"<<elapsedtime.elapsedTime() - delaycalc << ntris::newline;
			delaycalc = elapsedtime.elapsedTime();

			window.display();

			Log::update<sf::Int64>("display delay", elapsedtime.elapsedTime() - delaycalc);
			//std::cout << "display delay"<<elapsedtime.elapsedTime() - delaycalc << ntris::newline;
			delaycalc = elapsedtime.elapsedTime();

			while (event_queue.size()>0)
			{
				sf::Event event = event_queue.pop();
				switch (event.type) {
				case sf::Event::Closed:
					close_window.store(true);
				break;
				case sf::Event::KeyPressed: {
					bool ctrl = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
					if (window.hasFocus()) {
						if (event.key.code == sf::Keyboard::F1) {
							cm.toggle_info_window();
							if (!cm.is_window_open())
								window.requestFocus();
						}
						if ((event.key.code == sf::Keyboard::F && ctrl)/* || Input::getInput()*/) {
							toggle_fullscreen(window);
						}
					}
				}
				break;
				case sf::Event::Resized:

				break;
				}
			}
			if (!ntris::fullscreen) {
				ntris::window_position = window.getPosition(); //move this out of the loop if it's slow
				//ntris::window_scale set
			}
			cm.refresh();
		}
		else {
			delay_manager->delay(timeperframe - elapsedtime.elapsedTime());
		}
	}

	Log::update<std::string>("system", std::string("Window terminating"));
	cm.refresh(true);
	cm.close_info_window();
}

Window::Window(const std::size_t& _width, const std::size_t& _height, const OPT& optimized)
{
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
	sf::Shader test_shader;
	if (ntris::shader)
		tilerend.set_shader("shaders/crt.glsl", sf::Shader::Fragment);

	ntris::four_thirds = config_saver.getFourThirds();
	ntris::fullscreen = config_saver.getFullscreen();
	ntris::window_scale = config_saver.setWindowScale(tilerend.width_pixels, tilerend.height_pixels, ntris::four_thirds);

	std::size_t window_width = tilerend.width_pixels * ntris::window_scale.x;
	std::size_t window_height = tilerend.height_pixels * ntris::window_scale.y;

	/*sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Nestris", sf::Style::Fullscreen);*/
	sf::RenderWindow window(sf::VideoMode(tilerend.width_pixels, tilerend.height_pixels), "Nestris");

	ntris::window_position = config_saver.setWindowPosition(window_width, window_height);

	window.setPosition(ntris::window_position);

	window.setSize(sf::Vector2u(window_width, window_height));

	//tilerend.load("texturesprite/sprites.txtupdated");

	window.setActive(false);
	std::thread render_thread(&Window::render, this, std::ref(window), std::ref(tilerend));

	sf::Event event;
	bool is_mouse_hidden = false;
	while (window.isOpen()&&!close_window) {
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
	}
	if (render_thread.joinable())
		render_thread.join();
	window.setActive(true);
	window.close();
}


void Window::toggle_fullscreen(sf::RenderWindow& window) {
	ntris::fullscreen = !ntris::fullscreen;
	if (ntris::fullscreen) {
		//window.create()
	}
}