#pragma once

#include <iostream>

#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>
#include <math.h>
#include <algorithm>

#include "Vector.h"

#include "Screen.h"

#include "Mesh.h"

#include "TriangleRaster.h"

class Camera {
public:
    Screen screen;

    Camera(int width, int height,float fieldOfView, const char* windowName);
    ~Camera();

    void renderMesh(Mesh& mesh);

    void clearBuffers();

    void displayBuffer();

    void convertDepthIntoGrayscaleAndDisplayTobuffer(float highest, float lowest);

private:

    TriangleRaster raster;

    void rotateVector(Vec2& original, Vec2& tramsformed, Vec2 pivot, float sin, float cos);
};