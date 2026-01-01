#include "Camera.h"

Camera::Camera(int width, int height,float fieldOfView, float scale){
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;

    halfWidth = width/2;
    halfHeight = height/2;
    
    fov = tanf(fieldOfView/2)*2;
    
    viewportScaleX = (halfWidth * scale) / fov;        
    viewportScaleY = -(halfHeight * scale) / fov;//flipped y axis (buffers are stored from top left pixels)

    amountOfPixels = width * height;
    bufferOffset = height;
    bufferSize = bufferOffset * 2;

    pixelBuffer = new pixel[amountOfPixels];

    pixelDepthBuffer = new float[amountOfPixels];   
}

Camera::~Camera(){
    delete[] pixelBuffer;
    delete[] pixelDepthBuffer;
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

const void Camera::projectVertex(Vec3& original,Vec3& transformed){
    transformed.x = (viewportScaleX * original.x / (original.z * fov)) + halfWidth;
    transformed.y = (viewportScaleY * original.y / (original.z * fov)) + halfHeight;
}

void Camera::rotateVector(Vec2& original,Vec2& tramsformed,Vec2 pivot,float sin,float cos){
    tramsformed.x = original.x-pivot.x;
    tramsformed.y = original.y-pivot.y;

    tramsformed.x = ((tramsformed.x *cos)-(tramsformed.y *sin))+pivot.x;
    tramsformed.y = ((tramsformed.x *sin)+(tramsformed.y *cos))+pivot.y;
}


