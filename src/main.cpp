#include "screen.h"
#include "vector.h"

#include <iostream>

#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <cmath>

#include <string>

#include <math.h>

vec3 objectVertexBuffer[5];

int objectTriangleBuffer[18] = {0,1,4,0,1,2,3,2,1,3,2,4,0,4,2,1,3,4};

vec3 objectVertex[5] = {{-1,1,2},{1,1,2},{-1,1,4},{1,1,4},{0,-1,3}};

class Camera {
public:
    int WINDOW_WIDTH,WINDOW_HEIGHT;

    Uint32* pixelBuffer;

    float* pixelDepthBuffer;

    float angle = 0;
    float fov;

    Camera(int width, int height,float fieldOfView, float scale){
        WINDOW_WIDTH = width;
        WINDOW_HEIGHT = height;

        halfWidth = width/2;
        halfHeight = height/2;
        
        fov = tanf(fieldOfView/2)*2;
        
        viewportScaleX = (halfWidth * scale) / fov;        
        viewportScaleY = (halfHeight * scale) / fov;

        amountOfPixels = width * height;
        bufferOffset = height;
        bufferSize = bufferOffset * 2;

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
		    rotateVector(objectVertex[i],objectVertexBuffer[i],{0,0,3},s,c);
            projectVertex(objectVertexBuffer[i],objectVertexBuffer[i]);
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
    int bufferOffset;
    int bufferSize;

    float viewportScaleX;
    float viewportScaleY;

    void projectVertex(vec3& original,vec3& transformed){
        transformed.x = (viewportScaleY * original.x / (original.z * fov)) + halfWidth;
        transformed.y = (viewportScaleY * original.y / (original.z * fov)) + halfHeight;
    }

    void rotateVector(vec3& original,vec3& tramsformed,vec3 pivot,float sin,float cos){
        float newX = original.x-pivot.x;
        float newZ = original.z-pivot.z;

        tramsformed.x = ((newX*cos)-(newZ*sin))+pivot.x;
        tramsformed.y = original.y;
        tramsformed.z = ((newX*sin)+(newZ*cos))+pivot.z;
    }

    void minMaxPlot(int x0, int y0,vec3 c0) {
        if (x0 < 0 || x0 > WINDOW_WIDTH -1 || y0 < 0 || y0 > WINDOW_HEIGHT-1){
            //::cout << "pixel out of range\n";
            return;
        }
        
        if (triangleBuffer[y0] == -1) {
            triangleBuffer[y0] = x0;
            triangleBuffer[y0 + bufferOffset] = x0;
            
            triangleWeights[y0] = c0;
            triangleWeights[y0 + bufferOffset] = c0;

        } else {
            if (x0 < triangleBuffer[y0]){
                triangleBuffer[y0] = x0;
                triangleWeights[y0] = c0;
            }
            else {
                if (x0 > triangleBuffer[y0 + bufferOffset]){
                    triangleBuffer[y0 + bufferOffset] = x0;
                    triangleWeights[y0 + bufferOffset] = c0;
                }
            }   
        }
    }

    inline void drawLine(vec2 p0, vec2 p1, vec3 w0, vec3 w1) {
        float dx = p1.x - p0.x;
        float dy = 1/(p1.y - p0.y);

        float slope = dx*dy;

        int start = static_cast<int>(SDL_roundf(p0.y));
        int end = static_cast<int>(SDL_roundf(p1.y));

        vec3 colourSlope = (w1 - w0)*dy;

        if(start>end){
            for (int y = end; y < start; y++)
            {
                minMaxPlot(static_cast<int>(p1.x),y,w1);
                p1.x += slope;
            
                w1 += colourSlope;
            }
        }else{
            for (int y = start; y < end; y++)
            {
                minMaxPlot(static_cast<int>(p0.x),y,w0);
                p0.x += slope;
            
                w0 += colourSlope;
            }
        } 
    }

    inline void triangle(vec3 p0, vec3 p1, vec3 p2) { 
        
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
        
        vec2 edgeVector1 = (vec2){p1.x - p0.x,p1.y - p0.y};
        vec2 edgeVector2 = (vec2){p2.x - p0.x,p2.y - p0.y};

        if(((edgeVector1.y*edgeVector2.x) - (edgeVector1.x*edgeVector2.y)) <= 0){
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
        if(maxY > bufferOffset -1) maxY = bufferOffset;

        drawLine({p0.x, p0.y}, {p1.x, p1.y}, {1,0,0}, {0,1,0});
        drawLine({p1.x, p1.y}, {p2.x, p2.y}, {0,1,0}, {0,0,1});
        drawLine({p2.x, p2.y}, {p0.x, p0.y}, {0,0,1}, {1,0,0});
        
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
            vec3 currentColour = triangleWeights[y];/* +1 i have no idea what this did before but when i removed it it fixed weird artifacts*/
            
            vec3 slope = (triangleWeights[y + bufferOffset] - triangleWeights[y]) / (triangleBuffer[y + bufferOffset] - triangleBuffer[y]);
            
            for (int x = std::max(triangleBuffer[y],0) + p; x < std::min(triangleBuffer[y + bufferOffset],WINDOW_WIDTH-1) + p; x++) {
                
                pixelZ = 1/((triangleWeights[y].x * invp0z) + (triangleWeights[y].y * invp1z) + (triangleWeights[y].z * invp2z));

                if (pixelDepthBuffer[x] > pixelZ || pixelDepthBuffer[x] == 0){
                    pixelDepthBuffer[x] = pixelZ;
                    
                    float U = ((triangleWeights[y].x * u0invp0z) + (triangleWeights[y].y * u1invp1z) + (triangleWeights[y].z * u2invp2z)) * pixelZ;
                    float V = ((triangleWeights[y].x * v0invp0z) + (triangleWeights[y].y * v1invp1z) + (triangleWeights[y].z * v2invp2z)) * pixelZ;

                    //shader start
                    
                    if(((static_cast<int>(V*10)-static_cast<int>(U*10)) & 1) == 0){
                        pixelBuffer[x] = 4294967295; //white (2^32)-1
                    }else{
                        pixelBuffer[x] = 255; //black (2^8)-1
                    }
                    
                    //shader end
                } 

                triangleWeights[y] += slope;
            }
            
            triangleBuffer[y] = -1;
            triangleBuffer[y+bufferOffset] = -1;
            p += WINDOW_WIDTH;
        }
    }
};

void count(int*array,int size){
    for(int i = 0; i< size; i++){
        *array=i;
        array ++;
    }
}

float avgFps = 0;
Uint8 frameCount = 0;

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failed to init SDL: %s", SDL_GetError());
        return 1;
    }

    Screen screen(1000, 1000);
    Camera camera(1000, 1000,120,1);

    std::cout << screen.error << "\n";

    bool running = true;
    SDL_Event event;

    int array[100];

    count(array, sizeof(array)/sizeof(int));
    
    std::cout << array[69] << "\n";

    int a;
    std::cout << a << "\n";

    while(running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }
        auto start = std::chrono::high_resolution_clock::now();

        camera.renderBuffer();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        avgFps = (avgFps * 0.99) + (10000/duration.count());
        
        //camera.convertDepthIntoGrayscaleAndDisplayTobuffer(0,10);

        screen.displayBuffer(camera.pixelBuffer);

        if((frameCount % 1000) == 0){
            SDL_Log("f:%d", static_cast<int>(avgFps));
        }

        frameCount ++;
    }
    SDL_Quit();
}