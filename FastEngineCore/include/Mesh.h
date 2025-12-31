#pragma once

#include "vector.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <stdexcept>

#include <algorithm>

struct triangle {
	int vertexA;
	int vertexB;
	int vertexC;
};

struct Mesh {
	vec3* verticies = nullptr;
	triangle* triangles = nullptr;

	int Nvets;
	int Ntris;
};

class MeshLoader {
public:

	object loadMesh(char* path);

	~MeshLoader();

	std::vector<object> LoadedMeshes;

private:
};