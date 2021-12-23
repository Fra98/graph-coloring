#include "DurationLogger.h"

DurationLogger::DurationLogger(const string &name) {
    cout << ">>>>>>>>>>>>>>> Starting " << name << endl;
    _name = name;
    _start = std::chrono::high_resolution_clock::now();
}

DurationLogger::~DurationLogger() {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(end - _start).count();
    cout << ">>>>>>>>>>>>>>> Ending " << _name << ", Duration: " << duration << endl;
}
