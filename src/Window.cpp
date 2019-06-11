#include"Window.hpp"
#include<cmath>
#include"ConsoleManager.hpp"
#include<chrono>
#include<thread>
#include<string>
#include<limits>
#include"enums.hpp"
#define SLEEP_SFML //high resolution clock isn't very high resolution
#define SLEEP_MICROSECONDS
constexpr unsigned long long MIN_DELAY_ERROR=1000; //todo put in enums as constexpr


class MyClock {
    #ifdef SLEEP_CX11
    std::chrono::high_resolution_clock::time_point starting_time=std::chrono::high_resolution_clock::now();
    #endif // SLEEP_CX11
    #ifdef SLEEP_SFML
    sf::Clock clock;
    #endif // SLEEP_SFML
    public:
    largest_uint elapsedTime() {
        #ifdef SLEEP_CX11
            #ifdef SLEEP_MILLISECONDS
                return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-starting_time).count();
            #endif // SLEEP_MILLISECONDS

            #ifdef SLEEP_MICROSECONDS
                return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-starting_time).count();
            #endif // SLEEP_MICROSECONDS

            #ifdef SLEEP_NANOSECONDS
                return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()-starting_time).count();
            #endif // SLEEP_NANOSECONDS
        #endif // SLEEP_CX11

        #ifdef SLEEP_SFML
            #ifdef SLEEP_MILLISECONDS
                return clock.getElapsedTime().asMilliseconds();
            #endif // SLEEP_MILLISECONDS

            #ifdef SLEEP_MICROSECONDS
                return clock.getElapsedTime().asMicroseconds();
            #endif // SLEEP_MICROSECONDS

            #ifdef SLEEP_NANOSECONDS
                return clock.getElapsedTime().asMicroseconds()/1000;
            #endif // SLEEP_NANOSECONDS
        #endif // SLEEP_SFML
    }

    void restart(){
        #ifdef SLEEP_CX11
        starting_time=std::chrono::high_resolution_clock::now();
        #endif // SLEEP_CX11
        #ifdef SLEEP_SFML
        clock.restart();
        #endif // SLEEP_SFML
    }
};


Window::Window(const std::size_t& _width, const std::size_t& _height, sf::Vector2f _scale, const OPT& optimized)
{
    sf::Transform state;
    state.scale(_scale);
    sf::RenderWindow window(sf::VideoMode(_width*_scale.x, _height*_scale.y), "Nestris");
	std::pair<largest_uint, largest_uint>  tilesize(8,8);
    const sf::Vector3<std::size_t> extra_render(16,16,64);
    TileRenderer tilerend(_width/8,_height/8,tilesize,TileRenderer::DRAWTEXTURE,extra_render);
    tilerend.load("texturesprite/sprites.txt");
    //tilerend.load("texturesprite/sprites.txtupdated");
    Engine _engine= Engine(tilerend.getTileContainer(),Engine::LEVELSELECT);

    sf::Event event;

    #ifdef SLEEP_MILLISECONDS
	largest_uint partspersecond{ 1000 };
    #endif // SLEEP_MILLISECONDS
    #ifdef SLEEP_MICROSECONDS
	largest_uint partspersecond{ 1000000 };
    #endif // SLEEP_MICROSECONDS
    #ifdef SLEEP_NANOSECONDS
	largest_uint partspersecond = { 1000000000 };
    #endif // SLEEP_NANOSECONDS
    //largest_uint timeperframe_avg=(long double) (partspersecond)/glb::ntsc_fps;
    largest_uint timeperframe_odd=(long double) (partspersecond)/glb::ntsc_fps_odd;
    largest_uint timeperframe_even=(long double) (partspersecond)/glb::ntsc_fps_even;
    bool odd_frame=false;
    largest_uint timeperframe=timeperframe_odd;
    MyClock elapsedtime;
    while (window.isOpen()) {
        odd_frame=!odd_frame; //all this could be unrolled into 2 if's in the cycle
        if (odd_frame) timeperframe=timeperframe_odd;
        else timeperframe=timeperframe_even;
        if (elapsedtime.elapsedTime()>=timeperframe) {

            if (elapsedtime.elapsedTime()>0) ConsoleManager::update<long double>("fps",partspersecond/(long double)elapsedtime.elapsedTime());

            elapsedtime.restart();
            largest_uint delaycalc=0;
            _engine.frame(inputManager.getInput());

            ConsoleManager::update<largest_uint>("input delay",elapsedtime.elapsedTime()-delaycalc);
            delaycalc=elapsedtime.elapsedTime();

            window.clear();//adds 15microseconds
            tilerend.drawmod(window, state);

            ConsoleManager::update<largest_uint>("draw delay",elapsedtime.elapsedTime()-delaycalc);
            delaycalc=elapsedtime.elapsedTime();

            window.display();

            delaycalc=elapsedtime.elapsedTime();
            ConsoleManager::update<largest_uint>("display delay",elapsedtime.elapsedTime()-delaycalc);

            while (window.pollEvent(event))
            {
				switch (event.type) {
				case sf::Event::Closed:
					window.close();
				break;
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::F1) {
						ConsoleManager::toggle_info_window();
						if (!ConsoleManager::is_window_open())
							window.requestFocus();
					}
				break;
				}
				
            }
            ConsoleManager::refresh();
        }
        else {
            switch(optimized) {
            case GENERAL:
                general_delay_manager(timeperframe-elapsedtime.elapsedTime());
                break;
            case SMALLEST:
                smallest_delay_manager(timeperframe-elapsedtime.elapsedTime());
                break;
            case SPAM:
                spam_delay_manager(timeperframe-elapsedtime.elapsedTime());
                break;
            case FULL:
                full_thread_delay_manager(timeperframe-elapsedtime.elapsedTime());
                break;
            case NOTHING:
                nothing_delay_manager(0);
                break;
            case ARRAY:
                array_delay_manager(timeperframe-elapsedtime.elapsedTime());
                break;
            case ARRAYLOG:
                array_delay_manager_log(timeperframe-elapsedtime.elapsedTime());
                break;
            case ARRAYBUCKET:
                array_delay_manager_bucket(timeperframe-elapsedtime.elapsedTime());
                break;
            default:
                general_delay_manager(timeperframe-elapsedtime.elapsedTime());
            }
        }
    }
    ConsoleManager::update<std::string>("system",std::string("Window terminating"));
    ConsoleManager::refresh(true);
}


