#pragma once

#include <iostream>

#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>
#include <math.h>

#include "Vector.h"

#include "Screen.h"

#include "Mesh.h"

#include "TriangleRaster.h"

class Camera {
public:

    Camera(int width, int height,float fieldOfView, const char* windowName);
    ~Camera();

    void renderMesh(Mesh& mesh);

    void clearBuffers();

    void displayBuffer();

    GLFWwindow* getWindow();

private:

    Screen* screen = nullptr;
    TriangleRaster* raster = nullptr;

    void rotateVector(Vec2& original, Vec2& tramsformed, Vec2 pivot, float sin, float cos);
};