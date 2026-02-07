#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cmath>

#include "Vector.h"
#include "Mat.h"
#include "Camera.h"
#include "Mesh.h"
#include "FrameTime.h"
#include "Transform.h"

int main(int argc, char* argv[]) {

    Camera camera(1000, 1000, 120, "Cpu renderer");

    Transform spawn;
    spawn.eulerAngles = Vec3{ 0,0,0 };
    spawn.position = Vec3{ 0,0,3 };
    spawn.scale = Vec3{ 1,1,1 };
    spawn.updateMatrix();

    Mesh triangle(spawn);
    Components::loadMesh("../../../assets/suzanne.obj", triangle);

    while(!glfwWindowShouldClose(camera.getWindow())) {
        
        if (glfwGetKey(camera.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(camera.getWindow(), GLFW_TRUE);
        }

        camera.clearBuffers();

        triangle.transform.eulerAngles += Vec3{ 0.01f,0.02f,0.005f};
        triangle.transform.updateMatrix();

        camera.renderMesh(triangle);

        camera.displayBuffer();

        std::cout << Helpers::getFrameRate() << "\n";
    }
}