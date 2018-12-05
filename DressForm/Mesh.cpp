#include "stdafx.h"
#include "Mesh.h"

Mesh::Mesh() {
	normal = Vertex(0, 0, 0);
}

Mesh::Mesh(Vertex _v1, Vertex _v2, Vertex _v3) {
	vertices.push_back(_v1);
	vertices.push_back(_v2);
	vertices.push_back(_v3);

	Vertex vec1 = _v1.subtract(_v2);
	Vertex vec2 = _v2.subtract(_v3);
	normal = vec1.cross(vec2);
}

Mesh::Mesh(vector<int> idxs, vector<Vertex> verts) {
	indices.insert(indices.end(), idxs.begin(), idxs.end());
	vertices.insert(vertices.end(), verts.begin(), verts.end());

	Vertex vec1 = vertices[0].subtract(vertices[1]);
	Vertex vec2 = vertices[1].subtract(vertices[2]);
	normal = vec1.cross(vec2);
}

Mesh::Mesh(int _i1, int _i2, int _i3, Vertex _v1, Vertex _v2, Vertex _v3) {
	indices.push_back(_i1);
	indices.push_back(_i2);
	indices.push_back(_i3);
	vertices.push_back(_v1);
	vertices.push_back(_v2);
	vertices.push_back(_v3);

	Vertex vec1 = _v1.subtract(_v2);
	Vertex vec2 = _v2.subtract(_v3);
	normal = vec1.cross(vec2);
}

Mesh::~Mesh() {

}

void Mesh::setMesh(Vertex _v1, Vertex _v2, Vertex _v3) {
	vertices.push_back(_v1);
	vertices.push_back(_v2);
	vertices.push_back(_v3);

	Vertex vec1 = _v1.subtract(_v2);
	Vertex vec2 = _v2.subtract(_v3);
	normal = vec1.cross(vec2);
}

void Mesh::setNormal(Vertex n) {
	normal = n;
}