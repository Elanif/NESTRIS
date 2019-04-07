#include "Renderer.h"

Renderer::Renderer(TileContainer* _tilecont, const std::size_t& _frameappearance)
    :tilecont(_tilecont), frameappearance(_frameappearance)
{

}
Renderer::Renderer()
{
    tilecont=NULL;
}

 void Renderer::hide(const nes_uchar& _hidecounter) {
     hidecounter=_hidecounter;
 }
 void Renderer::sleep(const nes_uchar& _sleepcounter) {
     sleepcounter=_sleepcounter;
 }
