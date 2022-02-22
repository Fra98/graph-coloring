#include "Colors.h"

Colors::Colors() : colors(DEFAULT_SIZE, false) {}

Colors::Colors(size_t size) {
    unsigned int newSize = nextHighestPowerOf2(size);
    newSize = std::max(newSize, unsigned(DEFAULT_SIZE));

    colors = boost::dynamic_bitset(newSize, false);
}

int Colors::findMinCol() const {
    boost::dynamic_bitset<> temp = colors;
    auto idx = temp.flip().find_first();

    if(idx == boost::dynamic_bitset<>::npos)
        return int(colors.size());
    else
        return int(idx);
}

void Colors::resizeColors(int maxCol) {
    auto newSize = nextHighestPowerOf2(maxCol+1);
    colors.resize(newSize);
}


void Colors::addColor(int col) {
    if(col >= colors.size())
        resizeColors(col);

    colors[col] = true;
}

