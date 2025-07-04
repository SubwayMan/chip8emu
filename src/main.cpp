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

    p->loadProgram(file);
    file.close();
    Display display;

    for (int i=0; i<128; ++i) {
        display.togglePixel(i, 10);
    }


    bool running = true;
    CPU::getInstance();
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
