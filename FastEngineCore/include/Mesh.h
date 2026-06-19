#pragma once

#include "Vector.h"
#include "Mat.h"
#include "Transform.h"

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
	
	Mesh(Transform inputTransform);
	~Mesh();

	void clearData();

	Vec3* verticies = nullptr;
	Triangle* triangles = nullptr;

	Vec3* verticiesCache = nullptr;

	size_t sizeVerts;
	size_t sizeTris;
	
	Transform transform;
};

namespace Components {
	void loadMesh(const char* path, Mesh& meshComponent);
}