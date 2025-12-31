#pragma once

#include <iostream>

#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>
#include <math.h>
#include <algorithm>

#include "vector.h"

#include "Screen.h"

#include "object.h"


class Camera {
public:
    int WINDOW_WIDTH,WINDOW_HEIGHT;

    pixel* pixelBuffer;

    float* pixelDepthBuffer;

    float angle = 0;
    float fov;

    Camera(int width, int height,float fieldOfView, float scale);

    ~Camera();

    void renderBuffer(objectLoader& objectHandler);
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

    void drawPixel(float w1, float w2, float w3,
        int x, int y,
        float u0invp0z, float u1invp1z, float u2invp2z, 
        float v0invp0z, float v1invp1z, float v2invp2z,
        float invp0z, float invp1z, float invp2z);

    inline float triangleArea(const Vec2& a, const Vec2& b, const Vec2& c);

    inline float min3(float a, float b, float c);

    inline float max3(float a, float b, float c);

    void drawTriangle(Vec3 p0, Vec3 p1, Vec3 p2);
};