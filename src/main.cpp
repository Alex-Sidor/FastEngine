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
    }

    ~Camera(){
        delete[] pixelBuffer;
        delete[] pixelDepthBuffer;
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

    void drawPixel(float w1, float w2, float w3,int x, int y,float u0invp0z, float u1invp1z, float u2invp2z, float v0invp0z, float v1invp1z, float v2invp2z, float invp0z, float invp1z, float invp2z){//temporary
        float pixelZ = 1/((w1 * invp0z) + (w2 * invp1z) + (w3 * invp2z));

        int p = x + (y*WINDOW_WIDTH);

        if (pixelDepthBuffer[p] > pixelZ || pixelDepthBuffer[p] == 0){
            pixelDepthBuffer[p] = pixelZ;
            
            float U = ((w1 * u0invp0z) + (w2 * u1invp1z) + (w3 * u2invp2z)) * pixelZ;
            float V = ((w1 * v0invp0z) + (w2 * v1invp1z) + (w3 * v2invp2z)) * pixelZ;
            
            if(((static_cast<int>(V*10)-static_cast<int>(U*10)) & 1) == 0){
                pixelBuffer[p] = 4294967295; //white (2^32)-1
            }else{
                pixelBuffer[p] = 255; //black (2^8)-1
            }
        } 
    }


    inline float triangleArea(const vec2& a, const vec2& b, const vec2& c) {
        return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
    }

    inline float min3(float a, float b, float c) {
        float m = a;
        if (b < m) m = b;
        if (c < m) m = c;
        return m;
    }

    inline float max3(float a, float b, float c) {
        float m = a;
        if (b > m) m = b;
        if (c > m) m = c;
        return m;
    }

    void triangle(vec3 p0, vec3 p1, vec3 p2) { 
        
        const float epsilon = 0;//-1e-6f; //compensate for floating point error

        if(p0.z <= 0 || p1.z <= 0 || p2.z <= 0){
            std::cout << "triangle behind viewport deleted\n";
            return;
        }
        
        vec2 edgeVector1 = (vec2){p1.x - p0.x,p1.y - p0.y};
        vec2 edgeVector2 = (vec2){p2.x - p0.x,p2.y - p0.y};

        float fullArea = triangleArea({p0.x,p0.y},{p1.x,p1.y},{p2.x,p2.y});

        if(fullArea >= 0){
          return;//dont draw the triangle if its backface
        }

        const float u0 = 0;
        const float u1 = 1;
        const float u2 = 0;

        const float v0 = 0;
        const float v1 = 0;
        const float v2 = 1; //temp uv mapping
        
        fullArea = 1 / fullArea;

        float u0invp0z = u0/p0.z;
        float u1invp1z = u1/p1.z;
        float u2invp2z = u2/p2.z;
        
        float v0invp0z = v0/p0.z;
        float v1invp1z = v1/p1.z;
        float v2invp2z = v2/p2.z;
        
        float invp0z = 1/p0.z;
        float invp1z = 1/p1.z;
        float invp2z = 1/p2.z; // calculate inverses once and multiply to save on division clock cycles

        int minX = static_cast<int>(std::floor(min3(p0.x,p1.x,p2.x)));
        int minY = static_cast<int>(std::floor(min3(p0.y,p1.y,p2.y)));
        int maxX = static_cast<int>(std::ceil(max3(p0.x,p1.x,p2.x)));
        int maxY = static_cast<int>(std::ceil(max3(p0.y,p1.y,p2.y)));

        for(int y = minY; y <  maxY; y++){
            for(int x = minX; x < maxX; x++){
                float w1 = triangleArea({p1.x,p1.y},{p2.x,p2.y},{(float)x,(float)y})*fullArea;
                if(w1 < epsilon) continue;
                
                float w2 = triangleArea({p2.x,p2.y},{p0.x,p0.y},{(float)x,(float)y})*fullArea;
                if(w2 < epsilon) continue;
                
                float w3 = (1.0f - w1) - w2;
                if(w3 < epsilon) continue;
                
                //pixelBuffer[x+(WINDOW_WIDTH*y)] = 4294967295;

                drawPixel(w1,w2,w3,x,y,u0invp0z,u1invp1z,u2invp2z,v0invp0z,v1invp1z,v2invp2z,invp0z,invp1z,invp2z);
            }
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
    Camera camera(1000, 1000,120,0.5);

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