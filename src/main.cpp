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

    float rotation = 0.1f;

    printf("%f %f\n", sinf(rotation), cosf(rotation));

    float data[3][3] = {
        {cosf(rotation),    -sinf(rotation),    0.0f},
        {sinf(rotation),    cosf(rotation),     0.0f},
        {0.0f,              0.0f,               1.0f} };

    Mat3x3 rotationMatrix(data);

    Vec3 testvec = { 1,1,0 };

    while(!glfwWindowShouldClose(camera.getWindow())) {
        
        testvec = Mat::multiplyMat3x3(testvec, rotationMatrix);

        printf("%f %f %f ", testvec.x, testvec.y, testvec.z);
        
        if (glfwGetKey(camera.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(camera.getWindow(), GLFW_TRUE);
        }

        for (int i = 0; i < triangle.sizeVerts; i++)
        {
            triangle.verticies[i] = Mat::multiplyMat3x3(triangle.verticies[i],rotationMatrix);
        }

        camera.clearBuffers();

        camera.renderMesh(triangle);
        camera.displayBuffer();

        std::cout << Helpers::getFrameRate() << "\n";
    }
}