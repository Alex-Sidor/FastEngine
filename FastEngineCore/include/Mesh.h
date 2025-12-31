#pragma once

#include "vector.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <stdexcept>

#include <algorithm>

class Mesh {
public:

	~Mesh();

	vec3* verticies = nullptr;
	triangle* triangles = nullptr;

	int sizeVerts;
	int sizeTris;

};

namespace Engine::Mesh {
	void loadMesh(char* path);
}