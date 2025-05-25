#ifndef SCREEN_H
#define SCREEN_H

#include <SDL3/SDL.h>
#include <stdexcept>

class Screen {
public:
    Screen(int x, int y);
    void displayBuffer(Uint32* buffer);

    std::string error;

private:
    int width;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
};
#endif