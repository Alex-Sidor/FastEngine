#include <cstring>

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>
#include <math.h>

#include "Screen.h"
#include "vector.h"
#include "Camera.h"


/*void readOutObjectFileContents() {
    std::ifstream objectFile("assets/cube.obj");

    std::string contents;

    while(getline (objectFile, contents)){
        char* fixedString;
        
        if(contents.starts_with("v")){
            SDL_Log("Vertex\n");
        }
        if(contents.starts_with("n")){
            SDL_Log("normal\n");
        }

        std::cout << contents[0];
    }

    objectFile.close();
}*/

void static readStuffAndWriteStuff(){
    using namespace std;
    
    ofstream MyWriteFile("assets/filename.txt");

    MyWriteFile << "file test";
    
    MyWriteFile.close();

    string myText;

    ifstream MyReadFile("assets/filename.txt");

    while (getline (MyReadFile, myText)) {
        cout << myText;
    }

    MyReadFile.close();
}

template <size_t N>
void count(int (&array)[N]) {
    for (size_t i = 0; i < N; ++i) {
        array[i] = i;
    }
}

float fps = 0;
int frameCount = 1;

int main(int argc, char* argv[]) {

    Screen screen(1000, 1000);
    Camera camera(1000, 1000,120,0.5);

    std::cout << screen.error << "\n";

    int array[100];

    count(array);
    
    std::cout << array[50] << "\n";

    //readStuffAndWriteStuff();

    //readOutObjectFileContents();

    while(!glfwWindowShouldClose(screen.window)) {
        if (glfwGetKey(screen.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(screen.window, GLFW_TRUE);
        }
        
        auto start = std::chrono::high_resolution_clock::now();

        camera.renderBuffer();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        fps = 1000000.0f / static_cast<float>(duration.count());
        
        //camera.convertDepthIntoGrayscaleAndDisplayTobuffer(0,7);

        screen.displayBuffer(camera.pixelBuffer);

        if((frameCount % 10) == 0){
            std::cout << fps << "\n";
        }

        frameCount++;
    }
}