void Window::general_delay_manager(largest_uint target_delay) {
    static largest_uint max_extra_delay=MIN_DELAY_ERROR;
    if (target_delay<=max_extra_delay) return;
    largest_uint real_delay=sleep_for_how_long(target_delay-max_extra_delay);
    if (real_delay>target_delay) {
        max_extra_delay=real_delay-target_delay+max_extra_delay;
    }
}

void Window::smallest_delay_manager(largest_uint target_delay) {
    static largest_uint max_extra_delay=MIN_DELAY_ERROR;
    if (target_delay<=max_extra_delay) return;
    largest_uint real_delay=sleep_for_how_long(target_delay-max_extra_delay);
    if (real_delay>max_extra_delay) {
        max_extra_delay=real_delay;
    }
}

void Window::array_delay_manager(largest_uint target_delay) {
    MyClock clock;
	constexpr std::size_t max_delay_size{ 1000000 }; //todo maybe group delays together
    static largest_uint arr_delay[max_delay_size]={MIN_DELAY_ERROR};
    if (target_delay>=max_delay_size) target_delay=max_delay_size-1;
    largest_uint reduced_delay=target_delay-clock.elapsedTime();
    while (clock.elapsedTime()<target_delay&& reduced_delay<=arr_delay[reduced_delay]) {
        reduced_delay=target_delay-clock.elapsedTime();
    };
    if (reduced_delay<=0) return;
    largest_uint real_delay=sleep_for_how_long(reduced_delay-arr_delay[reduced_delay]);
    if (real_delay>reduced_delay) {
        arr_delay[reduced_delay]=real_delay-reduced_delay;
    }
}

void Window::array_delay_manager_bucket(largest_uint target_delay) {
    //MyClock clock;
//    static largest_uint max_iterations=0;
//    static largest_uint max_clock=0;
//    static largest_uint sum_iterations=0;
//    static largest_uint sum_clock=0;
//    static largest_uint tot_iterations=0;
    #ifdef SLEEP_MILLISECONDS
	constexpr largest_uint partsperframe = largest_uint{ 1000 } / glb::ntsc_fps;
    #endif // SLEEP_MILLISECONDS
    #ifdef SLEEP_MICROSECONDS
    constexpr largest_uint partsperframe= largest_uint{ 1000000 } /glb::ntsc_fps;
    #endif // SLEEP_MICROSECONDS
    #ifdef SLEEP_NANOSECONDS
    constexpr largest_uint partsperframe= largest_uint{ 1000000000 } /glb::ntsc_fps;
    #endif // SLEEP_NANOSECONDS
	constexpr largest_uint bucket_size{ 200 };
    static largest_uint arr_delay[partsperframe/bucket_size+2]={1};
    if (target_delay<=0) return;
    if (target_delay>=partsperframe) target_delay=partsperframe;
    auto bucket_finder = [&bucket_size](largest_uint index) {
        return index/bucket_size;
    };
    //largest_uint counter=0;
    //target_delay-=clock.elapsedTime();
    while (target_delay>0 && target_delay<arr_delay[bucket_finder(target_delay)]) {
        target_delay-=bucket_size;
        //++counter;
        //target_delay-=clock.elapsedTime();
    }
    /*if (counter>max_iterations) max_iterations=counter;
    tot_iterations++;
    sum_iterations+=counter;
    sum_clock+=clock.elapsedTime();
    if (clock.elapsedTime()>max_clock) max_clock=clock.elapsedTime();*/
    using namespace std::string_literals;
    //ConsoleManager::update("system","max:"s+std::to_string(max_iterations)+"   avg iter:"+std::to_string((long double)sum_iterations/(long double)tot_iterations)+"   avg clock:"s+std::to_string((long double)sum_clock/(long double) tot_iterations)+ "   max clock:"s+std::to_string(max_clock));
    if (target_delay<0) return;
    largest_uint real_delay = sleep_for_how_long(bucket_finder(target_delay)*bucket_size-arr_delay[bucket_finder(target_delay)]);
    if (real_delay>bucket_finder(target_delay)*bucket_size) arr_delay[bucket_finder(target_delay)]=real_delay-bucket_finder(target_delay)*bucket_size;
}

