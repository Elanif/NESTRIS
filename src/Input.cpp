#include "Input.h"
#define MAXBUTTONS 8
#include<cstdio>
Input::Input()
    :leftandright(false)
{
    inputevent = new SDL_Event();
    activeinputs=new bool[MAXBUTTONS];
    prevactiveinputs=new bool[MAXBUTTONS];
    for (size_t buttoninit=0; buttoninit<MAXBUTTONS; ++buttoninit) prevactiveinputs[buttoninit]=activeinputs[buttoninit]=false;
    setup(255); //tasti della tastiera
}

void Input::update(const Uint8* _keyboardstate, int*_inputdependancies, const size_t& _keys, const size_t& _buttons) {
    delete[] prevactiveinputs;
    prevactiveinputs=activeinputs;
    bool* _activeinputs= new bool[_buttons];
    for (size_t buttoninit=0; buttoninit<MAXBUTTONS; ++buttoninit) _activeinputs[buttoninit]=false;
    for (size_t keycode=0; keycode<_keys; ++keycode) //MEGLIO FARLO ALL'INCONTRARIO
        if (_keyboardstate[keycode]) {
            _activeinputs[_inputdependancies[keycode]]=true;
            //printf("keycode active = %d\n",keycode);
        }
    activeinputs=_activeinputs;
    //axis handling?
    if (activeinputs[RIGHT]) {
        if (activeinputs[LEFT]) {
            activeinputs[LEFT]=false;
            leftandright=true;
        }
    }
    #ifdef DEBUG
    for (size_t i=0; i<MAXBUTTONS; ++i)
        printf("%d ",activeinputs[i]);
    printf("\n");
    #endif
    if (activeinputs[DOWN]&&activeinputs[UP]); //up and down stop working until both are held up
}

ActiveInputs Input::getInput() {
    const Uint8* keyboardstate=SDL_GetKeyboardState(NULL);
    this->update(keyboardstate, inputdependancies, 255, MAXBUTTONS);
    return ActiveInputs(MAXBUTTONS, prevactiveinputs, activeinputs,leftandright);
}

void Input::setup(const size_t& joypadbuttons) { //fix parameters
    inputdependancies=new int[joypadbuttons];
    inputdependancies[SDL_SCANCODE_LEFT]=LEFT;
    inputdependancies[SDL_SCANCODE_UP]=UP;
    inputdependancies[SDL_SCANCODE_RIGHT]=RIGHT;
    inputdependancies[SDL_SCANCODE_DOWN]=DOWN;
    inputdependancies[SDL_SCANCODE_A]=SELECT;
    inputdependancies[SDL_SCANCODE_S]=START;
    inputdependancies[SDL_SCANCODE_C]=A;//inputdependancies[SDL_SCANCODE_Z]=B;
    inputdependancies[SDL_SCANCODE_V]=B;//inputdependancies[SDL_SCANCODE_X]=A;
}

Input::~Input() {
    delete inputevent;
    delete[] inputdependancies;
    delete[] activeinputs;
    delete[] prevactiveinputs;
}

bool Input::windowClosed() const {
    while (SDL_PollEvent(inputevent))
        if( inputevent->type == SDL_QUIT ) return true;
    return false;
}
