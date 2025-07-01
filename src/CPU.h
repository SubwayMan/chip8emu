#pragma once
#include <mutex>

class CPU {
    static CPU* instance;
    static std::mutex mtx;
    
    unsigned int PC: 12;
    unsigned int indexRegister: 16;
    char delayTimer;
    char soundTimer;
    char reg[16];
    CPU() {

    }

public:
    CPU(const CPU& obj) = delete;
    static CPU* getInstance();
};
