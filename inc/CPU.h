#pragma once
#include <mutex>
#include <stack>
#include <thread>
#include "Memory.h"

class CPU {
    static CPU* instance;
    static std::mutex mtx;
    
    unsigned int PC: 12;
    unsigned int indexRegister: 16;
    char delayTimer;
    char soundTimer;
    char reg[16];
    std::stack<unsigned int> st; // holds 2 byte values
    std::thread dltThread;
    std::thread stThread;
    std::mutex dltMtx;
    std::mutex stMtx;

    void delayTimerThread();
    void soundTimerThread();
    void CPUcycle();

    CPU();
    ~CPU();

public:
    CPU(const CPU& obj) = delete;
    static CPU* getInstance();
    void setDelayTimer(unsigned char val);
    void setSoundTimer(unsigned char val);
};

