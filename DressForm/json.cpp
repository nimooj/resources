#include "stdafx.h"
#include "json.h"

JSONVertex::JSONVertex() {
	key = "";
	x = -1;
	y = -1;
	z = -1;
}

JSONVertex::JSONVertex(string _key, float _x, float _y, float _z) {
	key = _key;
	x = _x;
	y = _y;
	z = _z;
}

JSONVertex::~JSONVertex() {

}
