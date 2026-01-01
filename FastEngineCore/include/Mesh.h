#pragma once

#include "Vector.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <stdexcept>

#include <algorithm>

struct Triangle {
	int vertexA;
	int vertexB;
	int vertexC;
};

class Mesh {
public:

	~Mesh();

	Vec3* verticies = nullptr;
	Triangle* triangles = nullptr;

	int sizeVerts;
	int sizeTris;

};

namespace Components {
	Mesh loadMesh(char* path);
}