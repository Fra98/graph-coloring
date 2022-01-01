#include "DurationLogger.h"

DurationLogger::DurationLogger(const string &name) {
    _name = name;
}

void DurationLogger::start() {
    cout << ">>>>>>>>>>>>>>> Starting " << _name << endl;
    _start = std::chrono::high_resolution_clock::now();
}

void DurationLogger::stop() {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(end - _start).count();
    cout << ">>>>>>>>>>>>>>> Ending " << _name << ", Duration: " << duration << endl;
}

