//
// Created by fra on 18/10/21.
//

#ifndef SRC_COLORS_H
#define SRC_COLORS_H

#include <vector>
#include <iostream>
#include <boost/dynamic_bitset.hpp>

#define DEFAULT_SIZE 32
#define COLS_FULL -1

class Colors {
    boost::dynamic_bitset<> colors;

public:
    Colors();

    explicit Colors(int size);

    [[nodiscard]] int findMinCol() const;

    void addColor(int col);

    int resizeColors();
};

#endif //SRC_COLORS_H
