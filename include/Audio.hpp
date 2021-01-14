#ifndef AUDIO_H
#define AUDIO_H
#include"ntris.hpp"
#include"StaticClass.hpp"
#include<SFML/Audio.hpp>

class Audio : sf::NonCopyable {
public:
	void init();
	void setVolumeMusic(float const& volume);
	void setVolumeSFX(float const& volume);
	void muteMusic();
	void unMuteMusic();
	void muteSFX();
	void unMuteSFX();
	void playMusic(std::size_t const& theme_number, bool const& fast);
	void stopMusic();
	void playPiece();
	void playTetris();
	void PlayClearLines();
	void PlaySelectMenu();
	void PlayPieceLocking();

private:
	sf::SoundBuffer move_or_rotate_piece;
	sf::SoundBuffer tetris;
	sf::SoundBuffer clear_lines;
	sf::SoundBuffer piece_locking;
	sf::SoundBuffer select_menu;
	sf::SoundBuffer move_menu;
	sf::Sound sfx; //do sfx's override each other? like tetris/piece movement/line clearring?
							//if they don't I have to make more sf::Sounds

	sf::Music music_theme;

	bool is_music_playing=false;
	std::size_t current_theme=0;
	bool music_fast=false;
	float volume_music=0.f;
	bool muted_music=false;

	float volume_SFX=0.f;
	bool muted_SFX=false;
};
#endif