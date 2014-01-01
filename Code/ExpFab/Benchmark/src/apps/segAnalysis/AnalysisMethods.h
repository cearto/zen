#ifndef ANALYSISMETHODS_H_
#define ANALYSISMETHODS_H_
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Segmentations.h"
#include "R3Graphics/R3Graphics.h"
using namespace std;

/****************************************
 * Utilities
 ***************************************/

/*
 *  Make a filename by combine dir, name and suffix
 */
string MakeName(char* dir, const char* name, char* suffix) {
	string fn = dir;
	if(fn.at(fn.length()-1) != '/') fn += "/";
	fn += name;
	fn += suffix;
	return fn;
}

/*
 * Judge whether a segment is too small
 */
bool tooSmall(Segmentation* segmentation, Segment* segment) {
	RNScalar totalArea = segmentation->area;
	RNScalar segmentArea = segment->area;
	int segmentFaces = segment->faces.NEntries();
	// a segment is too small if it has less than 3 faces are its percentage area is less than 0.01% 
	return (segmentFaces < 3 || segmentArea/totalArea < 0.0001);
}

/*
 * map segmentId to the index of segmenation->segments
 */
int* mapId2Segment(Segmentation* segmentation) {
	int nseg = segmentation->segments.NEntries();
	int* mapping = new int[nseg];
	for(int i = 0; i < nseg; i ++) {
		Segment* segment = segmentation->segments[i];
		R3MeshFace* face = segment->faces[0];
		int id = segmentation->mesh->FaceSegment(face);
		mapping[id] = i;
	} 
	return mapping;
}

/*
 * pre-compute which segment is too small
 */
bool* findTooSmallSegments(Segmentation* segmentation) {
	int nseg = segmentation->segments.NEntries();
	bool* isTooSmall = new bool[nseg];
	for(int i = 0; i < nseg; i ++) {
		Segment* segment = segmentation->segments[i];
		if(tooSmall(segmentation, segment)) {
			isTooSmall[i] = true;
		}
		else isTooSmall[i] = false;
	}
	return isTooSmall;
}


/*
 * judge whether two double is equal
 */
bool equal(double d1, double d2) {
	double e = 0.000001;
	double diff = d1 - d2;
	if (diff < 0) diff = -diff;
	return diff < e; 
}



/****************************************
 * For analysis about segment areas
 ***************************************/

/*
 * Analyze segment areas
 */
static int analyzeSegmentArea(
		Segmentation** segmentations, int n, 
		char* nameDirStat, char* id, 
		int print_verbose = 0) 
{

	if (print_verbose) cout << "Processing segment area info " << endl;
	list<double> area;
	list<double> areaRatio;
	// acquiring statistics
	for(int i = 0; i < n; i ++) {
		// get the non-trivial areas
		Segmentation* segmentation = segmentations[i];
		list<double> tmp;
		double totalArea = 0; // total area of non-too-small faces
		int count = 0;	// total number of non-too-small faces
		for(int j = 0; j < segmentation->segments.NEntries(); j ++) {
			if(!tooSmall(segmentation, segmentation->segments[j])) {
				tmp.push_back(segmentation->segments[j]->area);
				totalArea += segmentation->segments[j]->area;
				count ++;
			}
		}
		if (print_verbose) {
			printf("  segmentation %2d: totalArea = %.3f, nSegments = %3d, non-trivial segments = %3d\n", 
				i, totalArea, segmentation->segments.NEntries(), count);
		}
		// process results derived above
		double avgArea = totalArea / count;
		list<double>::iterator iter;
		for(iter = tmp.begin(); iter != tmp.end(); iter ++) {
			double a = *iter;
			area.push_back(a/totalArea);  // area in percentage
			areaRatio.push_back(a/avgArea); // area divided by avgArea
		}
	}
	// output statistics
	ofstream fout;
	list<double>::iterator it;
	// area
	string name = MakeName(nameDirStat, id, (char*)".area");
	fout.open(name.c_str());
	if(!fout.is_open()) {cerr << "Cannot dump area info, exit!\n"; return 0;}
	for(it = area.begin(); it != area.end(); it ++) {
		fout << (*it) << endl;
	}
	fout.close();
	if (print_verbose) cout << "Area info dumped" << endl;
	// areaRatio
	name = MakeName(nameDirStat, id, (char*)".areaRatio");
	fout.open(name.c_str());
	if(!fout.is_open()) {cerr << "Cannot dump area info, exit!\n"; return 0;}
	for(it = areaRatio.begin(); it != areaRatio.end(); it ++) {
		fout << (*it) << endl;
	}
	fout.close();
	if (print_verbose) cout << "AreaRatio dumped" << endl;
	
	// return success
	return 1;
}



