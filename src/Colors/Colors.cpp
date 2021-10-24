//
// Created by fra on 18/10/21.
//

#include "Colors.h"

Colors::Colors() : colors(DEFAULT_SIZE, false) {}

Colors::Colors(int size) : colors(size, false) {}

int Colors::findMinCol() {
    for(int i=0; i<colors.size(); i++)
        if(!colors[i])
            return i;
    return COLS_FULL;
}

int Colors::resizeColors() {
    unsigned int old_size = colors.size();
    colors.resize(old_size*2);
    return static_cast<int>(old_size);    // return index of first new slot generated
}


void Colors::addColor(int col) {
    if(col >= colors.size())
        resizeColors();

    colors[col] = true;
}

