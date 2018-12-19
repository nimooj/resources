#include "stdafx.h"

#include "Body.h"

Body::Body() {
	layerNum = 0;
	interval = 0;
	expand = 1.02;

	bust = 0;
	waist = 0;
	hip = 0;
	bustIdx = -1;
	waistIdx = -1;
	hipIdx = -1;
}

Body::~Body() {
}

void Body::setInterval(float in) {
	interval = in;
}

bool Body::importVertices() {
	ifstream f;
	string s;
	f.open("makehuman/Result/vertices.txt");

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

	/*
	Vertex left = Vertex(), right = Vertex();
	f.open("makehuman/Result/nipples.txt");
	if (f.is_open()) {
		while (getline(f, s)) {
			istringstream iss(s);
			Vertex n;
			iss >> n.x >> n.y >> n.z;
			n.set();

			n.x = (int)(n.x * 1000) / 1000.0f;
			n.y = (int)(n.y * 1000) / 1000.0f;
			n.z = (int)(n.z * 1000) / 1000.0f;
			
			nipples.push_back(n);
			if (n.x > 0) {
				if (n.z > left.z) {
					left = n;
				}
			}
			else if (n.x < 0) {
				if (n.z > right.z) {
					right = n;
				}
			}
		}
	}
	sort(nipples.begin(), nipples.end(), [](Vertex &a, Vertex&b) { return a.z > b.z;  });
	nippleL = left;
	nippleR = right;
	f.close();
	*/

	// Sort by descending order
	sort(vertices.begin(), vertices.end(), [](Vertex &a, Vertex &b) { return a.y > b.y; });

	setInterval(0.6); // MIN 0.18; IDEAL 0.35

	return true;
}

