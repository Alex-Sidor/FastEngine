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

        Mat3x3 mat = Mat::createMatrixFromEuler(mesh.transform.eulerAngles);

        //transform
        for (int i = 0; i < mesh.sizeVerts; i++) {
            mesh.verticiesCache[i] = Mat::multiplyMat3x3(mesh.verticies[i], 
                mat) + mesh.transform.position;
        }

        //camera rotation
        mat = Mat::createMatrixFromEuler(transform.eulerAngles);

        for (int i = 0; i < mesh.sizeVerts; i++) {
            mesh.verticiesCache[i] = Mat::multiplyMat3x3(mesh.verticies[i] - transform.position,
                mat);
        }

        //project
        for (int i = 0; i < mesh.sizeVerts; i++) {
            raster->projectVertex(mesh.verticiesCache[i]);
        }

        for (int t = 0; t < mesh.sizeTris; t++) {
            
            Triangle currentTri = mesh.triangles[t];

            Vec3 a = mesh.verticiesCache[currentTri.vertexA];
            Vec3 b = mesh.verticiesCache[currentTri.vertexB];
            Vec3 c = mesh.verticiesCache[currentTri.vertexC];

            if (a.z <= 0 || b.z <= 0 || c.z <= 0) {
                //std::cout << "triangle behind viewport deleted\n";
                return;
            }

            raster->drawTriangle(a,b,c);
        }
    }
}

void Camera::clearBuffers()
{
    raster->clearBuffers();
}