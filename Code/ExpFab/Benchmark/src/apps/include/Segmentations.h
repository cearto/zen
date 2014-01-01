/*
 * Segmentations.h
 * 
 * Type definition of Segment, Segmentation, as well as
 * functions to read mesh, and create segmentation from 3D model
 */


#ifndef SEGMENTATIONS_
#define SEGMENTATIONS_


/*
 * Include files
 */
#include "R3Shapes/R3Shapes.h"
#include <fstream>
using namespace std;

static int print_verbose = 0;
static int print_debug = 0;


/*
 * Definition for Segment
 */
struct Segment {
	Segment(void) : faces(), area(0), index(-1), id(0), parent(-1) { name[0]=0; };
	RNArray<R3MeshFace *> faces;
	RNScalar area;
	char name[256];
	int index;
	int id;
	int parent;
	~Segment() {faces.Empty();};
};



/*
 * Definition for Segmentation
 */
struct Segmentation {
	R3Mesh *mesh;
	RNArray<Segment *> segments;
	RNScalar area;
	Segmentation(R3Mesh *mesh) : mesh(mesh), segments(), area(0) {};
	Segment* findSegById(int id);
	~Segmentation();
};

Segment* Segmentation::findSegById(int id){
	int n = segments.NEntries();
	for(int i = 0; i < n; i ++){
		Segment* seg = segments.Kth(i);
		if(seg->id == id) return seg;
	}
	return NULL;
}

Segmentation::~Segmentation() {
	int n = segments.NEntries();
	for (int i = 0; i < n; i ++) delete segments[i];
	segments.Empty();
}



/*
 * Read in the Mesh
 */
static R3Mesh *
ReadMesh(const char *filename){

	// Start statistics
	RNTime start_time;
	start_time.Read();

	// Allocate mesh
	R3Mesh *mesh = new R3Mesh();
	assert(mesh);

	// Read mesh from file
	if (!mesh->ReadFile(filename)) {
		delete mesh;
		return NULL;
	}

	// Check if mesh is valid
	assert(mesh->IsValid());

	// Print statistics
	if (print_verbose) {
		printf("Read mesh from %s ...\n", filename);
		printf("  Time = %.2f seconds\n", start_time.Elapsed());
		printf("  # Faces = %d\n", mesh->NFaces());
		printf("  # Edges = %d\n", mesh->NEdges());
		printf("  # Vertices = %d\n", mesh->NVertices());
		fflush(stdout);
	}

	// Return mesh
	return mesh;
}

/*
 * Read in a mesh with segment info
 * fnSeg correspondes to each face in fnModel(the ply file)
 */
static R3Mesh* ReadMesh(const char* fnModel, const char* fnSeg) {
	
	// Start Statistics
	RNTime start_time;
	start_time.Read();

	// Allocate mesh
	R3Mesh* mesh = new R3Mesh();
	assert(mesh);

	// Read mesh
	if (!mesh->ReadFile(fnModel)) {
		delete mesh;
		return NULL;
	}
	assert(mesh->IsValid());

	// Read segmentation info
	ifstream fin;
	fin.open(fnSeg, ios::in);
	if (!fin.is_open()) {printf("Cannot read seg info from %s\n", fnSeg); exit(1); }
	int i = 0;
	int seg;
	while(!fin.eof() && i < mesh->NFaces()) {
		R3MeshFace* face = mesh->Face(i);
		fin >> seg;
		mesh->SetFaceSegment(face, seg);
		i ++;
	}
	if (i != mesh->NFaces()) { fprintf(stderr, "Input error!\n"); delete mesh; return NULL; }
	if(print_verbose) {
		printf("Read mesh from %s and %s ...\n", fnModel, fnSeg);
		printf("  Time = %.2f seconds\n", start_time.Elapsed());
		fflush(stdout);	
	}

	// Return
	fin.close();
	return mesh; 
}


/*
 * Write out the mesh
 */
static int WriteMesh(R3Mesh* mesh, const char* filename) {

        // Start statistics
        RNTime start_time;
        start_time.Read();

        // Write out mesh
        mesh->WriteFile(filename);

        // Print Statistics
        if (print_verbose) {
                printf("Write mesh to %s ...\n", filename);
                printf("  Time = %.2f seconds\n", start_time.Elapsed());
                printf("  # Faces = %d\n", mesh->NFaces());
                printf("  # Edges = %d\n", mesh->NEdges());
                printf("  # Vertices = %d\n", mesh->NVertices());
                fflush(stdout);
        }

        // return success
        return 1;
}



/*
 * Get Segmentation
 */
static Segmentation *
CreateSegmentation(R3Mesh *mesh){

	// Start statistics
	RNTime start_time;
	start_time.Read();

	// Allocate segmentation
  	Segmentation *segmentation = new Segmentation(mesh);
  	if (!segmentation) {
  		fprintf(stderr, "Unable to allocate segmentation\n");
  		return NULL;
  	}

  	// Allocate mapping from segment ID to segment
  	const int MAX_SEGMENTS = 1024 * 1024;
  	Segment **map = new Segment * [MAX_SEGMENTS];
  	for (int i = 0; i < MAX_SEGMENTS; i++) map[i] = NULL;

  	// Assign each face to segment
  	for (int i = 0; i < mesh->NFaces(); i++) {
  		R3MeshFace *face = mesh->Face(i);
  		int id = mesh->FaceSegment(face);
  		if (id >= MAX_SEGMENTS) id = MAX_SEGMENTS-1;

  		// Create segment
  		Segment *segment = map[id];
  		if (!segment) {
  			segment = new Segment();
  			segment->index = segmentation->segments.NEntries();
  			segment->id = id;
  			segmentation->segments.Insert(segment);
  			map[id] = segment;
  		}

  		// Add face to segment
  		RNScalar area = mesh->FaceArea(face);
  		mesh->SetFaceData(face, segment);
  		segment->faces.Insert(face);
  		segment->area += area;
  		segmentation->area += area;
  	}

  	// Deallocate mapping from segment ID to segment
  	delete [] map;

  	// Print statistics
  	if (print_verbose) {
  		printf("Created segmentation ...\n");
  		printf("  Time = %.2f seconds\n", start_time.Elapsed());
  		printf("  Area = %g\n", segmentation->area);
		printf("  # Segments = %d\n", segmentation->segments.NEntries());
		for (int i = 0; i < segmentation->segments.NEntries(); i++) {
			Segment *segment = segmentation->segments.Kth(i);
			printf("    %d : %d (%.2f%%) %g (%.2f%%)\n", i,
				segment->faces.NEntries(), 100.0 * segment->faces.NEntries() / segmentation->mesh->NFaces(),
				segment->area, (segmentation->area > 0) ? 100.0 * segment->area / segmentation->area : 0);
		}
		fflush(stdout);
  	}

  	// Return segmentation
  	return segmentation;
}

int OutputSegmentation(R3Mesh* mesh, const char* filename) {
	ofstream fout;
	fout.open(filename, ios::out);
	if(!fout.is_open()) return 0;
	for(int i = 0; i < mesh->NFaces(); i ++) {
		R3MeshFace* face = mesh->Face(i);
		int id = mesh->FaceSegment(face);
		fout << id << endl;
	}
	fout.close();
	return 1;
}


#endif /*SEGMENTATIONS_*/
