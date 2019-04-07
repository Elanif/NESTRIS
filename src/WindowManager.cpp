#include"WindowManager.h"

WindowManager::WindowManager(SDL_Window* _window, Input* _inputmanager)
    :window(_window), inputmanager(_inputmanager)
{
    active=true;
    Renderer _render= Renderer(window,WM_LEVELSELECT);
    Input WMInput=Input();
    while (active) {
        if (WMInput.windowClosed()) {
            active=false;
            break;
        }
        _render.frame(WMInput.getInput());
        SDL_Delay( 1000.0/60.0 );
    }
}
