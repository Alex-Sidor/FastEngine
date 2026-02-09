#include "TriangleRaster.h"

TriangleRaster::TriangleRaster(int width, int height, float fieldOfView)
{
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;

    halfWidth = width / 2;
    halfHeight = height / 2;

    fov = tanf(fieldOfView / 2) * 2;

    viewportScaleX = (halfWidth) / fov;
    viewportScaleY = -(halfHeight) / fov;//flipped y axis (buffers are stored from top left pixels)

    amountOfPixels = width * height;
    bufferOffset = height;
    bufferSize = bufferOffset * 2;

    pixelBuffer = new pixel[amountOfPixels];
    pixelDepthBuffer = new float[amountOfPixels];
}

void TriangleRaster::clearBuffers() {
    memset(pixelBuffer, 0, sizeof(pixel) * amountOfPixels);
    memset(pixelDepthBuffer, 0, sizeof(float) * amountOfPixels);
}

TriangleRaster::~TriangleRaster()
{
    if (pixelBuffer) {
        delete[] pixelBuffer;
    }
    if (pixelDepthBuffer) {
        delete[] pixelDepthBuffer;
    }
}

void TriangleRaster::drawPixel(float w1, float w2, float w3, int x, int y, float u0invp0z, float u1invp1z, float u2invp2z, float v0invp0z, float v1invp1z, float v2invp2z, float invp0z, float invp1z, float invp2z) {//temporary
    float pixelZ = 1 / ((w1 * invp0z) + (w2 * invp1z) + (w3 * invp2z));

    int p = x + (y * WINDOW_WIDTH);

    if (pixelDepthBuffer[p] > pixelZ || pixelDepthBuffer[p] == 0) {
        pixelDepthBuffer[p] = pixelZ;

        float U = ((w1 * u0invp0z) + (w2 * u1invp1z) + (w3 * u2invp2z)) * pixelZ;
        float V = ((w1 * v0invp0z) + (w2 * v1invp1z) + (w3 * v2invp2z)) * pixelZ;

        /*if (((static_cast<int>(V * 50) - static_cast<int>(U * 50)) & 1) == 0) {// uv checkerboard
            pixelBuffer[p] = { 34,177,76 }; // white
        }
        else {
            pixelBuffer[p] = { 177,30,94 };// black
        }*/

        float offset = (x*V + U + x + y)/100;

        Vec3 grad = Vec3{ 34,177,76 } - Vec3{ 177, 30, 94 };
        grad = Vec3{ offset * grad.x,offset * grad.y,offset * grad.z };
        grad += { 177, 30, 94 };

        pixelBuffer[p] = { static_cast<uint8_t>(grad.x),static_cast<uint8_t>(grad.y),static_cast<uint8_t>(grad.z) };
    }
}

void TriangleRaster::projectVertex(Vec3& vertex) {
    vertex.x = (viewportScaleX * vertex.x / vertex.z) + halfWidth;
    vertex.y = (viewportScaleY * vertex.y / vertex.z) + halfHeight;
}

float TriangleRaster::triangleArea(const Vec2& a, const Vec2& b, const Vec2& c) {
    return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

float TriangleRaster::min3(float a, float b, float c) {
    float m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

float TriangleRaster::max3(float a, float b, float c) {
    float m = a;
    if (b > m) m = b;
    if (c > m) m = c;
    return m;
}

void TriangleRaster::drawTriangle(Vec3 p0, Vec3 p1, Vec3 p2) {

    const float epsilon = 0;//-1e-6f; //compensate for floating point error

    float fullArea = triangleArea({ p0.x,p0.y }, { p1.x,p1.y }, { p2.x,p2.y });

    if (fullArea <= epsilon) {
        return;//dont draw the triangle if its backface
    }

    const float u0 = 0;
    const float u1 = 1;
    const float u2 = 0;

    const float v0 = 0;
    const float v1 = 0;
    const float v2 = 1; //temp uv mapping

    fullArea = 1 / fullArea;

    float u0invp0z = u0 / p0.z;
    float u1invp1z = u1 / p1.z;
    float u2invp2z = u2 / p2.z;

    float v0invp0z = v0 / p0.z;
    float v1invp1z = v1 / p1.z;
    float v2invp2z = v2 / p2.z;

    float invp0z = 1 / p0.z;
    float invp1z = 1 / p1.z;
    float invp2z = 1 / p2.z; // calculate inverses once and multiply to save on division clock cycles

    int minX = std::max(0, (int)min3(p0.x, p1.x, p2.x));
    int minY = std::max(0, (int)min3(p0.y, p1.y, p2.y));
    int maxX = std::min(WINDOW_WIDTH, (int)max3(p0.x, p1.x, p2.x) + 1);
    int maxY = std::min(WINDOW_HEIGHT, (int)max3(p0.y, p1.y, p2.y) + 1);

    for (int y = minY; y < maxY; y++) {
        for (int x = minX; x < maxX; x++) {
            float w1 = triangleArea({ p1.x,p1.y }, { p2.x,p2.y }, { (float)x,(float)y }) * fullArea;
            if (w1 < epsilon) continue;

            float w2 = triangleArea({ p2.x,p2.y }, { p0.x,p0.y }, { (float)x,(float)y }) * fullArea;
            if (w2 < epsilon) continue;

            float w3 = (1.0f - w1) - w2;
            if (w3 < epsilon) continue;

            drawPixel(w1, w2, w3, x, y, u0invp0z, u1invp1z, u2invp2z, v0invp0z, v1invp1z, v2invp2z, invp0z, invp1z, invp2z);
        }
    }
}