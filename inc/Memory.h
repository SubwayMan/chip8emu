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
    static const unsigned int programStartAddress = 0x200;
    static const unsigned int maxAddress = 0xFFF;
    MemoryManager(const MemoryManager& obj) = delete;
    static MemoryManager* getInstance();
    unsigned char loadAddr(unsigned int addr);
    void loadProgram(std::fstream &file);
};
