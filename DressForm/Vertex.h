#pragma once

#include <math.h>
#include <vector>
#include <iostream>
using namespace std;

class Vertex {
public:
	Vertex();
	Vertex(float, float, float);
	Vertex(int, float, float, float);
	Vertex(int, float, float, float, vector<int>);
	Vertex(Vertex*);
	~Vertex();
	
	int idx;
	float x, y, z;

	bool isNull();
	void setIdx(int);
	void set();
	void set(float, float, float);
	void del();
	void print();

	bool operator==(const Vertex& v);
	bool operator!=(const Vertex& v);

	float distance(Vertex);
	Vertex closest(vector<Vertex>);
	Vertex closest(float, float, float, float);

	Vertex subtract(Vertex);
	Vertex add(Vertex);
	Vertex add(float);
	Vertex multiply(float);
	Vertex divide(float);
	Vertex cross(Vertex);
	Vertex dot(Vertex);

private:
	bool nullFlag;
};