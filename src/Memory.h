#pragma once
#include <mutex>
#include <fstream>

class MemoryManager {
    static MemoryManager* instance;
    static std::mutex mtx;
    char mem[4096];

    MemoryManager();

    void loadFont(const unsigned char* const font);

public:
    MemoryManager(const MemoryManager& obj) = delete;
    static MemoryManager* getInstance();
    unsigned char loadAddr(unsigned int addr);
    void loadProgram(std::fstream &file);
};
