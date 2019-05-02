#include"Window.h"
#include<cmath>
#include"ConsoleManager.h"
#include<chrono>
#include<thread>
#include<string>
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
    sf::Int64 elapsedTime() {
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
    sf::Vector2u tilesize(8,8);
    const sf::Vector3<std::size_t> extra_render(16,16,64);
    TileRenderer tilerend(_width/8,_height/8,tilesize,TileRenderer::DRAWTEXTURE,extra_render);
    tilerend.load("texturesprite/sprites.txt");
    //tilerend.load("texturesprite/sprites.txtupdated");
    Engine _engine= Engine(tilerend.getTileContainer(),Engine::LEVELSELECT);

    sf::Event event;

    #ifdef SLEEP_MILLISECONDS
    sf::Int64 partspersecond=1000ll;
    #endif // SLEEP_MILLISECONDS
    #ifdef SLEEP_MICROSECONDS
    sf::Int64 partspersecond=1000000ll;
    #endif // SLEEP_MICROSECONDS
    #ifdef SLEEP_NANOSECONDS
    sf::Int64 partspersecond=1000000000ll;
    #endif // SLEEP_NANOSECONDS
    //sf::Int64 timeperframe_avg=(long double) (partspersecond)/glb::ntsc_fps;
    sf::Int64 timeperframe_odd=(long double) (partspersecond)/glb::ntsc_fps_odd;
    sf::Int64 timeperframe_even=(long double) (partspersecond)/glb::ntsc_fps_even;
    bool odd_frame=false;
    sf::Int64 timeperframe=timeperframe_odd;
    MyClock elapsedtime;
    while (window.isOpen()) {
        odd_frame=!odd_frame; //all this could be unrolled into 2 if's in the cycle
        if (odd_frame) timeperframe=timeperframe_odd;
        else timeperframe=timeperframe_even;
        if (elapsedtime.elapsedTime()>=timeperframe) {

            if (elapsedtime.elapsedTime()>0) ConsoleManager::update<long double>("fps",partspersecond/(long double)elapsedtime.elapsedTime());

            elapsedtime.restart();
            sf::Int64 delaycalc=0;
            _engine.frame(inputManager.getInput());

            ConsoleManager::update<sf::Int64>("input delay",elapsedtime.elapsedTime()-delaycalc);
            delaycalc=elapsedtime.elapsedTime();

            window.clear();//adds 15microseconds
            tilerend.drawmod(window, state);

            ConsoleManager::update<sf::Int64>("draw delay",elapsedtime.elapsedTime()-delaycalc);
            delaycalc=elapsedtime.elapsedTime();

            window.display();

            delaycalc=elapsedtime.elapsedTime();
            ConsoleManager::update<sf::Int64>("display delay",elapsedtime.elapsedTime()-delaycalc);

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            ConsoleManager::print();
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
    ConsoleManager::print(true);
}


void Window::general_delay_manager(sf::Int64 target_delay) {
    static sf::Int64 max_extra_delay=MIN_DELAY_ERROR;
    if (target_delay<=max_extra_delay) return;
    sf::Int64 real_delay=sleep_for_how_long(target_delay-max_extra_delay);
    if (real_delay>target_delay) {
        max_extra_delay=real_delay-target_delay+max_extra_delay;
    }
}

void Window::smallest_delay_manager(sf::Int64 target_delay) {
    static sf::Int64 max_extra_delay=MIN_DELAY_ERROR;
    if (target_delay<=max_extra_delay) return;
    sf::Int64 real_delay=sleep_for_how_long(target_delay-max_extra_delay);
    if (real_delay>max_extra_delay) {
        max_extra_delay=real_delay;
    }
}

void Window::array_delay_manager(sf::Int64 target_delay) {
    MyClock clock;
    constexpr std::size_t max_delay_size=1000000; //todo maybe group delays together
    static sf::Int64 arr_delay[max_delay_size]={MIN_DELAY_ERROR};
    if (target_delay>=max_delay_size) target_delay=max_delay_size-1;
    sf::Int64 reduced_delay=target_delay-clock.elapsedTime();
    while (clock.elapsedTime()<target_delay&& reduced_delay<=arr_delay[reduced_delay]) {
        reduced_delay=target_delay-clock.elapsedTime();
    };
    if (reduced_delay<=0) return;
    sf::Int64 real_delay=sleep_for_how_long(reduced_delay-arr_delay[reduced_delay]);
    if (real_delay>reduced_delay) {
        arr_delay[reduced_delay]=real_delay-reduced_delay;
    }
}

void Window::array_delay_manager_bucket(sf::Int64 target_delay) {
    //MyClock clock;
//    static sf::Int64 max_iterations=0;
//    static sf::Int64 max_clock=0;
//    static sf::Int64 sum_iterations=0;
//    static sf::Int64 sum_clock=0;
//    static sf::Int64 tot_iterations=0;
    #ifdef SLEEP_MILLISECONDS
    constexpr sf::Int64 partsperframe=1000/glb::ntsc_fps;
    #endif // SLEEP_MILLISECONDS
    #ifdef SLEEP_MICROSECONDS
    constexpr sf::Int64 partsperframe=1000000/glb::ntsc_fps;
    #endif // SLEEP_MICROSECONDS
    #ifdef SLEEP_NANOSECONDS
    constexpr sf::Int64 partsperframe=1000000000/glb::ntsc_fps;
    #endif // SLEEP_NANOSECONDS
    constexpr sf::Int64 bucket_size=200;
    static sf::Int64 arr_delay[partsperframe/bucket_size+2]={1};
    if (target_delay<=0) return;
    if (target_delay>=partsperframe) target_delay=partsperframe;
    auto bucket_finder = [&bucket_size](sf::Int64 index) {
        return index/bucket_size;
    };
    //sf::Int64 counter=0;
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
    sf::Int64 real_delay = sleep_for_how_long(bucket_finder(target_delay)*bucket_size-arr_delay[bucket_finder(target_delay)]);
    if (real_delay>bucket_finder(target_delay)*bucket_size) arr_delay[bucket_finder(target_delay)]=real_delay-bucket_finder(target_delay)*bucket_size;
}



void Window::array_delay_manager_log(sf::Int64 target_delay) {
    #ifdef SLEEP_MILLISECONDS
    constexpr unsigned long long partsperframe=1000/glb::ntsc_fps;
    #endif // SLEEP_MILLISECONDS
    #ifdef SLEEP_MICROSECONDS
    constexpr unsigned long long partsperframe=1000000/glb::ntsc_fps;
    #endif // SLEEP_MICROSECONDS
    #ifdef SLEEP_NANOSECONDS
    constexpr unsigned long long partsperframe=1000000000/glb::ntsc_fps;
    #endif // SLEEP_NANOSECONDS
    constexpr std::size_t array_length=std::log2(partsperframe)+1; //constexpr std::size_t array_length=std::ceil(std::log2(partsperframe)); //ceil isn't constexpr for some reason
    static sf::Int64 arr_delay[array_length]={MIN_DELAY_ERROR};
    constexpr sf::Int64 max_delay_allowed=1u<<array_length;
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
    target_delay=1u<<result_index;
    //ConsoleManager::update("system", std::to_string(target_delay)+std::string("    "));
    sf::Int64 real_delay=sleep_for_how_long(target_delay-arr_delay[result_index]);
    if (real_delay>target_delay) arr_delay[result_index]=real_delay-target_delay;
}

void Window::spam_delay_manager(sf::Int64 target_delay) {
    static sf::Int64 smallest_delay_possible=MIN_DELAY_ERROR;
    MyClock clock;
    sf::Int64 _late_time=clock.elapsedTime();
    while(_late_time+smallest_delay_possible<target_delay) {
        sf::Int64 _prev_time = _late_time;
        sleep(1);
        sf::Int64 _late_time=clock.elapsedTime();
        if (_late_time-_prev_time>smallest_delay_possible)smallest_delay_possible=_late_time-_prev_time;
    }
}

void Window::full_thread_delay_manager(sf::Int64 target_delay) {
    static sf::Int64 smallesttimeunit=MIN_DELAY_ERROR;
    MyClock clock;
    sf::Int64 _last_time=clock.elapsedTime();
    while(clock.elapsedTime()+smallesttimeunit<target_delay) {
        sf::Int64 __temp=(clock.elapsedTime()-_last_time);
        if (__temp>smallesttimeunit) smallesttimeunit=__temp;
    };
}

void Window::nothing_delay_manager(sf::Int64 target_delay) {
}

sf::Int64 Window::sleep_for_how_long(sf::Int64 _delay) {
    static MyClock clock;
    clock.restart();
    sleep(_delay);
    return clock.elapsedTime();
}
void Window::sleep(sf::Int64 _delay) {
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
sf::Int64 smallesttimeunit=sf::Int64(0);
    sf::Int64 totaltimewaited=0;
    sf::Clock onesecondinit;
    sf::Clock elapsedtime=sf::Clock();
    sf::Int64 testnumber=0;
    std::vector<sf::Int64> timetests;
    while (onesecondinit.getElapsedTime()<sf::Time(sf::milliseconds(1000))) {
        elapsedtime.restart();
        sf::sleep(sf::microseconds(1));
        sf::Int64 _timetemp=elapsedtime.getElapsedTime().asMicroseconds();
        timetests.push_back(_timetemp);
        //printf("%d ",(int)_timetemp);
        totaltimewaited+= _timetemp;
        if (_timetemp>smallesttimeunit)smallesttimeunit=_timetemp;
        ++testnumber;
    }
    double mean=(double)totaltimewaited/(double)testnumber;
    double variance=0;
    for (sf::Int64 i=0; i<testnumber; ++i) {
        variance+=(timetests[i]-mean)*(timetests[i]-mean);
    }
    variance/=(double)testnumber-1;
    double sd=sqrt(variance);
    printf("mean =%f, sd=%f, variance=%f, biggest hiccup=%d\n",mean,sd,variance,(int)smallesttimeunit);
    */

/* data renderer
sf::Int64 datawidth;
    window.clear();
    for (datawidth=0; datawidth<_width;++datawidth) {
        int rectheight=0;
        sf::RectangleShape istogramma;
        sf::Int64 widthtemp=smallesttimeunit/_width;
        sf::Int64 left=datawidth*widthtemp;
        sf::Int64 right=datawidth*(widthtemp+1);
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
    sf::Int64 smallesttimeunit=sf::Int64(0);
    sf::Clock onesecondinit;
    sf::Clock elapsedtime=sf::Clock();
    while (onesecondinit.getElapsedTime()<sf::Time(sf::milliseconds(1000))) {
        elapsedtime.restart();
        sf::sleep(sf::microseconds(1));
        sf::Int64 _timetemp = elapsedtime.getElapsedTime().asMicroseconds();
        if (_timetemp>smallesttimeunit) smallesttimeunit=_timetemp;
    }
    sf::Int64 timeperframe=1000000./60.0988;
    printf("%d\n",(int)smallesttimeunit);
    elapsedtime.restart();
    while (window.isOpen()) {
        if (optimized) {
            sf::Int64 sleeptime=timeperframe-elapsedtime.getElapsedTime().asMicroseconds();
            sleeptime-=smallesttimeunit;
            sf::sleep(sf::microseconds(sleeptime));
        }
        else {
            sf::Int64 cycleduration=(timeperframe-elapsedtime.getElapsedTime().asMicroseconds())/smallesttimeunit;
            for (sf::Int64 i=0; i<cycleduration; ++i)
                sf::sleep(sf::microseconds(1));
            /COMMENTSTARTif (timeperframe-elapsedtime.getElapsedTime().asMicroseconds()<smallesttimeunit)
                sf::sleep(sf::microseconds(1));COMMENTEND/
        }
        if (elapsedtime.getElapsedTime().asMicroseconds()+sf::Int64(1)>=timeperframe) {
            printf("fps=%f\n",(double)sf::Int64(1000000)/(double)elapsedtime.getElapsedTime().asMicroseconds());
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
