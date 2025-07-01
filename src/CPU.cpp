#include "CPU.h"
#include <iostream>

CPU* CPU::instance = nullptr;
std::mutex CPU::mtx;

CPU* CPU::getInstance() {
    std::lock_guard<std::mutex> lock(mtx);
    if (instance == nullptr) {
        instance = new CPU();
    }
    return instance;
}

void CPU::delayTimerThread() {
    // 60hz
    while (true) {
        dltMtx.lock();
        if (delayTimer > 0) {
            delayTimer--;
            std::cout << "delay" << std::endl;
        }
        dltMtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
    }
}

void CPU::soundTimerThread() {
    // 60hz
    while (true) {
        stMtx.lock();
        if (soundTimer > 0) {
            soundTimer--;
            std::cout << "beep" << std::endl;
        }
        stMtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
    }
}

CPU::CPU(): PC{MemoryManager::programStartAddress}, indexRegister{0}, delayTimer{0}, soundTimer{0} {
    dltThread = std::thread(&CPU::delayTimerThread, this);
    stThread = std::thread(&CPU::soundTimerThread, this);
}

CPU::~CPU() {
    dltThread.join();
    stThread.join();
}

void CPU::setDelayTimer(unsigned char val) {
    std::lock_guard<std::mutex> lock(dltMtx);
    delayTimer = val;
}

void CPU::setSoundTimer(unsigned char val) {
    std::lock_guard<std::mutex> lock(stMtx);
    soundTimer = val;
}