bool Body::importJoints() {
	ifstream f;
	string s;
	f.open("makehuman/Result/joints.txt");

	vector<string> jnames;
	vector<Vertex> tmpjoints;

	if (f.is_open()) {
		while (getline(f, s)) {
			istringstream iss(s);
			string jname;
			Vertex v;
			iss >> jname >> v.x >> v.y >> v.z;
			v.set();

			v.x = (int)(v.x * 1000) / 1000.0f;
			v.y = (int)(v.y * 1000) / 1000.0f;
			v.z = (int)(v.z * 1000) / 1000.0f;

			jnames.push_back(jname);
			tmpjoints.push_back(v);
		}
	}
	else return false;

	// Sort by descending order

	//joints.reserve(tmpjoints.size());
	joints.resize(tmpjoints.size());

	for (int i = 0; i < jnames.size(); i++) {
		string name = jnames[i];
		if (name == "NECKHIGH")
			joints[NECKHIGH] = tmpjoints[i];

		if (name == "NECKMID")
			joints[NECKMID] = tmpjoints[i];

		if (name == "NECKLOW")
			joints[NECKLOW] = tmpjoints[i];

		if (name == "CLAVICLEL")
			joints[CLAVICLEL] = tmpjoints[i];

		if (name == "CLAVICLER")
			joints[CLAVICLER] = tmpjoints[i];

		if (name == "SHOULDERL")
			joints[SHOULDERL] = tmpjoints[i];

		if (name == "SHOULDERR")
			joints[SHOULDERR] = tmpjoints[i];

		if (name == "WRISTL")
			joints[WRISTL] = tmpjoints[i];

		if (name == "WRISTR")
			joints[WRISTR] = tmpjoints[i];

		if (name == "BELLYBUTTON")
			joints[BELLYBUTTON] = tmpjoints[i];

		if (name == "PELVISMID")
			joints[PELVISMID] = tmpjoints[i];

		if (name == "PELVISL")
			joints[PELVISL] = tmpjoints[i];

		if (name == "PELVISR")
			joints[PELVISR] = tmpjoints[i];

		if (name == "ANKLEL")
			joints[ANKLEL] = tmpjoints[i];

		if (name == "ANKLER")
			joints[ANKLER] = tmpjoints[i];
	}

	f.close();

	//sort(joints.begin(), joints.end(), [](Vertex &a, Vertex &b) { if (a.y == b.y) return a.x > b.x; else return a.y > b.y; });
	return true;
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

		case TORSO: 
			if (vertices.size() == 0 || joints.size() == 0) break;
			thighMax = Vertex();
			thighMax.z = -5;

			for (vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); it++) {
				float x, y, z;
				x = (*it).x;
				y = (*it).y;
				z = (*it).z;
				float scale = 1.065; 
				float callibrateY = 0.11;
				float callibrateZ = 0.03;

				if (y < joints[NECKMID].y && y > joints[BELLYBUTTON].y && x >= joints[SHOULDERR].x && x <= joints[SHOULDERL].x) {
					result.push_back(Vertex(x*scale, y + callibrateY, (z + callibrateZ)*scale));
					//result.push_back(Vertex(x, y, z));
				}
				//if (y <= joints[BELLYBUTTON].y && y > joints[PELVISL].y - 0.5 && x >= (joints[WRISTR].x + joints[SHOULDERR].x) / 2 && x <= (joints[WRISTL].x + joints[SHOULDERL].x) / 2) {
				if (y <= joints[BELLYBUTTON].y && y > (joints[PELVISL].y + joints[ANKLEL].y)/2 && x >= (joints[WRISTR].x + joints[SHOULDERR].x) / 2 && x <= (joints[WRISTL].x + joints[SHOULDERL].x) / 2) {
					result.push_back(Vertex(x*scale, y + callibrateY, (z + callibrateZ)*scale));
					//result.push_back(Vertex(x, y, z));
				}

				if (y <= joints[PELVISL].y) {
					if (z > thighMax.z)
						thighMax = Vertex(x, y, z);
				}
			}
			break;

		case FOOTLESS :
			if (vertices.size() == 0 || joints.size() == 0) break;
			thighMax = Vertex();
			thighMax.z = -5;

			for (vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); it++) {
				float x, y, z;
				x = (*it).x;
				y = (*it).y;
				z = (*it).z;
				float scale = 1.065; 
				float callibrateY = 0.11;
				float callibrateZ = 0.03;

				if (y < joints[NECKMID].y && y > joints[BELLYBUTTON].y && x >= joints[SHOULDERR].x && x <= joints[SHOULDERL].x) {
					result.push_back(Vertex(x*scale, y + callibrateY, (z + callibrateZ)*scale));
					//result.push_back(Vertex(x, y, z));
				}
				//if (y <= joints[BELLYBUTTON].y && y > joints[PELVISL].y - 0.5 && x >= (joints[WRISTR].x + joints[SHOULDERR].x) / 2 && x <= (joints[WRISTL].x + joints[SHOULDERL].x) / 2) {
				if (y <= joints[BELLYBUTTON].y && y > joints[ANKLEL].y && x >= (joints[WRISTR].x + joints[SHOULDERR].x) / 2 && x <= (joints[WRISTL].x + joints[SHOULDERL].x) / 2) {
					result.push_back(Vertex(x*scale, y + callibrateY, (z + callibrateZ)*scale));
					//result.push_back(Vertex(x, y, z));
				}

				if (y <= joints[PELVISL].y) {
					if (z > thighMax.z)
						thighMax = Vertex(x, y, z);
				}
			}
			break;

		case LOWERBODY :
			if (vertices.size() == 0 || joints.size() == 0) break;
			thighMax = Vertex();
			thighMax.z = -5;

			for (vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); it++) {
				float x, y, z;
				x = (*it).x;
				y = (*it).y;
				z = (*it).z;
				float scale = 1.065; 
				float callibrateY = 0.11;
				float callibrateZ = 0.03;

				if (y <= joints[BELLYBUTTON].y && y > joints[ANKLEL].y && x >= (joints[WRISTR].x + joints[SHOULDERR].x) / 2 && x <= (joints[WRISTL].x + joints[SHOULDERL].x) / 2) {
					result.push_back(Vertex(x*scale, y + callibrateY, (z + callibrateZ)*scale));
				}
			}
			break;

		case LEGS :
			if (vertices.size() == 0 || joints.size() == 0) break;

			for (vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); it++) {
				float x, y, z;
				x = (*it).x;
				y = (*it).y;
				z = (*it).z;
				float scale = 1.065; 
				float callibrateY = 0.11;
				float callibrateZ = 0.03;

				if (y <= joints[BELLYBUTTON].y && y > joints[ANKLEL].y && x >= (joints[WRISTR].x + joints[SHOULDERR].x) / 2 && x <= (joints[WRISTL].x + joints[SHOULDERL].x) / 2) {
					result.push_back(Vertex(x*scale, y + callibrateY, (z + callibrateZ)*scale));
				}
			}
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

