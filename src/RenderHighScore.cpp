#include"RenderHighScore.hpp"
#include"TextWriter.hpp"
#include<string>
#include"Audio.hpp"

RenderHighScore::RenderHighScore(TileContainer * _tilecont, const nes_ushort& _frameappearance)
	:Renderer(_tilecont, _frameappearance)
{

}

void RenderHighScore::update(const ActiveInputs& _input, const nes_ushort& _framecounter, GameplayContainer& _gameplay_container, Audio& _audio) {
    if (_input.getPress(ntris::Start)) {
        submitted = true;
        unsigned int _total_lines = _gameplay_container.levellineshandler.getLines();
        unsigned int _level_start = _gameplay_container.levellineshandler.get_starting_level();
        unsigned int _level_end = _gameplay_container.levellineshandler.get_real_level();
        unsigned int _tetris_percentage = _gameplay_container.levellineshandler.getTetrisPercentage();
        _gameplay_container.scorehandler.storeScore(_total_lines, _level_start, _level_end, _tetris_percentage);
    }
}

void RenderHighScore::render(const nes_ushort& _framecounter, GameplayContainer& _gameplay_container) {
    tilecont->reset();
    renderCornice();
    TextWriter::write(std::string("A-TYPE"), tilecont, { 13,2 }, 0x30);
    praise(_gameplay_container);
    renderStatistics(_gameplay_container);
    _gameplay_container.scorehandler.renderTopScores();
}

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
    size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1ull; // Extra space for '\0'
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
} //https://stackoverflow.com/a/26221725

void RenderHighScore::renderStatistics(GameplayContainer& _gameplay_container) {

    nes_uchar informationy = 9;
    nes_uchar scorex = 5;
    nes_uchar lvx = 12;
    nes_uchar linesx = 22;
    nes_uchar ttx = 18;

    TextWriter::write(std::string("SCORE"), tilecont, { scorex,informationy });
    sf::Vector2u score_pos = sf::Vector2u(scorex, informationy+2);
    unsigned int realscore = _gameplay_container.scorehandler.getScore().realscore();
    std::string score_zeroes = string_format("%06d", realscore);
    if (realscore > 999999u) {
        sf::Vector2u score_pos_shift = sf::Vector2u(scorex - 1, informationy+2);
        TextWriter::write(score_zeroes, tilecont, score_pos_shift);
    }
    else
        TextWriter::write(score_zeroes, tilecont, score_pos);

    //level
    TextWriter::write(std::string("LV"), tilecont, { lvx,informationy });
    sf::Vector2u start_lv_pos = sf::Vector2u(lvx, informationy +2);
    unsigned int level_start = _gameplay_container.levellineshandler.get_starting_level();
    unsigned int level_end = _gameplay_container.levellineshandler.get_real_level();
    std::string start_lv_zeroes = string_format("%02d", level_start);
    std::string end_lv_zeroes = string_format("%02d", level_end);
    TextWriter::write(start_lv_zeroes + "»" + end_lv_zeroes, tilecont, start_lv_pos);

    //tetris percentage
    TextWriter::write(std::string("TT%"), tilecont, { ttx,informationy });
    sf::Vector2u tt_pos = sf::Vector2u(ttx, informationy +2);
    nes_uchar tetris_percentage = _gameplay_container.levellineshandler.getTetrisPercentage();
    std::string tt_zeroes = string_format("%02d", tetris_percentage);
    TextWriter::write(tt_zeroes, tilecont, tt_pos);

    //lines
    TextWriter::write(std::string("LINES"), tilecont, { linesx,informationy });
    sf::Vector2u lines_pos = sf::Vector2u(linesx, informationy+2);
    unsigned int total_lines = _gameplay_container.levellineshandler.getLines();
    std::string lines_zeroes = string_format("%03d", total_lines);
    TextWriter::write(lines_zeroes, tilecont, lines_pos);

    //drought
    nes_uchar informationy_secondrow = 9+4;
    nes_uchar droughtx = 5;
    TextWriter::write(std::string("DROUGHT"), tilecont, { droughtx,informationy_secondrow });
    sf::Vector2u drought_pos = sf::Vector2u(droughtx, informationy_secondrow + 2);
    unsigned int drought = _gameplay_container.piecehandler.getDrought();
    std::string drought_zeroes = string_format("%02d", drought);
    TextWriter::write(drought_zeroes, tilecont, drought_pos);

    //std::string score_zeroes = string_format("%06d", _gameplay_container.scorehandler.getScore().realscore());
    //TextWriter::write(std::string("SCORE-")+score_zeroes, tilecont, { 4,9 });

    //std::string tetris_percantage_zereos = string_format("%02d", _gameplay_container.levellineshandler.getTetrisPercentage());
    //TextWriter::write(std::string("TETRIS%-") + tetris_percantage_zereos, tilecont, { 18,9 });

    //std::string level_start_zeroes = string_format("%02d", _gameplay_container.levellineshandler.get_starting_level());
    //TextWriter::write(std::string("LVL START-") + level_start_zeroes, tilecont, { 4,10 });

    //std::string level_end_zeroes = string_format("%02d", _gameplay_container.levellineshandler.get_real_level());
    //TextWriter::write(std::string("LVL END-") + level_end_zeroes, tilecont, { 18,10 });

    //std::string drought = string_format("%02d", _gameplay_container.piecehandler.getDrought());
    //TextWriter::write(std::string("LONGEST drought-") + drought, tilecont, { 4,11 });
}

