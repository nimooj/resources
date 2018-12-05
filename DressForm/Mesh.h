#pragma once

#include <math.h>
#include <vector>

#include "Vertex.h"

using namespace std;

class Mesh {
public:
	Mesh();
	Mesh(Vertex, Vertex, Vertex);
	Mesh(vector<int>, vector<Vertex>);
	Mesh(int, int, int, Vertex, Vertex, Vertex);
	~Mesh();
	vector<int> indices;
	vector<Vertex> vertices;
	Vertex normal;

	void setMesh(Vertex, Vertex, Vertex);
	void setNormal(Vertex);
};
