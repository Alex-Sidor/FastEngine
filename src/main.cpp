#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cmath>

#include "Vector.h"
#include "Mat.h"
#include "Camera.h"
#include "Mesh.h"
#include "FrameTime.h"

int main(int argc, char* argv[]) {

    Camera camera(1000, 1000,120, "Cpu renderer");

    Mesh triangle;
    Components::loadMesh("../../../assets/cube.obj", triangle);

    float data[3][3] = {
        {cosf(0.1f), 0.0f, sinf(0.1f)},
        {0.0f, 1.0f, 0.0f},
        {-sinf(0.1f), 0.0f, cosf(0.1f)} };

    Mat3x3 rotationMatrix(data);

    while(!glfwWindowShouldClose(camera.getWindow())) {
        if (glfwGetKey(camera.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(camera.getWindow(), GLFW_TRUE);
        }

        for (int i = 0; i < triangle.sizeVerts; i++)
        {
            triangle.verticies[i] = Vector::multiplyMat3x3(triangle.verticies[i],rotationMatrix);
        }

        camera.clearBuffers();

        camera.renderMesh(triangle);
        camera.displayBuffer();

        std::cout << Helpers::getFrameRate() << "\n";
    }
}