//
// Created by fra on 15/02/22.
//

#ifndef SRC_RANGE_H
#define SRC_RANGE_H

class Range {
    unsigned int _totalSize;
    unsigned int _numSplits;
    unsigned int _splitSize;

public:
    Range(unsigned int totalSize, unsigned int numSplits) : _totalSize(totalSize), _numSplits(numSplits) {
        _splitSize = totalSize / numSplits;
    };

    [[nodiscard]] unsigned int getStart(int index) const {
        return index * _splitSize;
    };

    [[nodiscard]] unsigned int getEnd(int index) const {
        if(index+1 == _numSplits)       // last thread take all remaining elements
            return _totalSize;
        return (index+1) * _splitSize;
    };
};


#endif //SRC_RANGE_H
