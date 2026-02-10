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

#include "Transform.h"

#include "Mesh.h"
#include "Mat.h"

#include "TriangleRaster.h"

class Camera {
public:

    Camera(int width, int height,float fieldOfView, const char* windowName);
    ~Camera();

    void renderMesh(Mesh& mesh);

    void clearBuffers();
    void displayBuffer();

    GLFWwindow* getWindow();

    Transform transform;

    Screen* screen = nullptr;
private:


    TriangleRaster* raster = nullptr;
};