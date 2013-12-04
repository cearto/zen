#ifndef EVALUATIONMETHODS_
#define EVALUATIONMETHODS_
#include "Segmentations.h"
#include "Entry.h"

/**************************************************************************************************
 **************************************************************************************************
 * EvalMethod 1 : Consistency Error ***************************************************************
 **************************************************************************************************
 **************************************************************************************************/

/*
 * Evaluate both global and local consistency error (GCE & LCE)
 * see Martin et. al in ICCV 01'
 */
struct Entry_CE*
EvaluateConsistencyError(Segmentation* s1, Segmentation* s2){

	// Start statistics
	RNTime start_time;
	start_time.Read();

	int i, j;
	double GCE = 0, LCE = 0, GCEa = 0, LCEa = 0;

	// Number of segments in s1 and s2
	int nSeg1 = s1->segments.NEntries();
	int nSeg2 = s2->segments.NEntries();

	// Number of faces in segments(should be the same for s1 and s2)
	int nFaces = s1->mesh->NFaces();
	
	// only compute under non-trial cases, otherwise, consistency errors are just 0
	if(nSeg1 != 1 && nSeg2 != 1 && nSeg1 != nFaces && nSeg2 != nFaces) {
		// Allocate metrics to store the difference between sets of segments
		// numFaces based
		double *SetDifference12 = new double[nSeg1*nSeg2];		// s1 sets over s2 sets
		double *SetDifference21 = new double[nSeg2*nSeg1];		// s2 sets over s1 sets
		double *Intersection = new double[nSeg1*nSeg2];			
		// area based
		double *AreaDifference12 = new double[nSeg1*nSeg2];		// s1 areas over s2 areas
		double *AreaDifference21 = new double[nSeg2*nSeg1];		// s2 areas over s1 areas
		double *AreaIntersection = new double[nSeg1*nSeg2];
		for(i = 0; i < nSeg1*nSeg2; i ++){
			// numFaces based
			SetDifference12[i] = 0;
			Intersection[i] = 0;
			SetDifference21[i] = 0;
			// area based
			AreaDifference12[i] = 0;
			AreaIntersection[i] = 0;
			AreaDifference21[i] = 0;
		}
		
		// get intersections
		for(int k = 0; k < nFaces; k ++) {
			R3MeshFace *f1 = s1->mesh->Face(k);
			R3MeshFace *f2 = s2->mesh->Face(k);
			// assume that a is within 0..(nSeg1-1), b is within 0..(nSeg2-1)
			int a = s1->mesh->FaceSegment(f1);
			int b = s2->mesh->FaceSegment(f2);
			Intersection[a*nSeg2+b] += 1;
			AreaIntersection[a*nSeg2+b] += s1->mesh->FaceArea(f1);	
		}
		
		// get normalized set/area differences
		// faces in segments[i] does not necessarily have the segment id i
		double *nSegFaces1 = new double[nSeg1];
		double *nSegFaces2 = new double[nSeg2];
		double *areaSeg1 = new double[nSeg1];
		double *areaSeg2 = new double[nSeg2];
		for(i = 0; i < nSeg1; i ++) {
			R3MeshFace *f = s1->segments[i]->faces[0];
			int segId = s1->mesh->FaceSegment(f);
			nSegFaces1[segId] = s1->segments[i]->faces.NEntries();
			areaSeg1[segId] = s1->segments[i]->area;
		}
		for(i = 0; i < nSeg2; i ++) {
			R3MeshFace *f = s2->segments[i]->faces[0];
			int segId = s2->mesh->FaceSegment(f);
			nSegFaces2[segId] = s2->segments[i]->faces.NEntries();
			areaSeg2[segId] = s2->segments[i]->area;
		}
		for(i = 0; i < nSeg1; i ++) {
			for(j = 0; j < nSeg2; j ++) {
				SetDifference12[i*nSeg2+j] = 1 - Intersection[i*nSeg2+j]/nSegFaces1[i];
				SetDifference21[j*nSeg1+i] = 1 - Intersection[i*nSeg2+j]/nSegFaces2[j];
				AreaDifference12[i*nSeg2+j] = 1 - AreaIntersection[i*nSeg2+j]/areaSeg1[i];
				AreaDifference21[j*nSeg1+i] = 1 - AreaIntersection[i*nSeg2+j]/areaSeg2[j];
			}
		}
		
		// get GCE/LCE/GCEa/LCEa
		double sumE12 = 0, sumE21 = 0, sumMin = 0;
		double sumEa12 = 0, sumEa21 = 0, sumMina = 0;
		for(i = 0; i < nSeg1; i ++) {
			for(j = 0; j < nSeg2; j ++) {
				int frequency = int(Intersection[i*nSeg2+j]);
				sumE12 += SetDifference12[i*nSeg2+j] * frequency;
				sumE21 += SetDifference21[j*nSeg1+i] * frequency;
				sumMin += min(SetDifference12[i*nSeg2+j],SetDifference21[j*nSeg1+i]) * frequency;
				sumEa12 += AreaDifference12[i*nSeg2+j] * frequency;
				sumEa21 += AreaDifference21[j*nSeg1+i] * frequency;
				sumMina += min(AreaDifference12[i*nSeg2+j],AreaDifference21[j*nSeg1+i]) * frequency;
			}
		}
		// numFaces based
		GCE = min(sumE12, sumE21) / nFaces;
		LCE = sumMin / nFaces;
		// area based
		GCEa = min(sumEa12, sumEa21) / nFaces;
		LCEa = sumMina / nFaces;
		
		// Release memories
		delete []SetDifference12;
		delete []SetDifference21;
		delete []Intersection;
		delete []AreaDifference12;
		delete []AreaDifference21;
		delete []AreaIntersection;
		delete []nSegFaces1;
		delete []nSegFaces2;
		delete []areaSeg1;
		delete []areaSeg2;
	}
		
	// Output Results
	if (print_verbose) {
		printf("Evaluation of Consistency Error ...\n");
		printf("  GCE  = %.2f \n", GCE);
		printf("  LCE  = %.2f \n", LCE);
		printf("  GCEa = %.2f \n", GCEa);
		printf("  LCEa = %.2f \n", LCEa);
		printf("  Time = %.2f seconds\n", start_time.Elapsed());
		fflush(stdout);
	}	

	// return the booking entry
	Entry_CE *e = new Entry_CE;
	e->GCE = GCE;
	e->LCE = LCE;
	e->GCEa = GCEa;
	e->LCEa = LCEa;
	return e;
}


