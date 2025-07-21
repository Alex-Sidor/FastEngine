#include "screen.h"

Screen::Screen(int x, int y) {
    width = x * sizeof(Uint32);

    if (!SDL_CreateWindowAndRenderer("Fast Engine",x,y,0,&window,&renderer)) {
        error = "Couldn't create Renderer: ";
        error += SDL_GetError();
        throw std::runtime_error(error);
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, x, y);
    if (!texture) {
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
