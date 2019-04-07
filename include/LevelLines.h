#ifndef LEVELLINES_H
#define LEVELLINES_H
#include"Renderer.h"


class LevelLines : public Renderer
{
    public:
        LevelLines() {};
        LevelLines(SDL_Window * _window, const size_t& _frameappearance, const char& _level);
        const char& getlevel() const;
        void addlines(const char& _clearedlines);
    private:
        size_t lines;
        char level;
        unsigned short linestolevelup;
};

#endif // LEVELLINES_H
