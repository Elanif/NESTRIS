#ifndef AUDIO_H
#define AUDIO_H
#include"ntris.hpp"
#include"StaticClass.hpp"
#include<SFML/Audio.hpp>
#include<SFML/System/Time.hpp>
#include<thread>
#include<mutex>
#include<condition_variable>
#include"SafeQueue.hpp"

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
	//priority over SFX comes into place when the sound is actually played, not when it's issued, i.e. after the frames reported below
	void playGameOverMusic() {};
	void playHighScoreMusic() {};
	void stopMusic();
	void playPieceMove(); //played 1 frame after the piece is moved, or 1 frame early if it's moved with das
	void playPieceRotate(); //played 1 frame after the piece rotates
	void playPieceDrop(); //played 4 frames after the pieces drops
	void playTopOut();
	void playLevelUp(); //plays as soon as the last 2 columns of blocks at the edges are done being erased, interrupting tetris sound
	void playTetris(); //the tetris sound is 4~ frames of clear lines, 6~ frames of square, 8 frames of the beginning of the tetris sound, and then the whole tetris sound
						//maybe the first x~ frames of noise depend on the frame mod 4
						//for now I'll use an edited one
	void playClearLines();
	void playMenuMove();
	void playMenuSelect(); 
	void playMenuSelectShort(); //selecting the level makes a shorter sound than selecting the music
	void playOffset(sf::SoundBuffer const& _buffer, sf::Sound& _sound, sf::Time const& _offset);

	sf::SoundBuffer piece_move;
	sf::SoundBuffer piece_rotate;
	sf::SoundBuffer piece_drop;
	sf::SoundBuffer tetris;
	sf::SoundBuffer clear_lines;
	sf::SoundBuffer menu_select;
	sf::SoundBuffer menu_select_short;
	sf::SoundBuffer menu_move;
	sf::SoundBuffer level_up;
private:

	//no sfx plays if the level up jingle is playing
	bool check_if_level_up();
	//no sfx plays if the tetris jingle is playing, except for level up
	bool check_if_tetris();
	bool tetris_active = false;
	bool level_up_active = false;
	sf::SoundBuffer top_out;
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


	std::mutex sfx_mutex;
	std::thread music_thread;
	SafeQueue<std::tuple<sf::SoundBuffer&, sf::Time>> play_queue;
	std::condition_variable new_sound;
	std::atomic<bool> close_thread = false;
};
#endif