#include "Sprite.h"


Sprite::Sprite() {
    for (size_t i=0; i<8; ++i)
        for (size_t j=0; j<8; ++j)
            arr[i][j]=0;
}

void Sprite::print() {
    for (size_t i=0; i<8; ++i) {
        for (size_t j=0; j<8; ++j)
            printf("%d ",arr[j][i]);
        printf("\n");
    }
}


