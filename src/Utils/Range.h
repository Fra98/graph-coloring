#ifndef SRC_RANGE_H
#define SRC_RANGE_H

class Range {
    size_t _totalSize;
    size_t _numSplits;
    size_t _splitSize;

public:
    Range(size_t totalSize, size_t numSplits) : _totalSize(totalSize), _numSplits(numSplits) {
        _splitSize = totalSize / numSplits;
    };

    [[nodiscard]] size_t getStart(unsigned int index) const {
        return index * _splitSize;
    };

    [[nodiscard]] size_t getEnd(unsigned int index) const {
        if(index+1 == _numSplits)       // last thread take all remaining elements
            return _totalSize;
        return (index+1) * _splitSize;
    };
};


#endif //SRC_RANGE_H