/**************************************************************************************************
 **************************************************************************************************
 * EvalMethod 2 : Hamming Distance ****************************************************************
 **************************************************************************************************
 **************************************************************************************************/

/* 
 * Evaluate Hamming Distance, Huang et. al, ICIP 95'
 */
struct Entry_HD*
EvaluateHammingDistance(Segmentation* s1, Segmentation* s2){
	
	// Start statistics
	RNTime start_time;
	start_time.Read();
	
	// Variables
	int i, j;
	int nSeg1 = s1->segments.NEntries();
	int nSeg2 = s2->segments.NEntries();
	
	// Total area, should be the same for s1 and s2
	double area = s1->area;
	
	// Keep the intersetion areas
	double **intersection = new double*[nSeg1];
	for(i = 0; i < nSeg1; i ++){
		intersection[i] = new double[nSeg2];
		for(int j = 0; j < nSeg2; j ++) {
			intersection[i][j] = 0;
		}
	}
	int nFaces = s1->mesh->NFaces();
	for(int k = 0; k < nFaces; k ++) {
		R3MeshFace *f1 = s1->mesh->Face(k);
		R3MeshFace *f2 = s2->mesh->Face(k);
		int i = s1->mesh->FaceSegment(f1);
		int j = s2->mesh->FaceSegment(f2);
		intersection[i][j] += s1->mesh->FaceArea(f1);
	}
	
	// Find the best matches
	int *matchForS1 = new int[nSeg1];
	int *matchForS2 = new int[nSeg2];
	for(i = 0; i < nSeg1; i ++){
		int index = 0;
		double largest = -10000;
		for(j = 0; j < nSeg2; j ++){
			if(intersection[i][j] > largest){
				largest = intersection[i][j];
				index =j;
			}
		}
		matchForS1[i] = index;
	}
	for(j = 0; j < nSeg2; j ++){
		int index = 0;
		double largest = -10000;
		for(i = 0; i < nSeg1; i ++){
			if(intersection[i][j] > largest){
				largest = intersection[i][j];
				index = i;
			}
		}
		matchForS2[j] = index;
	}
	
	// Compute directional Hamming distance
	// dh12 = sum(areaSeg2[j]-intersection[matchForS2[j]][j])
	//      = sum(areaSeg2[j]) - sum(intersection[matchForS2[j]][j])
	//      = area - sum(intersection[matchForS2[j]][j]
	double dh12 = area, dh21 = area;
	for(j = 0; j < nSeg2; j ++){
		dh12 -= intersection[matchForS2[j]][j];
	}
	for(i = 0; i < nSeg1; i ++){
		dh21 -= intersection[i][matchForS1[i]];
	}
	
	// Booking
	Entry_HD* e_HD = new Entry_HD;
	e_HD->distance = (dh12 + dh21) / (2 * area);
	e_HD->missingRate = dh12 / area;
	e_HD->falseAlarmRate = dh21 / area;
	
	// Output Results
	if (print_verbose) {
		printf("Evaluation of Hamming Distance ...\n");
		printf("  HammingDistance  = %.2f \n", e_HD->distance);
		printf("  MissingRate  = %.2f \n", e_HD->missingRate);
		printf("  FalseAlarmRate = %.2f \n", e_HD->falseAlarmRate);
		printf("  Time = %.2f seconds\n", start_time.Elapsed());
		fflush(stdout);
	}
	
	// Release memories
	for(i = 0; i < nSeg1; i ++) 
		delete []intersection[i];
	delete []intersection;
	delete []matchForS1;
	delete []matchForS2;
		
	// Return the booking entry
	return e_HD;	
}


