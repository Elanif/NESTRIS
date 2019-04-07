#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include"SDL.h"
#include<string>
#include<cstdio>
#include<cstring>

template<class ImageType>
class ImageLoader
{
    public:
        ImageLoader(SDL_Surface* _screenSurface, std::string base_path, SDL_Window* _window);
        virtual ~ImageLoader();
    protected:
    private:
        ImageType* blockimages[10][3];
        ImageType* OptimizeImage(SDL_Surface* _unoptimized, SDL_Surface* _screenSurface, SDL_Renderer *_renderer);
};

#endif
