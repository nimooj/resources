#include "stdafx.h"

#include "Body.h"

Body::Body() {
	layerNum = 0;
	interval = 0;
	expand = 1.01;
}

Body::~Body() {
}

void Body::setInterval(float in) {
	interval = in;
}

bool Body::importVertices() {
	ifstream f;
	string s;
	f.open("verts/defaultvertices.txt");

	if (f.is_open()) {
		while (getline(f, s)) {
			istringstream iss(s);
			Vertex v;
			iss >> v.x >> v.y >> v.z;
			v.set();

			v.x = (int)(v.x * 1000) / 1000.0f;
			v.y = (int)(v.y * 1000) / 1000.0f;
			v.z = (int)(v.z * 1000) / 1000.0f;
			
			// push only x >= 0.
			if (v.x >= 0) {
				vertices.push_back(v);
			}
		}
	}
	else
		return false;

	f.close();

	// mirror by y axis.
	int l = vertices.size();
	for (int i = 0; i < l; i++) {
		float x = -vertices[i].x;
		float y = vertices[i].y;
		float z = vertices[i].z;

		vertices.push_back(Vertex(x, y, z));
	}

	// Sort by descending order
	sort(vertices.begin(), vertices.end(), [](Vertex &a, Vertex &b) { return a.y > b.y; });

	setInterval(0.4); // MIN 0.18; IDEAL 0.35
	//separateSections(vertices, interval);

	return true;
}

bool Body::importJoints() {
	ifstream f;
	string s;
	f.open("verts/defaultjoints.txt");

	if (f.is_open()) {
		while (getline(f, s)) {
			istringstream iss(s);
			Vertex v;
			iss >> v.x >> v.y >> v.z;
			v.set();
			joints.push_back(v);
		}
	}
	else return false;

	// Sort by descending order
	sort(joints.begin(), joints.end(), [](Vertex &a, Vertex &b) { return a.y > b.y; });

	f.close();

	return true;
}

void Body::separateSections(vector<Vertex> v, float _interval) {
	interval = _interval;

	// Apex as a topmost layer
	float h = v[0].y - interval;
	layerNum = 2;
	layerIdx.push_back(0);
	layerIdx.push_back(1);

	int count = 0;
	int vs = v.size();

	for (int i = 1; i < vs; i++) {
		//if (v[i].y >= h - interval && v[i].y < h + interval) {
		if ( v[i].y >= h - interval) {
			v[i].y = h; // flatten DOWN y-val.
			count++;
		}
		else {
			h = v[i].y;
			// Save the starting idx of each layers.
			layerNum += 1;
			layerIdx.push_back(i);
			count = 0;
		}
	}
}

vector<Vertex> Body::severBody(int process) {
	vector<Vertex> result;

	switch (process) {
		case ARMLESS :
			if (vertices.size() == 0 || joints.size() == 0) break;
		
			for (vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); it++) {
				float x, y, z;
				x = (*it).x;
				y = (*it).y;
				z = (*it).z;

				if (x >= joints[SHOULDERR].x && x <= joints[SHOULDERL].x) { // Only for non-obese type for now.
					result.push_back(Vertex(x, y, z));
				}
			}

			break;

		case WITHARM : 
			break;
		
		case HEADLESS:
			break;

		case WITHHEAD :
			break;

		case HEAD :
			break;
			
		case NECK :
			break;

		case TORSO :
			if (vertices.size() == 0 || joints.size() == 0) break;
		
			for (vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); it++) {
				float x, y, z;
				x = (*it).x;
				y = (*it).y;
				z = (*it).z;

				if ((x >= joints[SHOULDERR].x && x <= joints[SHOULDERL].x) && (y < joints[NECKMID].y - 0.05 && y > joints[PELVISL].y - interval * 1.1)) { // Only for non-obese type for now.
					result.push_back(Vertex(x, y, z));
				}
			}

			break;

		case ARMS :
			break;

		case LEGS :
			break;
		
		default :
			break;

		}

	return result;
}

