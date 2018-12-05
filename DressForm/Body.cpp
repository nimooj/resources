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

	if (f.is_open()) {
		while (getline(f, s)) {
			istringstream iss(s);
			Vertex v;
			iss >> v.x >> v.y >> v.z;
			v.set();

			v.x = (int)(v.x * 1000) / 1000.0f;
			v.y = (int)(v.y * 1000) / 1000.0f;
			v.z = (int)(v.z * 1000) / 1000.0f;

			joints.push_back(v);
		}
	}
	else return false;

	// Sort by descending order
	sort(joints.begin(), joints.end(), [](Vertex &a, Vertex &b) { if (a.y == b.y) return a.x > b.x; else return a.y > b.y; });

	// Callibrate bellybutton & wrists
	// When belly button is higher than wrists level
	if (joints[8].y == joints[9].y) {
		Vertex tmp_bellybutton = joints[7];
		joints[7] = joints[8];
		joints[8] = joints[9];
		joints[9] = tmp_bellybutton;
	}

	f.close();

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
				//else if (y <= joints[BELLYBUTTON].y && y > joints[PELVISL].y - 0.5 && x >= (joints[WRISTR].x + joints[SHOULDERR].x) / 2 && x <= (joints[WRISTL].x + joints[SHOULDERL].x) / 2) {
				else if (y <= joints[BELLYBUTTON].y && y > ((joints[PELVISL].y + joints[ANKLEL].y)/2) && x >= (joints[WRISTR].x + joints[SHOULDERR].x) / 2 && x <= (joints[WRISTL].x + joints[SHOULDERL].x) / 2) {
					result.push_back(Vertex(x*scale, y + callibrateY, (z + callibrateZ)*scale));
					//result.push_back(Vertex(x, y, z));
				}

				if (y <= joints[PELVISL].y) {
					if (z > thighMax.z)
						thighMax = Vertex(x, y, z);
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

vector<Vertex> Body::alignY(vector<Vertex> v) {
	float l = v[0].y;
	for (int i = 0; i < v.size(); i++) {
		if (v[i].y > l - interval) {
			v[i].y = l; 
		}
		else {
			l -= interval;
		}
	}

	return v;
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
	float l = tl/100.0f;
	float diff = 0.15;

	layers = new LinkedList<vector<Vertex>>();
	LinkedList<vector<Vertex>>* tmp = new LinkedList<vector<Vertex>>();
	int count = 0;

	// 1. Get ellipse & circumferences of each convexhulls
	for (vector<Vertex>::iterator it = v.begin(); it != v.end(); it++ ) {
		float x = (*it).x;
		float y = (*it).y;
		float z = (*it).z;

		if (y > l - diff && l < l + diff) { // Add verts to the layer
			layerVerts.push_back(Vertex(x, l, z));
			count++;
		}
		else { // Generate layer
			if (count > 0) { // When more than one vertex in the layer
				GrahamScan g = GrahamScan(layerVerts);
				vector<Vertex> convexedLayer(g.GenerateConvexHull());

				vector<Vertex> ellipse = sweep(idx, convexedLayer, layerVerts);
				float currCirc = GetCircumference(ellipse);


				/*
				vector<Vertex> subEllipse;
				if (circs.size() > 0) {
					// Generate sub layer for smoothness
					int div = 2;

					for (int i = div; i > 0; i--) {
						for (int j = 0; j < ellipse.size(); j++) {
							//float xDiff = (layers->curr->value[j].x - ellipse[j].x) * i/div;
							//float zDiff = (layers->curr->value[j].z - ellipse[j].z) * i/div;
							float xDiff = (tmp->curr->value[j].x - ellipse[j].x) * i/div;
							float zDiff = (tmp->curr->value[j].z - ellipse[j].z) * i/div;
							subEllipse.push_back(Vertex(ellipse[j].x + xDiff, ellipse[j].y + interval * i / div, ellipse[j].z + zDiff));
						}

						//layers->append(subEllipse);
						tmp->append(subEllipse);
						circs.push_back(GetCircumference(subEllipse));

						subEllipse.clear();
					}
				}
				*/

				circs.push_back(currCirc); // Do not save sub layers circumference

				float secondtolastIdx = 0;
				if (circs.size() > 1) {
					secondtolastIdx = circs.size() - 2;
				}

				tmp->append(ellipse);
			}

			layerVerts.clear();
			l -= interval;
			count = 0;
		}
		idx++;
	}

	float minimum = 100;
	int minIdx = 0;
	float maximum1 = 0, maximum2 = 0;
	int maxIdx1 = 0, maxIdx2 = 0;

	for (int i = 1; i < circs.size(); i++) {
		if (i < circs.size() / 2 && circs[i] > maximum1) {
			maximum1 = circs[i];
			maxIdx1 = i;
		}

		if (circs[i] < minimum && i > maxIdx1) {
			minimum = circs[i];
			minIdx = i;
		}

		if (i > circs.size() / 2 && circs[i] > maximum2) {
			maxIdx2 = i;
			maximum2 = circs[i];
		}
	}

	bustIdx = maxIdx1 + 1;
	waistIdx = minIdx;
	hipIdx = maxIdx2;

	cout << bustIdx << ", " << waistIdx << ", " << hipIdx << endl;

	// 2. Extract neck(top-most), bust, hip, waist
	tmp->curr = tmp->head;
	for (int i = 0; i < tmp->length; i++) {
		if (i == 0) {
			// Heighten collar level
			//for (int j = 0; j < tmp->curr->value.size(); j++) {
				//tmp->curr->value[j].y += 0.15;
			//}
			layers->append(tmp->curr->value);
		}
		else if (i == bustIdx) {
			layers->append(tmp->curr->value);
			bust = GetCircumference(tmp->curr->value);
		}
		else if (i == waistIdx) {
			layers->append(tmp->curr->value);
			waist = GetCircumference(tmp->curr->value);
		}
		else if (i == hipIdx) {
			layers->append(tmp->curr->value);
			hip = GetCircumference(tmp->curr->value);
		}
		else {
			layers->append(tmp->curr->value);
		}

		tmp->curr = tmp->curr->next;
	}
	delete tmp;
	 
	/* Expand dress */
	vector<Vertex> lastLayer = layers->curr->value;
	vector<Vertex> tail;
	l = lastLayer[0].y;
	while (l >= joints[ANKLEL].y - interval) {
		for (int i = 0; i < lastLayer.size(); i++) {
			tail.push_back(Vertex(lastLayer[i].x * expand, l, lastLayer[i].z * expand));
		}
		layers->append(tail);
		l -= interval;
		tail.clear();
	}
	/* *** */

	ifstream sizeFile("makehuman/Result/sizes.txt");
	vector<string> st;

	string line;
	while (getline(sizeFile, line)) {
		istringstream iss(line);
		st.push_back(line);
	}

	SetSize(atof(st[2].c_str()), atof(st[3].c_str()), atof(st[4].c_str()));
}

void Body::GetCircularDressForm(vector<Vertex> v) {
	vector<Vertex> layerVerts;
	float l = (v[0].y * 100)/100.0f + interval;

	layers = new LinkedList<vector<Vertex>>();
	LinkedList<vector<Vertex>>* tmp = new LinkedList<vector<Vertex>>();
	int count = 0;

	float radius = 3.0f;
	vector<Vertex> circle;
	for (int degree = 0; degree < 360; degree += 10) {
		float radian = (degree* M_PI) / 180;
		float cx = radius * cos(radian);
		float cy = l;
		float cz = radius * sin(radian);
		circle.push_back(Vertex(cx, cy, cz));
	}

	Vertex maxX = Vertex(), maxZ = Vertex(), minX = Vertex(), minZ = Vertex();
	maxX.x = -5;
	maxZ.z = -5;
	minX.x = 5;
	minZ.z = 5;
	// 1. Get ellipse & circumferences of each convexhulls
	for (vector<Vertex>::iterator it = v.begin(); it != v.end(); it++ ) {
		float x = (*it).x;
		float y = (*it).y;
		float z = (*it).z;

		if (y >= l) { // Add verts to the layer
			//layerVerts.push_back(Vertex(x, l - interval*2/3, z));
			layerVerts.push_back(Vertex(x, l, z));
			if (x > maxX.x) maxX.set(x, l, z);
			if (z > maxZ.z) maxZ.set(x, l, z);
			if (x < minX.x) minX.set(x, l, z);
			if (z < minZ.z) minZ.set(x, l, z);
			count++;
		}
		else { // Generate layer
			if (count > 0) { // When more than one vertex in the layer
				vector<Vertex> preEllipse;
				vector<Vertex> ellipse;
				// Get center
				Vertex Center = Vertex((minX.x + maxX.x)/2, l, (minZ.z + maxZ.z)/2);

				// Get top Dists from center from layerVerts
				vector<tuple<float, int>> dists;
				for (int i = 0; i < layerVerts.size(); i++) {
					dists.push_back(make_tuple(Center.distance(layerVerts[i]), i));
				}
				// sort by descending dists
				sort(dists.begin(), dists.end(), [](tuple<float, int> a, tuple<float, int> b) {return get<0>(a) > get<0>(b); });
				int topDists = 36;
				if (dists.size() < topDists)
					topDists = dists.size();

				for (int i = 0; i < topDists; i++) {
					preEllipse.push_back(layerVerts[get<1>(dists[i])]);
				}


				for (int i = 0; i < circle.size(); i++) {
					Vertex closest = circle[i].closest(preEllipse);
					ellipse.push_back(closest);
					circle[i].y -= interval;
				}

				float currCirc = GetCircumference(ellipse);

				circs.push_back(currCirc); // Do not save sub layers circumference

				tmp->append(ellipse);
			}

			layerVerts.clear();
			l -= interval;
			count = 0;
			maxX.del();
			maxZ.del();
			minX.del();
			minZ.del();
			maxX.x = -5;
			maxZ.z = -5;
			minX.x = 5;
			minZ.z = 5;
		}
	}

	float minimum = 100;
	int minIdx = 0;
	float maximum1 = 0, maximum2 = 0;
	int maxIdx1 = 0, maxIdx2 = 0;

	for (int i = 1; i < circs.size(); i++) {
		if (i < circs.size() / 2 && circs[i] > maximum1) {
			maximum1 = circs[i];
			maxIdx1 = i;
		}
		if (circs[i] < minimum && i > maxIdx1) {
			minimum = circs[i];
			minIdx = i;
		}

		if (i > circs.size() / 2 && circs[i] > maximum2) {
			maxIdx2 = i;
			maximum2 = circs[i];
		}
	}

	bustIdx = maxIdx1 + 1;
	waistIdx = minIdx;
	hipIdx = maxIdx2;

	//cout << bustIdx << ", " << waistIdx << ", " << hipIdx << endl;

	// 2. Extract neck(top-most), bust, hip, waist
	tmp->curr = tmp->head;
	for (int i = 0; i < tmp->length; i++) {
		if (i == 0) {
			// Heighten collar level
			//for (int j = 0; j < tmp->curr->value.size(); j++) {
				//tmp->curr->value[j].y += 0.15;
			//}
			layers->append(tmp->curr->value);
		}
		else if (i == bustIdx) {
			layers->append(tmp->curr->value);
			bust = GetCircumference(tmp->curr->value);
		}
		else if (i == waistIdx) {
			layers->append(tmp->curr->value);
			waist = GetCircumference(tmp->curr->value);
		}
		else if (i == hipIdx) {
			layers->append(tmp->curr->value);
			hip = GetCircumference(tmp->curr->value);
		}
		else {
			layers->append(tmp->curr->value);
		}

		tmp->curr = tmp->curr->next;
	}
	delete tmp;
	 
	/* Expand dress */
	/*
	vector<Vertex> lastLayer = layers->curr->value;
	vector<Vertex> tail;
	while (l >= joints[ANKLEL].y - interval) {
		for (int i = 0; i < lastLayer.size(); i++) {
			tail.push_back(Vertex(lastLayer[i].x * expand, l, lastLayer[i].z * expand));
		}
		layers->append(tail);
		l -= interval;
		tail.clear();
	}
	*/
	/* *** */

	ifstream sizeFile("makehuman/Result/sizes.txt");
	vector<string> st;

	string line;
	while (getline(sizeFile, line)) {
		istringstream iss(line);
		st.push_back(line);
	}

	//SetSize(atof(st[2].c_str()), atof(st[3].c_str()), atof(st[4].c_str()));
}

void Body::GetIndex(vector<Vertex> v) {
	int idx = 0;
	float l = v[0].y;
	float diff = 0.15;
	float bustHeight = 0;
	float hipHeight = 0;
	vector<Vertex> layerVerts;
	vector<Vertex> bustVecs, accum2;
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
			if (hipIdx == -1 && abs(l - joints[PELVISMID].y) < diff) {
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
			else if (idx == shoulderTopIdx) {
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			else if (idx == shoulderTopMidIdx) {
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			else if (idx == shoulderMidIdx) {
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			else if (idx == shoulderMidBottomIdx) {
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			else if (idx == shoulderBottomIdx) {
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			else if (idx == shoulderBottomBustIdx) {
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}
			else if (idx == bustIdx) {
				ellipse = generateUniformLayer(layerVerts);
				bustHeight = ellipse[0].y;
				bustVecs = ellipse;
				bust = GetCircumference(ellipse);
				circs.push_back(bust);
				layers->append(ellipse);
			}
			else if (idx == waistIdx) {
				ellipse = generateUniformLayer(layerVerts);
				waist = GetCircumference(ellipse);
				circs.push_back(waist);
				/*
				layers->append(ellipse);
				*/
			}
			else if (idx == hipIdx) {
				ellipse = generateUniformLayer(layerVerts);
				hipHeight = ellipse[0].y;
				hip = GetCircumference(ellipse);
				circs.push_back(hip);
				/*
				layers->append(ellipse);
				*/
			}
			else {
				ellipse = generateUniformLayer(layerVerts);
				circs.push_back(GetCircumference(ellipse));
				/*
				layers->append(ellipse);
				*/
			}

			if (hipIdx != -1 && idx >= hipIdx) {
				accum2.insert(accum2.end(), layerVerts.begin(), layerVerts.end());
			}

			idx++;
			l -= diff * 2;
			layerVerts.clear();
		}
	}

	for (int i = 0; i < accum2.size(); i++) {
		accum2[i].y = hipHeight - diff * 2;
	}
	vector<Vertex> hipVecs = generateUniformLayer(accum2);

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

	for (int i = 0; i < accum2.size(); i++) {
		accum2[i].y = hipHeight - diff * 2;
	}

	vector<Vertex> ellipse = generateUniformLayer(accum2);
	layers->append(ellipse);

	l = hipHeight - diff * 4;

	/* Expand dress */
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
	/*
	*/
	/* *** */

	/*
	ifstream sizeFile("Result/sizes.txt");
	vector<string> st;

	string line;
	while (getline(sizeFile, line)) {
		istringstream iss(line);
		st.push_back(line);
	}

	//SetSize(atof(st[2].c_str()), atof(st[3].c_str()), atof(st[4].c_str()));
	*/

	// Auto Export
	WriteToOBJ();
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

vector<Vertex> Body::sweep(int idx, vector<Vertex> v, vector<Vertex> d) {
	vector<Vertex> c; 
	vector<Vertex> quad2, quad4;
	Vertex minX = Vertex(), maxX = Vertex(), minZ = Vertex(), maxZ = Vertex();
	float yVal = v[0].y;

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

		if (v[i].x > 0 && v[i].z < 0)
			quad4.push_back(v[i]);

		if (v[i].x < 0 && v[i].z > 0)
			quad2.push_back(v[i]);
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

	float maxDist = 0;
	Vertex maxDistV;
	int slice = 10;

	// Quad 4
	for (float degree = 0; degree < includedAngle + includedAngle/slice; degree += includedAngle/slice, idx++) {
		float radian = (degree* M_PI) / 180;

		float nx = a * cos(radian) + center.x;
		float ny = yVal;
		float nz = -b * sin(radian) + center.z;

		Vertex newV = Vertex(nx, ny, nz);
		/*
		Vertex closest = newV.closest(quad4);
		newV.x = closest.x;
		newV.z = closest.z;
		*/
		
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

		Vertex newV = Vertex(nx, ny, nz);
		/*
		Vertex closest = newV.closest(quad2);
		newV.x = closest.x;
		newV.z = closest.z;
		*/

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

	return c;
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

	float radius = (abs(minZ.z - center.z) > abs(maxZ.z - center.z))? abs(minZ.z - center.z) + 1 : abs(maxZ.z - center.z) + 1;

	radius = 5;
	vector<Vertex> circle;
	for (int degree = 0; degree < 360; degree += 10) {
		float radian = (degree * M_PI) / 180;
		float x = -radius * cos(degree) + center.x;
		float z = radius * sin(degree) + center.z;
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

	Vertex v1 = Vertex(1, yVal, 0);
	Vertex v2;
	tuple<float, Vertex> set;
	vector<tuple<float, Vertex>> tmp;

	for (vector<Vertex>::iterator it = intersections.begin(); it != intersections.end(); it++) {
		v2 = (*it);
		float angle = acos((v1.x*v2.x + v1.z*v2.z)/(sqrt(pow(v1.x, 2) + pow(v1.z, 2))*(sqrt(pow(v2.x, 2) + pow(v2.z, 2))))) * 180/M_PI; // degrees
		if (v2.z < 0) angle = 360 - angle;
		tmp.push_back(make_tuple(angle, v2));
	}

	sort(tmp.begin(), tmp.end(), [](tuple<float, Vertex>& a, tuple<float, Vertex>& b) { return get<0>(a) < get<0>(b);  });

	for (int i = 0; i < tmp.size(); i++) {
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

void Body::WriteToOBJ() {
	if (layers->length > 0) {
		ofstream outfile("makehuman/Result/Dress.obj");
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
		cout << "obj write ended" << endl;
	}
}
