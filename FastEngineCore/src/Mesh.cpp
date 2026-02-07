#include "Mesh.h"

Mesh::Mesh(Transform inputTransform)
{
	transform = inputTransform;
	transform.updateMatrix();
}

Mesh::~Mesh()
{
	clearData();
}

void Mesh::clearData()
{
	if (verticies) {
		delete[] verticies;
		verticies = nullptr;

		sizeVerts = 0;
	}

	if (triangles) {
		delete[] triangles;
		triangles = nullptr;

		sizeTris = 0;
	}
}

void Components::loadMesh(const char* path, Mesh& meshComponent)
{
	using namespace std;

	//accumulate the tris and verts while reading then put into a set size arr
	vector<Vec3> verticiesAcc;
	vector<Triangle> trianglesAcc;

	ifstream MeshFile(path);

	if (!MeshFile.is_open()) {
		printf("Could not open file: %s\n", path);
		throw std::runtime_error("Could not open file: " + std::string(path));
	}

	std::string line;
	while (std::getline(MeshFile, line)) {
		std::stringstream current(line);
		std::string type;

		if (!(current >> type)) continue;

		if (type == "v") {

			std::string a, b, c;

			current >> a >> b >> c;

			verticiesAcc.push_back(Vec3{ stof(a), stof(b), stof(c) });
		}
		else if (type == "f") {
			std::string a, b, c;

			current >> a >> b >> c;

			trianglesAcc.push_back(Triangle{ stoi(a) - 1, stoi(b) - 1, stoi(c) - 1 });
		}
	}
	MeshFile.close();

	Vec3* verticies = new Vec3[verticiesAcc.size()];
	Triangle* triangles = new Triangle[trianglesAcc.size()];

	copy(verticiesAcc.begin(), verticiesAcc.end(), verticies);
	copy(trianglesAcc.begin(), trianglesAcc.end(), triangles);

	meshComponent.clearData();

	meshComponent.verticies = verticies;
	meshComponent.triangles = triangles;
	meshComponent.sizeVerts = verticiesAcc.size();
	meshComponent.sizeTris = trianglesAcc.size();
}