constexpr largest_uint log2(largest_uint _logarg) {
	constexpr std::size_t max_bits = std::numeric_limits<unsigned long long>::digits;
	for (std::size_t i = 0; i < max_bits && _logarg>0; ++i) {
		_logarg <<= std::size_t{ 1 };
	}
	return _logarg;
}


void Window::array_delay_manager_log(largest_uint target_delay) {
    #ifdef SLEEP_MILLISECONDS
    constexpr unsigned long long partsperframe=1000/glb::ntsc_fps;
    #endif // SLEEP_MILLISECONDS
    #ifdef SLEEP_MICROSECONDS
    constexpr unsigned long long partsperframe=1000000/glb::ntsc_fps;
    #endif // SLEEP_MICROSECONDS
    #ifdef SLEEP_NANOSECONDS
    constexpr unsigned long long partsperframe=1000000000/glb::ntsc_fps;
    #endif // SLEEP_NANOSECONDS
	constexpr std::size_t array_length = log2(partsperframe) + 1; //constexpr std::size_t array_length=std::ceil(std::log2(partsperframe)); //ceil isn't constexpr for some reason
    static largest_uint arr_delay[array_length]={MIN_DELAY_ERROR};
	constexpr largest_uint max_delay_allowed = largest_uint{ 1 } << array_length;
    if (target_delay<=0) return;
    std::size_t index=0;
    std::size_t result_index=-1;
    if (target_delay>max_delay_allowed) target_delay=max_delay_allowed;
    unsigned long long t_delay=target_delay;
    while (t_delay>0) {
        if (arr_delay[index]<target_delay) result_index=index;
        t_delay>>=1;
        index++;
    }
    if (result_index>=array_length||result_index<0) return;
    target_delay=largest_uint(1)<<result_index;
    //ConsoleManager::update("system", std::to_string(target_delay)+std::string("    "));
    largest_uint real_delay=sleep_for_how_long(target_delay-arr_delay[result_index]);
    if (real_delay>target_delay) arr_delay[result_index]=real_delay-target_delay;
}

void Window::spam_delay_manager(largest_uint target_delay) {
    static largest_uint smallest_delay_possible=MIN_DELAY_ERROR;
    MyClock clock;
    largest_uint _late_time=clock.elapsedTime();
    while(_late_time+smallest_delay_possible<target_delay) {
        largest_uint _prev_time = _late_time;
        sleep(1);
        largest_uint _late_time=clock.elapsedTime();
        if (_late_time-_prev_time>smallest_delay_possible)smallest_delay_possible=_late_time-_prev_time;
    }
}

void Window::full_thread_delay_manager(largest_uint target_delay) {
    static largest_uint smallesttimeunit=MIN_DELAY_ERROR;
    MyClock clock;
    largest_uint _last_time=clock.elapsedTime();
    while(clock.elapsedTime()+smallesttimeunit<target_delay) {
        largest_uint __temp=(clock.elapsedTime()-_last_time);
        if (__temp>smallesttimeunit) smallesttimeunit=__temp;
    };
}

void Window::nothing_delay_manager(largest_uint target_delay) {
}

largest_uint Window::sleep_for_how_long(largest_uint _delay) {
    static MyClock clock;
    clock.restart();
    sleep(_delay);
    return clock.elapsedTime();
}
void Window::sleep(largest_uint _delay) {
    if (_delay<=0) return;
    #ifdef SLEEP_CX11
        #ifdef SLEEP_MILLISECONDS
        std::this_thread::sleep_for(std::chrono::milliseconds(_delay));
        return;
        #endif
        #ifdef SLEEP_MICROSECONDS
        std::this_thread::sleep_for(std::chrono::microseconds(_delay));
        return;
        #endif
        #ifdef SLEEP_NANOSECONDS
        std::this_thread::sleep_for(std::chrono::nanoseconds(_delay));
        return;
    #endif
    #else
    // SLEEP_CX11
        #ifdef SLEEP_SFML
            #ifdef SLEEP_MILLISECONDS
            sf::sleep(sf::milliseconds(_delay));
            return;
            #endif
            #ifdef SLEEP_MICROSECONDS
            sf::sleep(sf::microseconds(_delay));
            return;
            #endif
            #ifdef SLEEP_NANOSECONDS
            sf::sleep(sf::microseconds(_delay/1000+1));
            return;
            #endif // SLEEP_NANOSECONDS
        #endif // SLEEP_SFML
    #endif
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
