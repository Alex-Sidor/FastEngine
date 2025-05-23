#include "screen.h"

Screen::Screen(int x, int y) {
    width = x * sizeof(Uint32);

    window = SDL_CreateWindow("Fast Engine", x, y, SDL_WINDOW_OPENGL);
    if (!window) {
        isError = true;
        error = "Couldn't create Window: ";
        error += SDL_GetError();
        throw std::runtime_error(error);
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        isError = true;
        error = "Couldn't create Renderer: ";
        error += SDL_GetError();
        throw std::runtime_error(error);
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, x, y);
    if (!texture) {
        isError = true;
        error = "Couldn't create Texture: ";
        error += SDL_GetError();
        throw std::runtime_error(error);
    }
}

void Screen::displayBuffer(Uint32* buffer) {
    SDL_UpdateTexture(texture, NULL, buffer, width);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
