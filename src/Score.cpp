#include"Score.hpp"
#include"TextWriter.hpp"
#include"Log.hpp"

Score::Score(TileContainer *_tilecont, const nes_ushort& _frameappearance)
:Renderer(_tilecont, _frameappearance)
{
    nes_uchar scorearray[3]={0x00,0x00,0x01};
    /*top_scores.insert(std::pair(10000u, ScoreContainer(10000u)));
    top_scores.insert(std::pair(5000u, ScoreContainer(5000u)));
    top_scores.insert(std::pair(1000u, ScoreContainer(1000u)));*/
}
//
//Score::Score(TileContainer *_tilecont, const nes_ushort& _frameappearance, const bool& _maxout)
//:Score(_tilecont,_frameappearance)
//{
//    maxout=false;
//    nes_uchar scorearray[3] = { 0x00,0x00,0x01 };
//    top_scores.insert(std::pair(10000u, ScoreContainer(scorearray)));
//    top_scores.insert(std::pair(5000u, ScoreContainer(scorearray)));
//    top_scores.insert(std::pair(1000u, ScoreContainer(scorearray)));
//}

void Score::renderInGameScores() {
    if (hidecounter>0) {
        --hidecounter;
        return;
    }
    else {
        using namespace std::string_literals;
        if (ntris::lineclearframecounter>0) {
            if (top_scores.empty())
                TextWriter::write("000000", tilecont, { ntris::topscorex,ntris::topscorey });
            else
                TextWriter::write(top_scores.begin()->second.getScoreString(),tilecont,{ntris::topscorex,ntris::topscorey}); //these 2 lines
            TextWriter::write(scoretemp.getScoreString(),tilecont,{ntris::scorex,ntris::scorey}); //are useless right now
        }
        else {
            scoretemp=score;
            if (top_scores.empty())
                TextWriter::write("000000", tilecont, { ntris::topscorex,ntris::topscorey });
            else
                TextWriter::write(top_scores.begin()->second.getScoreString(),tilecont,{ntris::topscorex,ntris::topscorey});
            TextWriter::write(score.getScoreString(),tilecont,{ntris::scorex,ntris::scorey});
        }
    }
}

void Score::storeScore(unsigned int const& _total_lines, unsigned int const& _level_start, unsigned int const& _level_end, unsigned int const& tetris_percentage)
{
    score.total_lines = _total_lines;
    score.level_start = _level_start;
    score.level_end = _level_end;
    score.tetris_percentage = tetris_percentage;
    unsigned int real_score = score.realscore();
    top_scores.insert(std::pair(real_score, score));
    score = ScoreContainer(0u);
}


ScoreContainer Score::getScore() {
    return score;
}

//this function capped the score at 999999
void Score::lastdigitcheck() { //TODO make ScoreContainer specific
    /*if (maxout) {
        nes_uchar A=score[2]&0xF0;
        if (A-0xA0>=0) score[0]=score[1]=score[2]=0x99;
    }*/
}
template<typename ... Args>
inline std::string string_format(const std::string& format, Args ... args)
{
    size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1ull; // Extra space for '\0'
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
} //https://stackoverflow.com/a/26221725

void Score::renderTopScores()
{
    using namespace std::string_literals;
    renderTopScoresCornice();
    nes_uchar informationy = 17;
    nes_uchar scorex = 5;
    nes_uchar lvx = 12;
    nes_uchar linesx = 22;
    nes_uchar ttx = 18;
    TextWriter::write(std::string("SCORE"), tilecont, { scorex,informationy }, 0x30);
    TextWriter::write(std::string("LV"), tilecont, { lvx,informationy }, 0x30);
    TextWriter::write(std::string("TT%"), tilecont, { ttx,informationy }, 0x30);
    TextWriter::write(std::string("LINES"), tilecont, { linesx,informationy }, 0x30);
    auto it = top_scores.cbegin();
    for (std::size_t i = 1; i <= std::min((decltype(top_scores)::size_type) 3,top_scores.size()); ++i,++it) {
        //scores
        sf::Vector2u score_pos = sf::Vector2u(scorex, informationy + 2*i);
        std::string score_zeroes = string_format("%06d", it->first);
        if (it->first > 999999u) {
            sf::Vector2u score_pos_shift = sf::Vector2u(scorex-1, informationy + 2 * i);
            TextWriter::write(score_zeroes, tilecont, score_pos_shift);
        }
        else 
            TextWriter::write(score_zeroes, tilecont, score_pos);

        //level
        sf::Vector2u start_lv_pos = sf::Vector2u(lvx, informationy + 2 * i);
        std::string start_lv_zeroes = string_format("%02d", it->second.level_start);
        std::string end_lv_zeroes = string_format("%02d", it->second.level_end);
        TextWriter::write(start_lv_zeroes + "»" + end_lv_zeroes, tilecont, start_lv_pos);

        //tetris percentage
        sf::Vector2u tt_pos = sf::Vector2u(ttx, informationy + 2 * i);
        std::string tt_zeroes = string_format("%02d", it->second.tetris_percentage);
        TextWriter::write(tt_zeroes, tilecont, tt_pos);

        //lines
        sf::Vector2u lines_pos = sf::Vector2u(linesx, informationy + 2 * i);
        std::string lines_zeroes = string_format("%03d", it->second.total_lines);
        TextWriter::write(lines_zeroes, tilecont, lines_pos);
    }
}