/****************************************
 * For analysis about diheral angle
 ***************************************/

/*
 * Analyze dihedral angles
 */
static int analyzeDihedralAngle(
		Segmentation** segmentations, int n, 
		char* nameDirStat, char* id, 
		int print_verbose = 0) 
{
	if (print_verbose) cout << "Processing dihedral angle info " << endl;
	list<double> angleOnCut;
	list<double> angleNotOnCut;
	int nEdges = segmentations[0]->mesh->NEdges(); // should be the same for all segmentations
	
	// acquiring statistics
	for(int i = 0; i < n; i ++) {
		Segmentation* segmentation = segmentations[i];
		R3Mesh* mesh = segmentation->mesh;
		// get the mapping from segmentId to the index in segmentation->segments
		int* mapping = mapId2Segment(segmentation);
		// precompute whether a segment is too small
		bool* isTooSmall = findTooSmallSegments(segmentation);
		// scan all edges
		for(int j = 0; j < nEdges; j ++) {
			R3MeshEdge* edge = mesh->Edge(j);
			R3MeshFace* face1 = mesh->FaceOnEdge(edge, 0);
			R3MeshFace* face2 = mesh->FaceOnEdge(edge, 1);
			R3Vector edge_vector = mesh->EdgeVector(edge);
			if (mesh->FaceOnEdge(edge, 0) == face2) edge_vector.Flip();
			R3Vector cross_normals = mesh->FaceNormal(face1) % mesh->FaceNormal(face2);
			RNBoolean convex = (cross_normals.Dot(edge_vector) >= 0);
			RNScalar cosine = mesh->FaceNormal(face1).Dot(mesh->FaceNormal(face2));
			RNScalar acosine = (RNIsEqual(cosine * cosine, 1.0)) ? 0.0 : acos(cosine);
			RNScalar exterior_angle = (convex) ? (RN_PI + acosine) : (RN_PI - acosine); // 2PI=convex, PI=flat, 0=concave
			int id1 = mesh->FaceSegment(face1);
			int id2 = mesh->FaceSegment(face2);
			if (id1 != id2 && !isTooSmall[mapping[id1]] && !isTooSmall[mapping[id2]])
				// an edge on the segments'(not too small) boundary
				angleOnCut.push_back(exterior_angle);
			else // an normal edge
				angleNotOnCut.push_back(exterior_angle);
		}
		// clean up
		delete mapping;
		delete isTooSmall;
	}
	
	// output results
	ofstream fout;
	list<double>::iterator it;
	// those edges on cuts
	string name = MakeName(nameDirStat, id, (char*)".angleOnCut");
	fout.open(name.c_str());
	for(it = angleOnCut.begin(); it != angleOnCut.end(); it ++) {
		fout << (*it) << endl;
	}
	fout.close();
	if (print_verbose)	cout << "dehedral angle on cut info dumped" << endl;
	// those edges not on cuts
	name = MakeName(nameDirStat, id, (char*)".angleNotOnCut");
	fout.open(name.c_str());
	for(it = angleNotOnCut.begin(); it != angleNotOnCut.end(); it ++) {
		fout << (*it) << endl;
	}
	fout.close();
	if(print_verbose) cout << "dehedral angle not on cut info dumped" << endl;	
				
	// return success
	return 1;
}



/****************************************
 * For analysis about curvature
 ***************************************/

/*
 * struct Curvature
 */
struct Curvature{
	double curv[6];
};
ostream& operator<<(ostream& out, Curvature& c) {
	for(int i = 0; i < 5; i ++) 
		out << c.curv[i] << " ";
	out << c.curv[5] << " ";
	return out;
}

/*
 * Analyze curvature
 */
