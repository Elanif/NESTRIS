#ifndef SOUND_H
#define SOUND_H
#include<SFML/Audio.hpp>
#include<unordered_map>

class Sound
{
public:

    enum samples {
        move_cursor = 12,
        menu_start = 13,
        move_piece = 14,
        rotate_piece = 16,
        level_up = 17,
        drop_piece = 18,
        tetris = 20,
        clear_line = 21,
        top_out = 24
    };

    static void init();
    static void play(samples sample);

private:

    //at the moment there's only sq1 and noi sound
    static sf::Sound sq1; //first square wave channel
    static sf::Sound sq2; //second square wave channel
    static sf::Sound tri; //triangle wave channel
    static sf::Sound noi; //noise channel

    static std::vector<sf::SoundBuffer> sq1buffer;
    static std::vector<sf::SoundBuffer> noibuffer;
};

#endif // SOUND_H