vector<Vertex> Body::severBody(int process, vector<Vertex> v) {
	vector<Vertex> result;

	switch (process) {
		case ARMLESS :
			if (v.size() == 0 || joints.size() == 0) break;
		
			for (vector<Vertex>::iterator it = v.begin(); it != v.end(); it++) {
				float x, y, z;
				x = (*it).x;
				y = (*it).y;
				z = (*it).z;

				if (x >= joints[SHOULDERR].x && x <= joints[SHOULDERL].x) { // Only for non-obese type for now.
					result.push_back(Vertex(x, y, z));
				}
			}

			break;

		case WITHARM : 
			break;
		
		case HEADLESS:
			if (v.size() == 0 || joints.size() == 0) break;
		
			for (vector<Vertex>::iterator it = v.begin(); it != v.end(); it++) {
				float x, y, z;
				x = (*it).x;
				y = (*it).y;
				z = (*it).z;

				if (y <= joints[NECKMID].y) {
					result.push_back(Vertex(x, y, z));
				}
			}

			break;

		case WITHHEAD :
			break;

		case FOOTLESS:
			if (v.size() == 0 || joints.size() == 0) break;
		
			for (vector<Vertex>::iterator it = v.begin(); it != v.end(); it++) {
				float x, y, z;
				x = (*it).x;
				y = (*it).y;
				z = (*it).z;

				if (y >= joints[ANKLEL].y) {
					result.push_back(Vertex(x, y, z));
				}
			}
			break;

		case WITHFOOT: 
			break;

		default :
			break;

		}

	return result;
}

vector<Vertex> Body::GetVertices() {
	importVertices();
	return vertices;
}

vector<Vertex> Body::GetJoints() {
	importJoints();
	return joints;
}

int Body::GetLayerNum() {
	return layerNum;
}

vector<int> Body::GetLayerIdx() {
	return layerIdx;
}

float Body::GetCircumference(vector<Vertex> v) {
	float circum = 0;
	for (vector<Vertex>::iterator it = v.begin(); it != v.end() - 1; it++) {
		Vertex now = *it;
		Vertex next = *(it + 1);
		Vertex nn = now.subtract(next);

		circum += sqrt(pow(nn.x, 2) + pow(nn.y, 2) + pow(nn.z, 2));
	}
	Vertex last = v[v.size() - 1];
	Vertex first = v[0];
	Vertex nn = first.subtract(last);
	circum += sqrt(pow(nn.x, 2) + pow(nn.y, 2) + pow(nn.z, 2));

	return circum * 10;
}

void Body::GetWholeConvexHull() {
	layers = new LinkedList<vector<Vertex>>();

	// Generate convex hull for each layers but the topmost one
	for (int i = 1; i < layerNum - 1; i++) {
		vector<Vertex> layerVerts;

		// push vertices in layer
		for (int j = layerIdx[i]; j < layerIdx[i + 1]; j++) {
			layerVerts.push_back(vertices[j]);
		}

		// Draw convex hull
		GrahamScan g = GrahamScan(layerVerts);
		vector<Vertex> convexedLayer(g.GenerateConvexHull());
		layers->append(convexedLayer);
	}
}


void Body::SetDressSize(float e) {
	expand = e;
}