static int analyzeCurvature(
		Segmentation** segmentations, int n, 
		char* nameDirStat, char* id, char* nameCurvRaw, 
		int print_verbose = 0) 
{
	if (print_verbose) cout << "Processing curvature info " << endl;
	list<Curvature*> curvOnCut;
	list<Curvature*> curvNotOnCut;
	int nVertices = segmentations[0]->mesh->NVertices();
	int nEdges = segmentations[0]->mesh->NEdges();
			
	// compute curvature
	ifstream fin(nameCurvRaw);
	if(!fin.is_open()) {cerr << "Cannot read curvature info from " << nameCurvRaw << endl; return 0;}
	if (print_verbose) cout << "  Reading curvature info from " << nameCurvRaw << endl;
	Curvature* *curvRaw = new Curvature*[nVertices];
	for(int i = 0; i < nVertices; i ++) {
		Curvature* c = new Curvature;
		for(int j = 0; j < 6; j ++)
			fin >> c->curv[j];
		curvRaw[i] = c;
	}
	fin.close();
	
	// acquire curvature info
	for(int i = 0; i < n; i ++) {
		Segmentation* segmentation = segmentations[i];
		R3Mesh* mesh = segmentation->mesh;
		// get the mapping from segmentId to the index in segmentation->segments
		int* mapping = mapId2Segment(segmentation);
		// precompute whether a segment is too small
		bool* isTooSmall = findTooSmallSegments(segmentation);
		// clear all mark in vertices
		for(int j = 0; j < nVertices; j ++) {
			R3MeshVertex* v = mesh->Vertex(j);
			mesh->SetVertexMark(v, 0);
		}
		// scan all edges to find those vertices on or off cuts
		for(int j = 0; j < nEdges; j ++) {
			R3MeshEdge* edge = mesh->Edge(j);
			R3MeshFace* face1 = mesh->FaceOnEdge(edge, 0);
			R3MeshFace* face2 = mesh->FaceOnEdge(edge, 1);
			R3MeshVertex* v1 = mesh->VertexOnEdge(edge, 0);
			R3MeshVertex* v2 = mesh->VertexOnEdge(edge, 1);
			int id1 = mesh->FaceSegment(face1);
			int id2 = mesh->FaceSegment(face2);
			if (id1 != id2 && !isTooSmall[mapping[id1]] && !isTooSmall[mapping[id2]]) {
				// an edge on the segments'(not too small) boundary
				mesh->SetVertexMark(v1, 1);
				mesh->SetVertexMark(v2, 1);
			}
		}
		// store curvature info
		for(int j = 0; j < nVertices; j ++) {
			R3MeshVertex* v = mesh->Vertex(j);
			if(mesh->VertexMark(v) == 1)
				curvOnCut.push_back(curvRaw[j]);
			else
				curvNotOnCut.push_back(curvRaw[j]);
		}
		// clean up
		delete mapping;
		delete isTooSmall;
	}	
	
	// output 
	ofstream fout;
	list<Curvature*>::iterator it;
	string name = MakeName(nameDirStat, id, (char*)".curvOnCut");
	fout.open(name.c_str());
	for(it = curvOnCut.begin(); it != curvOnCut.end(); it ++) {
		fout << *(Curvature*)(*it) << endl;
	}
	fout.close();
	if (print_verbose) cout << "curvature on cut info dumped" << endl;
	name = MakeName(nameDirStat, id, (char*)".curvNotOnCut");
	fout.open(name.c_str());
	for(it = curvNotOnCut.begin(); it != curvNotOnCut.end(); it ++) {
		fout << *(Curvature*)(*it) << endl;
	}
	fout.close();
	if (print_verbose) cout << "curvature not on cut info dumped" << endl;
	
	// clean up
	for(int i = 0; i < nVertices; i ++)
		delete curvRaw[i];
	delete curvRaw;

	// return success
	return 1;
}
				


/****************************************
 * For analysis about convexity
 * (it needs outside input of hull information computed with Qhull)
 ***************************************/

/*
 * struct ConvexityInfo
 */
struct ConvexityInfo {
	double hullArea;
	double hullVol;
	// the following two are defined according to Kreavoy07
	double convexity;
	double compactness;
	bool isInvalid() {
		return (hullArea == -1 || hullVol == -1 || hullArea == 0.0 || hullVol == 0.0);
	}
};
ostream& operator<<(ostream& out, ConvexityInfo& c) {
	out << c.convexity << " " << c.compactness << " "
		<< c.hullArea << " " << c.hullVol;
	return out;
}

/*
 * create a scene from a mesh 
 */
