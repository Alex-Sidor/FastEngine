#include "screen.h"
#include "vector.h"

#include <iostream>

#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <cmath>

#include <math.h>

vec3 objectVertexBuffer[5];

int objectTriangleBuffer[18] = {0,1,4,0,1,2,3,1,2,2,3,4,0,2,4,3,1,4};

vec3 objectVertex[5] = {{-1,1,2},{1,1,2},{-1,1,4},{1,1,4},{0,-1,3}};

class Camera {
public:
    int WINDOW_WIDTH,WINDOW_HEIGHT;

    Uint32* pixelBuffer;

    float* pixelDepthBuffer;

    float angle = 0;
    float fov;
    
    float viewportScaleX;
    float viewportScaleY;

    Camera(int width, int height,float fieldOfView, float scale){
        WINDOW_WIDTH = width;
        WINDOW_HEIGHT = height;

        halfWidth = width/2;
        halfHeight = height/2;
        
        fov = tanf(fieldOfView/2)*2;
        
        viewportScaleX = (halfWidth * scale) / fov;        
        viewportScaleY = (halfHeight * scale) / fov;

        amountOfPixels = width * height;
        largestSize = height;
        bufferSize = largestSize * 2;

        pixelBuffer = new Uint32[amountOfPixels];   

        pixelDepthBuffer = new float[amountOfPixels];   
        
        triangleBuffer = new int[bufferSize];
        memset(triangleBuffer, -1, bufferSize*sizeof(int));
        
        triangleWeights = new vec3[bufferSize]; 
    }

    ~Camera(){
        delete[] pixelBuffer;
        delete[] pixelDepthBuffer;
        
        delete[] triangleBuffer;
        delete[] triangleWeights;
    }

    void renderBuffer(){
        SDL_memset4(pixelBuffer, 0x000000FF, amountOfPixels);//set screen to black
        memset(pixelDepthBuffer, 0, sizeof(float) * amountOfPixels);

        angle += 0.01;
        
        float s = sinf(angle);
        float c = cosf(angle);

        for(int i = 0; i < 5; i++){
		objectVertexBuffer[i] = rotateVector(objectVertex[i],{0,0,3},s,c);
	}

        for(int i = 0;i < 18; i+=3){
            triangle(objectVertexBuffer[objectTriangleBuffer[i]],objectVertexBuffer[objectTriangleBuffer[i+1]],objectVertexBuffer[objectTriangleBuffer[i+2]]);
        }
    }

    void convertDepthIntoGrayscaleAndDisplayTobuffer(float highest, float lowest){ 
        for(int i = 0;i < amountOfPixels; i++){
            Uint8 singleChannelColour = static_cast<Uint8>(((lowest - pixelDepthBuffer[i]) / (highest-lowest))*255);

            pixelBuffer[i] = (singleChannelColour << 24) + (singleChannelColour << 16) + (singleChannelColour << 8) + 255;
        }
    }

private:
    int* triangleBuffer;
    vec3* triangleWeights;

    int halfWidth;
    int halfHeight;
    int amountOfPixels;
    int largestSize;
    int bufferSize;

    vec3 rotateVector(vec3 original,vec3 pivot,float sin,float cos){
	float newX = original.x-pivot.x;
	float newZ = original.z-pivot.z;

        return (vec3){((newX*cos)-(newZ*sin))+pivot.x,original.y,((newX*sin)+(newZ*cos))+pivot.z};
    }

    void minMaxPlot(int x0, int y0,vec3 c0) {
        if (x0 < 0 || x0 > WINDOW_WIDTH -1 || y0 < 0 || y0 > WINDOW_HEIGHT-1) return;
        
        if (triangleBuffer[y0] == -1) {
            triangleBuffer[y0] = x0;
            triangleBuffer[y0 + largestSize] = x0;
            
            triangleWeights[y0] = c0;
            triangleWeights[y0 + largestSize] = c0;

        } else {
            if (x0 < triangleBuffer[y0]){
                triangleBuffer[y0] = x0;
                triangleWeights[y0] = c0;
            }
            else {
                if (x0 > triangleBuffer[y0 + largestSize]){
                    triangleBuffer[y0 + largestSize] = x0;
                    triangleWeights[y0 + largestSize] = c0;
                }
            }   
        }
    }

    inline void drawLine(vec2 p0, vec2 p1, vec3 c0, vec3 c1) {
        float dx = p1.x - p0.x;
        float dy = p1.y - p0.y;

        float slope = dx/dy;

        int start = static_cast<int>(SDL_roundf(p0.y));
        int end = static_cast<int>(SDL_roundf(p1.y));

        vec3 colourSlope = ((c1 - c0)/dy);

        if(start>end){
            for (int y = end; y < start; y++)
            {
                minMaxPlot(static_cast<int>(p1.x),y,c1);
                p1.x += slope;
            
                c1 += colourSlope;
            }
        }else{
            for (int y = start; y < end; y++)
            {
                minMaxPlot(static_cast<int>(p0.x),y,c0);
                p0.x += slope;
            
                c0 += colourSlope;
            }
        } 
    }

