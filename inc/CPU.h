#pragma once
#include <mutex>
#include <stack>
#include <thread>
#include "Memory.h"
#include "Display.h"

class CPU {
    static CPU* instance;
    static std::mutex mtx;
    
    unsigned int PC: 12;
    uint16_t indexRegister: 16;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint8_t reg[16];
    std::stack<unsigned int> st; // holds 2 byte values
    std::thread dltThread;
    std::thread stThread;
    std::thread mainThread;
    // We mutex the delay timer and sound timer as they may be modified
    // from multiple threads.
    // We assume all registers including PC and I will only be modified from the CPU main cycle thread, so they are not mutexed.
    std::mutex dltMtx;
    std::mutex stMtx;
    // for signalling processor state:
    // 0xFF for terminated
    // 0x01 for invalid instruction error
    uint8_t state = 0;
    // for communication with memory, display
    MemoryManager* memory;
    Display* display;

    void delayTimerThread();
    void soundTimerThread();
    void CPUthread();

    CPU();
    ~CPU();

public:
    static const int CPUSpeed = 10; // cycles per second
    CPU(const CPU& obj) = delete;
    static CPU* getInstance();
    void setDelayTimer(uint8_t val);
    void setSoundTimer(uint8_t val);
};

