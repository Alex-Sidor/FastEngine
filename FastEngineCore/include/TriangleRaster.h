#pragma once

class TriangleRaster {
public:

    void drawTriangle(Vec3 p0, Vec3 p1, Vec3 p2);

private:

    inline float min3(float a, float b, float c);
    inline float max3(float a, float b, float c);

    inline float triangleArea(const Vec2& a, const Vec2& b, const Vec2& c);

    void drawPixel(float w1, float w2, float w3,
        int x, int y,
        float u0invp0z, float u1invp1z, float u2invp2z,
        float v0invp0z, float v1invp1z, float v2invp2z,
        float invp0z, float invp1z, float invp2z);
}