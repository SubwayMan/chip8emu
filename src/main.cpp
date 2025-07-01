#include <iostream>
#include <fstream>
#include "Memory.h"
#include "CPU.h"

int main(int argc, char* argv[]) {

    if (argc <= 1) {
        std::cout << "No file passed into emulator. Usage: ./emu <path/to/file>";
        return 1;
    }

    std::string filename = argv[1];
    std::fstream file = std::fstream(filename, std::ios::binary | std::ios::in);

    auto p = MemoryManager::getInstance();
    auto cpu = CPU::getInstance();

    p->loadProgram(file);
    file.close();
    
    for (int i=0; i<16; ++i) {
        std::cout << std::hex << (int)p->loadAddr(0x200 + i) << " ";
    }
    std::cout << std::endl;
    for (int i=0; i<6; ++i) {
        std::cout << std::hex << (int)p->loadAddr(0x200 + 0x80 + i) << " ";
    }
    std::cout << std::endl;
    for (int i=0; i<5; ++i) {
        std::cout << "memaddr " << std::hex << (int)p->loadAddr(0x50 + i) << std::endl;
    }
    cpu->setDelayTimer(5);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    cpu->setSoundTimer(5);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
