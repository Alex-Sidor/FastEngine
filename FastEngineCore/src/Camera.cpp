#include "Camera.h"

Camera::Camera(int width, int height,float fieldOfView, const char* windowName){
    raster = new TriangleRaster(width, height, fieldOfView);

    screen = new Screen(width, height, windowName);
}

Camera::~Camera() {
    if (!raster) {
        delete raster;
    }
    if (!screen) {
        delete screen;
    }
}

void Camera::displayBuffer()
{
    screen->setTexture(raster->pixelBuffer);

    screen->updateScreen();
}

GLFWwindow* Camera::getWindow()
{
    if (screen) {
        return screen->window;
    }
    return nullptr;
}

void Camera::renderMesh(Mesh& mesh){

    if (!(mesh.triangles && mesh.verticies)) {
        std::cout << "Empty mesh rendered\n";
        return;
    }

    if (raster) {
        for (int t = 0; t < mesh.sizeTris; t++) {
            
            Triangle currentTri = mesh.triangles[t];

            Vec3 a = mesh.verticies[currentTri.vertexA];
            Vec3 b = mesh.verticies[currentTri.vertexB];
            Vec3 c = mesh.verticies[currentTri.vertexC];

            raster->drawTriangle(a,b,c);
        }
    }
}

void Camera::clearBuffers()
{
    raster->clearBuffers();
}

void Camera::rotateVector(Vec2& original,Vec2& tramsformed,Vec2 pivot,float sin,float cos){
    tramsformed.x = original.x-pivot.x;
    tramsformed.y = original.y-pivot.y;

    tramsformed.x = ((tramsformed.x *cos)-(tramsformed.y *sin))+pivot.x;
    tramsformed.y = ((tramsformed.x *sin)+(tramsformed.y *cos))+pivot.y;
}


