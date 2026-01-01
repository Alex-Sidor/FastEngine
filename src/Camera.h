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


class Camera {
public:
    int WINDOW_WIDTH,WINDOW_HEIGHT;

    pixel* pixelBuffer;

    float* pixelDepthBuffer;

    float angle = 0;
    float fov;

    Camera(int width, int height,float fieldOfView, float scale);

    ~Camera();

    void renderMesh(Mesh& mesh);

    void clearBuffers();

    void convertDepthIntoGrayscaleAndDisplayTobuffer(float highest, float lowest);

private:
    int halfWidth;
    int halfHeight;
    int amountOfPixels;
    int bufferOffset;
    int bufferSize;

    float viewportScaleX;
    float viewportScaleY;

    const void projectVertex(Vec3& original,Vec3& transformed);

    void rotateVector(Vec2& original, Vec2& tramsformed, Vec2 pivot, float sin, float cos);
};