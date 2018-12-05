#include "stdafx.h"
#include "Vertex.h"

Vertex::Vertex() {
	nullFlag = true;
	idx = 0;
	x = 0;
	y = 0;
	z = 0;
}

Vertex::Vertex(float _x, float _y, float _z) {
	nullFlag = false;
	idx = 0;
	x = _x;
	y = _y;
	z = _z;
}

Vertex::Vertex(int i, float _x, float _y, float _z) {
	nullFlag = false;
	idx = i;
	x = _x;
	y = _y;
	z = _z;
}

Vertex::Vertex(Vertex* v) {
	nullFlag = false;
	idx = v->idx;
	x = v->x;
	y = v->y;
	z = v->z;
}

Vertex::Vertex(int i, float _x, float _y, float _z, vector<int> _neighbors) {
	nullFlag = false;
	idx = i;
	x = _x;
	y = _y;
	z = _z;
}

Vertex::~Vertex() {
	nullFlag = true;
	idx = 0;
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
	idx = 0;
	x = 0;
	y = 0;
	z = 0;
}

bool Vertex::operator==(const Vertex& v) {
	if (x == v.x && y == v.y && z == v.z)
		return true;
	else
		return false;
}

bool Vertex::operator!=(const Vertex& v) {
	if (x == v.x && y == v.y && z == v.z)
		return false;
	else
		return true;
}

float Vertex::distance(Vertex v) {
	return sqrt(pow(x - v.x, 2) + pow(y - v.y, 2) + pow(z - v.z, 2));
}

Vertex Vertex::closest(vector<Vertex> v) {
	float min = 100;
	Vertex closestV;
	for (vector<Vertex>::iterator i = v.begin(); i != v.end(); i++) {
		Vertex v = (*i);
		float dist = distance(v);

		if (dist < min) {
			min = dist;
			closestV = v;
		}
	}
	return closestV;
}

Vertex Vertex::closest(float x1, float y1, float x2, float y2) {
	Vertex n;

	if (x1 == x2) {
		n = Vertex(x1, y, 0);
	}
	else if (y1 == y2) {
		n = Vertex(x, y1, 0);
	}
	else {
		float slope1 = (y1 - y2) / (x1 - x2);
		float intercept1 = y1 - slope1 * x1;

		float slope2 = -1/slope1;
		float intercept2 = y - slope2 * x;

		float nx = (intercept2 - intercept1) / (slope1 - slope2);
		float ny = (slope1 * intercept1 - slope2 * intercept2) / (slope1 - slope2);

		n = Vertex(nx, ny, 0);
	}

	return n;
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

Vertex Vertex::add(float i) {
	return Vertex(x + i, y + i, z + i);
}

Vertex Vertex::multiply(float n) {
	return Vertex(n * x, n * y, n * z);
}

Vertex Vertex::divide(float n) {
	return Vertex(x / n, y / n, z / n);
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