/**************************************************************************************************
 **************************************************************************************************
 * EvalMethod 3 : Cut Seperation ******************************************************************
 **************************************************************************************************
 **************************************************************************************************/  

/*
 * Vertex Data
 */
struct VertexData {
	R3MeshVertex *vertex;
	RNScalar distance;
	VertexData **heap_backpointer;
};


/*
 * Function to print vertex values to a file
 */
static void
PrintVertexValues(R3Mesh *mesh, char *filename){
	
	// Open file
	FILE *fp = fopen(filename, "w");
	if (!fp) {
		fprintf(stderr, "Unable to open %s\n", filename);
		return;
	}

	// Print values
	for (int i = 0; i < mesh->NVertices(); i++) {
		R3MeshVertex *vertex = mesh->Vertex(i);
		double value = mesh->VertexValue(vertex);
		fprintf(fp, "%f\n", value);
	}

	// Close file
	fclose(fp);
}


/*
 * A function inspired by Dijkstra's algorithm to compute the
 * distance of each vertex from the closest edge
 */
static int
ComputeDistancesFromVertices(R3Mesh *mesh, const RNArray <R3MeshVertex *>& source_vertices){

	// Allocate data for all vertices
	VertexData *vertex_datas = new VertexData [ mesh->NVertices() ];
	if (!vertex_datas) {
		fprintf(stderr, "Unable to allocate vertex data\n");
		return 0;
	}

	// Initialize data for all vertices
	for (int i = 0; i < mesh->NVertices(); i++) {
		R3MeshVertex *vertex = mesh->Vertex(i);
		VertexData *vertex_data = &vertex_datas[i];
		vertex_data->vertex = vertex;
		vertex_data->distance = FLT_MAX;
		vertex_data->heap_backpointer = NULL;
	}

	// Allocate heap
	VertexData tmp;

	RNHeap<VertexData *> heap(&tmp, &tmp.distance, &tmp.heap_backpointer);

	// Load all source vertices onto heap and initialize distances to zero
	for (int i = 0; i < source_vertices.NEntries(); i++) {
		R3MeshVertex *source_vertex = source_vertices.Kth(i);
		int vertex_index = mesh->VertexID(source_vertex);
		// assert(mesh->Vertex(vertex_index) == vertex);
		VertexData *vertex_data = &vertex_datas[vertex_index];
		vertex_data->distance = 0;
		heap.Push(vertex_data);
	}

	// Iteratively pop the vertex with least distance off the heap
	while (!heap.IsEmpty()) {
		VertexData *vertex_data = heap.Pop();
		assert(vertex_data->distance < FLT_MAX);
		R3MeshVertex *vertex = vertex_data->vertex;
		for (int i = 0; i < mesh->VertexValence(vertex); i++) {
			R3MeshEdge *edge = mesh->EdgeOnVertex(vertex, i);
			R3MeshVertex *neighbor_vertex = mesh->VertexAcrossEdge(edge, vertex);
			int neighbor_index = mesh->VertexID(neighbor_vertex);
			assert(mesh->Vertex(neighbor_index) == neighbor_vertex);
			VertexData *neighbor_data = &vertex_datas[neighbor_index];
			RNScalar old_distance = neighbor_data->distance;
			RNScalar new_distance = mesh->EdgeLength(edge) + vertex_data->distance;
			if (new_distance < old_distance) {
				neighbor_data->distance = new_distance;
				if (old_distance < FLT_MAX) heap.Update(neighbor_data);
				else heap.Push(neighbor_data);
			}
		}
	}

	RNScalar maxDist = -1, minDist = FLT_MAX, totalDist = 0;
	// Put the values into the mesh vertices
	for (int i = 0; i < mesh->NVertices(); i++) {
		R3MeshVertex *vertex = mesh->Vertex(i);
		mesh->SetVertexValue(vertex, vertex_datas[i].distance);
		if (vertex_datas[i].distance > maxDist)
			maxDist = vertex_datas[i].distance;
		else if (vertex_datas[i].distance < minDist)
			minDist = vertex_datas[i].distance;
		totalDist += vertex_datas[i].distance;
	}

	if (print_verbose) {
		printf("  Maximum distance of a vertex from a cut = %f\n", maxDist);
		printf("  Minimum distance of a vertex from a cut = %f\n", minDist);
		printf("  Total distance of vertices from cuts = %f\n", totalDist);
		printf("  Average distance of a vertex from a cut = %f\n", totalDist/mesh->NVertices());
		fflush(stdout);
	}

	// Deallocate vertex data
	delete [] vertex_datas;

	// Return success
	return 1;
}


