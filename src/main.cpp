#include <cstring>

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>
#include <math.h>

#include "vector.h"
#include "Camera.h"
#include "Mesh.h"

float fps = 0;
int frameCount = 1;

int main(int argc, char* argv[]) {

    Camera camera(1000, 1000,120, "Cpu renderer");

    Mesh triangle;
    Components::loadMesh("../../../assets/cube.obj", triangle);

    while(!glfwWindowShouldClose(camera.getWindow())) {
        if (glfwGetKey(camera.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(camera.getWindow(), GLFW_TRUE);
        }
        
        auto start = std::chrono::high_resolution_clock::now();

        camera.clearBuffers();

        camera.renderMesh(triangle);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        fps = 1000000.0f / static_cast<float>(duration.count());

        camera.displayBuffer();

        if((frameCount % 10) == 0){
            std::cout << fps << "\n";
        }

        frameCount++;
    }
}