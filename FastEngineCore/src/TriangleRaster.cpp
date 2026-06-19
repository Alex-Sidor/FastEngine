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
    bufferSize = height * 2;

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

void TriangleRaster::drawPixel(float w1, float w2, float w3, int p, float u0invp0z, float u1invp1z, float u2invp2z, float v0invp0z, float v1invp1z, float v2invp2z, float invp0z, float invp1z, float invp2z) {//temporary
    float pixelZ = 1 / ((w1 * invp0z) + (w2 * invp1z) + (w3 * invp2z));

    if (pixelDepthBuffer[p] > pixelZ || pixelDepthBuffer[p] == 0) {
        pixelDepthBuffer[p] = pixelZ;

        float U = ((w1 * u0invp0z) + (w2 * u1invp1z) + (w3 * u2invp2z)) * pixelZ;
        float V = ((w1 * v0invp0z) + (w2 * v1invp1z) + (w3 * v2invp2z)) * pixelZ;

        /*if (((static_cast<int>(V * 5) - static_cast<int>(U * 5)) & 1) == 0) {// uv checkerboard
            pixelBuffer[p] = { 34,177,76 };
        }
        else {
            pixelBuffer[p] = { 177,30,94 };
        }*/


        float gridSize = 1.0f;
        float lineThickness = 0.05f;
        float gridX = sin(U * gridSize * 3.14159f);
        float gridY = sin(V * gridSize * 3.14159f);

        bool isGridLine = (abs(gridX) > (1.0f - lineThickness)) ||
            (abs(gridY) > (1.0f - lineThickness));

        float distToCenter = sqrtf((U - 0.5f) * (U - 0.5f) + (V - 0.5f) * (V - 0.5f));
        float wave = sin(distToCenter * 20.0f);

        Vec3 baseColor = { 20, 50, 100 };
        Vec3 glowColor = { 0, 255, 200 };

        Vec3 finalColor;
        if (isGridLine) {
            finalColor = glowColor;
        }
        else {
            float intensity = 0.2f + (wave * 0.1f);
            finalColor = { baseColor.x * intensity, baseColor.y * intensity, baseColor.z * intensity };
        }

        pixelBuffer[p] = {
            static_cast<uint8_t>(finalColor.x),
            static_cast<uint8_t>(finalColor.y),
            static_cast<uint8_t>(finalColor.z)
        };
    }
}

void TriangleRaster::projectVertex(Vec3& vertex) {
    vertex.x = (viewportScaleX * vertex.x / vertex.z) + halfWidth;
    vertex.y = (viewportScaleY * vertex.y / vertex.z) + halfHeight;
}

float TriangleRaster::triangleArea(const Vec2& a, const Vec2& b, const Vec2& c) {
    return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

inline float TriangleRaster::edgeFunction(const Vec3& a, const Vec3& b, const float x, const float y)
{
    return (x - a.x) * (b.y - a.y) - (y - a.y) * (b.x - a.x);
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

    float area = edgeFunction(p0, p1, p2.x, p2.y);

    if (area >= epsilon) {
        return;//dont draw the triangle if its backface
    }

    area = 1.0f / area; //inverse it so you can multiply by it (faster)

    int minX = std::max(0, (int)min3(p0.x, p1.x, p2.x));
    int minY = std::max(0, (int)min3(p0.y, p1.y, p2.y));
    int maxX = std::min(WINDOW_WIDTH, (int)max3(p0.x, p1.x, p2.x) + 1);
    int maxY = std::min(WINDOW_HEIGHT, (int)max3(p0.y, p1.y, p2.y) + 1);


    const float u0 = 0;
    const float u1 = 1;
    const float u2 = 0;

    const float v0 = 0;
    const float v1 = 0;
    const float v2 = 1; //temp uv mapping

    float u0invp0z = u0 / p0.z;
    float u1invp1z = u1 / p1.z;
    float u2invp2z = u2 / p2.z;

    float v0invp0z = v0 / p0.z;
    float v1invp1z = v1 / p1.z;
    float v2invp2z = v2 / p2.z;

    float invp0z = 1 / p0.z;
    float invp1z = 1 / p1.z;
    float invp2z = 1 / p2.z; // calculate inverses once and multiply to save on division clock cycles


    //calculate weight deltas
    float w0dx = p2.y - p1.y;
    float w1dx = p0.y - p2.y;
    float w2dx = p1.y - p0.y;

    float w0dy = p1.x - p2.x;
    float w1dy = p2.x - p0.x;
    float w2dy = p0.x - p1.x;

    //find starting weights
    float w0Start = edgeFunction(p1, p2, (float)minX, (float)minY);
    float w1Start = edgeFunction(p2, p0, (float)minX, (float)minY);
    float w2Start = edgeFunction(p0, p1, (float)minX, (float)minY);

    int layer = (minY * WINDOW_WIDTH) + minX;

    for (int y = minY; y < maxY; y++) {
        
        float w0 = w0Start;
        float w1 = w1Start;
        float w2 = w2Start;
        
        int p = layer;

        for (int x = minX; x < maxX; x++) {
            if (w0 <= 0 && w1 <= 0 && w2 <= 0) {
                
                float c0 = w0 * area;
                float c1 = w1 * area;
                float c2 = w2 * area;
                
                drawPixel(c0, c1, c2, p, u0invp0z, u1invp1z, u2invp2z, v0invp0z, v1invp1z, v2invp2z, invp0z, invp1z, invp2z);
            }

            w0 += w0dx;
            w1 += w1dx;
            w2 += w2dx;

            p++;
        }

        w0Start += w0dy;
        w1Start += w1dy;
        w2Start += w2dy;

        layer += WINDOW_WIDTH;
    }
}