#include "Display.h"
#include <iostream>

Display::Display(int w, int h) : width(w), height(h) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL Init failed: " << SDL_GetError() << "\n";
        std::exit(1);
    }

    window = SDL_CreateWindow("SDL Display", width * 4, height * 4, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
        std::exit(1);
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << "\n";
        std::exit(1);
    }

    pixels = new bool*[height];
    for (int y = 0; y < height; ++y) {
        pixels[y] = new bool[width]{};
    }
}

Display::~Display() {
    for (int y = 0; y < height; ++y) {
        delete[] pixels[y];
    }
    delete[] pixels;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::togglePixel(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        pixels[y][x] = !pixels[y][x];
    }
}

bool Display::getPixel(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return pixels[y][x];
    }
    return false;
}

void Display::clear() {
    for (int y = 0; y < height; ++y) {
        std::fill(pixels[y], pixels[y] + width, false);
    }
}

void Display::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (pixels[y][x]) {
                SDL_RenderPoint(renderer, x, y);
            }
        }
    }

    SDL_RenderPresent(renderer);
}
