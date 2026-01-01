#include <cstring>

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>
#include <math.h>

#include "Screen.h"
#include "vector.h"
#include "Camera.h"
#include "Mesh.h"

float fps = 0;
int frameCount = 1;

int main(int argc, char* argv[]) {

    Screen screen(1000, 1000,"Cpu renderer");
    Camera camera(1000, 1000,120,0.5);

    Mesh triangle = Engine::Mesh::loadMesh("../../../assets/cube.obj");

    while(!glfwWindowShouldClose(screen.window)) {
        if (glfwGetKey(screen.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(screen.window, GLFW_TRUE);
        }
        
        auto start = std::chrono::high_resolution_clock::now();

        camera.renderBuffer(triangle);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        fps = 1000000.0f / static_cast<float>(duration.count());
        
        //camera.convertDepthIntoGrayscaleAndDisplayTobuffer(0,7);

        screen.setTexture(camera.pixelBuffer);
        screen.updateScreen();

        if((frameCount % 10) == 0){
            std::cout << fps << "\n";
        }

        frameCount++;
    }
}