R3Scene* createScene(R3Mesh* mesh) {
	
	R3Scene* scene = new R3Scene();
	assert(scene);
	
	// get vertices
	RNArray<R3TriangleVertex *> vertices;
	for(int i = 0; i < mesh->NVertices(); i ++) {
		R3MeshVertex* v = mesh->Vertex(i);
		R3Point point = mesh->VertexPosition(v);
		R3Vector normal = mesh->VertexNormal(v);
		R2Point texcoords(0, 0);		 
		R3TriangleVertex *vertex = new R3TriangleVertex(point, normal, texcoords);	            
		vertices.Insert(vertex);
	}
	
	// get nodes;
	for(int i = 0; i < mesh->NFaces(); i ++) {
		R3MeshFace* face = mesh->Face(i);
		int i0 = mesh->VertexID(mesh->VertexOnFace(face, 0));
		int i1 = mesh->VertexID(mesh->VertexOnFace(face, 1));
		int i2 = mesh->VertexID(mesh->VertexOnFace(face, 2));
		R3TriangleVertex *v0 = vertices.Kth(i0);
		R3TriangleVertex *v1 = vertices.Kth(i1);
		R3TriangleVertex *v2 = vertices.Kth(i2);
		R3Triangle *triangle = new R3Triangle(v0, v1, v2);
		R3Material *material = &R3default_material;
		R3Node *node = new R3Node(triangle, material);
		scene->InsertNode(node);				
	}
	
	return scene;
}

/*
 * distance from a face centroid to the hull along the normal direction
 */
double distance(R3Mesh* mesh, R3MeshFace* face, R3Scene* hull) {
	
	// prepare a ray
	R3Point faceCentroid = mesh->FaceCentroid(face);
	R3Vector faceNormal = mesh->FaceNormal(face);
	R3Ray ray(faceCentroid, faceNormal);
	
	// shot a ray to the hull
	R3Node *node;
	R3Point point;
	R3Vector normal;
	RNScalar t;
	bool intersect = hull->Intersects(ray, &node, &point, &normal, &t);
	if (!intersect) {
		R3Ray ray1(faceCentroid, -faceNormal); 
		hull->Intersects(ray, &node, &point, &normal, &t);
	}
	R3Vector dist = point - faceCentroid;
	return dist.Length();	
}

/*
 * compute convexity, see Kreavoy07
 */
void computeConvexity(R3Mesh* mesh, Segment* segment, R3Mesh* hull, ConvexityInfo* convInfo) {
	
	// create a scene with only the hull
	R3Scene *scene = createScene(hull);
	
	// for every face in the segment
	double sum = 0;
	for(int i = 0; i < segment->faces.NEntries(); i ++) {
		R3MeshFace* face = segment->faces[i];
		double area = mesh->FaceArea(face);
		double dist = distance(mesh, face, scene);
		sum += dist * area;
	}
	sum /= (segment->area * mesh->AverageRadius());
	convInfo->convexity = sum;
	
	delete scene;
}

/*
 * Analyze convexity
 */