void Body::GetLooseDressForm(vector<Vertex> v) {
	int idx = 0;
	float l = v[0].y;
	float diff = 0.15;
	float bustHeight = 0;
	float hipHeight = 0;
	vector<Vertex> layerVerts;
	vector<Vertex> bustVecs, skirt;
	layers = new LinkedList<vector<Vertex>>();

	float bustLv = (joints[NECKMID].y + joints[BELLYBUTTON].y) / 2;
	float shoulderTopLv = (joints[SHOULDERL].y + joints[NECKMID].y) / 2;
	float shoulderBottomLv = (joints[SHOULDERL].y + (joints[NECKMID].y + joints[BELLYBUTTON].y) / 2) / 2;
	float shoulderMidLv = (shoulderTopLv + shoulderBottomLv) / 2;
	int shoulderTopMidIdx = -1, shoulderMidBottomIdx = -1, shoulderBottomBustIdx = -1;
	
	for (vector<Vertex>::iterator it = v.begin(); it != v.end(); it++) {
		float x = (*it).x;
		float y = (*it).y;
		float z = (*it).z;
		
		if (y >= l - diff && y < l + diff) {
			if (bustIdx == -1 && abs(l - bustLv) < diff) {
				bustIdx = idx;
			}
			if (abs(l - shoulderTopLv) < diff) {
				shoulderTopIdx = idx;
			}
			if (abs(l - (shoulderTopLv + shoulderMidLv)/2) < diff) {
				shoulderTopMidIdx = idx;
			}
			if (abs(l - shoulderMidLv) < diff) {
				shoulderMidIdx = idx;
			}
			if (abs(l - (shoulderBottomLv + shoulderMidLv)/2) < diff) {
				shoulderMidBottomIdx = idx;
			}
			if (abs(l - shoulderBottomLv) < diff) {
				shoulderBottomIdx = idx;
			}
			if (abs(l - (shoulderBottomLv + bustLv)/2) < diff) {
				shoulderBottomBustIdx = idx;
			}
			if (waistIdx == -1 && abs(l - (joints[PELVISMID].y + joints[CLAVICLEL].y)/2) < diff) {
				waistIdx = idx;
			}
			if (hipIdx == -1 && (abs(l - joints[PELVISMID].y) <= diff || l <= joints[PELVISMID].y + diff)) {
				hipIdx = idx;
			}
			layerVerts.push_back(Vertex(x, l, z));
		}
		else {
			vector<Vertex> ellipse;
			if (idx == 0) {
				/*
				*/
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			if (idx == shoulderTopIdx) {
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			if (idx == shoulderTopMidIdx) {
				/*
				*/
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			if (idx == shoulderMidIdx) {
				/*
				*/
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			if (idx == shoulderMidBottomIdx) {
				/*
				*/
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			if (idx == shoulderBottomIdx) {
				/*
				*/
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			if (idx == shoulderBottomBustIdx) {
				/*
				*/
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			if (idx == bustIdx) {
				/*
				*/
				ellipse = generateUniformLayer(layerVerts);
				bustHeight = ellipse[0].y;
				bustVecs = ellipse;
				bust = GetCircumference(ellipse);
				circs.push_back(bust);
				layers->append(ellipse);
			}
			if (idx == waistIdx) {
				/*
				*/
				ellipse = generateUniformLayer(layerVerts);
				waist = GetCircumference(ellipse);
				circs.push_back(waist);
			}
			if (idx == hipIdx) {
				/*
				*/
				ellipse = generateUniformLayer(layerVerts);
				hipHeight = ellipse[0].y;
				hip = GetCircumference(ellipse);
				circs.push_back(hip);
			}

			if (hipIdx != -1 && idx >= hipIdx) {
				skirt.insert(skirt.end(), layerVerts.begin(), layerVerts.end());
			}

			idx++;
			l -= diff * 2;
			layerVerts.clear();
		}
	}

	/*
	*/
	for (int i = 0; i < skirt.size(); i++) {
		skirt[i].y = hipHeight - diff * 2;
	}
	vector<Vertex> hipL = generateUniformLayer(skirt);
	vector<Vertex> hipVecs;
	hipVecs.insert(hipVecs.end(), hipL.begin(), hipL.end());

	l = bustHeight - diff * 2;

	vector<Vertex> subEllipse;
	int div = (bustHeight - hipHeight) / (diff * 2);
	for (int i = div; i >= 0; i--) {
		for (int j = 0; j < bustVecs.size(); j++) {
			float xDiff = (hipVecs[j].x - bustVecs[j].x) * i / div;
			float zDiff = (hipVecs[j].z - bustVecs[j].z) * i / div;
			subEllipse.push_back(Vertex(hipVecs[j].x - xDiff, l, hipVecs[j].z - zDiff));
		}
		layers->append(subEllipse);
		l -= diff * 2;
		subEllipse.clear();
	}

	/*
	for (int i = 0; i < skirt.size(); i++) {
		skirt[i].y = hipHeight - diff * 2;
	}
	*/

	vector<Vertex> ellipse = generateUniformLayer(skirt);
	layers->append(ellipse);

	l = hipHeight - diff * 4;

	expand = 1.01;
	vector<Vertex> tail;
	while (l >= joints[ANKLEL].y - diff) {
		vector<Vertex> lastLayer = layers->curr->value;

		for (int i = 0; i < lastLayer.size(); i++) {
			tail.push_back(Vertex(lastLayer[i].x * expand, l, lastLayer[i].z * expand));
		}

		layers->append(tail);
		l -= diff * 2;
		tail.clear();
	}

	// Auto Export
	WriteToOBJ(DRESS);
}

void Body::GetTightDressForm(vector<Vertex> v) {
	int idx = 0;
	float l = v[0].y;
	float diff = 0.15;
	float bustHeight = 0;
	float hipHeight = 0;
	vector<Vertex> layerVerts;
	layers = new LinkedList<vector<Vertex>>();

	float bustLv = (joints[NECKMID].y + joints[BELLYBUTTON].y) / 2;
	float shoulderTopLv = (joints[SHOULDERL].y + joints[NECKMID].y) / 2;
	float shoulderBottomLv = (joints[SHOULDERL].y + (joints[NECKMID].y + joints[BELLYBUTTON].y) / 2) / 2;
	float shoulderMidLv = (shoulderTopLv + shoulderBottomLv) / 2;
	int shoulderTopMidIdx = -1, shoulderMidBottomIdx = -1, shoulderBottomBustIdx = -1;
	
	for (vector<Vertex>::iterator it = v.begin(); it != v.end(); it++) {
		float x = (*it).x;
		float y = (*it).y;
		float z = (*it).z;
		
		if (y >= l - diff && y < l + diff) {
			if (bustIdx == -1 && abs(l - bustLv) < diff) {
				bustIdx = idx;
			}
			/*
			if (abs(l - shoulderTopLv) < diff) {
				shoulderTopIdx = idx;
			}
			if (abs(l - (shoulderTopLv + shoulderMidLv)/2) < diff) {
				shoulderTopMidIdx = idx;
			}
			if (abs(l - shoulderMidLv) < diff) {
				shoulderMidIdx = idx;
			}
			if (abs(l - (shoulderBottomLv + shoulderMidLv)/2) < diff) {
				shoulderMidBottomIdx = idx;
			}
			if (abs(l - shoulderBottomLv) < diff) {
				shoulderBottomIdx = idx;
			}
			if (abs(l - (shoulderBottomLv + bustLv)/2) < diff) {
				shoulderBottomBustIdx = idx;
			}
			*/
			if (waistIdx == -1 && abs(l - (joints[PELVISMID].y + joints[CLAVICLEL].y)/2) < diff) {
				waistIdx = idx;
			}
			if (hipIdx == -1 && (abs(l - joints[PELVISMID].y) <= diff || l <= joints[PELVISMID].y + diff)) {
				hipIdx = idx;
			}
			layerVerts.push_back(Vertex(x, l, z));
		}
		else {
			vector<Vertex> ellipse;
			if (idx == 0) {
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			/*
			if (idx == shoulderTopIdx) {
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			if (idx == shoulderTopMidIdx) {
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			if (idx == shoulderMidIdx) {
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			if (idx == shoulderMidBottomIdx) {
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			if (idx == shoulderBottomIdx) {
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			if (idx == shoulderBottomBustIdx) {
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			*/
			if (idx == bustIdx) {
				ellipse = generateUniformLayer(layerVerts);
				bustHeight = ellipse[0].y;
				bust = GetCircumference(ellipse);
				circs.push_back(bust);
				layers->append(ellipse);
			}
			else if (idx == waistIdx) {
				ellipse = generateUniformLayer(layerVerts);
				waist = GetCircumference(ellipse);
				circs.push_back(waist);
				layers->append(ellipse);
			}
			else if (idx == hipIdx) {
				ellipse = generateUniformLayer(layerVerts);
				hipHeight = ellipse[0].y;
				hip = GetCircumference(ellipse);
				circs.push_back(hip);
				layers->append(ellipse);
			}
			else {
				ellipse = generateUniformLayer(layerVerts);
				if (ellipse.size() == 36)
					layers->append(ellipse);
			}

			idx++;
			l -= diff * 2;
			layerVerts.clear();
		}
	}

	// Auto Export
	WriteToOBJ(DRESS);
}

void Body::GetTop(vector<Vertex> v) {
	int idx = 0;
	float l = v[0].y;
	float diff = 0.15;
	float bustHeight = 0;
	float hipHeight = 0;
	vector<Vertex> layerVerts;
	layers = new LinkedList<vector<Vertex>>();

	float bustLv = (joints[NECKMID].y + joints[BELLYBUTTON].y) / 2;
	
	for (vector<Vertex>::iterator it = v.begin(); it != v.end(); it++) {
		float x = (*it).x;
		float y = (*it).y;
		float z = (*it).z;

		if (y >= l - diff && y < l + diff) {
			if (bustIdx == -1 && abs(l - bustLv) < diff) {
				bustIdx = idx;
			}
			if (waistIdx == -1 && abs(l - (joints[PELVISMID].y + joints[CLAVICLEL].y) / 2) < diff) {
				waistIdx = idx;
			}
			if (hipIdx == -1 && (abs(l - joints[PELVISMID].y) <= diff || l <= joints[PELVISMID].y + diff)) {
				hipIdx = idx;
			}
			layerVerts.push_back(Vertex(x, l, z));
		}
		else {
			vector<Vertex> ellipse;
			if (idx == 0) {
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			else if (idx == bustIdx) {
				ellipse = generateUniformLayer(layerVerts);
				bustHeight = ellipse[0].y;
				bust = GetCircumference(ellipse);
				circs.push_back(bust);
				layers->append(ellipse);
			}
			else if (idx == waistIdx) {
				ellipse = generateUniformLayer(layerVerts);
				waist = GetCircumference(ellipse);
				circs.push_back(waist);
				layers->append(ellipse);
			}
			else if (idx == hipIdx) {
				ellipse = generateUniformLayer(layerVerts);
				hipHeight = ellipse[0].y;
				hip = GetCircumference(ellipse);
				circs.push_back(hip);
				layers->append(ellipse);
			}

			if (hipHeight == 0) {
				ellipse = generateUniformLayer(layerVerts);
				if (ellipse.size() == 36)
					layers->append(ellipse);
			}

			idx++;
			l -= diff * 2;
			layerVerts.clear();
		}
	}

	// Auto Export
	WriteToOBJ(TOP);
}

void Body::GetSkirt(vector<Vertex> v) {
	int idx = 0;
	float l = v[0].y;
	float diff = 0.15;
	float bustHeight = 0;
	float hipHeight = 0;
	vector<Vertex> layerVerts;
	layers = new LinkedList<vector<Vertex>>();
	
	for (vector<Vertex>::iterator it = v.begin(); it != v.end(); it++) {
		float x = (*it).x;
		float y = (*it).y;
		float z = (*it).z;
		
		if (y >= l - diff && y < l + diff) {
			if (waistIdx == -1) {
				waistIdx = 0;
			}
			if (hipIdx == -1 && abs(l - joints[PELVISMID].y) <= diff) {
				hipIdx = idx;
			}
			layerVerts.push_back(Vertex(x, l, z));
		}
		else {
			vector<Vertex> ellipse;
			if (idx == waistIdx) {
				ellipse = generateUniformLayer(layerVerts);
				waist = GetCircumference(ellipse);
				circs.push_back(waist);
				layers->append(ellipse);
			}
			else if (idx == hipIdx) {
				ellipse = generateUniformLayer(layerVerts);
				hipHeight = ellipse[0].y;
				hip = GetCircumference(ellipse);
				circs.push_back(hip);
			}

			if (idx > waistIdx && hipIdx == -1) {
				vector<Vertex> a = layers->curr->value;
				for (vector<Vertex>::iterator av = a.begin(); av != a.end(); av ++ ) {
					(*av).y = l;
				}
				layerVerts.insert(layerVerts.end(), a.begin(), a.end());
				ellipse = generateUniformLayer(layerVerts);
				layers->append(ellipse);
			}

			if (hipIdx != -1 && idx >= hipIdx) {
				vector<Vertex> a = layers->curr->value;
				for (vector<Vertex>::iterator av = a.begin(); av != a.end(); av ++ ) {
					(*av).y = l;
				}
				layerVerts.insert(layerVerts.end(), a.begin(), a.end());
				ellipse = generateUniformLayer(layerVerts);
				layers->append(ellipse);
			}

			idx++;
			l -= diff * 2;
			layerVerts.clear();
		}
	}

	// Auto Export
	WriteToOBJ(SKIRT);
}

void Body::GetPants(vector<Vertex> v) {
	int idx = 0;
	float l = v[0].y;
	float diff = 0.15;
	float hipHeight = 0;
	float crotchHeight = (3*joints[BELLYBUTTON].y + joints[ANKLEL].y)/4;
	vector<Vertex> layerVerts;
	vector<Vertex> legL, legR;
	layers = new LinkedList<vector<Vertex>>();
	
	for (vector<Vertex>::iterator it = v.begin(); it != v.end(); it++) {
		float x = (*it).x;
		float y = (*it).y;
		float z = (*it).z;
		
		if (y >= l - diff && y < l + diff) {
			if (waistIdx == -1) {
				waistIdx = 0;
			}
			if (hipIdx == -1 && abs(l - joints[PELVISMID].y) <= diff) {
				hipIdx = idx;
			}

			if (hipIdx != -1 && idx > hipIdx) {
				if (x < 0) {
					legR.push_back(Vertex(x, l, z));
				}
				else {
					legL.push_back(Vertex(x, l, z));
				}
			}

			layerVerts.push_back(Vertex(x, l, z));
		}
		else {
			vector<Vertex> ellipse;
			if (idx == waistIdx) {
				ellipse = generateUniformLayer(layerVerts);
				waist = GetCircumference(ellipse);
				circs.push_back(waist);
				layers->append(ellipse);
			}
			else if (idx == hipIdx) {
				vector<Vertex> a = layers->curr->value;
				for (vector<Vertex>::iterator at = a.begin(); at != a.end(); at++)
					(*at).y = l;
				layerVerts.insert(layerVerts.end(), a.begin(), a.end());
				ellipse = generateUniformLayer(layerVerts);
				hipHeight = ellipse[0].y;
				hip = GetCircumference(ellipse);
				circs.push_back(hip);
				layers->append(ellipse);
			}
			else if (hipIdx == -1 || l > crotchHeight) {
				vector<Vertex> a = layers->curr->value;
				for (vector<Vertex>::iterator at = a.begin(); at != a.end(); at++)
					(*at).y = l;
				layerVerts.insert(layerVerts.end(), a.begin(), a.end());
				ellipse = generateUniformLayer(layerVerts);
				if (ellipse.size() == 36)
					layers->append(ellipse);
			}
			else if (hipIdx != -1 && idx > hipIdx) {
				if (x < 0)
					ellipse = generateUniformLayer(legR);
				else 
					ellipse = generateUniformLayer(legL);

				//if (ellipse.size() == 36)
				layers->append(ellipse);
			}

			idx++;
			l -= diff * 2;
			layerVerts.clear();
		}
	}

	// Auto Export
	WriteToOBJ(PANTS);
}

vector<Vertex> Body::generateUniformLayer(vector<Vertex> v) {
	float yVal = v[0].y;
	vector<Vertex> intersections, uniformLayer;

	GrahamScan g = GrahamScan(v);
	vector<Vertex> convexHull(g.GenerateConvexHull());

	Vertex minX = Vertex(), maxX = Vertex(), minZ = Vertex(), maxZ = Vertex();
	minX.x = 10;
	minZ.z = 10;
	maxX.x = -10;
	maxZ.z = -10;
	for (vector<Vertex>::iterator it = convexHull.begin(); it != convexHull.end(); it++) {
		Vertex n = (*it);
		if (n.x < minX.x) minX = n;
		if (n.x > maxX.x) maxX = n;
		if (n.z < minZ.z) minZ = n;
		if (n.z > maxZ.z) maxZ = n;
	}
	Vertex center = Vertex((minX.x + maxX.x)/2, yVal, (minZ.z + maxZ.z)/2);

	//float radius = (abs(minZ.z - center.z) > abs(maxZ.z - center.z))? abs(minZ.z - center.z) + 1 : abs(maxZ.z - center.z) + 1;

	float radius = 5;
	vector<Vertex> circle;
	for (int degree = 0; degree < 360; degree += 10) {
		float radian = (degree * M_PI) / 180;
		float x = -radius * cos(degree) + center.x;
		float z = radius * sin(degree) + center.z;
		//float x = -abs(maxX.x)*5 * cos(degree) + center.x;
		//float z =  abs(maxZ.z)*5 * sin(degree) + center.z;
		circle.push_back(Vertex(x, yVal, z));
	}

	for (int i = 0; i < circle.size(); i++) {
		int prev = 0, curr = 0;
		for (int j = 0; j < convexHull.size(); j++) {
			if (j == 0) {
				prev = convexHull.size() - 1;
				curr = 0;
			}
			else {
				prev = j - 1;
				curr = j;
			}

			Vertex c = (convexHull[curr].subtract(convexHull[prev])).cross(circle[i].subtract(center));

			float det = c.x + c.y + c.z;

			if (fabs(det) < 0.0001)  continue;
			Vertex p, q, r, s;
			float t;
			p = convexHull[curr].subtract(convexHull[prev]);
			q = center.subtract(convexHull[prev]);
			r = circle[i].subtract(center);
			s = q.cross(r);
			t = (s.x + s.y + s.z)/det;
			Vertex intersection = convexHull[prev].add(p.multiply(t));

			bool sameQuad = true;
			float cvh_bx, cvh_sx, cvh_bz, cvh_sz;
			if (convexHull[curr].x >= convexHull[prev].x) {
				cvh_bx = convexHull[curr].x;
				cvh_sx = convexHull[prev].x;
			}
			else {
				cvh_bx = convexHull[prev].x;
				cvh_sx = convexHull[curr].x;
			}

			if (convexHull[curr].z >= convexHull[prev].z) {
				cvh_bz = convexHull[curr].z;
				cvh_sz = convexHull[prev].z;
			}
			else {
				cvh_bz = convexHull[prev].z;
				cvh_sz = convexHull[curr].z;
			}

			if (intersection.x * circle[i].x < 0)
				sameQuad = false;

			if (sameQuad && (intersection.x >= cvh_sx && intersection.x <= cvh_bx) && (intersection.z >= cvh_sz && intersection.z <= cvh_bz)) {
				intersections.push_back(Vertex(intersection.x, yVal, intersection.z));

				// One intersection for each circle[i]
				break;
			}

		}
	}


	Vertex v1 = Vertex(0, yVal, 1);
	Vertex v2;
	tuple<float, Vertex> set;
	vector<tuple<float, Vertex>> tmp;

	for (vector<Vertex>::iterator it = intersections.begin(); it != intersections.end(); it++) {
		v2 = (*it);
		float angle = acos((v1.x*v2.x + v1.z*v2.z)/(sqrt(pow(v1.x, 2) + pow(v1.z, 2))*(sqrt(pow(v2.x, 2) + pow(v2.z, 2))))) * 180/M_PI; // degrees
		//if (v2.z < 0) angle = 360 - angle;
		if (v2.x > 0) angle = 360 - angle;
		tmp.push_back(make_tuple(angle, v2));
	}

	sort(tmp.begin(), tmp.end(), [](tuple<float, Vertex>& a, tuple<float, Vertex>& b) { return get<0>(a) < get<0>(b);  });

	for (int i = 0; i < tmp.size(); i++) {
	//for (int i = 0; i < 10; i++) {
		uniformLayer.push_back(get<1>(tmp[i]));
	}

	return uniformLayer;
}

void Body::SetSize(float b, float w, float h) {
	// TODO :: Scale joint positions

	float bscale = b / circs[bustIdx];
	float wscale = w / circs[waistIdx];
	float hscale = h / circs[hipIdx];

	circs.clear();

	// Neck to (bust - 1)
	layers->curr = layers->head;

	for (int i = 0; i < bustIdx; i++) {
		for (int j = 0; j < layers->curr->value.size(); j++) {
			layers->curr->value[j].x *= bscale;
			layers->curr->value[j].z *= bscale;
		}
		circs.push_back(GetCircumference(layers->curr->value));
		layers->curr = layers->curr->next;
	}

	// Bust to waist
	for (int i = bustIdx; i <= waistIdx; i++) {
		for (int j = 0; j < layers->curr->value.size(); j++) {
			layers->curr->value[j].x *= bscale * (waistIdx - i) / (waistIdx - bustIdx) + wscale * (i - bustIdx) / (waistIdx - bustIdx);
			layers->curr->value[j].z *= bscale * (waistIdx - i) / (waistIdx - bustIdx) + wscale * (i - bustIdx) / (waistIdx - bustIdx);
		}
		circs.push_back(GetCircumference(layers->curr->value));
		layers->curr = layers->curr->next;
	}

	/*
	// Waist
	for (int j = 0; j < layers->curr->value.size(); j++)
	{
			layers->curr->value[j].x *= wscale;
			layers->curr->value[j].z *= wscale;
	}
	circs.push_back(GetCircumference(layers->curr->value));
	layers->curr = layers->curr->next;
	*/

	// (Waist + 1) to hip
	for (int i = waistIdx + 1; i <= hipIdx; i++) {
		for (int j = 0; j < layers->curr->value.size(); j++) {
			layers->curr->value[j].x *= wscale * (hipIdx - i) / (hipIdx - waistIdx) + hscale * (i - waistIdx) / (hipIdx - waistIdx);
			layers->curr->value[j].z *= wscale * (hipIdx - i) / (hipIdx - waistIdx) + hscale * (i - waistIdx) / (hipIdx - waistIdx);
		}
		circs.push_back(GetCircumference(layers->curr->value));
		layers->curr = layers->curr->next;
	}

	// (Hip + 1) to dress bottom
	for (int i = hipIdx + 1; i < layers->length;  i++) {
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

void Body::WriteToOBJ(int type) {
	if (layers->length > 0) {
		string root = "makehuman/Result/";
		string filename;
		if (type == DRESS) {
			filename = "Dress.obj";
		}
		else if (type == TOP) {
			filename = "Top.obj";
		}
		else if (type == SKIRT) {
			filename = "Skirt.obj";
		}
		else if (type == PANTS) {
			filename = "Pants.obj";
		}

		ofstream outfile(root + filename);
		outfile << "# Generated OBJ file\n# By Minjoo Kang" << endl << endl << endl;

		layers->curr = layers->head;
		for (int i = 0; i < layers->length; i++) {
			for (int j = 0; j < layers->curr->value.size(); j++) {
				Vertex v = layers->curr->value[j];

				outfile << "v " << v.x << " " << v.y << " " << v.z << endl;
			}
			layers->curr = layers->curr->next;
		}

		layers->curr = layers->head;
		for (int i = 0; i < layers->length - 1; i++) {
			int lsize = layers->curr->value.size();
			for (int j = 0; j < lsize - 1; j++) {
				int first = lsize * i + j;
				int second = lsize * i + (j + 1);
				int third = lsize * (i + 1) + (j + 1);
				int forth = lsize * (i + 1) + j;
				outfile << "f " << first + 1 << " " << third + 1 << " " << forth + 1 << endl;
				outfile << "f " << first + 1 << " " << second + 1 << " " << third + 1 << endl;
			}
			int first = lsize * (i + 1) - 1;
			int second = lsize * i;
			int third = lsize * (i + 1);
			int forth = lsize * (i + 2) - 1;
			outfile << "f " << first + 1 << " " << third + 1 << " " << forth + 1 << endl;
			outfile << "f " << first + 1 << " " << second + 1 << " " << third + 1 << endl;
		}

		outfile.close();

		ofstream endflag("makehuman/Result/dressform.txt");
		endflag.close();
	}
}
