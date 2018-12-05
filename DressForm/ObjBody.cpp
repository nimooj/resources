#include "stdafx.h"
#include "ObjBody.h"

ObjBody::ObjBody() {
}

ObjBody::~ObjBody() {
}

bool ObjBody::importVertices() {
	ifstream f;
	string s;
	int idx = 1;
	f.open("makehuman/Result/vertices.txt");

	if (f.is_open()) {
		while (getline(f, s)) {
			istringstream iss(s);
			Vertex v;
			iss >> v.x >> v.y >> v.z;
			v.set();

			v.idx = idx;
			v.x = (int)(v.x * 1000) / 1000.0f;
			v.y = (int)(v.y * 1000) / 1000.0f;
			v.z = (int)(v.z * 1000) / 1000.0f;

			vertices.push_back(v);

			idx++;
		}
	}
	else
		return false;

	f.close();

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
	f.close();

	// Sort by descending order
	sort(vertices.begin(), vertices.end(), [](Vertex &a, Vertex &b) { return a.y > b.y; });
	sort(nipples.begin(), nipples.end(), [](Vertex &a, Vertex&b) { return a.z > b.z;  });
	nippleL = left;
	nippleR = right;

	return true;
}

bool ObjBody::importJoints() {
	ifstream f;
	string s;
	f.open("makehuman/Result/joints.txt");

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
	sort(joints.begin(), joints.end(), [](Vertex &a, Vertex &b) { if (a.y == b.y) return a.x > b.x; else return a.y > b.y; });

	f.close();

	return true;
}

bool ObjBody::importFaces() {
	ifstream f;
	string s;
	f.open("makehuman/Result/faces.txt");

	if (f.is_open()) {
		while (getline(f, s)) {
			istringstream iss(s);
			int f1, f2, f3, f4;
			iss >> f1 >> f2 >> f3 >> f4;
			mesh.push_back(Mesh(f2, f4, f1, vertices[f2 - 1], vertices[f4 - 1], vertices[f1 - 1]));
			mesh.push_back(Mesh(f4, f2, f3, vertices[f4 - 1], vertices[f2 - 1], vertices[f3 - 1]));
		}
	}
	else
		return false;

	f.close();
	return true;
}

vector<Mesh> ObjBody::updateMesh(vector<int> i, vector<Mesh> m) {
	vector<Mesh> result;

	int max = 0;
	for (vector<int>::iterator vi = i.begin(); vi != i.end(); vi++) {
		int index = (*vi);
		if (index > max)
			max = index;
	}

	vector<bool> exists(max + 1, false);

	for (vector<int>::iterator vi = i.begin(); vi != i.end(); vi++) {
		int index = (*vi);
		exists[index] = true;
	}

	for (vector<Mesh>::iterator mi = m.begin(); mi != m.end(); mi++) {
		int mIdx1 = (*mi).indices[0];
		int mIdx2 = (*mi).indices[1];
		int mIdx3 = (*mi).indices[2];

		if (mIdx1 < exists.size() && mIdx2 < exists.size() && mIdx3 < exists.size()) {
			if (exists[mIdx1] && exists[mIdx2] && exists[mIdx3])
				result.push_back(Mesh((*mi).indices, (*mi).vertices));
		}
	}

	return result;
}