    void triangle(vec3 p0, vec3 p1, vec3 p2) { 
        
        if(p0.z <= 0 || p1.z <= 0 || p2.z <= 0){
            std::cout << "behind viewport - move forwards\n";
            return;
        }
        
        float u0 = 0;
        float u1 = 1;
        float u2 = 0;

        float v0 = 0;
        float v1 = 0;
        float v2 = 1;

        p0.x = (viewportScaleY * p0.x / (p0.z * fov)) + halfWidth;
        p1.x = (viewportScaleY * p1.x / (p1.z * fov)) + halfWidth;
        p2.x = (viewportScaleY * p2.x / (p2.z * fov)) + halfWidth;
        
        p0.y = (viewportScaleY * p0.y / (p0.z * fov)) + halfHeight;
        p1.y = (viewportScaleY * p1.y / (p1.z * fov)) + halfHeight;
        p2.y = (viewportScaleY * p2.y / (p2.z * fov)) + halfHeight;
        
        vec2 v1d = (vec2){p1.x - p0.x,p1.y - p0.y};
        vec2 v2d = (vec2){p2.x - p0.x,p2.y - p0.y};

        if(((v1d.y*v2d.x) - (v1d.x*v2d.y)) <= 0){
          return;
        }
        
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

        drawLine((vec2){p0.x, p0.y}, (vec2){p1.x, p1.y}, (vec3){1,0,0}, (vec3){0,1,0});
        drawLine((vec2){p1.x, p1.y}, (vec2){p2.x, p2.y}, (vec3){0,1,0}, (vec3){0,0,1});
        drawLine((vec2){p2.x, p2.y}, (vec2){p0.x, p0.y}, (vec3){0,0,1}, (vec3){1,0,0});
        
        float pixelZ;
        
        int p = WINDOW_WIDTH * minY;
        
        float u0invp0z = u0/p0.z;
        float u1invp1z = u1/p1.z;
        float u2invp2z = u2/p2.z;
        
        float v0invp0z = v0/p0.z;
        float v1invp1z = v1/p1.z;
        float v2invp2z = v2/p2.z;
        
        float invp0z = 1/p0.z;
        float invp1z = 1/p1.z;
        float invp2z = 1/p2.z;
        
        for (int y = minY; y < maxY+1; y++) {          
            vec3 currentColour = triangleWeights[y];
            
            vec3 slope = (triangleWeights[y + largestSize] - triangleWeights[y]) / (triangleBuffer[y + largestSize] - triangleBuffer[y]);
            
            for (int x = triangleBuffer[y]; x < triangleBuffer[y + largestSize]/* +1 i have no idea what this did before but when i removed it it fixed weird artifacts*/; x++) {
                
                pixelZ = 1/((triangleWeights[y].x * invp0z) + (triangleWeights[y].y * invp1z) + (triangleWeights[y].z * invp2z));

                if (pixelDepthBuffer[p + x] > pixelZ || pixelDepthBuffer[p + x] == 0){
                    pixelDepthBuffer[p + x] = pixelZ;
                    
                    float U = ((triangleWeights[y].x * u0invp0z) + (triangleWeights[y].y * u1invp1z) + (triangleWeights[y].z * u2invp2z)) * pixelZ;
                    float V = ((triangleWeights[y].x * v0invp0z) + (triangleWeights[y].y * v1invp1z) + (triangleWeights[y].z * v2invp2z)) * pixelZ;

                    //shader start
                    
                    if(((static_cast<int>(V*10)-static_cast<int>(U*10)) & 1) == 0){
                        pixelBuffer[p + x] = 4294967295; //white (2^32)-1
                    }else{
                        pixelBuffer[p + x] = 255; //black (2^8)-1
                    }
                    
                    //shader end
                } 

                triangleWeights[y] += slope;
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
    Camera camera(1000, 1000,120,0.5);

    std::cout << screen.error << "\n";

    bool running = true;
    SDL_Event event;

    while(running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        auto start = std::chrono::high_resolution_clock::now();

        camera.renderBuffer();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        SDL_Log("%ld frames per second", 1000000/duration.count());
        
        //camera.convertDepthIntoGrayscaleAndDisplayTobuffer(0,10);

        screen.displayBuffer(camera.pixelBuffer);
    }
    SDL_Quit();
}
