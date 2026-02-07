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

            a = Mat::multiplyMat3x3(a, mesh.transform.rotationMatrix) + mesh.transform.position;
            b = Mat::multiplyMat3x3(b, mesh.transform.rotationMatrix) + mesh.transform.position;
            c = Mat::multiplyMat3x3(c, mesh.transform.rotationMatrix) + mesh.transform.position;

            raster->drawTriangle(a,b,c);
        }
    }
}

void Camera::clearBuffers()
{
    raster->clearBuffers();
}