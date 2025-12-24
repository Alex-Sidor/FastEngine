#include "object.h"

object objectLoader::loadObject(char* path)
{
	using namespace std;

	//accumulate the tris and verts while reading then put into a set size arr
	vector<vec3> verticiesAcc;
	vector<triangle> trianglesAcc;

	ifstream objectFile(path);

	if (!objectFile.is_open()) {
		printf("Could not open file: %s\n", path);
		throw std::runtime_error("Could not open file: " + std::string(path));
	}


	std::string line;
	while (std::getline(objectFile, line)) {
		std::stringstream current(line);
		std::string type;

		if (!(current >> type)) continue;

		if (type == "v") {
			
			std::string a, b, c;

			current >> a >> b >> c;

			verticiesAcc.push_back(vec3{ stof(a), stof(b), stof(c) });
		}
		else if (type == "f") {
			std::string a, b, c;

			current >> a >> b >> c;

			trianglesAcc.push_back( triangle{ stoi(a)-1, stoi(b)-1, stoi(c)-1 });
		}
	}

	vec3* verticies = new vec3[verticiesAcc.size()];
	triangle* triangles = new triangle[trianglesAcc.size()];

	copy(verticiesAcc.begin(), verticiesAcc.end(), verticies);
	copy(trianglesAcc.begin(), trianglesAcc.end(), triangles);

	object current ={ verticies, triangles, verticiesAcc.size() ,trianglesAcc.size() };

	objectsToDestroy.push_back(current);
	objectFile.close();

	return current;
}

objectLoader::~objectLoader()
{
	for (int i = 0; i < objectsToDestroy.size(); i++)
	{
		delete[] (objectsToDestroy[i].verticies);
		delete[] (objectsToDestroy[i].triangles);
	}
}
