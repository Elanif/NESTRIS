#include"Audio.hpp"
#include<SFML/Audio.hpp>
#include"Log.hpp"
#include<string>
#include"ntris.hpp"

void Audio::init()
{
	using namespace std::string_literals;
	if (!move_menu.loadFromFile("sounds/12sq1.wav"))
		Log::update_error("Couldn't load 12sq1.wav - move_menu");

	if (!move_menu.loadFromFile("sounds/12sq1.wav"))
		Log::update_error("Couldn't load 12sq1.wav - move_menu");

	for (std::size_t i = 1; i <= 3; ++i) {
		std::string music_theme_src = "sounds/music theme "s + ntris::to_string(i) + ".ogg"s;
		if (!music_theme.openFromFile(music_theme_src))
			Log::update_error("Failed to load"s + music_theme_src + " - Music theme "s + ntris::to_string(i));
		music_theme_src = "sounds/music theme "s + ntris::to_string(i) + " fast.ogg"s;

		if (!music_theme.openFromFile(music_theme_src))
			Log::update_error("Failed to load"s + music_theme_src + " - Music theme "s + ntris::to_string(i) +" fast"s);
	}
}

void Audio::playMusic(std::size_t const& theme_number, bool const& fast)
{
	using namespace std::string_literals;
	if (!fast) {
		std::string music_theme_src = "sounds/music theme "s + ntris::to_string(theme_number) + ".ogg"s;
		if (!music_theme.openFromFile(music_theme_src))
			Log::update_error("Failed to load"s + music_theme_src + " - Music theme "s + ntris::to_string(theme_number));
		else {
			music_theme.setLoop(true);
			music_theme.play();
		}
	}
	else {
		std::string music_theme_src = "sounds/music theme "s + ntris::to_string(theme_number) + " fast.ogg"s;
		if (!music_theme.openFromFile(music_theme_src))
			Log::update_error("Failed to load"s + music_theme_src + " - Music theme "s + ntris::to_string(theme_number) + " fast"s);
		else {
			music_theme.setLoop(true);
			music_theme.play();
		}
	}
}


void Audio::muteSFX() {
	muted_SFX = true;
	sfx.setVolume(0.f);
}

void Audio::unMuteSFX() {
	muted_SFX = false;
	sfx.setVolume(volume_SFX);
}

void Audio::muteMusic() {
	muted_music = true;
	music_theme.setVolume(0.f);
}

void Audio::unMuteMusic() {
	muted_music = false;
	music_theme.setVolume(volume_music);
}

void Audio::setVolumeSFX(float const& _volume) {
	volume_SFX = _volume;
	if (muted_SFX)
		unMuteSFX();
	sfx.setVolume(_volume);
}

void Audio::setVolumeMusic(float const& _volume) {
	volume_music= _volume;
	if (muted_music) 
		unMuteMusic();
	music_theme.setVolume(_volume);
}