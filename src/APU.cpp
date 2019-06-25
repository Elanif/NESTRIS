#include"APU.hpp"
#include<cmath>

std::vector<sf::Int16> APU::square_vector;
sf::SoundBuffer APU::square_buffer;
sf::Sound APU::sound;

int up_or_down(double hz, std::size_t i, double resolution) {
    if (std::fmod(i/resolution,1.L/hz)>=0.5L/hz) return 1;
    else return -1;
}



void APU::init() {
    double duration=5;
    double hz=40000;
    unsigned long long resolution=44100;
    square_vector.reserve(duration*resolution+1);
    sf::Int16 volume=2000;
    for (std::vector<sf::Int16>::size_type i=0; i<duration*resolution; ++i) {
        square_vector.push_back(volume*up_or_down(hz,i,resolution));
    }
    square_buffer.loadFromSamples(&square_vector[0], duration*resolution,1,resolution);
}

void APU::test() {
    sound.setBuffer(square_buffer);
    sound.play();
}
