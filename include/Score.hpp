#pragma once
#ifndef SCORE_H
#define SCORE_H
#include<cstdio>
#include"Renderer.hpp"
#include"ntris.hpp"
#include"TileContainer.hpp"
#include<map>

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
    explicit ScoreContainer(unsigned int _score) {  //bugged
        score[0]=(_score&0x0f)+((_score/10)%10)*0x0f;
        _score/=100;
        score[1]=(_score&0x0f)+((_score/10)%10)*0x0f;
        _score/=100;
        score[2]=(_score&0x0f)+((_score/10)%10)*0x0f;
        _score/=100;
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
    std::string getScoreString();
};
class Score : public Renderer
{
    public:
        Score(TileContainer *_tilecont, const nes_ushort& _frameappearance);
        Score(TileContainer *_tilecont, const nes_ushort& _frameappearance, const bool& _maxout);

        void softdrop(nes_uchar);
        void lineclear(const nes_uchar& level, const nes_uchar& linescleared) ; //TODO var type
        void render();
        void storeScore();

        ScoreContainer getScore();
    protected:

    private:
        TileContainer *_tilecont=nullptr;
        bool maxout;
        void bytechecklowdigit(const std::size_t& byte, const bool& andop);
        void bytecheckhighdigit(const std::size_t& byte, const bool& andop);
        void lastdigitcheck();
        ScoreContainer score;
        ScoreContainer scoretemp;
        std::multimap<unsigned int, ScoreContainer> top_scores;
		static nes_uchar pointsarray[10];

};


#endif // SCORE_H