void Body::GetDressForm(vector<Vertex> v) {
	// Get layers from already flattened v
	vector<Vertex> layerVerts;

	int idx = 0;
	int tl = v[0].y * 100;
	float l = tl/100.0f - interval;

	layers = new LinkedList<vector<Vertex>>();
	int count = 0;

	for (vector<Vertex>::iterator it = v.begin(); it != v.end(); it++ ) {
		float x = (*it).x;
		float y = (*it).y;
		float z = (*it).z;

		if (y >= l) { // Add verts to the layer
			layerVerts.push_back(Vertex(x, l, z));
			count++;
		}
		else { // Generate layer
			if (count > 0) { // When more than one vertex in the layer
				GrahamScan g = GrahamScan(layerVerts);
				vector<Vertex> convexedLayer(g.GenerateConvexHull());

				vector<Vertex> ellipse = sweep(idx, convexedLayer);
				vector<Vertex> subEllipse;

				float currCirc = GetCircumference(ellipse);

				if (circs.size() > 0) {
					// Generate sub layer for smoothness
					int div = 2;

					for (int i = div; i > 0; i--) {
						for (int j = 0; j < ellipse.size(); j++) {
							float xDiff = (layers->curr->value[j].x - ellipse[j].x) * i/div;
							float zDiff = (layers->curr->value[j].z - ellipse[j].z) * i/div;
							subEllipse.push_back(Vertex(ellipse[j].x + xDiff, ellipse[j].y + interval * i / div, ellipse[j].z + zDiff));
						}

						layers->append(subEllipse);
						circs.push_back(GetCircumference(subEllipse));

						subEllipse.clear();
					}
				}

				circs.push_back(currCirc); // Do not save sub layers circumference
				float lastIdx = 0;
				if (circs.size() > 1) {
					lastIdx = circs.size() - 2;
				}

				if (l > joints[BELLYBUTTON].y && currCirc > circs[lastIdx]) {
					bustIdx = circs.size() - 1;
				}

				else if (l <= joints[CLAVICLEL].y && l > joints[PELVISMID].y && currCirc < circs[lastIdx] ) {
					waistIdx = circs.size() - 1;
				}

				layers->append(ellipse);
			}

			layerVerts.clear();
			l -= interval;
			count = 0;
		}
		idx++;
	}

	for (int i = 0; i < circs.size(); i++) {
		if (i < circs.size() / 2) {
		}
	}

	hipIdx = circs.size() - 1;

	cout << bustIdx << ", " << waistIdx << ", " << hipIdx << endl;
	cout << circs.size() << endl;

	/* Puncture Armhole */
	//getArmholeOutline();

	//layers->curr = layers->head;
	//for (int i = 0; i < layers->length; i++) {
		//for (int j = 0; j < layers->curr->value.size(); j++) {

		//}
		//layers->curr = layers->curr->next;
	//}

	/* *** */

	/* Expand dress */
	while (l >= joints[ANKLEL].y - interval) {
		// Get last layer
		vector<Vertex> lastLayer = layers->curr->value;
		vector<Vertex> dress;

		for (int j = 0; j < lastLayer.size(); j++) {
			dress.push_back(Vertex(lastLayer[j].x *expand, l, lastLayer[j].z * expand));
		}
		layers->append(dress);

		l -= interval;
	}
	/* *** */

}

void Body::ExpandDress() {
}

bool Body::isArmhole(int idx) {
	for (int i = 0; i < armholeIdx.size(); i++) {
		if (idx == armholeIdx[i])
			return true;
	}
	return false;
}

bool Body::isArmhole(Vertex v) {
	if (abs(v.x - joints[SHOULDERR].x) < 0.1 && v.y > joints[BELLYBUTTON].y) {
		if (v.x <= 0)
			armholesR.push_back(Vertex(v.x, v.y, v.z));
		else
			armholesL.push_back(Vertex(v.x, v.y, v.z));
		return true;
	}
	else if (abs(v.x - joints[SHOULDERL].x) < 0.1 && v.y > joints[BELLYBUTTON].y) {
		if (v.x <= 0)
			armholesR.push_back(Vertex(v.x, v.y, v.z));
		else
			armholesL.push_back(Vertex(v.x, v.y, v.z));
		return true;
	}
	return false;
}

vector<Vertex> Body::alignArmholes(vector<Vertex> v) {
	float firstX = v[0].x;
	float yVal = v[0].y;
	if (v[0].x > 0) armholeLIdx.push_back(0);
	else armholeRIdx.push_back(0);
	vector<Vertex> r;

	for (int i = 0; i < v.size(); i++) {
		if (yVal != v[i].y) {
			yVal = v[i].y;
			if (v[i].x > 0) 
				armholeLIdx.push_back(i);
			else 
				armholeRIdx.push_back(i);
		}

		r.push_back(Vertex(firstX, v[i].y, v[i].z));
	}

	if (v[v.size() - 1].x > 0) armholeLIdx.push_back(v.size() - 1);
	else armholeRIdx.push_back(v.size() - 1);
	return r;
}

