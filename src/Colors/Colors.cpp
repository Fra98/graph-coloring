//
// Created by fra on 18/10/21.
//

#include "Colors.h"

Colors::Colors() : colors(DEFAULT_SIZE, false) {}

Colors::Colors(int size) : colors(size, false) {}

int Colors::findMinCol() const {
    boost::dynamic_bitset<> temp = colors;
    auto idx = temp.flip().find_first();

    if(idx == boost::dynamic_bitset<>::npos)
        return COLS_FULL;
    else return static_cast<int>(idx);
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

