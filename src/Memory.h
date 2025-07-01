#pragma once
#include <mutex>

class MemoryManager {
    static MemoryManager* instance;
    static std::mutex mtx;
    char mem[4096];

    MemoryManager();

    void loadFont(const unsigned char* const font);

public:
    MemoryManager(const MemoryManager& obj) = delete;
    static MemoryManager* getInstance();
    unsigned char load(unsigned int addr);
};