static int analyzeConvexity(
		Segmentation** segmentations, int n, 
		char* nameDirStat, char* id,
		char* namePathHull,
		int print_verbose = 0) 
{
	if (print_verbose) cout << "Processing convexity/compactness info " << endl;
	list<ConvexityInfo*> convexityInfo;
	
	// compute convexity/compactness for each segmentation
	for(int i = 0; i < n; i ++) {
		
		Segmentation* segmentation = segmentations[i];
		R3Mesh* mesh = segmentation->mesh;
		int nSegments = segmentation->segments.NEntries();
		
		// precompute whether a segment is too small
		bool* isTooSmall = findTooSmallSegments(segmentation);
		
		// acquire the directory storing the hull information
		string name(id);
		string index;
		std::stringstream out; out << i;
		name += "_"; name+= out.str();
		string str = MakeName(namePathHull, name.c_str(), (char*)(""));		
		char* nameDirHull = (char*)(str.c_str());
		if(print_verbose) cout << "  ConvHull info in " << nameDirHull << endl;
		
		// read in AreaVol.txt
		ifstream fin;
		string fn = MakeName(nameDirHull, (char*)"AreaVol", (char*)".txt");
		fin.open(fn.c_str(), ios::in);
		if (!fin.is_open()) {cerr << "  Cannot locate hull's area & volume info!\n"; return 0;}
		vector<ConvexityInfo*> convexityInfoSegmentation(nSegments);
		for(int j = 0; j < nSegments; j ++) {
			ConvexityInfo* convexityInfoSegment = new ConvexityInfo;
			int indexSegment;
			fin >> indexSegment;
			if(j != indexSegment) {cerr << "  Mismatch of the segment's info!\n"; return 0;}
			fin >> convexityInfoSegment->hullArea;
			fin >> convexityInfoSegment->hullVol;
			if(print_verbose) {
				printf("   semgentation %s_%d segment %d: hullArea = %4f, hullVol = %4f\n", id, i, j, 
					convexityInfoSegment->hullArea, convexityInfoSegment->hullVol);
			}
			convexityInfoSegmentation[j] = convexityInfoSegment;
		}
		fin.close();
		if(print_verbose) {
			printf("  %d convexity info acquired for segmentation %s_%d\n",
				int(convexityInfoSegmentation.size()), id, i);
		}
		
		// deal with each segment
		for(int j = 0; j < nSegments; j ++) {
			Segment* segment = segmentation->segments[j];
			ConvexityInfo* convInfo = convexityInfoSegmentation[j];
			// exit if the segment is too small or no hull info exists
			if(isTooSmall[j] || convInfo->isInvalid()) {
				convInfo->convexity = -1;
				convInfo->compactness = -1;
				delete convInfo;
				continue;
			}
			// first, compute compactness
			convInfo->compactness = convInfo->hullArea / pow(convInfo->hullVol, double(2)/3);
			// second, compute convexity
			std::stringstream out; out << j;
			string fnHull = MakeName(nameDirHull, out.str().c_str() ,(char*)".off");
			R3Mesh* hull = ReadMesh(fnHull.c_str());
			if(!hull) { cerr << "  Cannot read hull from" << fnHull << endl; return 0;}
			computeConvexity(mesh, segment, hull, convInfo);
			if(print_verbose) {
				printf("  %s_%d segment %d: convexity = %4f, compactness = %4f\n", id, i, j, 
					convInfo->convexity, convInfo->compactness);
			}
			// add the current info (which should be valid) to the final output list
			convexityInfo.push_back(convInfo);
			delete hull;
		}
		
		// clean up
		delete isTooSmall;
		delete nameDirHull;
	}

	// output 
	ofstream fout;
	list<ConvexityInfo*>::iterator it;
	string name = MakeName(nameDirStat, id, (char*)".convexity");
	fout.open(name.c_str());
	if (!fout.is_open()) { cerr << "Cannot dump convexity/compactness info\n"; return 0; }
	for(it = convexityInfo.begin(); it != convexityInfo.end(); it ++) {
		ConvexityInfo* conv = *it;
		fout << *conv << endl;
		delete conv; 
	}
	fout.close();
	if (print_verbose) cout << "Convexity/Compactness info dumped" << endl;

	// return success
	return 1;
}



/****************************************
 * For analysis about cut length
 ***************************************/

/*
 * Class EdgesOnBoundary: to store the edges that on boundary of a segment
 */
class EdgesOnBoundary{
	R3Mesh* mesh;
	vector<R3MeshEdge*> edges;
	int lastPos;	// last index of an uncheck edge
public:
	EdgesOnBoundary(R3Mesh* m=NULL):mesh(m) {lastPos = -1;}
	int Size() {return int(edges.size());}	// the number of boundary edges 
	void Add(R3MeshEdge* e) {edges.push_back(e);}
	R3MeshEdge* Kth(int k); 
	R3MeshEdge* GetAnUncheckedEdge();
	~EdgesOnBoundary() {edges.clear();} 
};
R3MeshEdge* EdgesOnBoundary::Kth(int k) {
	if (k >= 0 && k < Size()) {
		return edges[k];
	}
	return NULL;
}
R3MeshEdge* EdgesOnBoundary::GetAnUncheckedEdge() {
	for(int i = (lastPos+1); i < int(edges.size()); i ++) {
		R3MeshEdge* edge = edges[i];
		if(mesh->EdgeMark(edge) == 0) {lastPos = i; return edge;}
	}
	return NULL;
}

/*
 * Class Cut
 */
struct Node{
	R3MeshEdge *data;
	Node* prev;
	Node* next;
	Node(R3MeshEdge *t, Node *p = NULL, Node *n = NULL):data(t),prev(p),next(n) {}
};