void Body::getArmholeOutline() {

	/***********************
	* 1. Align xs' of the armholes.
	* 2. Rotate armholes along with the z-axis (90 degrees).
	* 3. Get outlines of the armholes.
	* 4. Rotate back with z-axis (-90 degrees).
	* 5. Replace armholesL, armholesR to outlineAL, outlineAR.
	*************************/
	vector<Vertex> nalignArmholesL = alignArmholes(armholesL);
	vector<Vertex> nalignArmholesR = alignArmholes(armholesR);
	vector<Vertex> outlineAL;
	vector<Vertex> outlineAR;

	Vertex minZL = Vertex(0, 0, 10);
	Vertex maxZL = Vertex(0, 0, -10);
	Vertex minZR = Vertex(0, 0, 10);
	Vertex maxZR = Vertex(0, 0, -10);
	float yLVal = nalignArmholesL[0].y;
	float yRVal = nalignArmholesR[0].y;
	// Identical sizes (armholesL && armholesR)
	for (int i = 0; i < nalignArmholesL.size(); i++) {
		// LEFT
		if (i >= armholeLIdx[0] && i < armholeLIdx[1]) {
			outlineAL.push_back(nalignArmholesL[i]);
			yLVal = nalignArmholesL[armholeLIdx[1]].y;
		}
		else if (i >= armholeLIdx[armholeLIdx.size() - 2] && i <= armholeLIdx[armholeLIdx.size() - 1]) {
			outlineAL.push_back(minZL);
			outlineAL.push_back(maxZL);
			outlineAL.push_back(nalignArmholesL[i]);
		}
		else {
			if (yLVal != nalignArmholesL[i].y) {
				yLVal = nalignArmholesL[i].y;
				outlineAL.push_back(minZL);
				outlineAL.push_back(maxZL);

				minZL.set(0, 0, 10);
				maxZL.set(0, 0, -10);
			}

			if (nalignArmholesL[i].z <= minZL.z) {
				minZL = nalignArmholesL[i];
			}
			if (nalignArmholesL[i].z > maxZL.z) {
				maxZL = nalignArmholesL[i];
			}
		}

		// RIGHT
		if (i >= armholeRIdx[0] && i < armholeRIdx[1]) {
			outlineAR.push_back(nalignArmholesR[i]);
			yRVal = nalignArmholesR[armholeRIdx[1]].y;
		}
		else if (i >= armholeRIdx[armholeRIdx.size() - 2] && i <= armholeRIdx[armholeRIdx.size() - 1]) {
			outlineAR.push_back(minZR);
			outlineAR.push_back(maxZR);
			outlineAR.push_back(nalignArmholesR[i]);
		}
		else {
			if (yRVal != nalignArmholesR[i].y) {
				yRVal = nalignArmholesR[i].y;
				outlineAR.push_back(minZR);
				outlineAR.push_back(maxZR);

				minZR.set(0, 0, 10);
				maxZR.set(0, 0, -10);
			}

			if (nalignArmholesR[i].z <= minZR.z) {
				minZR = nalignArmholesR[i];
			}
			if (nalignArmholesR[i].z > maxZR.z) {
				maxZR = nalignArmholesR[i];
			}
		}
	}
	armholesL.clear();
	armholesR.clear();
	armholesL.insert(armholesL.begin(), outlineAL.begin(), outlineAL.end());
	armholesR.insert(armholesR.begin(), outlineAR.begin(), outlineAR.end());
}

vector<Vertex> Body::sweep(int idx, vector<Vertex> v) {
	vector<Vertex> c; 
	float yVal = v[0].y;
	Vertex minX = Vertex(), maxX = Vertex(), minZ = Vertex(), maxZ = Vertex();

	for (int i = 0; i < v.size(); i++) {
		if (v[i].x < minX.x) {
			minX = v[i];
		}

		if (v[i].x > maxX.x) {
			maxX = v[i];
		}

		if (v[i].z < minZ.z) {
			minZ = v[i];
		}

		if (v[i].z > maxZ.z) {
			maxZ = v[i];
		}
	}

	Vertex center = Vertex((minX.x + maxX.x)/2, minX.y, (minZ.z + maxZ.z)/2);

	Vertex v1 = maxX.subtract(center);
	Vertex v2 = minZ.subtract(center);

	if (v1.z != 0) v1.z = 0.0;
	if (v2.x != 0) v2.x = 0.0;

	float includedAngle = acos((v1.x*v2.x + v1.z*v2.z)/(sqrt(pow(v1.x, 2) + pow(v1.z, 2))*(sqrt(pow(v2.x, 2) + pow(v2.z, 2))))) * 180/M_PI; // degrees
	if (includedAngle > 90) includedAngle = 90;

	float a = sqrt(pow(v1.x, 2) + pow(v1.z, 2));
	float b = sqrt(pow(v2.x, 2) + pow(v2.z, 2));

	int slice = 16;

	// Quad 4
	for (float degree = 0; degree < includedAngle + includedAngle/slice; degree += includedAngle/slice, idx++) {
		float radian = (degree* M_PI) / 180;

		float nx = a * cos(radian) + center.x;
		float ny = yVal;
		float nz = -b * sin(radian) + center.z;

		if (nx > maxX.x) nx = maxX.x;
		if (nz < minZ.z) nz = minZ.z;

		Vertex newV = Vertex(nx, ny, nz);
		isArmhole(newV);
		c.push_back(newV);
	}
	int csize = c.size();

	// Quad 3	
	for (int i = csize - 2; i >= 0; i--, idx++) {
		Vertex newV = Vertex(-c[i].x, c[i].y, c[i].z);
		isArmhole(newV);
		c.push_back(newV);
	}

	// Quad 2
	v1 = minX.subtract(center);
	v2 = maxZ.subtract(center);
	if (v1.z != 0) v1.z = 0.0;
	if (v2.x != 0) v2.x = 0.0;
	includedAngle = acos((v1.x*v2.x + v1.z*v2.z)/(sqrt(pow(v1.x, 2) + pow(v1.z, 2))*(sqrt(pow(v2.x, 2) + pow(v2.z, 2))))) * 180/M_PI; // degrees
	if (includedAngle > 90) includedAngle = 90;
	a = sqrt(pow(v1.x, 2) + pow(v1.z, 2));
	b = sqrt(pow(v2.x, 2) + pow(v2.z, 2));

	vector<Vertex> c2;

	for (float degree = 0; degree < includedAngle + includedAngle/slice; degree += includedAngle/slice, idx++) {
		float radian = (degree* M_PI) / 180;

		float nx = -a * cos(radian) + center.x;
		float ny = yVal;
		float nz = b * sin(radian) + center.z;

		if (nx < minX.x) nx = minX.x;
		if (nz > maxZ.z) nz = maxZ.z;

		Vertex newV = Vertex(nx, ny, nz);
		isArmhole(newV);
		c2.push_back(newV);
	}
	int c2size = c2.size();

	// Quad 1
	for (int i = c2size - 2; i >= 0; i--, idx++) {
		Vertex newV = Vertex(-c2[i].x, c2[i].y, c2[i].z);
		isArmhole(newV);
		c2.push_back(newV);
	}

	c.insert(c.end(), c2.begin(), c2.end());
	cout << c.size() << endl;

	return c;
}

