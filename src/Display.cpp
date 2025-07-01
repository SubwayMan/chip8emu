#include "Display.h"
#include "SDL3/SDL_render.h"
#include <iostream>

Display::Display(): width{128}, height{64}, scaleFactor{4}{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL Init failed: " << SDL_GetError() << "\n";
        std::exit(1);
    }

    window = SDL_CreateWindow("SDL Display", width * scaleFactor, height * scaleFactor, SDL_WINDOW_INPUT_FOCUS);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
        std::exit(1);
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << "\n";
        std::exit(1);
    }
    frameBuf = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        width, height);

    SDL_SetRenderTarget(renderer, frameBuf);

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
    // draw points to texture
    SDL_SetRenderTarget(renderer, frameBuf);
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
    
    // set renderer target to main window
    SDL_SetRenderTarget(renderer, nullptr);

    // Clear the window
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Define destination rectangle scaled to window size
    SDL_FRect destRect = {0, 0, (float)width * scaleFactor, (float)height * scaleFactor};
    // Copy scaled framebuffer to window
    SDL_RenderTexture(renderer, frameBuf, nullptr, &destRect);

    SDL_RenderPresent(renderer);

}