class Cut {
	R3Mesh *mesh;
	Node *head;
	double len; // total length
	int nEdges; // number of edges
	int segmentId; // the id of the segment that contains this cut
public:
	Cut(R3Mesh* m = NULL, int segId=-1) {mesh = m; head = NULL; len = 0; nEdges = 0; segmentId = segId;} 
	void Add(Node* n);
	double Length() { return len; }
	int Size() {return nEdges; }
	int HeadId() { return mesh->EdgeID(head->data); }
	void Adjust();
	void Print();
	int GetSegmentId() {return segmentId;}
	~Cut();
};
void Cut::Add(Node* n)  {
	nEdges ++;
	len += mesh->EdgeLength(n->data);
	if(head == NULL) { head = n; head->prev = head->next = head;}
	else {n->next = head->next; n->prev = head; head->next->prev = n; head->next = n;}
}
void Cut::Adjust()  { 
	// to make head the one with the lowest id number
	int minId = mesh->EdgeID(head->data);
	Node *newhead = head;
	Node *p = head->next;
	while(p != head) {
		int id = mesh->EdgeID(p->data);
		if(id < minId) {minId = id; newhead = p;}
		p = p->next;
	}
	head = newhead;
	// id of head->next should be lower than head->prev
	if(mesh->EdgeID(head->next->data) > mesh->EdgeID(head->prev->data)) {
		Node* p = head;
		do{ // swap the next and prev pointer of each node
			Node* next = p->next;
			Node* tmp = p->prev; p->prev = p->next; p->next = tmp;
			p = next;
		}while(p != head);
	}
}
void Cut::Print() {
	Node *p = head;
	int count = 0;
	do{
		printf("%d\t", mesh->EdgeID(p->data));
		if(count == 10) { count = 0; cout << endl;}
		p = p->next;
	}while(p != head);
	cout <<  mesh->EdgeID(p->data) << endl;
}
Cut::~Cut() {
	head->prev->next = NULL;
	Node *p = head, *q;
	while(p != NULL) { q = p->next; delete p; p = q; }
}

/*
 * whether an edge is on cut of a segment
 */
bool isOnCut(R3MeshEdge* edge, R3Mesh* mesh, int id) {
	R3MeshFace* f1 = mesh->FaceOnEdge(edge, 0);
	R3MeshFace* f2 = mesh->FaceOnEdge(edge, 1);
	int id1 = mesh->FaceSegment(f1);
	int id2 = mesh->FaceSegment(f2);
	return (f1 && f2 && (id1 != id2) && (id1 == id || id2 == id));
}

/*
 * find cuts of a segment 
 */
void findCutsInSegment(Segment *segment, EdgesOnBoundary* edgesOnBoundary, 
		R3Mesh *mesh, list<Cut*>& cuts, int print_verbose=0) {
	// Assume that no two cuts within the same segment share a same edge, 
	// which is true if the mesh is pre-processed
	
	// set marks to 0 for all edges on boundaries
	for(int i = 0; i < edgesOnBoundary->Size(); i ++) {
		R3MeshEdge* edge = edgesOnBoundary->Kth(i);
		mesh->SetEdgeMark(edge, 0);
	}
	
	// looking for loops
	int segmentId = segment->id;
	if(print_verbose) printf("  seeking for cuts in segment %d\n", segmentId);
	R3MeshEdge* seed = edgesOnBoundary->GetAnUncheckedEdge();
	while(seed) {
		// looking for a loop that contains seed
		Cut* cut = new Cut(mesh, segmentId);
		Node* node = new Node(seed);
		cut->Add(node);
		R3MeshVertex* vStart = mesh->VertexOnEdge(seed, 0);
		R3MeshVertex* v1 = mesh->VertexOnEdge(seed, 1);
		R3MeshEdge* previousEdge = seed;
		int length = 0;
		do {
			int n = mesh->VertexValence(v1);
			for(int i = 0; i < n; i ++) {
				R3MeshEdge* edge = mesh->EdgeOnVertex(v1, i);
				if(edge != previousEdge && isOnCut(edge, mesh, segmentId)) { // should not go back
					mesh->SetEdgeMark(edge, 1); // the edge has been processed
					Node* node = new Node(edge);
					cut->Add(node);
					length ++;
					previousEdge = edge;
					v1 = mesh->VertexAcrossEdge(edge, v1);
					break;
				}
			}
		} while(v1 != vStart && length < edgesOnBoundary->Size());
		// a cut is found, add it to cuts
		if (v1 == vStart && length <= edgesOnBoundary->Size()) {
			// if(print_verbose) cut->Print();
			cut->Adjust();
			// if(print_verbose) cut->Print();
			cuts.push_back(cut);
			seed = edgesOnBoundary->GetAnUncheckedEdge();
		}
		else {// if an unexpected loop is detected, stop dealing with the current segment
			delete cut;
			seed = NULL;
			if (print_verbose) printf("  A dead loop is found on segment %d\n", segmentId);
		}
	}	
}

