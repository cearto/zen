/* 
 * Pre-process a segmentation:
 * 1. map the segmentation from one mesh to another(the standard one)
 * 2. Assign different segment numbers to disconnected parts, 
 * at the same time, guarantee that segment numbers are from
 * 0 to (numSegment-1)
 */
#include "R3Shapes/R3Shapes.h"
#include "Segmentations.h"

static char* fnMeshIn = NULL;	// the original mesh file
static char* fnSegIn = NULL;	// the original seg file
static char* fnSegOut = NULL;	// the output seg file
static char* fnMeshOut = NULL;	// the mesh geometry to be mapped on
RNMark mesh_mark = 0;

static int
TransferSegmentation(R3Mesh *to_mesh, R3Mesh *from_mesh) {
  
	// Start statistics
	RNTime start_time;
	start_time.Read();

	// Build search tree
	R3MeshSearchTree from_tree(from_mesh);

	// Determine segment for each face of to_mesh
	R3MeshIntersection closest;
	for (int i = 0; i < to_mesh->NFaces(); i++) {
		R3MeshFace *to_face = to_mesh->Face(i);
		R3Point to_centroid = to_mesh->FaceCentroid(to_face);

		// Find closest face in from_mesh
		from_tree.FindClosest(to_centroid, closest);
		R3MeshFace *from_face = closest.face;
		assert(from_face);

		// Transfer segment id
		int segment = from_mesh->FaceSegment(from_face);
		to_mesh->SetFaceSegment(to_face, segment);
	}

	// Print statistics
	if (print_verbose) {
		printf("Transfered segmentation ...\n");
		printf("  Time = %.2f seconds\n", start_time.Elapsed());
		printf("  # Tree Nodes = %d\n", from_tree.NNodes());
		fflush(stdout);
	}

	// Return success
	return 1;
}

/*
 * Find connected parts within a segment
 */
void FindConnectedFaces(R3MeshFace* seed, RNArray<R3MeshFace *>& faces, Segment* segment, R3Mesh* mesh){	
	RNArray<R3MeshFace *> stack;
	stack.Insert(seed);
	int segid = mesh->FaceSegment(seed);
	while (!stack.IsEmpty()) {
		R3MeshFace *face = stack.Tail();
		stack.RemoveTail();
		if (mesh->FaceMark(face) == mesh_mark) {
			faces.Insert(face);
			for (int j = 0; j < 3; j ++){
				R3MeshFace* f = mesh->FaceOnFace(face, j);
				if(f && segid == mesh->FaceSegment(f) && mesh->FaceMark(face) == mesh_mark) stack.Insert(f);			
			}
			mesh->SetFaceMark(face, mesh_mark + 1);
		}
	}
}


/*
 * Find an unconsidered face
 */
R3MeshFace* FindAFace(Segment* segment, R3Mesh* mesh){
	for(int i = 0; i < segment->faces.NEntries(); i ++){
		R3MeshFace* f = segment->faces.Kth(i);
		if (mesh->FaceMark(f) == mesh_mark) return f;
	}
	return NULL;
}


/*
 * Set segment id to a group of faces
 */
void SetSegmentId(RNArray<R3MeshFace*> faces, int id, R3Mesh* mesh){
	for (int i = 0; i < faces.NEntries(); i ++){
		R3MeshFace* f = faces.Kth(i);
		mesh->SetFaceSegment(f, id);
	}
}


/*
 * Seperate parts
 */
