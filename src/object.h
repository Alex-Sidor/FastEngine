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

struct object {
	vec3* verticies = nullptr;
	triangle* triangles = nullptr;

	int Nvets;
	int Ntris;
};

class objectLoader {
public:

	object loadObject(char* path);

	~objectLoader();

	std::vector<object> objectsToDestroy;

private:
};