/*
 * find all cuts within a segmentation
 */
void findAllCuts(Segmentation* segmentation, list<double>& CutLengths, 
		EdgesOnBoundary* edgesOnBoundary[], int* mapping, bool* isTooSmall,
		int print_verbose=0) 
{
	list<Cut*> l;
	R3Mesh* mesh = segmentation->mesh;
	int nsegments = segmentation->segments.NEntries();
		
	// find all cuts for each segment
	for(int i = 0; i < nsegments; i ++) 
		findCutsInSegment(segmentation->segments[i], edgesOnBoundary[i], mesh, l, print_verbose);
	
	// sort Cuts according to headId
	vector<Cut*> v;
	list<Cut*>::iterator it;
	int n = l.size();
	if (n == 0) return;
	for(it = l.begin(); it != l.end(); it++ )
		v.push_back(*it);
	for(int i = 0; i < n-1; i ++) {
		for(int j = i+1; j < n; j ++) {
			Cut* cutI = (Cut*)v[i];
			Cut* cutJ = (Cut*)v[j];
			if(cutI->HeadId() > cutJ->HeadId()) {
				v[i] = cutJ; v[j] = cutI;
			}
		}
	}
	
	// print cuts found before delete repeated ones
	if(print_verbose) {
		printf("  Before delete repeated cuts, %d cuts are found from %d segments:\n", int(v.size()), nsegments);
		for(int i = 0; i < n; i ++) {
			Cut* current = (Cut*)v[i];
			printf("    Cut %3d: HeadId = %5d, Length = %.5f, nEdges = %d\n", 
					i, current->HeadId(), current->Length(), current->Size());
		}
	}
	
	// delete repeated cuts as well as trivial cuts(from a tooSmallFace) in the sorted v
	bool* toBeAdd = new bool[n];
	for(int i = 0; i < n ; i ++) toBeAdd[i] = true;
	int first = 0;
	do {
		// search a group of repeated cuts
		bool Small = isTooSmall[mapping[((Cut*)v[first])->GetSegmentId()]];
		int index = first + 1;
		while(index < n) {
			// judge whether the current Cut is the same of the previous one
			Cut* prev = (Cut*)v[index-1];
			Cut* current = (Cut*)v[index];
			if(current->HeadId() == prev->HeadId() 
				&& current->Size() == prev->Size()
				&& equal(current->Length(), prev->Length())) 
			{
				Small = Small || isTooSmall[mapping[current->GetSegmentId()]];
				index ++;
			}
			else
				break;
		}
		for(int i = first+1; i < index; i ++)
			toBeAdd[i] = false;
		if(Small) toBeAdd[first] = false;
		first = index;
	} while(first < n);
	
	// add lengths of the unique cuts to CutLengths
	int nUniqueCuts = 0;
	for(int i = 0; i < n; i ++) {
		if(toBeAdd[i]) {
			nUniqueCuts ++;
			Cut* c = (Cut*)v[i];
			CutLengths.push_back(c->Length());
		}
	}
	
	// print unique cuts
	if(print_verbose) {
		printf("  After delete repeated cuts, %d unique cuts are found from %d segments:\n", nUniqueCuts, nsegments);
		for(int i = 0; i < n; i ++) {
			if (toBeAdd[i]) {
				Cut* current = (Cut*)v[i];
				printf("    Cut %3d: HeadId = %5d, Length = %.5f, nEdges = %d\n", 
					i, current->HeadId(), current->Length(), current->Size());
			}
		}
	}
	
	// clean up
	for(int i = 0; i < n; i ++) {
		Cut* current = (Cut*)v[i];
		delete current;
	}
	delete toBeAdd;
}

/*
 * Analyze cut length
 */
