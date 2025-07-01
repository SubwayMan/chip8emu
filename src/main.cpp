#include <iostream>
#include "Memory.h"
#include "CPU.h"

int main() {
    auto p = MemoryManager::getInstance();
    auto cpu = CPU::getInstance();

    for (int i=0; i<5; ++i) {
        std::cout << "memaddr " << std::hex << (int)p->load(0x50 + i) << std::endl;
    }
    cpu->setDelayTimer(30);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cpu->setSoundTimer(30);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
