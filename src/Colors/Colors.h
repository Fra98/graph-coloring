//
// Created by fra on 18/10/21.
//

#ifndef SRC_COLORS_H
#define SRC_COLORS_H

#include <vector>
#include <iostream>

#define DEFAULT_SIZE 32
#define COLS_FULL -1

class Colors {
    std::vector<bool> colors;

public:
    Colors();

    explicit Colors(int size);

    int findMinCol();

    void addColor(int col);

    int resizeColors();
};

#endif //SRC_COLORS_H
