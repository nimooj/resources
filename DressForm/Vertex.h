#pragma once

#include <math.h>
#include <iostream>
using namespace std;

class Vertex {
public:
	Vertex();
	Vertex(float, float, float);
	Vertex(Vertex*);
	~Vertex();
	
	float x, y, z;

	bool isNull();
	void set();
	void set(float, float, float);
	void del();
	void print();

	float distance(Vertex);

	Vertex subtract(Vertex);
	Vertex add(Vertex);
	Vertex multiply(int);
	Vertex cross(Vertex);
	Vertex dot(Vertex);

private:
	bool nullFlag;
};