void Body::SetSize(float b, float w, float h) {
	// TODO :: Scale joint positions

	// bust : 8, waist : 16, hip : 23
	float bscale = b / circs[bustIdx];
	float wscale = w / circs[waistIdx];
	float hscale = h / circs[hipIdx];

	circs.clear();

	// Neck to bust
	layers->curr = layers->head;
	for (int i = 0; i < bustIdx + 1; i++) {
		for (int j = 0; j < layers->curr->value.size(); j++) {
			layers->curr->value[j].x *= bscale;
			layers->curr->value[j].z *= bscale;
		}
		circs.push_back(GetCircumference(layers->curr->value));
		layers->curr = layers->curr->next;
	}

	// Bust to waist
	for (int i = bustIdx + 1; i < waistIdx; i++) {
		for (int j = 0; j < layers->curr->value.size(); j++) {
			layers->curr->value[j].x *= (bscale * (waistIdx - i) / (waistIdx - 1 - bustIdx) + wscale * (i - (bustIdx + 1)) / (waistIdx - 1 - bustIdx));
			layers->curr->value[j].z *= (bscale* (waistIdx - i)/(waistIdx - 1 - bustIdx) + wscale * (i - (bustIdx + 1))/(waistIdx - 1 - bustIdx));
		}
		circs.push_back(GetCircumference(layers->curr->value));
		layers->curr = layers->curr->next;
	}

	// Waist
	for (int j = 0; j < layers->curr->value.size(); j++)
	{
			layers->curr->value[j].x *= wscale;
			layers->curr->value[j].z *= wscale;
	}
	circs.push_back(GetCircumference(layers->curr->value));
	layers->curr = layers->curr->next;

	// Waist to hip
	for (int i = waistIdx + 1; i < hipIdx; i++) {
		for (int j = 0; j < layers->curr->value.size(); j++) {
			layers->curr->value[j].x *= (wscale *(hipIdx - 1 - i)/(hipIdx - 1 - waistIdx) + hscale * (i - (waistIdx + 1))/(hipIdx - 1 - waistIdx));
			layers->curr->value[j].z *= (wscale *(hipIdx - 1 - i)/(hipIdx - 1 - waistIdx) + hscale * (i - (waistIdx + 1))/(hipIdx - 1- waistIdx));
		}
		circs.push_back(GetCircumference(layers->curr->value));
		layers->curr = layers->curr->next;
	}

	// Hip to dress bottom
	for (int i = hipIdx; i < layers->length;  i++) {
		for (int j = 0; j < layers->curr->value.size(); j++) {
			layers->curr->value[j].x *= hscale;
			layers->curr->value[j].z *= hscale;
		}
		circs.push_back(GetCircumference(layers->curr->value));
		layers->curr = layers->curr->next;
	}

	bust = circs[bustIdx];
	waist = circs[waistIdx];
	hip = circs[hipIdx];
}

