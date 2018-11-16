#pragma once

#include <vector>
#include <string>
using namespace std;

class JSONVertex {
private:
		string key;
		float x;
		float y;
		float z;

public:
	JSONVertex();
	JSONVertex(string, float, float, float);
	~JSONVertex();
};