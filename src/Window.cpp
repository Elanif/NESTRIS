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

	bool setFourThirds() {
		std::vector<bool> four_thirds = cfg.get<bool>("four_thirds");
		if (four_thirds.size() < 1) return true;
		if (four_thirds[0] == true) return true;
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
		cfg.overwrite<bool>("four_thirds", std::vector<bool>(1,true));
	}
	void saveWindowPosition() {
		saveConfig("window_position", ntris::window_position);
	}
	void save_on_exit() {
		_save_on_exit = true;
	}
	ConfigSaver(ConfigReader& _cfg) :cfg(_cfg) {};
	~ConfigSaver() {
		if (_save_on_exit) {
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
			_engine.frame(inputManager.getInput());

			Log::update<sf::Int64>("input delay", elapsedtime.elapsedTime() - delaycalc);
			delaycalc = elapsedtime.elapsedTime();
			//std::cout <<"input delay"<< elapsedtime.elapsedTime() - delaycalc << ntris::newline;
			window.clear();//adds 15microseconds
			tilerend.drawmod(window);

			Log::update<sf::Int64>("draw delay", elapsedtime.elapsedTime() - delaycalc);
			std::cout << elapsedtime.elapsedTime() - delaycalc << ntris::newline;
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
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::F1 && window.hasFocus()) {
						cm.toggle_info_window();
						if (!cm.is_window_open())
							window.requestFocus();
					}
				break;
				case sf::Event::Resized:

				break;
				}

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
	/*case ARRAY:
		delay_manager = std::make_unique<ArrayDelayManager>();
		break;*/
	/*case ARRAYLOG:
		delay_manager = std::make_unique<ArrayLogDelayManager>();
		break;*/
	/*case ARRAYBUCKET:
		delay_manager = std::make_unique<BucketArrayDelayManager>();
		break;*/
	default:
		delay_manager = std::make_unique<GeneralDelayManager>();
	}
	ConfigReader cfg(std::string("settings/config.txt"));
	ConfigSaver config_saver(cfg);

	std::pair<largest_uint, largest_uint>  tilesize = { ntris::tilesize.first, ntris::tilesize.second };
	const sf::Vector3<std::size_t> extra_render(16, 16, 64);
	TileRenderer tilerend(ntris::ntsc_tiles_x, ntris::ntsc_tiles_y, tilesize, TileRenderer::DRAWTEXTURE, extra_render);

	ntris::four_thirds = config_saver.setFourThirds();
	ntris::window_scale = config_saver.setWindowScale(tilerend.width_pixels, tilerend.height_pixels, ntris::four_thirds);

	std::size_t window_width = tilerend.width_pixels * ntris::window_scale.x;
	std::size_t window_height = tilerend.height_pixels * ntris::window_scale.y;

	sf::RenderWindow window(sf::VideoMode(tilerend.width_pixels, tilerend.height_pixels), "Nestris");

	ntris::window_position = config_saver.setWindowPosition(window_width, window_height);

	window.setPosition(ntris::window_position);

	window.setSize(sf::Vector2u(window_width, window_height));

	tilerend.load("texturesprite/sprites.txt");
	//tilerend.load("texturesprite/sprites.txtupdated");

	window.setActive(false);
	std::thread render_thread(&Window::render, this, std::ref(window), std::ref(tilerend));

	sf::Event event;
	while (window.isOpen()&&!close_window) {
		if (window.waitEvent(event)) {
			event_queue.push(event);
		}
	}
	if (render_thread.joinable())
		render_thread.join();
	window.setActive(true);
	window.close();
}


/* data analysis
largest_uint smallesttimeunit=largest_uint(0);
    largest_uint totaltimewaited=0;
    sf::Clock onesecondinit;
    sf::Clock elapsedtime=sf::Clock();
    largest_uint testnumber=0;
    std::vector<largest_uint> timetests;
    while (onesecondinit.getElapsedTime()<sf::Time(sf::milliseconds(1000))) {
        elapsedtime.restart();
        sf::sleep(sf::microseconds(1));
        largest_uint _timetemp=elapsedtime.getElapsedTime().asMicroseconds();
        timetests.push_back(_timetemp);
        //printf("%d ",(int)_timetemp);
        totaltimewaited+= _timetemp;
        if (_timetemp>smallesttimeunit)smallesttimeunit=_timetemp;
        ++testnumber;
    }
    double mean=(double)totaltimewaited/(double)testnumber;
    double variance=0;
    for (largest_uint i=0; i<testnumber; ++i) {
        variance+=(timetests[i]-mean)*(timetests[i]-mean);
    }
    variance/=(double)testnumber-1;
    double sd=sqrt(variance);
    printf("mean =%f, sd=%f, variance=%f, biggest hiccup=%d\n",mean,sd,variance,(int)smallesttimeunit);
    */