void Body::WriteToOBJ() {
	if (layers->length > 0) {
		ofstream outfile("body.obj");
		vector<Vertex> normals;

		outfile << "# Generated OBJ file\n# By Minjoo Kang" << endl << endl << endl;

		int idx = 0;
		layers->curr = layers->head;
		for (int i = 0; i < layers->length; i++) {
			for (int j = 0; j < layers->curr->value.size(); j++, idx++) {
				Vertex v = layers->curr->value[j];

				outfile << "v " << v.x << " " << v.y << " " << v.z << endl;
				if (isArmhole(v))
					armholeIdx.push_back(idx);
			}
			layers->curr = layers->curr->next;
		}

		layers->curr = layers->head;

		// Add 1 for vertex index
		for (int i = 0; i < layers->length - 1; i++) {
			int lsize = layers->curr->value.size();
			for (int j = 0; j < lsize - 1; j++) {
				int first = lsize * i + j;
				int second = lsize * i + (j + 1);
				int third = lsize * (i + 1) + (j + 1);
				int forth = lsize * (i + 1) + j;

				/*
				if (isArmhole(first) && isArmhole(second) && isArmhole(third) && isArmhole(forth)) {
					//continue;
				}
				else if (!isArmhole(first) && !isArmhole(second) && !isArmhole(third) && isArmhole(forth)) { // Top-right
					outfile << "f " << first + 1 << " " << second + 1<< " " << third + 1<< " " << endl;
				}
				else if (!isArmhole(first) && !isArmhole(second) && isArmhole(third) && !isArmhole(forth)) { // Top-left
					outfile << "f " << first + 1 << " " << second + 1<< " " << forth + 1<< endl;
				}
				else if (isArmhole(first) && !isArmhole(second) && !isArmhole(third) && !isArmhole(forth)) { // Bottom-right
					outfile << "f " << second + 1 << " "  << third + 1<< " " << forth + 1<< endl;
				}
				else if (!isArmhole(first) && isArmhole(second) && !isArmhole(third) && !isArmhole(forth)) { // Bottom-left
					outfile << "f " << first + 1 << " "  << third + 1<< " " << forth + 1<< endl;
				}
				else if (!isArmhole(first) && isArmhole(second) && isArmhole(third) && !isArmhole(forth)) { // Left edge
					//continue;
				}
				else if (isArmhole(first) && !isArmhole(second) && !isArmhole(third) && isArmhole(forth)) { // Right edge
					//continue;
				}
				//else if (!isArmhole(first) && !isArmhole(second) && !isArmhole(third) && !isArmhole(forth)) {
				else if (!isArmhole(first) && !isArmhole(second) && isArmhole(third) && isArmhole(forth)) { // Top edge
					 
				}
				else if (!isArmhole(first) && !isArmhole(second) && !isArmhole(third) && !isArmhole(forth)) {
					outfile << "f " << first + 1 << " " << second + 1<< " " << third + 1<< " " << forth + 1<< endl;
				}
				*/

				//  TODO: HANDLE ARMHOLE
				//outfile << "f " << first + 1 << " " << second + 1<< " " << third + 1<< " " << forth + 1<< endl;
				outfile << "f " << first + 1 << " " << second + 1<< " " << third + 1 << endl;
				outfile << "f " << first + 1 << " " << third + 1<< " " << forth + 1 << endl;
			}

			//outfile << "f " << lsize * (i + 1) - 1 + 1 << " " << lsize*i + 1 << " " << lsize * (i + 1) + 1  << " " << lsize * (i+2) - 1 + 1 << endl;
			outfile << "f " << lsize * (i + 1) - 1 + 1 << " " << lsize*i + 1 << " " << lsize * (i + 1) + 1  << endl;
			outfile << "f " << lsize * (i + 1) - 1 + 1 << " " <<  lsize * (i + 1) + 1 << " " << lsize * (i + 2) - 1  + 1 << endl;
			layers->curr = layers->curr->next;
		}

		outfile.close();
		cout << "obj write ended";
	}
}
