#include"Window.h"

Window::Window(const int& _width, const int& _height)
{
    window=initWindow(_width,_height);
    inputManager=initInput();
    active=true;
    Engine _engine= Engine(window,10); //TODO change 10
    while (active) {
        if (inputManager->windowClosed()) {
            active=false;
            break;
        }
        _engine.frame(inputManager->getInput());
        SDL_Delay( 1000.0/60.0 );
        SDL_UpdateWindowSurface(window);
    }
}

Input* Window::initInput() {
    return new Input();
}

SDL_Window* Window::initWindow(const int& _width, const int& _height) {
    SDL_Window* _window=NULL;
    SDL_Surface* _windowSurface=NULL;
    _window = SDL_CreateWindow( "Nestris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height, SDL_WINDOW_SHOWN );
    if( _window == NULL )
    {
        active=false;
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        active=true;
        _windowSurface = SDL_GetWindowSurface( _window );
        SDL_FillRect( _windowSurface, NULL, SDL_MapRGB( _windowSurface->format, 0xFF, 0xFF, 0xFF ) );
        SDL_UpdateWindowSurface( _window );
    }
    return _window;
}

Window::~Window()
{
    SDL_DestroyWindow(window );
    SDL_Quit();
}
