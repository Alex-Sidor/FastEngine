#include "screen.h"

#include <iostream>
#include <algorithm>
#include <chrono>
#include <cmath>

struct vec2
{
    float x;
    float y;
    
    vec2(float X, float Y){
        x = X;
        y = Y;
    }
};

struct colour
{
    float r;
    float g;
    float b;
    
    colour() : r(0), g(0), b(0) {}

    colour(float R, float G, float B){
        r = R;
        g = G;
        b = B;
    }
};

class Camera {
public:
    int WINDOW_WIDTH,WINDOW_HEIGHT;

    Uint32* pixelBuffer;

    float angle = 0;

    Camera(int width, int height){
        WINDOW_WIDTH = width;
        WINDOW_HEIGHT = height;

        halfWidth = width/2;
        halfHeight = height/2;

        amountOfPixels = width * height;
        largestSize = height;
        bufferSize = largestSize * 2;

        pixelBuffer = new Uint32[amountOfPixels];   
        SDL_memset4(pixelBuffer, 0x000000FF, amountOfPixels);
        
        triangleBuffer = new int[bufferSize];
        memset(triangleBuffer, -1, bufferSize*sizeof(int));
        
        triangleColours = new colour[bufferSize]; 
    }

    ~Camera(){
        delete[] pixelBuffer;
        delete[] triangleBuffer;
    }

    void renderBuffer(){
        SDL_memset4(pixelBuffer, 0x000000FF, amountOfPixels);

        float r = 200.0f;
        float cx = WINDOW_WIDTH/2;
        float cy = WINDOW_HEIGHT/2;

        vec2 p0(
            cx + SDL_cosf(angle) * r,
            cy + SDL_sinf(angle) * r
        );

        vec2 p1(
            cx + SDL_cosf(angle + 2.094f) * r,
            cy + SDL_sinf(angle + 2.094f) * r
        );

        vec2 p2(
            cx + SDL_cosf(angle + 4.188f) * r,
            cy + SDL_sinf(angle + 4.188f) * r
        );

        colour c0 = colour(255, 0, 0);
        colour c1 = colour(0, 255, 0);
        colour c2 = colour(0, 0, 255);

        triangle(p0, p1,p2, c0, c1, c2);

        angle += 0.001f;
    }

private:
    int* triangleBuffer;
    colour* triangleColours;

    int halfWidth;
    int halfHeight;
    int amountOfPixels;
    int largestSize;
    int bufferSize;

    void placePoint(int p,colour c0) {
        pixelBuffer[p] = (static_cast<Uint8>(c0.r) << 24) + (static_cast<Uint8>(c0.g) << 16) + (static_cast<Uint8>(c0.b) << 8) + 255;
    }

    void minMaxPlot(int x0, int y0,colour c0) {
        if (x0 < 0 || x0 > WINDOW_WIDTH -1 || y0 < 0 || y0 > WINDOW_HEIGHT-1) return;
        
        if (triangleBuffer[y0] == -1) {
            triangleBuffer[y0] = x0;
            triangleBuffer[y0 + largestSize] = x0;
            
            triangleColours[y0] = c0;
            triangleColours[y0 + largestSize] = c0;

        } else {
            if (x0 < triangleBuffer[y0]){
                triangleBuffer[y0] = x0;
                triangleColours[y0] = c0;
            }
            else {
                if (x0 > triangleBuffer[y0 + largestSize]){
                    triangleBuffer[y0 + largestSize] = x0;
                    triangleColours[y0 + largestSize] = c0;
                }
            }   
        }
    }

    void drawLine(vec2 p0, vec2 p1, colour c0, colour c1) {
        float dx = p1.x - p0.x;
        float dy = p1.y - p0.y;

        float slope = dx/dy;

        int start = static_cast<int>(SDL_roundf(p0.y));
        int end = static_cast<int>(SDL_roundf(p1.y));

        float slopeR = (c1.r - c0.r)/dy;

        float slopeG = (c1.g - c0.g)/dy;

        float slopeB = (c1.b - c0.b)/dy;

        if(start>end){
            std::swap(start,end);
            
            float temp = p0.x;
            p0.x = p1.x;
            p1.x = temp;

            temp = c0.r;
            c0.r = c1.r;
            c1.r = temp;

            temp = c0.g;
            c0.g = c1.g;
            c1.g = temp;

            temp = c0.b;
            c0.b = c1.b;
            c1.b = temp;
        }

        for (int y = start; y < end; y++)
        {
            minMaxPlot(static_cast<int>(p0.x),y,c0);
            p0.x += slope;
           
            c0.r += slopeR;
            c0.g += slopeG;
            c0.b += slopeB;
        }
    }

    void triangle(vec2 p0, vec2 p1, vec2 p2, colour c0, colour c1, colour c2) { 
        int minY, maxY;

        if (p0.y > p1.y) {
            if (p0.y > p2.y) maxY = p0.y;
            else maxY = p2.y;

            if (p1.y < p2.y) minY = p1.y;
            else minY = p2.y;
        } else {
            if (p1.y > p2.y) maxY = p1.y;
            else maxY = p2.y;

            if (p0.y < p2.y) minY = p0.y;
            else minY = p2.y;
        }
        
        minY = static_cast<int>(roundf(minY));
        maxY = static_cast<int>(roundf(maxY));

        if(minY < 0) minY = 0;
        if(maxY > largestSize -1) maxY = largestSize;

        drawLine(p0,p1,c0,c1);
        drawLine(p1,p2,c1,c2);
        drawLine(p2,p0,c2,c0);

        int p = WINDOW_WIDTH * minY;
        for (int y = minY; y < maxY+1; y++) {
            float dx = triangleBuffer[y + largestSize]-triangleBuffer[y];
            
            colour currentColour = triangleColours[y];
            
            float slopeR = (triangleColours[y + largestSize].r - triangleColours[y].r)/dx;

            float slopeG = (triangleColours[y + largestSize].g - triangleColours[y].g)/dx;

            float slopeB = (triangleColours[y + largestSize].b - triangleColours[y].b)/dx;
            
            for (int x = triangleBuffer[y]; x < triangleBuffer[y + largestSize] + 1; x++) {
                placePoint(p + x, currentColour);
                
                currentColour.r += slopeR;
                currentColour.g += slopeG;
                currentColour.b += slopeB;
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

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        SDL_Log("Frame Rendered in %lld triangles per second", 1000000/duration.count());
        screen.displayBuffer(camera.pixelBuffer);
    }
    SDL_Quit();
}