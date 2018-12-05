#pragma once

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>

#include "Vertex.h"
#include "Mesh.h"
#include "Body.h"
#include "LinkedList.h"
#include "GrahamScan.h"

#define NECKHIGH 0
#define NECKMID 1
#define NECKLOW 2
#define CLAVICLEL 3
#define CLAVICLER 4
#define SHOULDERL 5
#define SHOULDERR 6
#define WRISTL 7
#define WRISTR 8
#define BELLYBUTTON 9
#define PELVISMID 10
#define PELVISL 11
#define PELVISR 12
#define ANKLEL 13
#define ANKLER 14

#define ARMLESS 10
#define WITHARM 11
#define HEADLESS 12
#define WITHHEAD 13
#define FOOTLESS 14
#define WITHFOOT 15

#define HEAD 20
#define NECK 21
#define TORSO 22
#define ARMS 23
#define LEGS 24


using namespace std;

class ObjBody : public Body {
public:
	ObjBody();
	~ObjBody();

	float bust, waist, hip;
	int bustIdx, waistIdx, hipIdx;

	vector<Vertex> vertices;
	vector<Vertex> joints;
	vector<Mesh> mesh;

	LinkedList<vector<Vertex>>* layers;

	bool importVertices();
	bool importJoints();
	bool importFaces();

	void getIndex(vector<Vertex>);
	void SetSize(float, float, float);
	vector<Vertex> getDressForm();

	bool writeToObj();

private:
	Vertex nippleL, nippleR;
	vector<float> circs;

	void severBody(int);
	vector<Mesh> updateMesh(vector<int>, vector<Mesh>);
};
