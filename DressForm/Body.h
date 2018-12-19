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
#include <tuple>

//#include "json.h"
#include "Vertex.h"
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
#define LOWERBODY 16

#define HEAD 20
#define NECK 21
#define TORSO 22
#define ARMS 23
#define LEGS 24

#define DRESS 30
#define TOP 31
#define SKIRT 32
#define PANTS 33

using namespace std;

class Body {
public :
	Body();
	~Body();
	vector<Vertex> nipples;

	int bustIdx;
	int waistIdx;
	int hipIdx;
	int shoulderTopIdx;
	int shoulderMidIdx;
	int shoulderBottomIdx;

	float bust;
	float waist;
	float hip;

	Vertex thighMax;

	int GetLayerNum();
	LinkedList<vector<Vertex>>* layers;

	vector<Vertex> GetVertices();
	vector<Vertex> GetJoints();

	vector<Vertex> severBody(int); // separate parts from (whole) vertices

	vector<int> GetLayerIdx();

	float GetCircumference(vector<Vertex>);
	void GetLooseDressForm(vector<Vertex>);
	void GetTightDressForm(vector<Vertex>);

	void GetTop(vector<Vertex>);
	void GetSkirt(vector<Vertex>);
	void GetPants(vector<Vertex>);

	void SetSize(float, float, float);

	void WriteToOBJ(int);


private:
	float expand;
	float interval;
	vector<Vertex> vertices;
	vector<Vertex> joints;

	vector<float> circs;

	int layerNum;
	vector<int> layerIdx;

	void setInterval(float);

	bool importVertices();
	bool importJoints();

	vector<Vertex> generateUniformLayer(vector<Vertex>);
};