void RenderHighScore::renderCornice() {
    //frame around the screen
    tilecont->at(11, 1) = tilecont->at(3, 2) = tiletype(57, 0x0d, 0x30, 0x21, 0x00);
    tilecont->at(0, 0) = tilecont->at(3, 0) = tilecont->at(6, 0) = tilecont->at(22, 0) = tilecont->at(25, 0) = tilecont->at(29, 2) = tilecont->at(2, 4) = tilecont->at(1, 5) = tilecont->at(29, 6) = tilecont->at(2, 11) = tilecont->at(2, 12) = tilecont->at(29, 12) = tilecont->at(30, 12) = tilecont->at(29, 13) = tilecont->at(30, 13) = tilecont->at(31, 15) = tilecont->at(31, 16) = tilecont->at(2, 18) = tilecont->at(1, 19) = tilecont->at(29, 20) = tilecont->at(2, 21) = tilecont->at(29, 21) = tilecont->at(2, 22) = tilecont->at(30, 22) = tilecont->at(0, 24) = tilecont->at(29, 25) = tiletype(112, 0x0d, 0x30, 0x00, 0x00);
    tilecont->at(1, 0) = tilecont->at(9, 0) = tilecont->at(14, 0) = tilecont->at(16, 0) = tilecont->at(19, 0) = tilecont->at(23, 0) = tilecont->at(30, 0) = tilecont->at(27, 1) = tilecont->at(0, 2) = tilecont->at(1, 3) = tilecont->at(29, 4) = tilecont->at(31, 5) = tilecont->at(1, 8) = tilecont->at(29, 8) = tilecont->at(31, 8) = tilecont->at(0, 13) = tilecont->at(29, 15) = tilecont->at(1, 17) = tilecont->at(29, 18) = tilecont->at(0, 20) = tilecont->at(31, 22) = tilecont->at(0, 25) = tilecont->at(4, 27) = tilecont->at(11, 27) = tilecont->at(15, 27) = tilecont->at(20, 27) = tiletype(126, 0x0d, 0x30, 0x00, 0x00);
    tilecont->at(7, 0) = tilecont->at(1, 1) = tilecont->at(31, 1) = tilecont->at(0, 3) = tilecont->at(31, 3) = tilecont->at(1, 4) = tilecont->at(0, 6) = tilecont->at(2, 7) = tilecont->at(29, 9) = tilecont->at(2, 10) = tilecont->at(29, 11) = tilecont->at(30, 11) = tilecont->at(31, 11) = tilecont->at(0, 14) = tilecont->at(31, 14) = tilecont->at(2, 15) = tilecont->at(0, 17) = tilecont->at(1, 18) = tilecont->at(29, 19) = tilecont->at(30, 19) = tilecont->at(2, 20) = tilecont->at(30, 21) = tilecont->at(0, 23) = tilecont->at(1, 23) = tilecont->at(29, 23) = tilecont->at(31, 26) = tilecont->at(8, 27) = tilecont->at(9, 27) = tilecont->at(10, 27) = tilecont->at(14, 27) = tilecont->at(18, 27) = tilecont->at(19, 27) = tilecont->at(24, 27) = tilecont->at(29, 27) = tiletype(97, 0x0d, 0x30, 0x00, 0x00);
    tilecont->at(11, 3) = tilecont->at(3, 26) = tiletype(62, 0x0d, 0x30, 0x21, 0x00);
    tilecont->at(2, 0) = tilecont->at(10, 0) = tilecont->at(24, 0) = tilecont->at(0, 4) = tilecont->at(2, 8) = tilecont->at(0, 18) = tiletype(67, 0x0d, 0x30, 0x00, 0x00);
    tilecont->at(4, 0) = tilecont->at(3, 1) = tilecont->at(7, 1) = tilecont->at(22, 1) = tilecont->at(1, 6) = tilecont->at(30, 9) = tilecont->at(29, 10) = tilecont->at(1, 14) = tilecont->at(0, 16) = tilecont->at(30, 20) = tilecont->at(30, 23) = tilecont->at(29, 24) = tilecont->at(29, 26) = tiletype(157, 0x0d, 0x30, 0x00, 0x00);
    tilecont->at(5, 0) = tilecont->at(21, 0) = tilecont->at(2, 3) = tilecont->at(30, 8) = tilecont->at(1, 13) = tilecont->at(30, 15) = tilecont->at(2, 17) = tilecont->at(31, 18) = tilecont->at(31, 20) = tilecont->at(13, 27) = tiletype(143, 0x0d, 0x30, 0x00, 0x00);
    tilecont->at(8, 0) = tilecont->at(11, 0) = tilecont->at(18, 0) = tilecont->at(0, 1) = tilecont->at(2, 1) = tilecont->at(5, 1) = tilecont->at(6, 1) = tilecont->at(10, 1) = tilecont->at(21, 1) = tilecont->at(24, 1) = tilecont->at(25, 1) = tilecont->at(26, 1) = tilecont->at(29, 3) = tilecont->at(30, 3) = tilecont->at(0, 5) = tilecont->at(2, 5) = tilecont->at(29, 7) = tilecont->at(0, 8) = tilecont->at(2, 9) = tilecont->at(2, 13) = tilecont->at(31, 13) = tilecont->at(29, 14) = tilecont->at(30, 14) = tilecont->at(31, 17) = tilecont->at(0, 19) = tilecont->at(2, 19) = tilecont->at(31, 19) = tilecont->at(31, 21) = tilecont->at(29, 22) = tilecont->at(2, 23) = tiletype(128, 0x0d, 0x30, 0x00, 0x00);
    tilecont->at(12, 0) = tilecont->at(30, 7) = tilecont->at(0, 10) = tilecont->at(0, 12) = tilecont->at(29, 17) = tilecont->at(0, 22) = tilecont->at(30, 25) = tiletype(110, 0x0d, 0x30, 0x00, 0x00);
    tilecont->at(13, 0) = tilecont->at(31, 7) = tilecont->at(1, 10) = tilecont->at(1, 12) = tilecont->at(30, 17) = tilecont->at(1, 22) = tilecont->at(31, 25) = tiletype(111, 0x0d, 0x00, 0x00, 0x00);
    tilecont->at(15, 0) = tilecont->at(17, 0) = tilecont->at(31, 0) = tilecont->at(28, 1) = tilecont->at(31, 2) = tilecont->at(31, 4) = tilecont->at(1, 20) = tilecont->at(2, 25) = tilecont->at(31, 27) = tiletype(158, 0x0d, 0x30, 0x00, 0x00);
    tilecont->at(20, 1) = tilecont->at(28, 2) = tiletype(59, 0x0d, 0x30, 0x21, 0x00);
    tilecont->at(27, 0) = tilecont->at(29, 0) = tilecont->at(4, 1) = tilecont->at(9, 1) = tilecont->at(23, 1) = tilecont->at(30, 1) = tilecont->at(2, 2) = tilecont->at(30, 5) = tilecont->at(2, 6) = tilecont->at(1, 7) = tilecont->at(31, 9) = tilecont->at(31, 10) = tilecont->at(2, 14) = tilecont->at(1, 15) = tilecont->at(2, 16) = tilecont->at(31, 23) = tilecont->at(31, 24) = tilecont->at(2, 26) = tilecont->at(30, 26) = tilecont->at(7, 27) = tilecont->at(17, 27) = tilecont->at(23, 27) = tilecont->at(26, 27) = tiletype(129, 0x0d, 0x30, 0x00, 0x00);
    tilecont->at(20, 0) = tilecont->at(8, 1) = tilecont->at(30, 4) = tilecont->at(30, 10) = tilecont->at(1, 16) = tilecont->at(30, 18) = tilecont->at(30, 24) = tilecont->at(1, 25) = tilecont->at(0, 26) = tilecont->at(1, 26) = tilecont->at(5, 27) = tilecont->at(6, 27) = tilecont->at(12, 27) = tilecont->at(21, 27) = tilecont->at(22, 27) = tiletype(127, 0x0d, 0x30, 0x00, 0x00);
    tilecont->at(26, 0) = tilecont->at(0, 7) = tilecont->at(31, 12) = tilecont->at(0, 15) = tilecont->at(1, 24) = tiletype(65, 0x0d, 0x30, 0x00, 0x00);
    tilecont->at(28, 0) = tilecont->at(29, 1) = tilecont->at(30, 2) = tilecont->at(29, 5) = tilecont->at(25, 27) = tilecont->at(30, 27) = tiletype(142, 0x0d, 0x30, 0x00, 0x00);
    tilecont->at(12, 1) = tilecont->at(13, 1) = tilecont->at(14, 1) = tilecont->at(15, 1) = tilecont->at(16, 1) = tilecont->at(17, 1) = tilecont->at(18, 1) = tilecont->at(19, 1) = tilecont->at(4, 2) = tilecont->at(5, 2) = tilecont->at(6, 2) = tilecont->at(7, 2) = tilecont->at(8, 2) = tilecont->at(9, 2) = tilecont->at(10, 2) = tilecont->at(21, 2) = tilecont->at(22, 2) = tilecont->at(23, 2) = tilecont->at(24, 2) = tilecont->at(25, 2) = tilecont->at(26, 2) = tilecont->at(27, 2) = tiletype(58, 0x0d, 0x00, 0x21, 0x00);
    tilecont->at(1, 2) = tiletype(66, 0x0d, 0x00, 0x00, 0x00);
    tilecont->at(11, 2) = tilecont->at(3, 3) = tilecont->at(3, 4) = tilecont->at(3, 5) = tilecont->at(3, 6) = tilecont->at(3, 7) = tilecont->at(3, 8) = tilecont->at(3, 9) = tilecont->at(3, 10) = tilecont->at(3, 11) = tilecont->at(3, 12) = tilecont->at(3, 13) = tilecont->at(3, 14) = tilecont->at(3, 15) = tilecont->at(3, 16) = tilecont->at(3, 17) = tilecont->at(3, 18) = tilecont->at(3, 19) = tilecont->at(3, 20) = tilecont->at(3, 21) = tilecont->at(3, 22) = tilecont->at(3, 23) = tilecont->at(3, 24) = tilecont->at(3, 25) = tiletype(60, 0x0d, 0x00, 0x21, 0x00);
    tilecont->at(20, 2) = tilecont->at(28, 3) = tilecont->at(28, 4) = tilecont->at(28, 5) = tilecont->at(28, 6) = tilecont->at(28, 7) = tilecont->at(28, 8) = tilecont->at(28, 9) = tilecont->at(28, 10) = tilecont->at(28, 11) = tilecont->at(28, 12) = tilecont->at(28, 13) = tilecont->at(28, 14) = tilecont->at(28, 15) = tilecont->at(28, 16) = tilecont->at(28, 17) = tilecont->at(28, 18) = tilecont->at(28, 19) = tilecont->at(28, 20) = tilecont->at(28, 21) = tilecont->at(28, 22) = tilecont->at(28, 23) = tilecont->at(28, 24) = tilecont->at(28, 25) = tiletype(61, 0x0d, 0x00, 0x21, 0x00);
    tilecont->at(2, 24) = tiletype(113, 0x0d, 0x30, 0x00, 0x00);
    tilecont->at(12, 3) = tilecont->at(13, 3) = tilecont->at(14, 3) = tilecont->at(15, 3) = tilecont->at(16, 3) = tilecont->at(17, 3) = tilecont->at(18, 3) = tilecont->at(19, 3) = tilecont->at(4, 26) = tilecont->at(5, 26) = tilecont->at(6, 26) = tilecont->at(7, 26) = tilecont->at(8, 26) = tilecont->at(9, 26) = tilecont->at(10, 26) = tilecont->at(11, 26) = tilecont->at(12, 26) = tilecont->at(13, 26) = tilecont->at(14, 26) = tilecont->at(15, 26) = tilecont->at(16, 26) = tilecont->at(17, 26) = tilecont->at(18, 26) = tilecont->at(19, 26) = tilecont->at(20, 26) = tilecont->at(21, 26) = tilecont->at(22, 26) = tilecont->at(23, 26) = tilecont->at(24, 26) = tilecont->at(25, 26) = tilecont->at(26, 26) = tilecont->at(27, 26) = tiletype(63, 0x0d, 0x00, 0x21, 0x00);
    tilecont->at(20, 3) = tilecont->at(28, 26) = tiletype(64, 0x0d, 0x30, 0x21, 0x00);
    tilecont->at(30, 6) = tilecont->at(0, 9) = tilecont->at(0, 11) = tilecont->at(29, 16) = tilecont->at(0, 21) = tilecont->at(0, 27) = tilecont->at(2, 27) = tilecont->at(27, 27) = tiletype(95, 0x0d, 0x30, 0x00, 0x00);
    tilecont->at(31, 6) = tilecont->at(1, 9) = tilecont->at(1, 11) = tilecont->at(30, 16) = tilecont->at(1, 21) = tilecont->at(1, 27) = tilecont->at(3, 27) = tilecont->at(28, 27) = tiletype(96, 0x0d, 0x30, 0x00, 0x00);
    tilecont->at(16, 27) = tiletype(68, 0x0d, 0x30, 0x00, 0x00);

}

void RenderHighScore::praise(GameplayContainer& _gameplay_container)
{
    if (_gameplay_container.scorehandler.getScore().realscore() >= 999999) {
        TextWriter::write(std::string("CONGRATULATIONS"), tilecont, { 9,6 }, 0x16);
        TextWriter::write(std::string("YOU MAXED OUT"), tilecont, { 10,8 }, 0x30);
    }
    else if (_gameplay_container.scorehandler.getScore().realscore() >= 500000) {
        TextWriter::write(std::string("CONGRATULATIONS"), tilecont, { 9,6 }, 0x16);
        TextWriter::write(std::string("YOU'RE A TETRIS MASTER"), tilecont, { 5,8 }, 0x30);
    }
    else if (_gameplay_container.scorehandler.getScore().realscore() >= 100000) {
        TextWriter::write(std::string("CONGRATULATIONS"), tilecont, { 9,5 }, 0x16);
        TextWriter::write(std::string("YOU'RE A TETRIS"), tilecont, { 9,7 }, 0x30);
        TextWriter::write(std::string("APPRENTICE"), tilecont, { 11,8 }, 0x30);
    }
    else {
        TextWriter::write(std::string("better luck next time"), tilecont, { 6,7 }, 0x16);
    }
}
