#include "Camera.h"

Camera::Camera(int width, int height,float fieldOfView, float scale){

}

void Camera::renderMesh(Mesh& mesh){

    for (int t = 0; t < mesh.sizeTris; t++) {
            
        Triangle currentTri = mesh[t];

        Vec3 a = mesh.verticies[currentTri.vertexA];
        Vec3 b = mesh.verticies[currentTri.vertexB];
        Vec3 c = mesh.verticies[currentTri.vertexC];

        drawTriangle(a,b,c);
    }
}

void Camera::clearBuffers()
{
    for (int i = 0; i < amountOfPixels; i++)
    {
        pixelBuffer[i] = { 0, 0, 0 };
        pixelDepthBuffer[i] = 0;
    }
}

void Camera::convertDepthIntoGrayscaleAndDisplayTobuffer(float highest, float lowest){ 
    for(int i = 0;i < amountOfPixels; i++){
        uint8_t singleChannelColour = 255 * (lowest - pixelDepthBuffer[i]) / highest-lowest;

        pixelBuffer[i] = { singleChannelColour,singleChannelColour,singleChannelColour };
    }
}

void Camera::rotateVector(Vec2& original,Vec2& tramsformed,Vec2 pivot,float sin,float cos){
    tramsformed.x = original.x-pivot.x;
    tramsformed.y = original.y-pivot.y;

    tramsformed.x = ((tramsformed.x *cos)-(tramsformed.y *sin))+pivot.x;
    tramsformed.y = ((tramsformed.x *sin)+(tramsformed.y *cos))+pivot.y;
}


