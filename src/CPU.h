#pragma once
#include <mutex>
#include <stack>
#include <thread>

class CPU {
    static CPU* instance;
    static std::mutex mtx;
    
    unsigned int PC: 12;
    unsigned int indexRegister: 16;
    char delayTimer;
    char soundTimer;
    char reg[16];
    std::stack<unsigned int> st;
    std::thread dltThread;
    std::thread stThread;
    std::mutex dltMtx;
    std::mutex stMtx;

    void delayTimerThread();
    void soundTimerThread();

    CPU();
    ~CPU();

public:
    CPU(const CPU& obj) = delete;
    static CPU* getInstance();
    void setDelayTimer(unsigned char val);
    void setSoundTimer(unsigned char val);
};

