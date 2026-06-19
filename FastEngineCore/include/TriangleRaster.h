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

class TriangleRaster {
public:
    
    TriangleRaster(int width, int height, float fieldOfView);
    ~TriangleRaster();

    void clearBuffers();

    int WINDOW_WIDTH, WINDOW_HEIGHT;

    pixel* pixelBuffer = nullptr;
    float* pixelDepthBuffer = nullptr;

    float angle = 0;
    float fov;

    void drawTriangle(Vec3 p0, Vec3 p1, Vec3 p2);

    void TriangleRaster::projectVertex(Vec3& vertex);

private:

    int halfWidth;
    int halfHeight;
    int amountOfPixels;
    int bufferSize;

    float viewportScaleX;
    float viewportScaleY;

    inline float min3(float a, float b, float c);
    inline float max3(float a, float b, float c);

    inline float triangleArea(const Vec2& a, const Vec2& b, const Vec2& c);

    inline float edgeFunction(const Vec3& a, const Vec3& b, const float x, const float y);

    void drawPixel(float w1, float w2, float w3,
        int p,
        float u0invp0z, float u1invp1z, float u2invp2z,
        float v0invp0z, float v1invp1z, float v2invp2z,
        float invp0z, float invp1z, float invp2z);
};