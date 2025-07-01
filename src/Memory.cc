#include "Memory.h"

MemoryManager* MemoryManager::getInstance() {
    std::lock_guard<std::mutex> lock(mtx);
    if (instance == nullptr) {
        instance = new MemoryManager();
    }
    return instance;
}

