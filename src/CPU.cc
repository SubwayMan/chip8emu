#include "CPU.h"

CPU* CPU::getInstance() {
    std::lock_guard<std::mutex> lock(mtx);
    if (instance == nullptr) {
        instance = new CPU();
    }
    return instance;
}

