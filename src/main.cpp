#include <iostream>
#include "Memory.h"

int main() {
    auto p = MemoryManager::getInstance();
    for (int i=0; i<5; ++i) {
        std::cout << "memaddr " << std::hex << (int)p->load(0x50 + i) << std::endl;
    }
}
