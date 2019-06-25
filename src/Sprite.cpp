#include"Sprite.hpp"

void Sprite::print() {
    for (std::size_t i=0; i<8; ++i) {
        for (std::size_t j=0; j<8; ++j)
            printf("%d ",arr[j][i]);
        printf("\n");
    }
}


