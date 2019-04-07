#include "ImageLoader.h"

template<class ImageType>
ImageLoader<ImageType>::ImageLoader(SDL_Surface* _screenSurface, std::string base_path, SDL_Window* _window)
{
    bool _fail=false;
    //loading 3 colored blocks per the 10 levels
    char*_blockpath; //destroy
    strcpy(_blockpath,base_path.c_str());
    strcat(_blockpath,"block_");
    for (size_t _levelnumber=0; _levelnumber<10; ++_levelnumber) {
        for (size_t _blocknumber=0; _blocknumber<3; ++_blocknumber) {
            char * _fullpath;
            strcpy(_fullpath,"");
            sprintf(_fullpath,"%s_%d_%d.bpm",_blockpath,_levelnumber,_blocknumber);
            SDL_Surface *_unoptimized=SDL_LoadBMP(_fullpath); //destroy
            if (_unoptimized==NULL) printf("Unable to load block image [%d,%d]\n",_levelnumber,_blocknumber);
            else {
                blockimages[_levelnumber][_blocknumber]=OptimizeImage(_unoptimized, _screenSurface->format, NULL );//SDL_ConvertSurface(_unoptimized, _screenSurface->format, NULL );
            }
        }
    }
}
template<class ImageType>
ImageType* ImageLoader<ImageType>::OptimizeImage(SDL_Surface*_unoptimized, SDL_Surface*_screenSurface, SDL_Renderer *_renderer){
    return SDL_ConvertSurface(_unoptimized, _screenSurface->format, NULL);
}
template<class ImageType>
ImageLoader<ImageType>::~ImageLoader()
{
    //dtor
}
