#include <cstring>

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <algorithm>

#include <cmath>
#include <string>
#include <math.h>

#include "vector.h"
#include "Camera.h"
#include "Mesh.h"
#include "FrameTime.h"

int main(int argc, char* argv[]) {

    Camera camera(1000, 1000,120, "Cpu renderer");

    Mesh triangle;
    Components::loadMesh("../../../assets/cube.obj", triangle);

    while(!glfwWindowShouldClose(camera.getWindow())) {
        if (glfwGetKey(camera.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(camera.getWindow(), GLFW_TRUE);
        }

        camera.clearBuffers();

        camera.renderMesh(triangle);
        camera.displayBuffer();

        std::cout << Helpers::getFrameRate() << "\n";
    }
}