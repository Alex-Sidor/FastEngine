#include "screen.h"
#include "vector.h"

#include <iostream>

#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <cmath>

#include <math.h>

vec3 objectVertexBuffer[5] = {{-100,100,1},{100,100,1},{-100,100,3},{100,100,3},{0,-100,2}};
//0,1,4
int objectTriangleBuffer[18] = {0,1,4,0,1,2,3,1,2,2,3,4,0,2,4,3,1,4};

class Camera {
public:
    int WINDOW_WIDTH,WINDOW_HEIGHT;

    Uint32* pixelBuffer;

    float* pixelDepthBuffer;

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

        pixelDepthBuffer = new float[amountOfPixels];   
        for(int i = 0;i < amountOfPixels; i++){
            pixelDepthBuffer[i] = 0.0f;
        }
        
        triangleBuffer = new int[bufferSize];
        memset(triangleBuffer, -1, bufferSize*sizeof(int));
        
        triangleColours = new colour[bufferSize]; 
    }

    ~Camera(){
        delete[] pixelBuffer;
        delete[] pixelDepthBuffer;
        
        delete[] triangleBuffer;
    }

    void renderBuffer(){
        SDL_memset4(pixelBuffer, 0x000000FF, amountOfPixels);//set screen to black
        
        angle += 0.01;

        vec3 objectVertexBuffer[5] = {{sinf(angle + 3.92699081788) * 100,100,cosf(angle + 3.92699081788) + 2},{sinf(angle + 2.35619449073) * 100,100,cosf(angle + 2.35619449073) + 2
},{sinf(angle + 5.49778714504) * 100,100,cosf(angle + 5.49778714504) + 2},{sinf(angle + 0.785398163577) * 100,100,cosf(angle + 0.785398163577) + 2},{0,-100,2}};

        for(int i = 0;i < amountOfPixels; i++){
            pixelDepthBuffer[i] = 0.0f;
        }

        for(int i = 0;i < 18; i+=3){
            triangle(objectVertexBuffer[objectTriangleBuffer[i]],objectVertexBuffer[objectTriangleBuffer[i+1]],objectVertexBuffer[objectTriangleBuffer[i+2]]);
        }
    }

    void convertDepthIntoGrayscale(float highest, float lowest){
        
        if(highest == lowest){
            for(int i = 0;i < amountOfPixels; i++){
                if(pixelDepthBuffer[i] > highest && pixelDepthBuffer[i] != 0.0f){
                    highest = pixelDepthBuffer[i];
                }
                else{
                    if(pixelDepthBuffer[i] < lowest && pixelDepthBuffer[i] != 0.0f){
                        lowest = pixelDepthBuffer[i];
                    }
                }
            }
        }     

        for(int i = 0;i < amountOfPixels; i++){
            Uint8 singleChannelColour = static_cast<Uint8>(roundf(((lowest - pixelDepthBuffer[i]) / (highest-lowest))*255));

            pixelBuffer[i] = (static_cast<Uint8>(singleChannelColour) << 24) + (static_cast<Uint8>(singleChannelColour) << 16) + (static_cast<Uint8>(singleChannelColour) << 8) + 255;
        }

    }

private:
    int* triangleBuffer;
    colour* triangleColours;

    int halfWidth;
    int halfHeight;
    int amountOfPixels;
    int largestSize;
    int bufferSize;

    void shader(int p,colour c0, float z0, float z1, float z2) {

        float u0 = 0;
        float u1 = 1;
        float u2 = 0;

        float v0 = 0;
        float v1 = 0;
        float v2 = 1;

        float Up = (c0.r*(u0/z0))+(c0.g*(u1/z1))+(c0.b*(u2/z2));
        float Vp = (c0.r*(v0/z0))+(c0.g*(v1/z1))+(c0.b*(v2/z2));
        float Zp = (c0.r/z0)+(c0.g/z1)+(c0.b/z2);

        float U = Up/Zp;
        float V = Vp/Zp;

        pixelBuffer[p] = (static_cast<Uint8>(U*255) << 24) + (static_cast<Uint8>(V*255) << 16) + (static_cast<Uint8>(0) << 8) + 255;
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

        colour colourSlope = ((c1 - c0)/dy);

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
           
            c0 += colourSlope;
        }
    }

    void triangle(vec3 p0, vec3 p1, vec3 p2) { 
        if(p0.z <= 0 || p1.z <= 0 || p2.z <= 0){
            return;
        }
        
        float u0 = 0;
        float u1 = 1;
        float u2 = 0;

        float v0 = 0;
        float v1 = 0;
        float v2 = 1;

        p0.x = (p0.x / p0.z) + halfWidth;
        p1.x = (p1.x / p1.z) + halfWidth;
        p2.x = (p2.x / p2.z) + halfWidth;

        p0.y = (p0.y / p0.z) + halfHeight;
        p1.y = (p1.y / p1.z) + halfHeight;
        p2.y = (p2.y / p2.z) + halfHeight;

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

        drawLine((vec2){p0.x, p0.y}, (vec2){p1.x, p1.y} ,(colour){1,0,0}, (colour){0,1,0});
        drawLine((vec2){p1.x, p1.y}, (vec2){p2.x, p2.y} ,(colour){0,1,0}, (colour){0,0,1});
        drawLine((vec2){p2.x, p2.y}, (vec2){p0.x, p0.y} ,(colour){0,0,1}, (colour){1,0,0});

        int p = WINDOW_WIDTH * minY;
        for (int y = minY; y < maxY+1; y++) {
            float dx = triangleBuffer[y + largestSize]-triangleBuffer[y];
            
            colour currentColour = triangleColours[y];
            
            colour slope = (triangleColours[y + largestSize] - triangleColours[y])/dx;
            
            for (int x = triangleBuffer[y]; x < triangleBuffer[y + largestSize] + 1; x++) {
             

                
                
                float Zp = (triangleColours[y].r / p0.z) + (triangleColours[y].g/p1.z) + (triangleColours[y].b/p2.z);
                float pixelZ = 1/Zp;

                if (pixelDepthBuffer[p + x] > pixelZ || pixelDepthBuffer[p + x] == 0){
                    pixelDepthBuffer[p + x] = pixelZ;
                    
                    float Up = (triangleColours[y].r * (u0/p0.z)) + (triangleColours[y].g*(u1/p1.z)) + (triangleColours[y].b * (u2/p2.z));
                    float Vp = (triangleColours[y].r * (v0/p0.z)) + (triangleColours[y].g*(v1/p1.z)) + (triangleColours[y].b * (v2/p2.z));
                    
                    float U = Up/Zp;
                    float V = Vp/Zp;

                    //shader start
                
                    V = std::floorf(V*10);
                    U = (std::floorf(U*10))*11;

                    float c = std::fmod((U+V),2);
                    
                    if(c == 0){
                        pixelBuffer[p + x] = (static_cast<Uint8>(255) << 24) + (static_cast<Uint8>(255) << 16) + (static_cast<Uint8>(255) << 8) + 255;
                    }else{
                        pixelBuffer[p + x] = (static_cast<Uint8>(0) << 24) + (static_cast<Uint8>(0) << 16) + (static_cast<Uint8>(0) << 8) + 255;
                    }
                    
                    //shader end

                    
                } 

                triangleColours[y] += slope;
                
            }
            
            triangleBuffer[y] = -1;
            triangleBuffer[y+largestSize] = -1;
            p += WINDOW_WIDTH;
        }
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
        
        //camera.convertDepthIntoGrayscale(0.5,3.5);

        screen.displayBuffer(camera.pixelBuffer);
    }
    SDL_Quit();
}