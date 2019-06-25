#include"Sound.hpp"
#include<string>
std::vector<sf::SoundBuffer> Sound::sq1buffer(31);
std::vector<sf::SoundBuffer> Sound::noibuffer(31);

sf::Sound Sound::sq1;
sf::Sound Sound::sq2;
sf::Sound Sound::tri;
sf::Sound Sound::noi;

void Sound::init() {
    using namespace std::string_literals;
    std::vector<std::size_t> sq1files={12,13,14,16,17,18,20,21};
    for (const auto& i: sq1files) {
        if (!sq1buffer[i].loadFromFile("sounds/"s+std::to_string(i)+"sq1.wav")) ; //todo error
    }

    std::vector<std::size_t> noifiles={24,25};
    for (const auto& i: noifiles) {
        if (!noibuffer[i].loadFromFile("sounds/"s+std::to_string(i)+"noi.wav")) ;//todo error
    }

}

void Sound::play(samples sample) {
    switch (sample) { //not really expansion supportive
        case samples::top_out: {
            noi.stop();
            noi.setBuffer(noibuffer[(std::size_t) sample]);
            noi.play();
        }
        break;
        default: {
            sq1.stop();
            sq1.setBuffer(sq1buffer[(std::size_t) sample]);
            sq1.play();
        }
    }
}


//if you move a piece, rotate it, and drop it in the same frame, what plays?
