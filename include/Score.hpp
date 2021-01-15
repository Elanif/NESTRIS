#pragma once
#ifndef SCORE_H
#define SCORE_H
#include<cstdio>
#include"Renderer.hpp"
#include"ntris.hpp"
#include"TileContainer.hpp"
#include<map>
#include<sstream>
#include<iomanip>

class ScoreContainer {
public:
    ScoreContainer() {
        score[0]=score[1]=score[2]=0;
    }
    explicit ScoreContainer(nes_uchar _score[]) {
        score[0]=_score[0];
        score[1]=_score[1];
        score[2]=_score[2];
    }
    explicit ScoreContainer(unsigned int _score) {
        unsigned int _score0low = _score % 10;
        _score /= 10;
        unsigned int _score0high = _score % 10;
        _score /= 10;
        unsigned int _score1low = _score % 10;
        _score /= 10;
        unsigned int _score1high = _score % 10;
        _score /=10;
        unsigned int _score2low = _score % 10;
        _score /= 10;
        unsigned int _score2high = _score % 10;
        score[0]=_score0low+_score0high*16;
        score[1] = _score1low + _score1high * 16;
        score[2] = _score2low + _score2high * 16;
    }
    nes_uchar score[3];
    unsigned int realscore() const {
        unsigned int result=(score[0]&0x0f)+((score[0]&0xf0)/0x0f)*10;
        result+=(score[1]&0x0f)*100+((score[1]&0xf0)/0x0f)*1000;
        result+=(score[2]&0x0f)*10000+((score[2]&0xf0)/0x0f)*100000;
        return result;
    }
    bool operator>(const ScoreContainer& _sc) const {
        return this->realscore()>_sc.realscore();
    }
    const nes_uchar& operator[](const std::size_t& i) const {
        return score[i];
    }
    nes_uchar& operator[](const std::size_t& i) {
        return score[i];
    }

    std::string getScoreString() {
        std::stringstream _tmpstream;
        for (std::size_t i = 3; i-- > 0;)
            _tmpstream << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)score[i];
        return _tmpstream.str();
    }
    unsigned int total_lines=0;
    unsigned int level_start=0;
    unsigned int level_end=0;
    unsigned int tetris_percentage = 0;
};
class Score : public Renderer
{
    public:
        Score(TileContainer *_tilecont, const nes_ushort& _frameappearance);
        //I decided all scores will go over maxout
        //Score(TileContainer *_tilecont, const nes_ushort& _frameappearance, const bool& _maxout);

        void softdrop(nes_uchar);
        void lineclear(const nes_uchar& level, const nes_uchar& linescleared) ; //TODO var type
        void renderInGameScores();
        void renderTopScores();
        void storeScore(unsigned int const& _total_lines, unsigned int const& _level_start, unsigned int const& _level_end, unsigned int const& tetris_percentage);

        ScoreContainer getScore();
    protected:

    private:

        TileContainer *_tilecont=nullptr;
        void renderTopScoresCornice();
        //bool maxout;
        void bytechecklowdigit(const std::size_t& byte, const bool& andop);
        void bytecheckhighdigit(const std::size_t& byte, const bool& andop);
        void lastdigitcheck();
        ScoreContainer score;
        ScoreContainer scoretemp;
        std::multimap<unsigned int, ScoreContainer, std::greater<unsigned int>> top_scores;
		static nes_uchar pointsarray[10];

};


#endif // SCORE_H
