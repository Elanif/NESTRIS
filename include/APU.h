#ifndef APU_H
#define APU_H
#include<SFML/Audio.hpp>

class APU
{
    public:
        APU();
        static void init();
        static void test();
        static std::vector<sf::Int16> square_vector;
        static sf::SoundBuffer square_buffer;
        static sf::Sound sound;
    protected:

    private:
};

#endif // APU_H
