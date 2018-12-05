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

#define HEAD 20
#define NECK 21
#define TORSO 22
#define ARMS 23
#define LEGS 24

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
	vector<Vertex> armholesL;
	vector<Vertex> armholesR;
	vector<int> armholeIdx;

	vector<Vertex> GetVertices();
	vector<Vertex> GetJoints();

	vector<Vertex> severBody(int); // separate parts from (whole) vertices
	vector<Vertex> severBody(int, vector<Vertex>); // separate parts from (whole) vertices

	vector<Vertex> alignY(vector<Vertex>);

	vector<int> GetLayerIdx();
	float GetCircumference(vector<Vertex>);

	void GetWholeConvexHull();
	void GetDressForm(vector<Vertex>);
	void GetCircularDressForm(vector<Vertex>);

	void GetIndex(vector<Vertex>);

	void WriteToOBJ();

	void SetSize(float, float, float);
	void SetDressSize(float);

private:
	float expand;
	float interval;
	vector<Vertex> vertices;
	vector<Vertex> joints;
	//vector<JSONVertex> jointGroup;


	Vertex nippleL;
	Vertex nippleR;

	vector<float> circs;

	vector<Vertex> armless;
	vector<Vertex> arms;

	int layerNum;
	vector<int> layerIdx;

	vector<int> armholeLIdx;
	vector<int> armholeRIdx;
	void getArmholeOutline();

	void setInterval(float);

	bool importVertices();
	bool importJoints();

	vector<Vertex> generateUniformLayer(vector<Vertex>);
	vector<Vertex> sweep(int, vector<Vertex>, vector<Vertex>);

	bool isArmhole(int);
	bool isArmhole(Vertex);
	vector<Vertex> alignArmholes(vector<Vertex>);
};