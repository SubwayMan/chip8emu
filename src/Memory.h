#pragma once
#include <mutex>

class MemoryManager {
    static MemoryManager* instance;
    static std::mutex mtx;
    char mem[4096];

    MemoryManager() {

    }

public:
    MemoryManager(const MemoryManager& obj) = delete;
    static MemoryManager* getInstance();
};
