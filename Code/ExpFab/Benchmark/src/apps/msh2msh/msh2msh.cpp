// Source file for the mesh converter program



// Include files 

#include "R3Shapes/R3Shapes.h"



// Program arguments

char *input_name = NULL;
char *output_name = NULL;
int flip_faces = 0;
RNScalar scale_factor = 1.0;
RNLength max_edge_length = 0.0;
char *xform_name = NULL;
int print_verbose = 0;



static int
ReadMatrix(R4Matrix& m, const char *filename)
{
  // Open file
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "Unable to open matrix file: %s\n", filename);
    return 0;
  }

  // Read matrix from file
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      double value;
      fscanf(fp, "%lf", &value);
      m[i][j] = value;
    }
  }

  // Close file
  fclose(fp);

  // Return success
  return 1;
}



static RNScalar 
EdgeValue(R3MeshEdge *edge, void *data)
{
  // Return value associated with edge for heap sorting
  R3Mesh *mesh = (R3Mesh *) data;
  if (mesh->EdgeValue(edge) == 0) mesh->SetEdgeValue(edge, mesh->EdgeLength(edge));
  return mesh->EdgeValue(edge);
}



static int 
SplitEdges(R3Mesh *mesh, RNLength max_edge_length)
{
  // Create priority queue for long edges
  RNHeap<R3MeshEdge *> heap(EdgeValue, NULL, mesh, FALSE);

  // Add long edges to priority queue
  for (int i = 0; i < mesh->NEdges(); i++) {
    R3MeshEdge *edge = mesh->Edge(i);
    RNLength length = mesh->EdgeLength(edge);
    if (length > max_edge_length) heap.Push(edge);
  }

  // Split edges in longest to shortest order
  while (!heap.IsEmpty()) {
    R3MeshEdge *edge = heap.Pop();
    R3Point midpoint = mesh->EdgeMidpoint(edge);
    R3MeshVertex *vertex = mesh->SplitEdge(edge, midpoint);
    for (int i = 0; i < mesh->VertexValence(vertex); i++) {
      R3MeshEdge *edge = mesh->EdgeOnVertex(vertex, i);
      RNLength length = mesh->EdgeLength(edge);
      if (length > max_edge_length) heap.Push(edge);
    }
  }

  // Return success
  return 1;
}



int ParseArgs(int argc, char **argv)
{
  // Check number of arguments
  if (argc == 1) {
    printf("Usage: msh2msh fnMeshIn fnMeshOut [options]\n");
    exit(0);
  }

  // Parse arguments
  argc--; argv++;
  while (argc > 0) {
    if ((*argv)[0] == '-') {
      if (!strcmp(*argv, "-v")) print_verbose = 1;
      else if (!strcmp(*argv, "-flip")) flip_faces = 1;
      else if (!strcmp(*argv, "-scale")) { argv++; argc--; scale_factor = atof(*argv); }
      else if (!strcmp(*argv, "-xform")) { argv++; argc--; xform_name = *argv; }
      else if (!strcmp(*argv, "-max_edge_length")) { argv++; argc--; max_edge_length = atof(*argv); }
      else { fprintf(stderr, "Invalid program argument: %s", *argv); exit(1); }
      argv++; argc--;
    }
    else {
      if (!input_name) input_name = *argv;
      else if (!output_name) output_name = *argv;
      else { fprintf(stderr, "Invalid program argument: %s", *argv); exit(1); }
      argv++; argc--;
    }
  }

  // Check input filename
  if (!input_name) {
    fprintf(stderr, "You did not specify an input file name.\n");
    return 0;
  }

  // Check output filename
  if (!output_name) {
    fprintf(stderr, "You did not specify an output file name.\n");
    return 0;
  }

  // Return OK status 
  return 1;
}



int main(int argc, char **argv)
{
  // Check number of arguments
  if (!ParseArgs(argc, argv)) exit(1);

  // Create mesh
  R3Mesh *mesh = new R3Mesh();
  if (!mesh) {
    fprintf(stderr, "Unable to allocate mesh data structure.\n");
    exit(1);
  }

  // Read mesh
  if (!mesh->ReadFile(input_name)) {
    fprintf(stderr, "Unable to read file: %s\n", input_name);
    exit(1);
  }

  // Transform every vertex
  if (xform_name) {
    // Read xform
    R4Matrix m;
    if (ReadMatrix(m, xform_name)) {
      for (int i = 0; i < mesh->NVertices(); i++) {
        R3MeshVertex *v = mesh->Vertex(i);
        R3Point p = mesh->VertexPosition(v);
        mesh->SetVertexPosition(v, m * p);
      }
    }
  }

  // Scale every vertex
  if (scale_factor != 1.0) {
    for (int i = 0; i < mesh->NVertices(); i++) {
      R3MeshVertex *v = mesh->Vertex(i);
      R3Point p = mesh->VertexPosition(v);
      mesh->SetVertexPosition(v, p * scale_factor);
    }
  }

  // Flip every face
  if (flip_faces) {
    for (int i = 0; i < mesh->NFaces(); i++) {
      mesh->FlipFace(mesh->Face(i));
    }
  }

  // Split edges that are too long
  if (max_edge_length > 0) {
    SplitEdges(mesh, max_edge_length);
  }

  // Write mesh
  if (!mesh->WriteFile(output_name)) {
    fprintf(stderr, "Unable to write file: %s\n", output_name);
    exit(1);
  }
  
  // Clean up
  delete mesh;

  // Return success 
  return 0;
}

















