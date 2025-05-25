#include "screen.h"

#include <iostream>
#include <algorithm>
#include <chrono>
#include <cmath>

class Camera {
public:
    int WINDOW_WIDTH,WINDOW_HEIGHT;

    Uint32* pixelBuffer;

    float angle;

    Camera(int width, int height){
        WINDOW_WIDTH = width;
        WINDOW_HEIGHT = height;

        halfWidth = width/2;
        halfHeight = height/2;

        amountOfPixels = width * height;
        largestSize = height;
        bufferSize = largestSize * 2;

        pixelBuffer = new Uint32[amountOfPixels];   
        triangleBuffer = new int[bufferSize];

        angle = 0;

        memset(triangleBuffer, -1, bufferSize*sizeof(int));
        SDL_memset4(pixelBuffer, 0x000000FF, amountOfPixels);
    }

    ~Camera(){
        delete[] pixelBuffer;
        delete[] triangleBuffer;
    }

    void renderBuffer(){
        SDL_memset4(pixelBuffer, 0x000000FF, amountOfPixels);

        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            for (int y = 0; y < WINDOW_HEIGHT; y++)
            {
                triangle(x,y,x+5,y,x,y+5);
            }
        }
        
        //angle += 0.001;
    }

private:
    int* triangleBuffer;

    int halfWidth;
    int halfHeight;
    int amountOfPixels;
    int largestSize;
    int bufferSize;

    void placePoint(int p) {
        if (p >= 0 && p < amountOfPixels)
            pixelBuffer[p] = 0xFFFFFFFF;
    }

    void minMaxPlot(int x0, int y0) {
        if (x0 < 0 || x0 > WINDOW_WIDTH -1 || y0 < 0 || y0 > WINDOW_HEIGHT-1) return;
        
        if (triangleBuffer[y0] == -1) {
            triangleBuffer[y0] = x0;
            triangleBuffer[y0 + largestSize] = x0;
        } else {
            if (x0 < triangleBuffer[y0])
                triangleBuffer[y0] = x0;
            else {
                if (x0 > triangleBuffer[y0 + largestSize])
                    triangleBuffer[y0 + largestSize] = x0;
            }
        }
    }

    void drawLine(float x0, float y0, float x1, float y1) {
        float dx = x1 - x0;
        float dy = y1 - y0;

        float slope = dx/dy;

        int start = static_cast<int>(SDL_roundf(y0));
        int end = static_cast<int>(SDL_roundf(y1));

        if(start>end){
            std::swap(start,end);
            std::swap(x0,x1);
        }
        //float x = x0;
        for (int y = start; y < end; y++)
        {
            minMaxPlot(static_cast<int>(x0),y);
            x0 += slope;
        }
    }

    void triangle(float x1, float y1, float x2, float y2, float x3, float y3) { 
        int minY, maxY;

        if (y1 > y2) {
            if (y1 > y3) maxY = y1;
            else maxY = y3;

            if (y2 < y3) minY = y2;
            else minY = y3;
        } else {
            if (y2 > y3) maxY = y2;
            else maxY = y3;

            if (y1 < y3) minY = y1;
            else minY = y3;
        }

        
        minY = static_cast<int>(roundf(minY));
        maxY = static_cast<int>(roundf(maxY));

        if(minY < 0) minY = 0;
        if(maxY > largestSize -1) maxY = largestSize;

        drawLine(x1, y1, x2, y2);
        drawLine(x2, y2, x3, y3);
        drawLine(x3, y3, x1, y1);

        int p = WINDOW_WIDTH * minY;
        for (int y = minY; y < maxY; y++) {
            for (int x = triangleBuffer[y]; x < triangleBuffer[y + largestSize] + 1; x++) {
                placePoint(p + x);
            }
            triangleBuffer[y] = -1;
            triangleBuffer[y+largestSize] = -1;
            p += WINDOW_WIDTH;
        }

        /*for (int i = 0; i < bufferSize; i++)
        {
            if(triangleBuffer[i] != -1){
                std::cout << i << "uncleaned buffer position\n";
            }
        }*/
    }
};

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failed to init SDL: %s", SDL_GetError());
        return 1;
    }

    Screen screen(1000, 1000);
    Camera camera(1000, 1000);

    std::cout << screen.error << "\n";

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        auto start = std::chrono::high_resolution_clock::now();

        camera.renderBuffer();
        screen.displayBuffer(camera.pixelBuffer);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        SDL_Log("Frame Rendered in %lld triangles per second", 1000000000000/duration.count());
    }
    SDL_Quit();
}