#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Screen {
public:
    Screen(int x, int y);
    ~Screen();

    void displayBuffer(const uint32_t* buffer);

    std::string error;

private:
    int width;
    int height;

    GLFWwindow* window = nullptr;
    GLuint texture = 0;
};
