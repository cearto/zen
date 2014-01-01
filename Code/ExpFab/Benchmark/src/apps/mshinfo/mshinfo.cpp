// Source file for the mesh info program



// Include files 

#include "R3Shapes/R3Shapes.h"
#include "Segmentations.h"
static char* fnMesh = NULL;
static char* fnSeg = NULL;


RNArea SurfaceArea(R3Mesh *mesh)
{
  // Sum surface area
  RNArea sum = 0.0;
  for (int i = 0; i < mesh->NFaces(); i++) {
    R3MeshFace *face = mesh->Face(i);
    sum += mesh->FaceArea(face);
  }
  return sum;
}



RNScalar AverageVertexValence(R3Mesh *mesh)
{
  // Sum surface valence
  RNScalar sum = 0.0;
  for (int i = 0; i < mesh->NVertices(); i++) {
    R3MeshVertex *vertex = mesh->Vertex(i);
    sum += mesh->VertexValence(vertex);
  }
  return sum / mesh->NVertices();
}



RNAngle AverageEdgeInteriorAngle(R3Mesh *mesh)
{
  // Sum dihedral angles
  int count = 0;
  RNAngle sum = 0.0;
  for (int i = 0; i < mesh->NEdges(); i++) {
    R3MeshEdge *edge = mesh->Edge(i);
    RNAngle angle = mesh->EdgeInteriorAngle(edge);
    if (RNIsZero(angle)) continue;
    sum += angle;
    count++;
  }
  return sum / count;
}



int NumBoundaryEdges(R3Mesh *mesh)
{
  // Count number of boundary edges
  int count = 0;
  for (int i = 0; i < mesh->NEdges(); i++) {
    R3MeshEdge *edge = mesh->Edge(i);
    R3MeshFace *face0 = mesh->FaceOnEdge(edge, 0);
    R3MeshFace *face1 = mesh->FaceOnEdge(edge, 1);
    if (!face0 || !face1) count++;
  }
  return count;
}



int NumConnectedComponents(R3Mesh *mesh)
{
  // Get fresh mark value
  R3mesh_mark++;

  // Iterate finding connected components
  int count = 0;
  int start = 0;
  for (;;) {
    // Find an unmarked face
    R3MeshFace *seed = NULL;
    for (; start < mesh->NFaces(); start++) {
      R3MeshFace *face = mesh->Face(start);
      if (mesh->FaceMark(face) != R3mesh_mark) {
        seed = face;
        break;
      }
    }

    // Check if found a new component
    if (!seed) break;
    else count++;

    // Mark connected component 
    RNArray<R3MeshFace *> stack;
    stack.InsertTail(seed);
    while (!stack.IsEmpty()) {
      R3MeshFace *face = stack.Tail();
      stack.RemoveTail();
      mesh->SetFaceMark(face, R3mesh_mark);
      for (int i = 0; i < 3; i++) {
        R3MeshFace *neighbor = mesh->FaceOnFace(face, i);
        if ((neighbor) && (mesh->FaceMark(neighbor) != R3mesh_mark)) {
          stack.InsertTail(neighbor);
        }
      }
    }
  }

  // Return number of connected components
  return count;
}


int ParseArgs(int argc, char** argv) {
  if(argc == 1) {
    printf("Usage: mshinfo fnMesh [-seg fnSeg] \n");
    exit(1);
  } 
  argc--; argv++;
  while(argc > 0) {
    if ((*argv)[0] == '-') {
       if (!strcmp(*argv, "-seg")) {argc--;argv++; fnSeg = *argv;}
       else { fprintf(stderr, "Invalid program argument: %s", *argv); exit(1); }
       argv++; argc--;
    }
    else {
      if (!fnMesh) fnMesh = *argv;
      else { fprintf(stderr, "Invalid program argument: %s", *argv); exit(1); }
      argv++; argc--;
    }
  }
  if (!fnMesh) {
    fprintf(stderr, "Please specify mesh names!\n"); return 0;
  }
  return 1;
}
   

int main(int argc, char **argv){

  // Check number of arguments 
  if(!ParseArgs(argc, argv)) exit(1);
  
  // Create mesh
  R3Mesh *mesh;  
  if(fnSeg) mesh = ReadMesh(fnMesh, fnSeg);
  else mesh = ReadMesh(fnMesh);
  if (!mesh) {
    fprintf(stderr, "Unable to allocate mesh data structure.\n"); exit(1);
  }

  // Compute stats
  int num_boundaries = NumBoundaryEdges(mesh);
  int num_components = NumConnectedComponents(mesh);
  RNScalar avg_vertex_valence = AverageVertexValence(mesh);
  RNArea total_face_area = SurfaceArea(mesh);
  RNAngle avg_edge_interior_angle = AverageEdgeInteriorAngle(mesh);
  const R3Box& bbox = mesh->BBox();

  // Print stats
  printf("Total faces = %d\n", mesh->NFaces());
  printf("Total edges = %d\n", mesh->NEdges());
  printf("Total vertices = %d\n", mesh->NVertices());
  printf("Boundary edges = %d ( %g%% )\n", num_boundaries, 100.0 * num_boundaries/ mesh->NEdges());
  printf("Connected components = %d\n", num_components);
  printf("Surface area = %g\n", total_face_area);
  printf("Average vertex valence = %g\n", avg_vertex_valence);
  printf("Average edge interior angle = %g\n", avg_edge_interior_angle);
  printf("Bounding box = ( %g %g %g ) ( %g %g %g )\n", bbox[0][0], bbox[0][1], bbox[0][2], bbox[1][0], bbox[1][1], bbox[1][2]);
  printf("Axial lengths = ( %g %g %g )\n", bbox.XLength(), bbox.YLength(), bbox.ZLength());
  print_verbose = true;
  Segmentation* s = CreateSegmentation(mesh);

  // clean up
  delete mesh;
  delete s;

  // Return success 
  return 0;
}


