#include"Audio.hpp"
#include<SFML/Audio.hpp>
#include"Log.hpp"

sf::SoundBuffer Audio::move_menu;

void Audio::init()
{
	sf::SoundBuffer move_menu;
	if (!move_menu.loadFromFile("sounds/15sq1.wav"))
		Log::update_error("Couldn't load 12sq1.wav - move_menu");
	sf::Sound move_menu_sound;
	move_menu_sound.setBuffer(move_menu);
	move_menu_sound.setVolume(100.f);
	move_menu_sound.play();
	
}
