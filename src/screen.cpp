#include "screen.h"
#include <iostream>

Screen::Screen(int x, int y) : width(x), height(y) {
    if (!glfwInit()) {
        error = "Failed to initialize GLFW";
        throw std::runtime_error(error);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);


    window = glfwCreateWindow(width, height, "FastEngine", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

Screen::~Screen() {
    if (texture) glDeleteTextures(1, &texture);
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

void Screen::displayBuffer(const uint32_t* buffer) {
    if (glfwWindowShouldClose(window)) return;

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
        GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer);

    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f((float)width, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f((float)width, (float)height);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, (float)height);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glfwSwapBuffers(window);
    glfwPollEvents();
}