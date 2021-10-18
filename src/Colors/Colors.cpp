//
// Created by fra on 18/10/21.
//

#include "Colors.h"

Colors::Colors() {
    colors.reserve(MAX_SIZE);
    for(int i=0; i<MAX_SIZE; i++)
        colors[i] = false;
}

int Colors::findMinCol() {
    for(int i=0; i<MAX_SIZE; i++)
        if(!colors[i])
            return i;
    return COLS_FULL;
}

void Colors::addColor(int col) {
    colors[col] = true;
}