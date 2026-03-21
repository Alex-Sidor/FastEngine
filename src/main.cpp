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

    auto time = std::chrono::steady_clock::now();

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

        triangle.transform.eulerAngles += Vec3{ 1.0f,2.0f,0.5f} * dt;

        camera.renderMesh(triangle);

        camera.displayBuffer();

        
        float fps = Helpers::getFrameRate();

        dt = 1 / fps;

        auto now = std::chrono::steady_clock::now();

        std::chrono::duration<float> elapsed = now - time;

        if (elapsed.count() >= 0.5f) {
            int intFps = static_cast<int>(fps);
            std::string title = "FPS: " + std::to_string(intFps);
            glfwSetWindowTitle(camera.getWindow(), title.c_str());

            time = now;
        }
    }
}