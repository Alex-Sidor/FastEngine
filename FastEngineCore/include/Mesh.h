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
	Mesh();
	~Mesh();

	void clearData();

	Vec3* verticies;
	Triangle* triangles;

	int sizeVerts;
	int sizeTris;

};

namespace Components {
	void loadMesh(const char* path, Mesh& meshComponent);
}