/*
 * Evaluate Cut Discrepancy
 */
struct Entry_CD*
EvaluateCutDiscrepancy(Segmentation *s1, Segmentation *s2){
	// start statistics
	RNTime start_time;
	start_time.Read();

	RNScalar sum = 0, count = 0;

	// Do the process both ways
    	for (int dir = 1; dir <= 2; dir++) {
    		R3Mesh *mesh1 = (dir == 1) ? s1->mesh : s2->mesh;
    		R3Mesh *mesh2 = (dir == 1) ? s2->mesh : s1->mesh;

    		if (print_verbose)
    			printf("Evaluating, using the cuts of segmentation %d and the vertices of %d ...\n", (dir ==1) ? 1 : 2, (dir ==1) ? 2 : 1);

	    	RNArray <R3MeshEdge *> cut_edges;

    		// Get the edges on cuts
    		for (int i = 0; i < mesh1->NEdges(); i++) {
    			R3MeshEdge *edge = mesh1->Edge(i);
	    		R3MeshFace *facea = mesh1->FaceOnEdge(edge, 0);
    			R3MeshFace *faceb = mesh1->FaceOnEdge(edge, 1);
    			if (facea && faceb) {
				int segmenta = mesh1->FaceSegment(facea);
				int segmentb = mesh1->FaceSegment(faceb);
				if (segmenta != segmentb)
					cut_edges.Insert(edge);
    			}
    		}	

	    	// Get the vertices on cuts, using the edges obtained above
    		R3mesh_mark++;
	    	RNArray <R3MeshVertex *> cut_vertices;
    		for (int i = 0; i < cut_edges.NEntries(); i++) {
    			R3MeshEdge *edge = cut_edges.Kth(i);
    			for (int j = 0; j < 2; j++) {
    				R3MeshVertex *vertex = mesh1->VertexOnEdge(edge, j);
	    			if (mesh1->VertexMark(vertex) != R3mesh_mark) {
    					cut_vertices.Insert(vertex);
    					mesh1->SetVertexMark(vertex, R3mesh_mark);
    				}
    			}
    		}

    		// Get the corresponding vertices in the other mesh
    		RNArray <R3MeshVertex *> cut_vertices2;
    		for (int i = 0; i < cut_vertices.NEntries(); i++) {
    			R3MeshVertex *vertex = cut_vertices.Kth(i);
	    		int id = mesh1->VertexID(vertex);
    			R3MeshVertex *vertex2 = mesh2->Vertex(id);	
    			cut_vertices2.Insert(vertex2);
    		}

    		ComputeDistancesFromVertices(mesh2, cut_vertices2);
    	}

    	RNScalar minDist = FLT_MAX, maxDist = -1;

	// Get the distances between vertices on cuts in different meshes
    	for (int dir = 1; dir <=2; dir++) {
    		R3Mesh *mesh1 = (dir == 1) ? s1->mesh : s2->mesh;
	    	R3Mesh *mesh2 = (dir == 1) ? s2->mesh : s1->mesh;

    		for (int i = 0; i < mesh1->NVertices(); i++) {
    			R3MeshVertex *vertex1 = mesh1->Vertex(i);
	    		RNScalar value1 = mesh1->VertexValue(vertex1);
    			if (value1 != 0) continue;
    			R3MeshVertex *vertex2 = mesh2->Vertex(i);
	    		RNScalar value2 = mesh2->VertexValue(vertex2);
    			if (value2 < minDist) minDist = value2;
    			if (value2 > maxDist) maxDist = value2;
    			sum += value2;
	    		count++;
    		}
    	}

   	if (print_verbose) {
    		printf("Evaluation of Cut Discrepancy...\n");
    		printf("  Time = %.2f seconds\n", start_time.Elapsed());
    		printf("  Maximum distance between vertices on cuts in different meshes = %f\n", maxDist);
    		printf("  Minimum distance between vertices on cuts in different meshes = %f\n", minDist);
    		fflush(stdout);
    	}

    	Entry_CD *e = NULL;
    	if (count > 0 && minDist < FLT_MAX) {
    		// Average distance from vertices to centroid
	    	double d = s1->mesh->AverageRadius();	
    		RNScalar score = sum/count/d;
		if (print_verbose)
			printf("  The score (average distance between vertices on a cut) is %f\n", score);
    		e = new Entry_CD;
    		e->CD = score;
    	}
    	else if (print_verbose)
    		printf("  Neither of the meshes has been segmented, hence the score is 0\n");

	// Print distances
	if (print_debug) {
		PrintVertexValues(s1->mesh, (char*)("input.dat"));
		PrintVertexValues(s2->mesh, (char*)("reference.dat"));
	}
    
    	return e;
}



