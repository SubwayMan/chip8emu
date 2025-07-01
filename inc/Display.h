#pragma once

#include <SDL3/SDL.h>

class Display {
    int width, height;
    int scaleFactor;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* frameBuf;
    bool** pixels;

public:
    Display();
    ~Display();

    void togglePixel(int x, int y);
    void clear();
    void render();
    bool getPixel(int x, int y) const;
};
