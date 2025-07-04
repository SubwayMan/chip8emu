#include "CPU.h"
#include "Display.h"
#include <iostream>
#include <bitset>

#define VF reg[0xF] // carry register

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

CPU::CPU(): PC{MemoryManager::programStartAddress}, indexRegister{0}, delayTimer{0}, soundTimer{0}, memory{nullptr}, display{nullptr}{
    dltThread = std::thread(&CPU::delayTimerThread, this);
    stThread = std::thread(&CPU::soundTimerThread, this);
    mainThread = std::thread(&CPU::CPUthread, this);
}

CPU::~CPU() {
    dltThread.join();
    stThread.join();
    mainThread.join();
}

void CPU::setDelayTimer(uint8_t val) {
    std::lock_guard<std::mutex> lock(dltMtx);
    delayTimer = val;
}

void CPU::setSoundTimer(uint8_t val) {
    std::lock_guard<std::mutex> lock(stMtx);
    soundTimer = val;
}

void CPU::CPUthread() {
    while (true) {
        auto memory = MemoryManager::getInstance();
        unsigned int data = memory->loadAddr(PC);
        data <<= 8;
        data += memory->loadAddr(PC + 1);
        std::cout << std::hex << data << " : " << std::hex <<  PC << ": ";
        PC+=2;
        // decode step
        switch (data & 0xF000) {
            case 0:
                switch (data & 0xFFF) {
                    case 0x0E0:
                        // clear screen
                        std::cout << "clear screen\n";
                        Display::getInstance()->clear();
                        break;
                    case 0x0EE:
                        // return from subroutine / break
                        break;
                }
                break;
            case 0x1000:
                // jump
            {
                unsigned int dest = 0xFFF & data;
                std::cout << "Jump to " << std::hex << dest << "\n";
                PC = dest;
                break;
            }
            case 0x2000:
                // call subroutine
                break;
            case 0x3000:
                // skip if register equal to immediate
                break;
            case 0x4000:
                // skip if register not equal to immediate
                break;
            case 0x5000:
                // skip if registers equal
                break;
            case 0x6000:
            {
                // set register
                uint8_t vx = static_cast<uint8_t>((data & 0x0F00) >> 8);
                uint8_t imm = static_cast<uint8_t>(data & 0x00FF);
                std::cout << "set register " << std::hex << static_cast<int>(vx) << " to " << std::hex << static_cast<int>(imm) << "\n";
                reg[vx] = imm;
                break;
            }
            case 0x7000:
            {
                // add immediate to register
                uint8_t vx = static_cast<uint8_t>((data & 0x0F00) >> 8);
                uint8_t imm = static_cast<uint8_t>(data & 0x00FF);
                std::cout << "Add " << std::hex << static_cast<int>(imm) << " to register " << std::hex << static_cast<int>(vx) << "\n";
                reg[vx] += imm;
                break;
            }
            case 0x8000:
                // arithmetic
                switch (data & 0xF) {
                    case 0:
                        // set reg X to value of reg Y
                        break;
                    case 1:
                        // set reg X to OR of X, Y
                        break;
                    case 2:
                        // set reg X to AND of X, Y
                        break;
                    case 3:
                        // set reg X to XOR of X, Y
                        break;
                    case 4:
                        // set reg X to ADD: X + Y
                        break;
                    // subtraction instructions will set the VF register to 1 (carry bit) if the left operand is greater than the second operand.
                    case 5:
                        // set reg X to SUB: X - Y
                        break;
                    // for shifting instructions, there are differences between the original CHIP-8 for COSMAC and  the newer CHIP-48/SUPERCHIP implementations.
                    // Old versions copied VY into VX and then shifted VX,
                    // while newer versions shift VX in-place without affecting VY.
                    // VF (carry flag) is set to the shifted-out bit.
                    case 6:
                        // right shift X
                        break;
                    case 7:
                        // set reg X to SUB: Y - X 
                        break;
                    case 0xE:
                        // left shift X
                        break;
                }
                break;
            case 0x9000:
                // skip if registers not equal
                break;
            case 0xA000:
            {
                uint16_t imm = static_cast<uint16_t>(data & 0x0FFF);
                std::cout << "set index register to " << std::hex << imm << std::endl;
                indexRegister = imm;
                // set index register to immediate
                break;
            }

            case 0xB000:
                // jump with offset: jump to value of V0+immediate
                // slightly different in CHIP-48, where it jumps to XNN + VX
                break;
            case 0xC000:
                // generate random number into VX, AND with immediate
            case 0xD000:
            {
                // display/draw
                uint8_t vx = static_cast<uint8_t>((data & 0x0F00) >> 8);
                uint8_t vy = static_cast<uint8_t>((data & 0x00F0) >> 4);
                if (vx >= 16 || vy >= 16) {
                    state = 3;
                    throw "Invalid register access";
                }
                uint8_t x = reg[vx] & 0x3F; // mod 64
                uint8_t y = reg[vy] & 0x1F; // mod 32
                uint8_t n = static_cast<uint8_t>(data & 0x000F);

                std::cout << "draw sprite at (" << std::hex << static_cast<int>(x) << ", " << std::hex << static_cast<int>(y) << ") of height " << std::hex << static_cast<int>(n) << std::endl;
                VF = 0;

                auto display = Display::getInstance();
                for (uint8_t i=0; i<n; ++i) {

                    uint8_t row = MemoryManager::getInstance()->loadAddr(indexRegister + i);
                    std::cout << std::bitset<8>(row) << std::endl;
                    auto tx = x;
                    for (uint8_t bitmask = 0x80; bitmask != 0; bitmask >>= 1) {
                        if (bitmask & row) {
                            if (display->getPixel(tx, y)) {
                                VF = 1;
                            }
                            display->togglePixel(tx, y);
                        }
                        tx++;
                        if (x >= 0x3F) break;
                    }
                    y++;
                    if (y >= 0x1F) break;
                }


                break;

            }
            case 0xE000:
                // keyboard input: skip if key
                switch (data & 0xFF) {
                    case 0x9E:
                        // skip if VX pressed
                        break;
                    case 0xA1:
                        // skip if VX NOT pressed
                        break;
                }
                break;
            case 0xF000:
                switch (data & 0xFF) {
                    // manipulate and read timers
                    case 0x07:
                        // read value of delay timer into VX
                        break;
                    case 0x15:
                        // set delay timer to value of VX
                        break;
                    case 0x18:
                        // set sound timer to value of VX
                        break;
                    // add to index register
                    case 0x1E:
                        break;
                    // block until key release, dump keycode into VX
                    case 0x0A:
                        break;
                    // grab value from VX, set index register to address of that value's font character
                    case 0x29:
                        break;
                    // output the contents of VX as a three digit decimal number into memory, starting at current position of index register
                    case 0x33:
                        break;
                    // memory load & store: reads / writes all registers from V0 to VX (inclusive)
                    // inconsistent between classic and CHIP48
                    // classic will increase index register, CHIP48 will not
                    case 0x55:
                        // store memory
                        break;
                    case 0x65:
                        // load memory
                        break;
                    // custom instruction to signify the end of the program.
                    // Blocks forever.
                    case 0xFF:
                        PC -= 2;
                        if (state != 0x01) {
                            state = 0x01;
                            std::cout << "Program terminated." << "\n";
                        }
                        break;
                }
                
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds(static_cast<int>(1E9 / CPU::CPUSpeed)));
    }
}