/* data renderer
largest_uint datawidth;
    window.clear();
    for (datawidth=0; datawidth<_width;++datawidth) {
        int rectheight=0;
        sf::RectangleShape istogramma;
        largest_uint widthtemp=smallesttimeunit/_width;
        largest_uint left=datawidth*widthtemp;
        largest_uint right=datawidth*(widthtemp+1);
        for (std::size_t i=0; i<timetests.size(); ++i) {
            if (timetests[i]>=left&&timetests[i]<right) {
                rectheight++;
            }
        }
        istogramma.setSize(sf::Vector2f(2,rectheight));
        istogramma.setFillColor(sf::Color(244,244,244,244));
        istogramma.move(sf::Vector2f(2*datawidth,0));
        window.draw(istogramma);
    }
window.display();
scanf("%d",&datawidth);
*/

/*
Nes-like Graphics Emulator
I'm working on a nes project and taking inspiration from the tutorials I tried to create a tilemap[hyperlink]. The problem is that nes tiles work like this:link. This means that each tile in the texture has a variable color and I couldn't think of a smart way to convert the texture to the color I want each time, it seems impossible with my sf::Color and sf::Texture. I decided to build an sf::Image, convert it to a texture and draw it each frame, which resulted in 4fps. I thought that updating the whole image every frame was the culprit, but even after removing that part in the program the fps only went up to ~10fps. I resolved to using textures and I had this idea: since creating a texture for each color (50 different colors on the NES, 4 colors per tile, hundreds of Sprites) is impossible, I thought of creating a dynamically updated texture that updates when a new tile/color combination is requested, managed with a hash map or a container that lets me find the required texture in O(1) time. The problem is that I couldn't find a way to dynamically update a texture. Afterwards I realized that there's probably a way to render from multiple textures, should I create a Sprite for each tile and
 tie it to its texture?
Does anybody have a better idea?
For reference the nes (ntsc at least)

// NO DISTRIBUTION
Window::Window(const std::size_t& _width, const std::size_t& _height, const bool& optimized)
{
    //initWindow(_width,_height);
    //inputManager=initInput();
    sf::RenderWindow window(sf::VideoMode(_width, _height), "Nestris");
    TileRenderer tilerend(_width/8,_height/8);
    tilerend.load("Sprites.txt");
    Engine _engine= Engine(tilerend.getTileContainer(),10); //TODO change 10
    sf::Event event;
    largest_uint smallesttimeunit=largest_uint(0);
    sf::Clock onesecondinit;
    sf::Clock elapsedtime=sf::Clock();
    while (onesecondinit.getElapsedTime()<sf::Time(sf::milliseconds(1000))) {
        elapsedtime.restart();
        sf::sleep(sf::microseconds(1));
        largest_uint _timetemp = elapsedtime.getElapsedTime().asMicroseconds();
        if (_timetemp>smallesttimeunit) smallesttimeunit=_timetemp;
    }
    largest_uint timeperframe=1000000./60.0988;
    printf("%d\n",(int)smallesttimeunit);
    elapsedtime.restart();
    while (window.isOpen()) {
        if (optimized) {
            largest_uint sleeptime=timeperframe-elapsedtime.getElapsedTime().asMicroseconds();
            sleeptime-=smallesttimeunit;
            sf::sleep(sf::microseconds(sleeptime));
        }
        else {
            largest_uint cycleduration=(timeperframe-elapsedtime.getElapsedTime().asMicroseconds())/smallesttimeunit;
            for (largest_uint i=0; i<cycleduration; ++i)
                sf::sleep(sf::microseconds(1));
            /COMMENTSTARTif (timeperframe-elapsedtime.getElapsedTime().asMicroseconds()<smallesttimeunit)
                sf::sleep(sf::microseconds(1));COMMENTEND/
        }
        if (elapsedtime.getElapsedTime().asMicroseconds()+largest_uint(1)>=timeperframe) {
            printf("fps=%f\n",(double)largest_uint(1000000)/(double)elapsedtime.getElapsedTime().asMicroseconds());
            elapsedtime.restart();
            _engine.frame(inputManager.getInput());
            //SFML update window
            window.clear();
            tilerend.drawmod(window, sf::RenderStates());
            //window.draw(tilerend);
            window.display();
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
        }
    }
    printf("hello\n");
}
*/
