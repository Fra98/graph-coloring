#ifndef SRC_COLORS_H
#define SRC_COLORS_H

#include <vector>
#include <iostream>
#include <boost/dynamic_bitset.hpp>

#include "../Utils/Utils.h"

#define DEFAULT_SIZE 64

class Colors {
    boost::dynamic_bitset<> colors;

    void resizeColors(int maxCol);

public:
    Colors();

    explicit Colors(size_t size);

    [[nodiscard]] int findMinCol() const;

    void addColor(int col);
};

#endif //SRC_COLORS_H