/**************************************************************************************************
 **************************************************************************************************
 * EvalMethod 4 : Rand Index related **************************************************************
 **************************************************************************************************
 **************************************************************************************************/

/*
 * Get Combination(N,2)
 */
inline int CN2(int N) {
	return N * (N - 1) / 2;
}

inline double CN2(int N, double den) {
	return N / den * (N-1) / 2;
}

/*
 * Get Rand Index
 */
struct Entry_RI*
EvaluateRandIndex(Segmentation* s1, Segmentation* s2){
	
	// Start statistics
	RNTime start_time;
	start_time.Read();
	
	// variables
	double RI = 0;
	int nSeg1 = s1->segments.NEntries();
	int nSeg2 = s2->segments.NEntries();
	R3Mesh* mesh1 = s1->mesh;
	R3Mesh* mesh2 = s2->mesh;
	int nFaces = mesh1->NFaces();
	
	// deal with the extreme case for optimization: every face is a segmentation
	if(nSeg1 == nFaces || nSeg2==nFaces) {
		Segmentation* s;
		// s is the normal segmentation
		s = (nSeg1==nFaces)?s2:s1;
		int nSeg = s->segments.NEntries();
		for(int i = 0; i < nSeg; i ++) {
			int size = s->segments[i]->faces.NEntries();
			RI += CN2(size, nFaces);
		}
		RI /= ((nFaces-1)/2);		
	}
	// other cases
	else { 
		// construct a table
		int **n  = new int*[nSeg1];
		for(int i = 0; i < nSeg1; i ++) {
			n[i] = new int[nSeg2];
			for(int j = 0; j < nSeg2; j ++) {
				n[i][j] = 0;
			}
		}		
		for(int i = 0; i < nFaces; i ++) {
			int segId1 = mesh1->FaceSegment(mesh1->Face(i));
			int segId2 = mesh2->FaceSegment(mesh2->Face(i));
			n[segId1][segId2] ++;
		}
	
		// compute the value 1-RI
		for(int i = 0; i < nSeg1; i ++) {
			int size = s1->segments[i]->faces.NEntries();
			RI += CN2(size, nFaces);
		}
		for(int i = 0; i < nSeg2; i ++) {
			int size = s2->segments[i]->faces.NEntries();
			RI += CN2(size, nFaces);
		}
		for(int i = 0; i < nSeg1; i ++) {
			for(int j = 0; j < nSeg2; j ++) {
				int size = n[i][j];
				RI -= CN2(size, nFaces)*2;
			}
		}
		RI /= ((nFaces-1)/2);
		
		// cleaning
		for(int i = 0; i < nSeg1; i ++) {
			delete n[i];
		}	
		delete n;
	}
	
	// Booking, should notice that RI here is the complement of the traditional definition
	Entry_RI* e = new Entry_RI;
	e->RI = RI;
	
	// Output results
	if (print_verbose) {
		printf("Evaluating Rand Index ...\n");
		printf("  RI = %.2f \n", e->RI);
		printf("  Time = %.2f seconds\n", start_time.Elapsed());
		fflush(stdout);
	}
	
	// Return the booking entry
	return e;
}


#endif

