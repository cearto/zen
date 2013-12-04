#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include "R3Shapes/R3Shapes.h"
#include "Segmentations.h"
using namespace std;

/*
 * Global variables
 */
char* fnInput = NULL;
char* fnSeg = NULL;
char* dirOutput = NULL;


/*
 * Make a filename by combine dir, name and suffix
 */
string MakeName(char* dir, const char* name, char* suffix) {
	string fn = dir;
	if(fn.at(fn.length()-1) != '/') fn += "/";
	fn += name;
	fn += suffix;
	return fn;
}


/*
 * Set marks of vertices belonging to a segment
 */
void setVertexMarks(R3Mesh* mesh, Segment* segment, int mark) {
	int nFaces = segment->faces.NEntries();
	for(int i = 0; i < nFaces; i ++) {
		R3MeshFace* face = segment->faces[i];
		for(int j = 0; j < 3; j ++) {
			R3MeshVertex* v = mesh->VertexOnFace(face, j);
			mesh->SetVertexMark(v, mark);
		}
	}
}

/*
 * output segments
 */
void outputSegments(Segmentation* segmentation) {
	
	// Start statistics
	RNTime start_time;
	start_time.Read();
	
	// get parameters
	int nSegments = segmentation->segments.NEntries();
	R3Mesh* mesh = segmentation->mesh;
	
	// output each segment
	for(int i = 0; i < nSegments; i ++) {

		// get info about the current segment
		Segment* segment = segmentation->segments[i];
		int nFaces = segment->faces.NEntries();
		
		// initialize marks for all vertices in the segment
		setVertexMarks(mesh, segment, 0);
		
		// add vertices to a list
		list<R3MeshVertex*> vertices;
		for(int j = 0; j < nFaces; j ++) {
			R3MeshFace* face = segment->faces[j];
			for(int k = 0; k < 3; k ++) {
				R3MeshVertex* v = mesh->VertexOnFace(face, k);
				if (mesh->VertexMark(v) == 0) {
					mesh->SetVertexMark(v, 1);
					vertices.push_back(v);
				}
			}
		}
		
		// output to a file
		std::stringstream s; s << i;
		string name = MakeName(dirOutput, s.str().c_str(), (char*)".segment");
		ofstream fout;
		fout.open(name.c_str(), ios::out);
		if(!fout.is_open()) {cerr << "Cannot create file " <<  name << endl; exit(-1);}
		fout << vertices.size() << endl;
		list<R3MeshVertex*>::iterator it;
		for(it = vertices.begin(); it != vertices.end(); it ++) {
			R3MeshVertex* v = (R3MeshVertex*)(*it);
			R3Point p = mesh->VertexPosition(v);
			fout << p.X() << "\t" << p.Y() << "\t" << p.Z() << endl;
		}
		fout.close();
	}

	
	if (print_verbose) {
	  	printf("output segments ...\n");
	  	printf("  Time = %.2f seconds\n", start_time.Elapsed());
	}
}

/*
 * ParseArgs
 */
int ParseArgs(int argc, char **argv){

	// Check number of arguments
	if (argc < 3) {
		printf("Usage: outputSegments fnMesh [-seg fnSeg] dirOutput [-v]\n"); exit(0);
	}

	// Parse arguments
	argc--; argv++;
	while (argc > 0) {
		if ((*argv)[0] == '-') {
			if (!strcmp(*argv, "-v")) print_verbose = 1;
			else if (!strcmp(*argv, "-seg")) {argc--; argv++; fnSeg = *argv;}
			else { fprintf(stderr, "Invalid program argument: %s", *argv); exit(1); }
			argv++; argc--;
		}
		else {
			if (!fnInput) fnInput = *argv;
			else if (!dirOutput) dirOutput = *argv;
			else { fprintf(stderr, "Invalid program argument: %s", *argv); exit(1); }
			argv++; argc--;
		}
	}
	
	// Check input filename
	if (!fnInput || !dirOutput) {
		fprintf(stderr, "Please specify the input and output file names!\n"); return 0;
	}
	
	// Return OK status
	return 1;
}

/*
 * Main
 */
int main(int argc, char **argv) {
	
	// parse arguments
	if(!ParseArgs(argc, argv)) exit(-1);
	
	// read in the mesh
	R3Mesh *mesh;
	if(fnSeg) mesh = ReadMesh(fnInput, fnSeg);
	else mesh = ReadMesh(fnInput);
	if (!mesh) { cerr << "Cannot read " << fnInput << endl; exit(-1);}
	Segmentation *segmentation = CreateSegmentation(mesh);
	
	// output segments
	outputSegments(segmentation);
	
	// clean up
	delete mesh;
	delete segmentation;
	
	// return success
	return 1;
}

