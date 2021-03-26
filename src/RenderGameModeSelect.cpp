#include "RenderGameModeSelect.hpp"
#include"TextWriter.hpp"

RenderGameModeSelect::RenderGameModeSelect(TileContainer* _tilecont, const std::size_t& _frameappearance) :
	Renderer(_tilecont, _frameappearance)
{
}

RenderGameModeSelect::RenderGameModeSelect()
	:Renderer()
{
}

void RenderGameModeSelect::renderGameModeSelect(const bool& _reload, GameplayContainer& _gameplay_container)
{
	if (_reload) reload();

	//game type select
	sf::Vector2u game_type_array[2][2] = { {{8,7},{15,7} },{{20,7},{27,7}} };
	for (std::size_t i = 0; i < 2; ++i) {
		for (std::size_t j = 0; j < 2; ++j) {
			tilecont->at(game_type_array[i][j].x, game_type_array[i][j].y) = tiletype(87, 0x0d, 0x0d, 0x00, 0x00);
		}
	}
	if ((blink%4)!= 0) {
		tilecont->at(game_type_array[_gameplay_container.game_type][0].x, game_type_array[_gameplay_container.game_type][0].y) = tiletype(40, 0x0d, 0x30, 0x00, 0x00);
		tilecont->at(game_type_array[_gameplay_container.game_type][1].x, game_type_array[_gameplay_container.game_type][1].y) = tiletype(245, 0x0d, 0x30, 0x30, 0x30);
	}

	sf::Vector2u music_theme_array[4][2] = { {{13,17},{22,17} },{{13,19},{22,19}},{{13,21},{22,21}},{{13,23},{22,23}} };
	for (std::size_t i = 0; i < 4; ++i) {
		for (std::size_t j = 0; j < 2; ++j) {
			tilecont->at(music_theme_array[i][j].x, music_theme_array[i][j].y) = tiletype(87, 0x0d, 0x0d, 0x00, 0x00);
		}
	}
	if ((blink % 4) != 0) {
		tilecont->at(music_theme_array[_gameplay_container.music_theme-1][0].x, music_theme_array[_gameplay_container.music_theme-1][0].y) = tiletype(40, 0x0d, 0x30, 0x00, 0x00);
		tilecont->at(music_theme_array[_gameplay_container.music_theme-1][1].x, music_theme_array[_gameplay_container.music_theme-1][1].y) = tiletype(245, 0x0d, 0x30, 0x30, 0x30);
	}
	++blink;
}

void RenderGameModeSelect::updateGameModeSelect(const ActiveInputs& _input, GameplayContainer& _gameplay_container, Audio& _audio)
{
	if (_gameplay_container.music_theme == 4) _audio.stopMusic();
	else _audio.playMusic(_gameplay_container.music_theme, false);
	bool music_changed = false;
	if (_input.getPress(ntris::Start)) {
		_audio.playMenuSelect();
		selected_event = true;
	}
	else if (_input.getPress(ntris::Left)) {
		_audio.playMenuMove();
		if (_gameplay_container.game_type > 0)
			_gameplay_container.game_type--;
	}
	else if (_input.getPress(ntris::Right)) {
		_audio.playMenuMove();
		if (_gameplay_container.game_type < 1)
			_gameplay_container.game_type++;
	}
	else if (_input.getPress(ntris::Up)) {
		music_changed = true;
		_audio.playMenuMove();
		if (_gameplay_container.music_theme > 1)
			_gameplay_container.music_theme--;
	}
	else if (_input.getPress(ntris::Down)) {
		music_changed = true;
		_audio.playMenuMove();
		if (_gameplay_container.music_theme < 4)
			_gameplay_container.music_theme++;
	}
	ntris::default_music_theme = _gameplay_container.music_theme;
}

bool RenderGameModeSelect::getSelectedEvent()
{
	if (selected_event) {
		selected_event = false;
		return true;
	}
	return false;
}

