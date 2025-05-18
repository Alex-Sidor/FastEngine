#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <string>

#include <chrono>

class Camera {
public:
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;

    Uint32* pixelBuffer;

    bool isError = false;
    std::string error;

    Camera(int width, int height){
        WINDOW_WIDTH = width;
        WINDOW_HEIGHT = height;

        halfWidth = width/2;
        halfHeight = height/2;

        amountOfPixels = width * height;
        largestSize = height;
        bufferSize = largestSize * 2;

        pixelBuffer = new Uint32[amountOfPixels];   
        triangleBuffer = new Uint32[bufferSize];
    
        if (!SDL_CreateWindowAndRenderer("FastEngine", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
            isError = true;
            error = "Couldn't create Window and Renderer: ";
            error += SDL_GetError();
            error += "\n";
            return;
        }

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
        if (!texture) {
            isError = true;
            error = "Couldn't create Texture: ";
            error += SDL_GetError();
            error += "\n";
        }
    }

    ~Camera(){
        delete[] pixelBuffer;
        delete[] triangleBuffer;

        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }

    void checkForError(){
        if (isError) {
            SDL_Log("Camera Error: %s", error.c_str());
        }
    }

    void renderBuffer(){
        SDL_memset4(pixelBuffer, 0x000000FF, amountOfPixels);
        
        for (int i = 0; i < 10000; i++)
        {
            triangle(rand() % 1001 - 500, rand() % 1001 - 500,
                     rand() % 1001 - 500, rand() % 1001 - 500,
                     rand() % 1001 - 500, rand() % 1001 - 500);
        }

        SDL_UpdateTexture(texture, NULL, pixelBuffer,WINDOW_WIDTH*4);

        SDL_RenderTexture(renderer, texture, NULL, NULL);

        SDL_RenderPresent(renderer);
    }

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;

    Uint32* triangleBuffer;

    int halfWidth;
    int halfHeight;
    int amountOfPixels;
    int largestSize;
    int bufferSize;

    void placePoint(int p) {
        if (p >= 0 && p < amountOfPixels) {
            pixelBuffer[p] = 0xFFFFFFFF;
        }
    }

    void minMaxPlot(int x0, int y0) {
        if (x0 >= 0 && x0 < WINDOW_WIDTH && y0 >= 0 && y0 < WINDOW_HEIGHT) {
            int yMax = y0 + largestSize;
            if (triangleBuffer[y0] == -1) {
                triangleBuffer[y0] = x0;
                triangleBuffer[yMax] = x0;
            } else {
                if (x0 < triangleBuffer[y0])
                    triangleBuffer[y0] = x0;
                if (x0 > triangleBuffer[yMax])
                    triangleBuffer[yMax] = x0;
            }
        }
    }

    void drawLine(int x0, int y0, int x1, int y1) {
        bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);

        if (steep) {
            std::swap(x0, y0);
            std::swap(x1, y1);
        }

        if (x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }

        int dx = x1 - x0;
        int dy = std::abs(y1 - y0);
        int error = dx / 2;
        int ystep = (y0 < y1) ? 1 : -1;
        int y = y0;

        for (int x = x0; x <= x1; ++x) {
            if (steep)
                minMaxPlot(y, x);
            else
                minMaxPlot(x, y);

            error -= dy;
            if (error < 0) {
                y += ystep;
                error += dx;
            }
        }
    }

    void triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
        int ix1 = x1 + halfWidth;
        int iy1 = y1 + halfHeight;
        int ix2 = x2 + halfWidth;
        int iy2 = y2 + halfHeight;
        int ix3 = x3 + halfWidth;
        int iy3 = y3 + halfHeight;

        int minY = std::min({iy1, iy2, iy3});
        int maxY = std::max({iy1, iy2, iy3}) + 1;

        for (int i = minY; i < maxY; i++) {
            triangleBuffer[i] = -1;
            triangleBuffer[i + largestSize] = -1;
        }

        drawLine(ix1, iy1, ix2, iy2);
        drawLine(ix2, iy2, ix3, iy3);
        drawLine(ix3, iy3, ix1, iy1);

        int p = WINDOW_WIDTH * minY;

        for (int y = minY; y < maxY; y++) {
            if (triangleBuffer[y] != -1) {
                for (int x = triangleBuffer[y]; x < triangleBuffer[y + largestSize]; x++) {
                    placePoint(p + x);
                }
            }
            p += WINDOW_WIDTH;
        }
    }
};

Camera* camera = nullptr;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    camera = new Camera(1000, 1000);
    camera->checkForError();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    auto start = std::chrono::high_resolution_clock::now();

    camera->renderBuffer();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    SDL_Log("Frame Rendered in %lld µs", duration.count());

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    delete camera;
    SDL_Quit();
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}
