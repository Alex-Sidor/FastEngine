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
#include "Player.h"

float dt = 0;

int main(int argc, char* argv[]) {

    Camera camera(1000, 1000, 120, "Cpu renderer");

    Player mainPlayer(&camera);

    Transform spawn;
    spawn.position = Vec3{ 0,0,3 };

    Mesh triangle(spawn);
    Components::loadMesh("../../../assets/suzanne.obj", triangle);

    while(!glfwWindowShouldClose(camera.getWindow())) {
        
        if (glfwGetKey(camera.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(camera.getWindow(), GLFW_TRUE);
        }

        mainPlayer.update(camera.screen, dt);

        camera.clearBuffers();

        //triangle.transform.eulerAngles += Vec3{ 0.01f,0.02f,0.005f};

        camera.renderMesh(triangle);

        camera.displayBuffer();

        
        float fps = Helpers::getFrameRate();
        dt = 1 / fps;
        std::cout << fps << "\n";
    }
}