void Score::bytechecklowdigit(const std::size_t& byte, const bool& andop) {
    nes_uchar A=score[byte];
    if (andop) A=A&0x0f;
    if (A>=0x0A) score[byte]+=0x06; //originally A-0xOA>=0
}

void Score::bytecheckhighdigit(const std::size_t& byte, const bool& andop) {
    nes_uchar A=score[byte];
    if (andop) A=A&0xf0;
    if (A>=0xA0) { //originally A-0xA0>=0
        score[byte]+=0x60;
        ++score[byte+1];
    }
}

void Score::softdrop(nes_uchar helddownpoints) {
    if (helddownpoints>=2) {
        --helddownpoints;
        score[0]+=helddownpoints;
        bytechecklowdigit(0,true);
        bytecheckhighdigit(0,true);
    }
}
void Score::lineclear(const nes_uchar& level, const nes_uchar& linescleared) {
    for (std::size_t i=0; i<=level; ++i) {
        score[0]+=pointsarray[linescleared*2];
        bytecheckhighdigit(0,false); //for some reason it doesn't do &0xf0 in the original assembly code
        score[1]+=pointsarray[linescleared*2+1];
        bytechecklowdigit(1,true);
        bytecheckhighdigit(1,true);
        bytechecklowdigit(2,true);
        lastdigitcheck();
    }
}

nes_uchar Score::pointsarray[10]= {
    00, 00,
    0x40, 00,
    00, 0x01,
    00, 0x03,
    00, 0x12
};

void Score::renderTopScoresCornice() {

    // frame around top scores
    tilecont->at(27, 18) = tiletype(79, 0x0d, 0x00, 0x38, 0x00);
    tilecont->at(5, 16) = tilecont->at(6, 16) = tilecont->at(7, 16) = tilecont->at(8, 16) = tilecont->at(9, 16) = tilecont->at(10, 16) = tilecont->at(11, 16) = tilecont->at(12, 16) = tilecont->at(13, 16) = tilecont->at(14, 16) = tilecont->at(15, 16) = tilecont->at(16, 16) = tilecont->at(17, 16) = tilecont->at(18, 16) = tilecont->at(19, 16) = tilecont->at(20, 16) = tilecont->at(21, 16) = tilecont->at(22, 16) = tilecont->at(23, 16) = tilecont->at(24, 16) = tilecont->at(25, 16) = tilecont->at(26, 16) = tiletype(42, 0x0d, 0x00, 0x38, 0x00);
    tilecont->at(4, 16) = tiletype(41, 0x0d, 0x00, 0x38, 0x00);
    tilecont->at(27, 16) = tiletype(43, 0x0d, 0x00, 0x38, 0x00);
    tilecont->at(4, 18) = tiletype(77, 0x0d, 0x00, 0x38, 0x00);
    tilecont->at(4, 17) = tilecont->at(4, 19) = tilecont->at(4, 20) = tilecont->at(4, 21) = tilecont->at(4, 22) = tilecont->at(4, 23) = tiletype(44, 0x0d, 0x00, 0x38, 0x00);
    tilecont->at(27, 17) = tilecont->at(27, 19) = tilecont->at(27, 20) = tilecont->at(27, 21) = tilecont->at(27, 22) = tilecont->at(27, 23) = tiletype(45, 0x0d, 0x00, 0x38, 0x00);
    tilecont->at(5, 18) = tilecont->at(6, 18) = tilecont->at(7, 18) = tilecont->at(8, 18) = tilecont->at(9, 18) = tilecont->at(10, 18) = tilecont->at(11, 18) = tilecont->at(12, 18) = tilecont->at(13, 18) = tilecont->at(14, 18) = tilecont->at(15, 18) = tilecont->at(16, 18) = tilecont->at(17, 18) = tilecont->at(18, 18) = tilecont->at(19, 18) = tilecont->at(20, 18) = tilecont->at(21, 18) = tilecont->at(22, 18) = tilecont->at(23, 18) = tilecont->at(24, 18) = tilecont->at(25, 18) = tilecont->at(26, 18) = tilecont->at(5, 24) = tilecont->at(6, 24) = tilecont->at(7, 24) = tilecont->at(8, 24) = tilecont->at(9, 24) = tilecont->at(10, 24) = tilecont->at(11, 24) = tilecont->at(12, 24) = tilecont->at(13, 24) = tilecont->at(14, 24) = tilecont->at(15, 24) = tilecont->at(16, 24) = tilecont->at(17, 24) = tilecont->at(18, 24) = tilecont->at(19, 24) = tilecont->at(20, 24) = tilecont->at(21, 24) = tilecont->at(22, 24) = tilecont->at(23, 24) = tilecont->at(24, 24) = tilecont->at(25, 24) = tilecont->at(26, 24) = tiletype(47, 0x0d, 0x00, 0x38, 0x00);
    tilecont->at(4, 24) = tiletype(46, 0x0d, 0x00, 0x38, 0x00);
    tilecont->at(27, 24) = tiletype(48, 0x0d, 0x00, 0x38, 0x00);
}