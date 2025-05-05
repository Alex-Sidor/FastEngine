#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

double a = 0;

Uint32 pixelBuffer[WINDOW_WIDTH*WINDOW_HEIGHT];

static void placePoint(Uint32* pixels,int x, int y) {
    int p = x + WINDOW_WIDTH * y;
    //Uint8 firstByte = (colour >> 24) & 0xFF;
    pixels[p] = 0xFFFFFFFF;
}

static void DrawLine(Uint32* pixels, float x1, float y1, float x2, float y2)
{
    float division;
    float biggest;

    if (std::abs(x1 - x2) > std::abs(y1 - y2)) {
        division = (y2 - y1) / (x2 - x1);
        biggest = std::abs(x2 - x1);
    }else {
        division = (x2 - x1) / (y2 - y1);
        biggest = std::abs(y2 - y1);
    }

    for (float a = 0; a <= biggest; a++) {
        int x = static_cast<int>(std::round(((x2 - x1) * (a / biggest)) + x1 + (WINDOW_WIDTH / 2)));
        int y = static_cast<int>(std::round(((y2 - y1) * (a / biggest)) + y1 + (WINDOW_HEIGHT / 2)));

        if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT) {
            placePoint(pixels, x, y);
        }
    }

}

static void trongle(Uint32* pixels, float x1, float y1, float x2, float y2, float x3, float y3) {

    
    DrawLine(pixels, x1,y1,x2,y2);
    DrawLine(pixels, x2, y2, x3, y3);
    DrawLine(pixels, x3, y3, x1, y1);

}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("Example Renderer Streaming Textures", "1.0", "com.example.renderer-streaming-textures");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("FastEngine", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!texture) {
        SDL_Log("Couldn't create streaming texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{

    for (int i = 0; i < (WINDOW_WIDTH * WINDOW_HEIGHT);i++) {
        pixelBuffer[i] = 0x000000FF;
    }

    trongle(&pixelBuffer[0], sin(a)*200, cos(a) * 200, sin(a + 2.1) * 200, cos(a + 2.1) * 200, sin(a + 4.2) * 200, cos(a + 4.2) * 200);
    a+= 0.001;


    SDL_UpdateTexture(texture, NULL, pixelBuffer,WINDOW_WIDTH*sizeof(Uint32));

    //SDL_FRect dst_rect = { 0, 0, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT };
    
    SDL_RenderTexture(renderer, texture, NULL, NULL);// &dst_rect);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}