void ObjBody::severBody(int process) {
	vector<int> indices;
	vector<Vertex> result;

	switch (process) {
		case ARMLESS :
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

			for (vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); it++) {
				int i;
				float x, y, z;
				i = (*it).idx;
				x = (*it).x;
				y = (*it).y;
				z = (*it).z; 
				float scale = 1.065; 
				float callibrateY = 0.1086;
				float callibrateZ = 0.065;

				if (y < joints[NECKMID].y && y > joints[BELLYBUTTON].y && x >= joints[SHOULDERR].x && x <= joints[SHOULDERL].x) {
					result.push_back(Vertex(i, x*scale, y + callibrateY, (z + callibrateZ)*scale));
					indices.push_back(i);
				}
				else if (y <= joints[BELLYBUTTON].y && y > joints[PELVISL].y && x >= (joints[WRISTR].x + joints[SHOULDERR].x) / 2 && x <= (joints[WRISTL].x + joints[SHOULDERL].x) / 2) {
					result.push_back(Vertex(i, x*scale, y + callibrateY, (z + callibrateZ)*scale));
					indices.push_back(i);
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

	vector<Mesh> nm(updateMesh(indices, mesh));

	vertices.clear();
	mesh.clear();

	//vertices.insert(vertices.end(), result.begin(), result.end());
	//mesh.insert(mesh.end(), nm.begin(), nm.end());

	vertices = result;
	mesh = nm;
}

void ObjBody::getIndex(vector<Vertex> v) {
	int idx = 0;
	float l = v[0].y;
	float diff = 0.15;
	vector<Vertex> layerVerts;
	layers = new LinkedList<vector<Vertex>>();

	float radius = 3;
	vector<Vertex> circle;
	for (float degree = 0; degree < 360; degree += 10) {
		float radian = degree * M_PI / 180;
		float x = radius * cos(radian);
		float y = l;
		float z = radius * sin(radian);
		circle.push_back(Vertex(x, y, z));
	}

	for (vector<Vertex>::iterator it = v.begin(); it != v.end(); it++) {
		float x = (*it).x;
		float y = (*it).y;
		float z = (*it).z;
		
		if (y > l - diff && y < l + diff) {
			if (abs(l - nippleL.y) < diff) {
				bustIdx = idx;
			}
			if (abs(l - (joints[SHOULDERL].y + joints[NECKMID].y)/2) < diff) {
				shoulderTopIdx = idx;
			}
			if (abs(l - (joints[SHOULDERL].y + nippleL.y) / 2) < diff) {
				shoulderBottomIdx = idx;
			}
			if (abs(l - (joints[PELVISMID].y + joints[CLAVICLEL].y)/2) < diff) {
				waistIdx = idx;
			}
			if (abs(l - joints[PELVISMID].y) < diff) {
				hipIdx = idx;
			}
			layerVerts.push_back(Vertex(x, l, z));
		}
		else {
			vector<Vertex> ellipse;
			if (idx == 0) {
				for (int i = 0; i < circle.size(); i++) {
					Vertex closest = circle[i].closest(layerVerts);
					ellipse.push_back(closest);
				}
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
				//layers->append(layerVerts);
			}
			else if (idx == shoulderTopIdx) {
				for (int i = 0; i < circle.size(); i++) {
					Vertex closest = circle[i].closest(layerVerts);
					ellipse.push_back(closest);
				}
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
				//layers->append(layerVerts);
			}
			else if (idx == shoulderBottomIdx) {
				for (int i = 0; i < circle.size(); i++) {
					Vertex closest = circle[i].closest(layerVerts);
					ellipse.push_back(closest);
				}
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
				//layers->append(layerVerts);
			}
			else if (idx == bustIdx) {
				for (int i = 0; i < circle.size(); i++) {
					Vertex closest = circle[i].closest(layerVerts);
					ellipse.push_back(closest);
				}
				bust = GetCircumference(ellipse);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
				//layers->append(layerVerts);
			}
			else if (idx == waistIdx) {
				for (int i = 0; i < circle.size(); i++) {
					Vertex closest = circle[i].closest(layerVerts);
					ellipse.push_back(closest);
				}
				waist = GetCircumference(ellipse);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
				//layers->append(layerVerts);
			}
			else if (idx == hipIdx) {
				for (int i = 0; i < circle.size(); i++) {
					Vertex closest = circle[i].closest(layerVerts);
					ellipse.push_back(closest);
				}
				hip = GetCircumference(ellipse);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
				//layers->append(layerVerts);
			}
			else {
				for (int i = 0; i < circle.size(); i++) {
					Vertex closest = circle[i].closest(layerVerts);
					ellipse.push_back(closest);
				}
				bust = GetCircumference(ellipse);
				circs.push_back(GetCircumference(ellipse));
				layers->append(ellipse);
			}

			idx++;
			l -= diff * 2;
			layerVerts.clear();
		}
	}

	bustIdx = 3;
	waistIdx = 4;
	hipIdx = 5;

	ifstream sizeFile("makehuman/Result/sizes.txt");
	vector<string> st;

	string line;
	while (getline(sizeFile, line)) {
		istringstream iss(line);
		st.push_back(line);
	}

	//SetSize(atof(st[2].c_str()), atof(st[3].c_str()), atof(st[4].c_str()));
}

vector<Vertex> ObjBody::getDressForm() {
	severBody(TORSO);
	sort(vertices.begin(), vertices.end(), [](Vertex &a, Vertex &b) { return a.y > b.y; });
	//getIndex(vertices);
	int idx = 0;
	float l = vertices[0].y;
	float diff = 0.15;
	vector<int> indices;
	vector<Vertex> newVertices;
	vector<Vertex> layerVerts;
	layers = new LinkedList<vector<Vertex>>();

	for (vector<Vertex>::iterator v = vertices.begin(); v != vertices.end(); v++) {
		int idx = (*v).idx;
		float x = (*v).x;
		float y = (*v).y;
		float z = (*v).z;
		
		if (y > l - diff && y < l + diff) {
			if (abs(l - nippleL.y) < diff) {
				bustIdx = idx;
			}
			if (abs(l - (joints[SHOULDERL].y + joints[NECKMID].y)/2) < diff) {
				shoulderTopIdx = idx;
			}
			if (abs(l - (joints[SHOULDERL].y + nippleL.y) / 2) < diff) {
				shoulderBottomIdx = idx;
			}
			if (abs(l - (joints[PELVISMID].y + joints[CLAVICLEL].y)/2) < diff) {
				waistIdx = idx;
			}
			if (abs(l - joints[PELVISMID].y) < diff) {
				hipIdx = idx;
			}
			layerVerts.push_back(Vertex(idx, x, y, z));
		}
		else {
			GrahamScan g = GrahamScan(layerVerts);
			vector<Vertex> convexHull(g.GenerateConvexHull());

			for (int i = 0; i < convexHull.size(); i++) {
				indices.push_back(convexHull[i].idx);
				newVertices.push_back(convexHull[i]);
			}

			layers->append(convexHull);

			idx++;
			l -= diff * 2;
			layerVerts.clear();
		}
	}

	vector<Mesh> nm(updateMesh(indices, mesh));

	vertices.clear();
	mesh.clear();

	vertices = newVertices;
	mesh = nm;
	return vertices;
}

void ObjBody::SetSize(float b, float w, float h) {
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


bool ObjBody::writeToObj() {
	ofstream outfile("makehuman/Result/Dress.obj");
	outfile << "# Generated OBJ file\n# By Minjoo Kang" << endl << endl << endl;

	int max = 0;
	for (vector<Vertex>::iterator vi = vertices.begin(); vi != vertices.end(); vi++) {
		int index = (*vi).idx;
		float x = (*vi).x;
		float y = (*vi).y;
		float z = (*vi).z;

		if (index > max) max = index;

		outfile << "v " << x << " " << y << " " << z << endl;
	}

	int newIndex = 1;
	vector<int> id(max + 1, -1);
	for (vector<Vertex>::iterator vi = vertices.begin(); vi != vertices.end(); vi++, newIndex++) {
		int index = (*vi).idx;
		id[index] = newIndex;
	}

	for (vector<Mesh>::iterator mi = mesh.begin(); mi != mesh.end(); mi++) {
		int mIdx1 = (*mi).indices[0];
		int mIdx2 = (*mi).indices[1];
		int mIdx3 = (*mi).indices[2];

		outfile << "f " << id[mIdx1] << " " << id[mIdx2] << " " << id[mIdx3] << endl;
	}

	outfile.close();

	cout << "Created an obj file." << endl;

	return true;
}