static int analyzeCutLength(
		Segmentation** segmentations, int n, 
		char* nameDirStat, char* id, 
		int print_verbose = 0) 
{
	if (print_verbose) cout << "Processing length info " << endl;
	list<double> length;
	list<double> lengthRatio;
	int nEdges = segmentations[0]->mesh->NEdges();
	
	// acquire length info
	for(int i = 0; i < n; i ++) {
		Segmentation* segmentation = segmentations[i];
		R3Mesh* mesh = segmentation->mesh;
		int nSegments = segmentation->segments.NEntries();
		// get the mapping from segmentId to the index in segmentation->segments
		int* mapping = mapId2Segment(segmentation);
		// precompute whether a segment is too small
		bool* isTooSmall = findTooSmallSegments(segmentation);
		
		// get all edges on boundaries
		EdgesOnBoundary** edgesOnBoundary = new EdgesOnBoundary*[nSegments];
		for(int j = 0; j < nSegments; j ++) edgesOnBoundary[j] = new EdgesOnBoundary(mesh);
		for(int j = 0; j < nEdges; j ++) {
			R3MeshEdge* edge = mesh->Edge(j);
			mesh->SetEdgeMark(edge, 0);
			R3MeshFace* face1 = mesh->FaceOnEdge(edge, 0);
			R3MeshFace* face2 = mesh->FaceOnEdge(edge, 1);
			int id1 = mesh->FaceSegment(face1);
			int id2 = mesh->FaceSegment(face2);
			if(id1 != id2) {
				edgesOnBoundary[mapping[id1]]->Add(edge);
				edgesOnBoundary[mapping[id2]]->Add(edge);
			}
		}
		
		// get all unique boundaries and non-trivial of each segments
		list<double> CutLengths;
		if (print_verbose) cout << "  Seek for cuts in segmenation " << i << endl;
		findAllCuts(segmentation, CutLengths, edgesOnBoundary, mapping, isTooSmall, print_verbose);
		
		// process results
		double radius = mesh->AverageRadius();
		double avgLength = 0;
		list<double>::iterator it;
		for(it = CutLengths.begin(); it != CutLengths.end(); it ++) {
			double a = double(*it);
			length.push_back(a/radius);
			avgLength += a;
		}
		avgLength /= CutLengths.size();
		for(it = CutLengths.begin(); it != CutLengths.end(); it ++) {
			double a = double(*it);
			lengthRatio.push_back(a/avgLength);
		}
		
		// clean up
		for(int j = 0; j < nSegments; j ++) delete edgesOnBoundary[j];	
		delete edgesOnBoundary; 	
		delete mapping;
		delete isTooSmall;
	}
		
	// output
	ofstream fout;
	list<double>::iterator it;
	// length
	string name = MakeName(nameDirStat, id, (char*)".length");
	fout.open(name.c_str());
	for(it = length.begin(); it != length.end(); it ++) {
		fout << (*it) << endl;
	}
	fout.close();
	if (print_verbose) cout << "Cut length info dumped" << endl;
	// length ratio
	name = MakeName(nameDirStat, id, (char*)".lengthRatio");
	fout.open(name.c_str());
	for(it = lengthRatio.begin(); it != lengthRatio.end(); it ++) {
		fout << (*it) << endl;
	}
	fout.close();
	if (print_verbose) cout << "Cut length ratio dumped" << endl;
	
	// return success
	return 1;
}



/****************************************
 * For analysis about numSegments
 ***************************************/

/*
 * Analyze number of segments
 */

static int analyzeNumSegments(
                Segmentation** segmentations, int n,
                char* nameDirStat, char* id,
                int print_verbose = 0)
{
        if (print_verbose) cout << "Processing numSegments info " << endl;
        int* numAllSegments = new int[n];
		int* numBigSegments = new int[n]; 

        // acquiring statistics
        for(int i = 0; i < n; i ++) {
                Segmentation* segmentation = segmentations[i];
                bool* isTooSmall = findTooSmallSegments(segmentation);
		int nsegments = segmentation->segments.NEntries();
                numAllSegments[i] = nsegments;
		numBigSegments[i] = nsegments;
		for (int j = 0; j < nsegments; j ++)
			if(isTooSmall[j]) numBigSegments[i] --;
		// clean up
                delete isTooSmall;
        }

        // output results
        ofstream fout;
        string name = MakeName(nameDirStat, id, (char*)".nseg");
	fout.open(name.c_str());
        for(int i = 0; i < n; i ++) { 
                fout << numAllSegments[i] << '\t' << numBigSegments[i] << endl;
        }
        fout.close();
	
	// clean up
	delete []numAllSegments;
	delete []numBigSegments;

	// return success
        if (print_verbose)      cout << "NumSegments info dumped" << endl;
	return 1;
}



#endif /*ANALYSISMETHODS_H_*/

