#include "Memory.h"
#include <cstring>

MemoryManager* MemoryManager::instance = nullptr;
std::mutex MemoryManager::mtx;

const unsigned char FONT[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void MemoryManager::loadFont(const unsigned char* const font) {
    std::memcpy((mem + 0x50), font, 80);
}

MemoryManager* MemoryManager::getInstance() {
    std::lock_guard<std::mutex> lock(mtx);
    if (instance == nullptr) {
        instance = new MemoryManager();
    }
    return instance;
}

MemoryManager::MemoryManager() {
    loadFont(FONT);
}

unsigned char MemoryManager::loadAddr(unsigned int addr) {
    std::lock_guard<std::mutex> lock(mtx);
    return mem[addr];
}

void MemoryManager::loadProgram(std::fstream &file) {
    
    if (!file.is_open()) {
        throw std::runtime_error("File not found.");
    }
    file.seekg(0);

    // CHIP-8 programs are loaded starting at 0x200.
    unsigned int addr = MemoryManager::programStartAddress;
    while (file) {
        if (addr >= MemoryManager::maxAddress) 
            throw std::out_of_range("Program too large.");

        file.get(mem[addr++]);
    }
    mem[addr++] = 0xFF;
    mem[addr] = 0xFF; // load program termination byte



}
