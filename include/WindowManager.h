#ifndef _WINDOWMANAGER_H_
#define _WINDOWMANAGER_H_

#define WINDOWMANAGER_ENUM_START 0
#include"Input.h"
#include"Renderer.h"


class WindowManager
{
    public:
        enum MenuType {
            WM_LEVELSELECT=WINDOWMANAGER_ENUM_START+0
        };
        WindowManager(SDL_Window* _window, Input* _inputManager);
    protected:
    private:
        size_t currentmenu;
        bool active;
        SDL_Window * window;
        Input* inputmanager;
};

#endif
