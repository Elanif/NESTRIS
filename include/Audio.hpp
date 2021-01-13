#ifndef AUDIO_H
#define AUDIO_H
#include"ntris.hpp"
#include"StaticClass.hpp"
#include<SFML/Audio.hpp>
class Audio : StaticClass {
public:
	static void init();
	static void setVolume(float const& volume);
	static void mute();
	static void unMute();
	static void playMusic(std::size_t const& theme_number, bool const& fast);
	static void	stopMusic();
	static void playPiece();
	static void playTetris();
	static void PlayClearLines();
	static void PlaySelectMenu();
	static void PlayPieceLocking();

private:
	static sf::SoundBuffer move_or_rotate_piece;
	static sf::SoundBuffer tetris;
	static sf::SoundBuffer clear_lines;
	static sf::SoundBuffer piece_locking;
	static sf::SoundBuffer select_menu;
	static sf::SoundBuffer move_menu;
	static sf::Sound sfx; //do sfx's override each other? like tetris/piece movement/line clearring?
							//if they don't I have to make more sf::Sounds

	static sf::Music theme[3];

	static bool is_music_playing;
	static std::size_t current_theme;
	static bool fast_theme;
	static float volume;
	static bool muted;
};
#endif