#include "stdafx.h"
#include "Vertex.h"

Vertex::Vertex() {
	nullFlag = true;
	x = 0;
	y = 0;
	z = 0;
}

Vertex::Vertex(float _x, float _y, float _z) {
	nullFlag = false;
	x = _x;
	y = _y;
	z = _z;
}

Vertex::Vertex(Vertex* v) {
	nullFlag = false;
	x = v->x;
	y = v->y;
	z = v->z;
}

Vertex::~Vertex() {
	nullFlag = true;
	x = 0;
	y = 0;
	z = 0;
}

bool Vertex::isNull() {
	return nullFlag;
}

void Vertex::set() {
	nullFlag = false;
}

void Vertex::set(float _x, float _y, float _z) {
	nullFlag = false;
	x = _x;
	y = _y;
	z = _z;
}

void Vertex::del() {
	nullFlag = true;
	x = 0;
	y = 0;
	z = 0;
}

float Vertex::distance(Vertex v) {
	return sqrt(pow(x - v.x, 2) + pow(y - v.y, 2) + pow(z - v.z, 2));
}

void Vertex::print() {
	cout << x << ", " << y << ", " << z << endl;
}

Vertex Vertex::subtract(Vertex v) {
	return Vertex(x - v.x, y - v.y, z - v.z);
}

Vertex Vertex::add(Vertex v) {
	return Vertex(x + v.x, y + v.y, z + v.z);
}

Vertex Vertex::multiply(int n) {
	return Vertex(n * x, n * y, n * z);
}

Vertex Vertex::cross(Vertex v) {
	float nx = y * v.z - z * v.y;
	float ny = z * v.x - x * v.z;
	float nz = x * v.y - y * v.x;
	return Vertex(nx, ny, nz);
}

Vertex Vertex::dot(Vertex v) {
	return Vertex(x * v.x, y * v.y, z * v.z);
}
