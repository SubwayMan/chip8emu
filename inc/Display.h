#pragma once
#include <mutex>

#include <SDL3/SDL.h>

class Display {
    static Display* instance;
    static std::mutex mtx;

    int width, height;
    int scaleFactor;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* frameBuf;
    bool** pixels;
    Display();
    

public:
    ~Display();
    static Display* getInstance();

    void togglePixel(int x, int y);
    void clear();
    void render();
    bool getPixel(int x, int y) const;
};
