#include <iostream>
#include <fstream>
#include "Memory.h"
#include "CPU.h"
#include "Display.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

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
    Display display(128, 64);

    display.togglePixel(10, 10);
    display.togglePixel(20, 20);
    display.togglePixel(30, 30);

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        display.render();
        SDL_Delay(16);
    }

}