void RenderGameModeSelect::reload() {
	tilecont->reset();

	tilecont->at(0, 0) = tilecont->at(1, 0) = tilecont->at(9, 0) = tilecont->at(24, 0) = tilecont->at(29, 0) = tilecont->at(30, 0) = tilecont->at(0, 1) = tilecont->at(2, 1) = tilecont->at(19, 1) = tilecont->at(31, 1) = tilecont->at(2, 2) = tilecont->at(24, 3) = tilecont->at(27, 4) = tilecont->at(18, 6) = tilecont->at(17, 7) = tilecont->at(0, 9) = tilecont->at(24, 10) = tilecont->at(31, 10) = tilecont->at(0, 12) = tilecont->at(22, 12) = tilecont->at(27, 12) = tilecont->at(22, 13) = tilecont->at(28, 13) = tilecont->at(11, 16) = tilecont->at(4, 17) = tilecont->at(11, 17) = tilecont->at(4, 18) = tilecont->at(7, 18) = tilecont->at(26, 18) = tilecont->at(3, 19) = tilecont->at(30, 19) = tilecont->at(9, 25) = tilecont->at(26, 25) = tilecont->at(29, 25) = tilecont->at(0, 26) = tilecont->at(8, 26) = tilecont->at(9, 26) = tilecont->at(26, 26) = tilecont->at(0, 27) = tilecont->at(8, 27) = tilecont->at(18, 27) = tilecont->at(31, 27) = tiletype(112, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(19, 6) = tiletype(57, 0x0d, 0x30, 0x21, 0x00);
	tilecont->at(14, 0) = tilecont->at(18, 0) = tilecont->at(21, 0) = tilecont->at(6, 1) = tilecont->at(13, 1) = tilecont->at(20, 1) = tilecont->at(23, 1) = tilecont->at(28, 2) = tilecont->at(30, 2) = tilecont->at(0, 3) = tilecont->at(19, 4) = tilecont->at(23, 4) = tilecont->at(30, 4) = tilecont->at(5, 5) = tilecont->at(8, 5) = tilecont->at(12, 5) = tilecont->at(16, 5) = tilecont->at(0, 7) = tilecont->at(2, 7) = tilecont->at(4, 7) = tilecont->at(30, 7) = tilecont->at(3, 8) = tilecont->at(1, 9) = tilecont->at(13, 9) = tilecont->at(18, 9) = tilecont->at(23, 9) = tilecont->at(28, 9) = tilecont->at(30, 9) = tilecont->at(19, 10) = tilecont->at(27, 10) = tilecont->at(17, 11) = tilecont->at(25, 11) = tilecont->at(29, 11) = tilecont->at(1, 12) = tilecont->at(17, 12) = tilecont->at(23, 12) = tilecont->at(19, 13) = tilecont->at(29, 13) = tilecont->at(3, 14) = tilecont->at(6, 14) = tilecont->at(9, 14) = tilecont->at(13, 14) = tilecont->at(25, 14) = tilecont->at(29, 14) = tilecont->at(8, 15) = tilecont->at(24, 15) = tilecont->at(24, 16) = tilecont->at(30, 16) = tilecont->at(25, 17) = tilecont->at(26, 20) = tilecont->at(2, 21) = tilecont->at(29, 21) = tilecont->at(4, 22) = tilecont->at(7, 22) = tilecont->at(9, 22) = tilecont->at(24, 22) = tilecont->at(26, 22) = tilecont->at(2, 23) = tilecont->at(9, 23) = tilecont->at(24, 23) = tilecont->at(28, 23) = tilecont->at(6, 24) = tilecont->at(28, 24) = tilecont->at(30, 25) = tilecont->at(2, 26) = tilecont->at(12, 26) = tilecont->at(17, 26) = tilecont->at(15, 27) = tilecont->at(21, 27) = tilecont->at(27, 27) = tiletype(126, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(2, 0) = tilecont->at(8, 0) = tilecont->at(10, 0) = tilecont->at(17, 0) = tilecont->at(28, 0) = tilecont->at(31, 0) = tilecont->at(1, 2) = tilecont->at(17, 2) = tilecont->at(20, 2) = tilecont->at(24, 2) = tilecont->at(31, 3) = tilecont->at(0, 4) = tilecont->at(2, 4) = tilecont->at(22, 4) = tilecont->at(30, 5) = tilecont->at(5, 6) = tilecont->at(17, 6) = tilecont->at(31, 6) = tilecont->at(2, 8) = tilecont->at(6, 8) = tilecont->at(4, 9) = tilecont->at(12, 9) = tilecont->at(22, 9) = tilecont->at(18, 10) = tilecont->at(26, 10) = tilecont->at(0, 11) = tilecont->at(2, 11) = tilecont->at(20, 11) = tilecont->at(22, 11) = tilecont->at(17, 13) = tilecont->at(26, 13) = tilecont->at(2, 14) = tilecont->at(6, 15) = tilecont->at(11, 15) = tilecont->at(4, 16) = tilecont->at(5, 16) = tilecont->at(9, 16) = tilecont->at(1, 17) = tilecont->at(24, 17) = tilecont->at(3, 18) = tilecont->at(6, 18) = tilecont->at(8, 18) = tilecont->at(10, 18) = tilecont->at(30, 18) = tilecont->at(31, 18) = tilecont->at(1, 19) = tilecont->at(24, 19) = tilecont->at(27, 19) = tilecont->at(8, 21) = tilecont->at(11, 21) = tilecont->at(28, 21) = tilecont->at(5, 23) = tilecont->at(7, 23) = tilecont->at(9, 24) = tilecont->at(10, 24) = tilecont->at(26, 24) = tilecont->at(27, 24) = tilecont->at(0, 25) = tilecont->at(3, 25) = tilecont->at(8, 25) = tilecont->at(1, 26) = tilecont->at(11, 26) = tilecont->at(16, 26) = tilecont->at(28, 26) = tilecont->at(30, 26) = tilecont->at(31, 26) = tilecont->at(4, 27) = tilecont->at(5, 27) = tilecont->at(12, 27) = tilecont->at(20, 27) = tilecont->at(23, 27) = tiletype(97, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(6, 0) = tilecont->at(23, 0) = tilecont->at(8, 1) = tilecont->at(24, 1) = tilecont->at(28, 1) = tilecont->at(31, 2) = tilecont->at(24, 4) = tilecont->at(17, 5) = tilecont->at(30, 6) = tilecont->at(1, 7) = tilecont->at(3, 7) = tilecont->at(29, 9) = tilecont->at(6, 19) = tilecont->at(11, 22) = tilecont->at(25, 22) = tilecont->at(28, 22) = tilecont->at(29, 23) = tilecont->at(5, 24) = tilecont->at(11, 27) = tilecont->at(22, 27) = tilecont->at(30, 27) = tiletype(158, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(10, 7) = tilecont->at(22, 7) = tiletype(37, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(3, 0) = tilecont->at(25, 0) = tilecont->at(22, 2) = tilecont->at(25, 2) = tilecont->at(25, 4) = tilecont->at(28, 4) = tilecont->at(8, 9) = tilecont->at(10, 9) = tilecont->at(16, 9) = tilecont->at(23, 13) = tilecont->at(0, 14) = tilecont->at(28, 15) = tilecont->at(2, 16) = tilecont->at(27, 17) = tilecont->at(28, 19) = tilecont->at(6, 20) = tilecont->at(9, 20) = tilecont->at(30, 23) = tilecont->at(1, 24) = tilecont->at(24, 24) = tilecont->at(6, 26) = tilecont->at(24, 26) = tilecont->at(13, 27) = tiletype(95, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(11, 0) = tilecont->at(10, 1) = tilecont->at(17, 1) = tilecont->at(17, 3) = tilecont->at(20, 3) = tilecont->at(29, 3) = tilecont->at(2, 5) = tilecont->at(20, 5) = tilecont->at(22, 5) = tilecont->at(17, 8) = tilecont->at(6, 9) = tilecont->at(4, 10) = tilecont->at(12, 10) = tilecont->at(22, 10) = tilecont->at(20, 12) = tilecont->at(30, 12) = tilecont->at(0, 13) = tilecont->at(17, 14) = tilecont->at(20, 14) = tilecont->at(2, 15) = tilecont->at(30, 15) = tilecont->at(6, 16) = tilecont->at(1, 18) = tilecont->at(24, 18) = tilecont->at(10, 19) = tilecont->at(31, 19) = tilecont->at(3, 20) = tilecont->at(30, 20) = tilecont->at(1, 27) = tiletype(157, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(4, 0) = tilecont->at(19, 0) = tilecont->at(26, 0) = tilecont->at(23, 2) = tilecont->at(26, 2) = tilecont->at(26, 4) = tilecont->at(29, 4) = tilecont->at(5, 8) = tilecont->at(9, 9) = tilecont->at(11, 9) = tilecont->at(15, 9) = tilecont->at(17, 9) = tilecont->at(24, 9) = tilecont->at(31, 9) = tilecont->at(21, 10) = tilecont->at(25, 12) = tilecont->at(24, 13) = tilecont->at(31, 13) = tilecont->at(1, 14) = tilecont->at(5, 14) = tilecont->at(10, 15) = tilecont->at(29, 15) = tilecont->at(3, 16) = tilecont->at(0, 17) = tilecont->at(26, 17) = tilecont->at(28, 17) = tilecont->at(29, 19) = tilecont->at(7, 20) = tilecont->at(10, 20) = tilecont->at(0, 21) = tilecont->at(31, 21) = tilecont->at(6, 22) = tilecont->at(0, 23) = tilecont->at(11, 23) = tilecont->at(31, 23) = tilecont->at(2, 24) = tilecont->at(25, 24) = tilecont->at(29, 24) = tilecont->at(7, 26) = tilecont->at(18, 26) = tilecont->at(25, 26) = tilecont->at(14, 27) = tiletype(96, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(5, 0) = tilecont->at(27, 1) = tilecont->at(27, 3) = tilecont->at(17, 4) = tilecont->at(18, 5) = tilecont->at(1, 6) = tilecont->at(3, 6) = tilecont->at(29, 6) = tilecont->at(0, 8) = tilecont->at(29, 8) = tilecont->at(25, 9) = tilecont->at(1, 10) = tilecont->at(27, 11) = tilecont->at(28, 12) = tilecont->at(7, 17) = tilecont->at(29, 17) = tilecont->at(5, 19) = tilecont->at(25, 21) = tilecont->at(1, 22) = tilecont->at(29, 22) = tilecont->at(4, 24) = tilecont->at(5, 25) = tilecont->at(19, 26) = tilecont->at(22, 26) = tilecont->at(10, 27) = tilecont->at(29, 27) = tiletype(142, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(7, 0) = tilecont->at(13, 0) = tilecont->at(20, 0) = tilecont->at(27, 0) = tilecont->at(1, 1) = tilecont->at(5, 1) = tilecont->at(9, 1) = tilecont->at(29, 1) = tilecont->at(30, 1) = tilecont->at(0, 2) = tilecont->at(18, 2) = tilecont->at(19, 2) = tilecont->at(21, 2) = tilecont->at(27, 2) = tilecont->at(2, 3) = tilecont->at(1, 4) = tilecont->at(21, 4) = tilecont->at(27, 5) = tilecont->at(31, 5) = tilecont->at(0, 6) = tilecont->at(6, 6) = tilecont->at(18, 7) = tilecont->at(29, 7) = tilecont->at(31, 8) = tilecont->at(5, 9) = tilecont->at(0, 10) = tilecont->at(7, 10) = tilecont->at(15, 10) = tilecont->at(25, 10) = tilecont->at(1, 11) = tilecont->at(21, 11) = tilecont->at(23, 11) = tilecont->at(24, 11) = tilecont->at(31, 11) = tilecont->at(26, 12) = tilecont->at(2, 13) = tilecont->at(18, 13) = tilecont->at(21, 13) = tilecont->at(25, 13) = tilecont->at(27, 13) = tilecont->at(22, 14) = tilecont->at(28, 14) = tilecont->at(31, 14) = tilecont->at(5, 15) = tilecont->at(7, 15) = tilecont->at(10, 16) = tilecont->at(0, 18) = tilecont->at(5, 18) = tilecont->at(9, 18) = tilecont->at(11, 18) = tilecont->at(29, 18) = tilecont->at(2, 19) = tilecont->at(4, 19) = tilecont->at(7, 19) = tilecont->at(25, 19) = tilecont->at(26, 19) = tilecont->at(5, 20) = tilecont->at(8, 20) = tilecont->at(11, 20) = tilecont->at(1, 21) = tilecont->at(24, 21) = tilecont->at(27, 21) = tilecont->at(31, 22) = tilecont->at(1, 23) = tilecont->at(6, 23) = tilecont->at(8, 23) = tilecont->at(0, 24) = tilecont->at(3, 24) = tilecont->at(11, 24) = tilecont->at(4, 25) = tilecont->at(5, 26) = tilecont->at(10, 26) = tilecont->at(27, 26) = tilecont->at(29, 26) = tilecont->at(9, 27) = tilecont->at(19, 27) = tilecont->at(26, 27) = tiletype(128, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(22, 0) = tilecont->at(7, 1) = tilecont->at(11, 1) = tilecont->at(14, 1) = tilecont->at(15, 1) = tilecont->at(18, 3) = tilecont->at(3, 5) = tilecont->at(9, 5) = tilecont->at(10, 5) = tilecont->at(13, 5) = tilecont->at(14, 5) = tilecont->at(23, 5) = tilecont->at(4, 8) = tilecont->at(14, 9) = tilecont->at(19, 9) = tilecont->at(20, 9) = tilecont->at(26, 9) = tilecont->at(2, 10) = tilecont->at(5, 10) = tilecont->at(13, 10) = tilecont->at(20, 10) = tilecont->at(28, 10) = tilecont->at(29, 10) = tilecont->at(24, 12) = tilecont->at(30, 13) = tilecont->at(4, 14) = tilecont->at(10, 14) = tilecont->at(11, 14) = tilecont->at(14, 14) = tilecont->at(15, 14) = tilecont->at(18, 14) = tilecont->at(3, 15) = tilecont->at(9, 15) = tilecont->at(25, 15) = tilecont->at(26, 15) = tilecont->at(0, 16) = tilecont->at(7, 16) = tilecont->at(25, 16) = tilecont->at(26, 16) = tilecont->at(31, 16) = tilecont->at(30, 17) = tilecont->at(3, 21) = tilecont->at(4, 21) = tilecont->at(30, 21) = tilecont->at(2, 22) = tilecont->at(5, 22) = tilecont->at(10, 22) = tilecont->at(27, 22) = tilecont->at(10, 23) = tilecont->at(25, 23) = tilecont->at(26, 23) = tilecont->at(6, 25) = tilecont->at(31, 25) = tilecont->at(13, 26) = tilecont->at(14, 26) = tilecont->at(20, 26) = tilecont->at(2, 27) = tiletype(127, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(28, 6) = tiletype(59, 0x0d, 0x30, 0x21, 0x00);
	tilecont->at(12, 0) = tilecont->at(16, 0) = tilecont->at(12, 1) = tilecont->at(16, 1) = tilecont->at(19, 3) = tilecont->at(30, 3) = tilecont->at(4, 5) = tilecont->at(11, 5) = tilecont->at(15, 5) = tilecont->at(21, 5) = tilecont->at(24, 5) = tilecont->at(6, 7) = tilecont->at(3, 9) = tilecont->at(21, 9) = tilecont->at(27, 9) = tilecont->at(3, 10) = tilecont->at(6, 10) = tilecont->at(14, 10) = tilecont->at(30, 10) = tilecont->at(19, 11) = tilecont->at(31, 12) = tilecont->at(12, 14) = tilecont->at(16, 14) = tilecont->at(19, 14) = tilecont->at(21, 14) = tilecont->at(27, 14) = tilecont->at(4, 15) = tilecont->at(27, 15) = tilecont->at(31, 15) = tilecont->at(1, 16) = tilecont->at(8, 16) = tilecont->at(27, 16) = tilecont->at(31, 17) = tilecont->at(0, 20) = tilecont->at(5, 21) = tilecont->at(3, 22) = tilecont->at(27, 23) = tilecont->at(8, 24) = tilecont->at(7, 25) = tilecont->at(4, 26) = tilecont->at(15, 26) = tilecont->at(21, 26) = tilecont->at(3, 27) = tilecont->at(17, 27) = tiletype(129, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(23, 16) = tilecont->at(23, 17) = tilecont->at(23, 18) = tilecont->at(23, 19) = tilecont->at(23, 20) = tilecont->at(23, 21) = tilecont->at(23, 22) = tilecont->at(23, 23) = tilecont->at(23, 24) = tiletype(45, 0x0d, 0x00, 0x38, 0x00);
	tilecont->at(15, 0) = tilecont->at(5, 7) = tilecont->at(2, 9) = tilecont->at(18, 11) = tilecont->at(26, 14) = tilecont->at(7, 24) = tilecont->at(3, 26) = tilecont->at(16, 27) = tiletype(66, 0x0d, 0x00, 0x00, 0x00);
	tilecont->at(3, 1) = tilecont->at(25, 1) = tilecont->at(22, 3) = tilecont->at(25, 3) = tilecont->at(25, 5) = tilecont->at(28, 5) = tilecont->at(8, 10) = tilecont->at(10, 10) = tilecont->at(16, 10) = tilecont->at(23, 14) = tilecont->at(0, 15) = tilecont->at(28, 16) = tilecont->at(2, 17) = tilecont->at(27, 18) = tilecont->at(28, 20) = tilecont->at(6, 21) = tilecont->at(9, 21) = tilecont->at(30, 24) = tilecont->at(1, 25) = tilecont->at(24, 25) = tilecont->at(6, 27) = tilecont->at(24, 27) = tiletype(110, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(4, 1) = tilecont->at(26, 1) = tilecont->at(23, 3) = tilecont->at(26, 3) = tilecont->at(26, 5) = tilecont->at(29, 5) = tilecont->at(9, 10) = tilecont->at(11, 10) = tilecont->at(17, 10) = tilecont->at(24, 14) = tilecont->at(1, 15) = tilecont->at(29, 16) = tilecont->at(3, 17) = tilecont->at(28, 18) = tilecont->at(29, 20) = tilecont->at(7, 21) = tilecont->at(10, 21) = tilecont->at(0, 22) = tilecont->at(31, 24) = tilecont->at(2, 25) = tilecont->at(25, 25) = tilecont->at(7, 27) = tilecont->at(25, 27) = tiletype(111, 0x0d, 0x00, 0x00, 0x00);
	tilecont->at(18, 1) = tilecont->at(29, 2) = tilecont->at(21, 3) = tilecont->at(20, 4) = tilecont->at(7, 9) = tilecont->at(23, 10) = tilecont->at(30, 11) = tilecont->at(21, 12) = tilecont->at(20, 13) = tilecont->at(30, 14) = tilecont->at(2, 18) = tilecont->at(25, 18) = tilecont->at(11, 19) = tiletype(143, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(21, 1) = tilecont->at(6, 5) = tilecont->at(18, 12) = tilecont->at(7, 14) = tilecont->at(3, 23) = tiletype(68, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(8, 8) = tilecont->at(9, 8) = tilecont->at(10, 8) = tilecont->at(11, 8) = tilecont->at(12, 8) = tilecont->at(13, 8) = tilecont->at(14, 8) = tilecont->at(15, 8) = tilecont->at(20, 8) = tilecont->at(21, 8) = tilecont->at(22, 8) = tilecont->at(23, 8) = tilecont->at(24, 8) = tilecont->at(25, 8) = tilecont->at(26, 8) = tilecont->at(27, 8) = tiletype(63, 0x0d, 0x00, 0x16, 0x00);
	tilecont->at(22, 1) = tilecont->at(28, 3) = tilecont->at(18, 4) = tilecont->at(1, 5) = tilecont->at(7, 5) = tilecont->at(19, 5) = tilecont->at(2, 6) = tilecont->at(4, 6) = tilecont->at(1, 8) = tilecont->at(18, 8) = tilecont->at(30, 8) = tilecont->at(28, 11) = tilecont->at(19, 12) = tilecont->at(29, 12) = tilecont->at(1, 13) = tilecont->at(8, 14) = tilecont->at(6, 17) = tilecont->at(8, 17) = tilecont->at(10, 17) = tilecont->at(0, 19) = tilecont->at(9, 19) = tilecont->at(2, 20) = tilecont->at(4, 20) = tilecont->at(25, 20) = tilecont->at(31, 20) = tilecont->at(26, 21) = tilecont->at(30, 22) = tilecont->at(4, 23) = tilecont->at(11, 25) = tilecont->at(28, 25) = tilecont->at(23, 26) = tiletype(113, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(3, 2) = tilecont->at(3, 11) = tiletype(49, 0x0d, 0x00, 0x38, 0x00);
	tilecont->at(4, 2) = tilecont->at(5, 2) = tilecont->at(6, 2) = tilecont->at(7, 2) = tilecont->at(8, 2) = tilecont->at(9, 2) = tilecont->at(10, 2) = tilecont->at(11, 2) = tilecont->at(12, 2) = tilecont->at(13, 2) = tilecont->at(14, 2) = tilecont->at(15, 2) = tilecont->at(4, 11) = tilecont->at(5, 11) = tilecont->at(6, 11) = tilecont->at(7, 11) = tilecont->at(8, 11) = tilecont->at(9, 11) = tilecont->at(10, 11) = tilecont->at(11, 11) = tilecont->at(12, 11) = tilecont->at(13, 11) = tilecont->at(14, 11) = tilecont->at(15, 11) = tiletype(50, 0x0d, 0x00, 0x38, 0x00);
	tilecont->at(16, 2) = tilecont->at(16, 11) = tiletype(51, 0x0d, 0x00, 0x38, 0x00);
	tilecont->at(7, 6) = tiletype(57, 0x0d, 0x30, 0x16, 0x00);
	tilecont->at(19, 8) = tiletype(62, 0x0d, 0x30, 0x21, 0x00);
	tilecont->at(1, 3) = tilecont->at(31, 4) = tilecont->at(31, 7) = tilecont->at(26, 11) = tilecont->at(2, 12) = tilecont->at(27, 20) = tilecont->at(8, 22) = tilecont->at(28, 27) = tiletype(67, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(17, 23) = tilecont->at(18, 23) = tiletype(16, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(5, 3) = tiletype(17, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(3, 3) = tilecont->at(3, 12) = tiletype(52, 0x0d, 0x00, 0x38, 0x00);
	tilecont->at(6, 3) = tilecont->at(9, 7) = tiletype(11, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(21, 19) = tiletype(3, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(8, 6) = tilecont->at(9, 6) = tilecont->at(10, 6) = tilecont->at(11, 6) = tilecont->at(12, 6) = tilecont->at(13, 6) = tilecont->at(14, 6) = tilecont->at(15, 6) = tilecont->at(20, 6) = tilecont->at(21, 6) = tilecont->at(22, 6) = tilecont->at(23, 6) = tilecont->at(24, 6) = tilecont->at(25, 6) = tilecont->at(26, 6) = tilecont->at(27, 6) = tiletype(58, 0x0d, 0x00, 0x16, 0x00);
	tilecont->at(12, 3) = tilecont->at(12, 7) = tilecont->at(24, 7) = tilecont->at(12, 12) = tiletype(35, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(7, 3) = tilecont->at(5, 12) = tilecont->at(14, 17) = tilecont->at(14, 19) = tilecont->at(14, 21) = tiletype(23, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(0, 5) = tilecont->at(5, 17) = tilecont->at(9, 17) = tilecont->at(8, 19) = tilecont->at(1, 20) = tilecont->at(24, 20) = tilecont->at(10, 25) = tilecont->at(27, 25) = tiletype(65, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(8, 3) = tilecont->at(14, 3) = tilecont->at(14, 7) = tilecont->at(26, 7) = tilecont->at(14, 12) = tiletype(15, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(12, 15) = tiletype(41, 0x0d, 0x00, 0x38, 0x00);
	tilecont->at(7, 12) = tilecont->at(16, 17) = tilecont->at(16, 19) = tilecont->at(16, 21) = tiletype(29, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(11, 3) = tilecont->at(11, 7) = tilecont->at(23, 7) = tilecont->at(11, 12) = tiletype(30, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(13, 3) = tilecont->at(13, 7) = tilecont->at(25, 7) = tilecont->at(13, 12) = tiletype(26, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(28, 8) = tiletype(64, 0x0d, 0x30, 0x21, 0x00);
	tilecont->at(16, 3) = tilecont->at(16, 12) = tiletype(53, 0x0d, 0x00, 0x38, 0x00);
	tilecont->at(3, 4) = tilecont->at(3, 13) = tiletype(54, 0x0d, 0x00, 0x38, 0x00);
	tilecont->at(4, 4) = tilecont->at(5, 4) = tilecont->at(6, 4) = tilecont->at(7, 4) = tilecont->at(8, 4) = tilecont->at(9, 4) = tilecont->at(10, 4) = tilecont->at(11, 4) = tilecont->at(12, 4) = tilecont->at(13, 4) = tilecont->at(14, 4) = tilecont->at(15, 4) = tilecont->at(4, 13) = tilecont->at(5, 13) = tilecont->at(6, 13) = tilecont->at(7, 13) = tilecont->at(8, 13) = tilecont->at(9, 13) = tilecont->at(10, 13) = tilecont->at(11, 13) = tilecont->at(12, 13) = tilecont->at(13, 13) = tilecont->at(14, 13) = tilecont->at(15, 13) = tiletype(55, 0x0d, 0x00, 0x38, 0x00);
	tilecont->at(16, 4) = tilecont->at(16, 13) = tiletype(56, 0x0d, 0x00, 0x38, 0x00);
	tilecont->at(16, 6) = tiletype(59, 0x0d, 0x30, 0x16, 0x00);
	tilecont->at(7, 7) = tilecont->at(19, 7) = tiletype(60, 0x0d, 0x00, 0x16, 0x00);
	tilecont->at(16, 7) = tilecont->at(28, 7) = tiletype(61, 0x0d, 0x00, 0x16, 0x00);
	tilecont->at(13, 15) = tilecont->at(14, 15) = tilecont->at(15, 15) = tilecont->at(16, 15) = tilecont->at(17, 15) = tilecont->at(18, 15) = tilecont->at(19, 15) = tilecont->at(20, 15) = tilecont->at(21, 15) = tilecont->at(22, 15) = tiletype(42, 0x0d, 0x00, 0x38, 0x00);
	tilecont->at(21, 7) = tiletype(12, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(9, 12) = tilecont->at(18, 17) = tilecont->at(18, 19) = tilecont->at(18, 21) = tiletype(13, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(7, 8) = tiletype(62, 0x0d, 0x30, 0x16, 0x00);
	tilecont->at(21, 21) = tiletype(4, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(16, 8) = tiletype(64, 0x0d, 0x30, 0x16, 0x00);
	tilecont->at(6, 12) = tilecont->at(15, 17) = tilecont->at(15, 19) = tilecont->at(15, 21) = tiletype(31, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(8, 12) = tilecont->at(17, 17) = tilecont->at(17, 19) = tilecont->at(17, 21) = tiletype(19, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(23, 15) = tiletype(43, 0x0d, 0x00, 0x38, 0x00);
	tilecont->at(12, 16) = tilecont->at(12, 17) = tilecont->at(12, 18) = tilecont->at(12, 19) = tilecont->at(12, 20) = tilecont->at(12, 21) = tilecont->at(12, 22) = tilecont->at(12, 23) = tilecont->at(12, 24) = tiletype(44, 0x0d, 0x00, 0x38, 0x00);
	tilecont->at(20, 17) = tilecont->at(20, 19) = tilecont->at(20, 21) = tiletype(85, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(19, 17) = tilecont->at(19, 19) = tilecont->at(19, 21) = tiletype(84, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(21, 17) = tiletype(2, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(16, 23) = tiletype(25, 0x0d, 0x30, 0x00, 0x00);
	tilecont->at(12, 25) = tiletype(46, 0x0d, 0x00, 0x38, 0x00);
	tilecont->at(13, 25) = tilecont->at(14, 25) = tilecont->at(15, 25) = tilecont->at(16, 25) = tilecont->at(17, 25) = tilecont->at(18, 25) = tilecont->at(19, 25) = tilecont->at(20, 25) = tilecont->at(21, 25) = tilecont->at(22, 25) = tiletype(47, 0x0d, 0x00, 0x38, 0x00);
	tilecont->at(23, 25) = tiletype(48, 0x0d, 0x00, 0x38, 0x00);

}