static int SeparateParts(R3Mesh *mesh){
	
	// Start statistics
	RNTime start_time;
	start_time.Read();
	bool changed = false;

	// Create segmentation
	Segmentation *s1 = CreateSegmentation(mesh);
	if (!s1) exit(-1);
	int nSeg1 = s1->segments.NEntries();	
	// adjust segment ids
        for(int i = 0; i < nSeg1; i ++) {
                Segment* segment = s1->segments[i];
                for(int j = 0; j < segment->faces.NEntries(); j ++) {
                        R3MeshFace* face = segment->faces[j];
                        mesh->SetFaceSegment(face, i);
                        mesh->SetFaceMark(face, mesh_mark);
                }
        }

	// Control what to name the next segment
	int next = nSeg1;
	
	// Deal with each segments
	for(int i = 0; i < nSeg1; i ++){
		
		// pick this segment
		Segment* segment = s1->segments.Kth(i);
		int nFaces = segment->faces.NEntries();
		
		// pick a random face, find its connected parts
		R3MeshFace* seed = segment->faces.Kth(0);
		RNArray<R3MeshFace *> connected;
		FindConnectedFaces(seed, connected, segment, mesh);
		int considered = connected.NEntries();
		
		// continue work on the other parts
		while (considered < nFaces) { // there are unconnected parts
			changed = true;
			connected.Empty();
			seed = FindAFace(segment, mesh);
			FindConnectedFaces(seed, connected, segment, mesh);
			SetSegmentId(connected, next, mesh);
			next ++;
			considered += connected.NEntries();
		}		
		
		// clean up
		connected.Empty();
	}
	
	// Print statistics
	if (print_verbose) {
  		printf("Separate parts ...\n");
  		printf("  Time = %.2f seconds\n", start_time.Elapsed());
  		printf("  # Segments old = %d\n", s1->segments.NEntries());
		for (int i = 0; i < s1->segments.NEntries(); i++) {
			Segment *segment = s1->segments.Kth(i);
			printf("    %3d : %5d (%.2f%%) %g (%.2f%%)\n", i,
				segment->faces.NEntries(), 100.0 * segment->faces.NEntries() / s1->mesh->NFaces(),
				segment->area, (s1->area > 0) ? 100.0 * segment->area / s1->area : 0);
		}
		Segmentation *s2 = CreateSegmentation(mesh);
		if(!s2) exit(-1);
		int nSeg2 = s2->segments.NEntries();
		printf("  # Segments new = %d\n", nSeg2);
		for (int i = 0; i < nSeg2; i++) {
			Segment *segment = s2->segments.Kth(i);
			printf("    %3d : %5d (%.2f%%) %g (%.2f%%)\n", i,
				segment->faces.NEntries(), 100.0 * segment->faces.NEntries() / s2->mesh->NFaces(),
				segment->area, (s2->area > 0) ? 100.0 * segment->area / s2->area : 0);
		}
		fflush(stdout);
		// clean up
		delete s2;
  	}
	
	//clean up
	delete	s1;

	// return success
	if (!changed) return 1;
	else return 2; 
}


static int 
ParseArgs(int argc, char **argv) {
	// Check number of arguments
	if (argc == 1) {
		printf("Usage: segPreprocess fnMeshIn fnSegIn fnSegOut fnMeshOut [options]\n");
		exit(0);
	}

	// Parse arguments
	argc--; argv++;
	while (argc > 0) {
		if ((*argv)[0] == '-') {
			if (!strcmp(*argv, "-v")) print_verbose = 1;
			else if (!strcmp(*argv, "-debug")) print_debug = 1;
			argv++; argc--;
		}
		else {
			if (!fnMeshIn) fnMeshIn = *argv;
			else if (!fnSegIn) fnSegIn = *argv;
			else if (!fnSegOut) fnSegOut = *argv;
			else if (!fnMeshOut) fnMeshOut = *argv;
			else {fprintf(stderr, "Invalid program argument: %s", *argv); exit(1); }
			argv++; argc--;
		}
	}

	// Check input filename
	if (!fnMeshIn || !fnSegIn || !fnSegOut || !fnMeshOut) {
		fprintf(stderr, "missing input!\n");
		return 0;
	}

	// Return OK status 
	return 1;
}


int 
main(int argc, char **argv) {

	// Check number of arguments
	if (!ParseArgs(argc, argv)) exit(1);

	// Read input mesh
	R3Mesh *mesh = ReadMesh(fnMeshIn, fnSegIn);
	if (!mesh) exit(-1);

	// Read source mesh
   	R3Mesh *source_mesh = ReadMesh(fnMeshOut);
   	if (!source_mesh) exit(-1);

   	// Transfer segmentation from source
   	if (!TransferSegmentation(source_mesh, mesh)) exit(-1);
  
	// Separate parts
	if (!SeparateParts(mesh)) exit(-1);

	// Output results
	if (!OutputSegmentation(mesh, fnSegOut)) exit(-1);

	// Clean up
	delete mesh;
	delete source_mesh;

	// Return success 
	return 0;
}

