#pragma once

#include <SDL3/SDL.h>

class Display {
public:
    Display(int width = 128, int height = 64);
    ~Display();

    void togglePixel(int x, int y);
    void clear();
    void render();
    bool getPixel(int x, int y) const;

private:
    int width, height;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool** pixels;
};
