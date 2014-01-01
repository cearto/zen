// Source file for the interactive mesh segmentation program


////////////////////////////////////////////////////////////////////////
// Include files
////////////////////////////////////////////////////////////////////////

#include "R3Graphics/R3Graphics.h"
#include "Segmentations.h"
#include <freeglut.h>


////////////////////////////////////////////////////////////////////////
// Type definitions
////////////////////////////////////////////////////////////////////////

struct Path {
  Path(void) : length(0) {};
  Path(const Path& path) : vertices(path.vertices), length(path.length) {};
  RNArray<R3MeshVertex *> vertices;
  RNScalar length;
};

struct Cut {
  Cut(void) {};
  RNArray<R3MeshVertex *> anchors;
  RNArray<Path *> paths;
};



////////////////////////////////////////////////////////////////////////
// Program arguments
////////////////////////////////////////////////////////////////////////

static char *input_name = NULL;
static char *output_name = NULL;
static char *image_name = NULL;


////////////////////////////////////////////////////////////////////////
// GUI stuff
////////////////////////////////////////////////////////////////////////

// GLUT variables 

static int GLUTwindow = 0;
static int GLUTwindow_width = 800;
static int GLUTwindow_height = 600;
static int GLUTmouse[2] = { 0, 0 };
static int GLUTbutton[3] = { 0, 0, 0 };
static int GLUTmodifiers = 0;



// Display variables

static int show_segments = 1;
static int show_anchors = 1;
static int show_paths = 1;
static int show_faces = 1;
static int show_edges = 0;
static int show_vertices = 0;
static int show_backfacing = 1;
static R3Point center(0,0,0);



// Color variables 

const int ncolors = 30;
static GLfloat colors[ncolors][4] = {
  {.5,.5,.5,1}, {.7,0,0,1}, {0,.7,0,1}, {0,0,.7,1}, {.7,0,.7,1}, {0,.7,.7,1},
  {1,.3,.7,1}, {1,.7,.3,1}, {.7,1,.3}, {.3,1,.7,1}, {.7,.3,1,1}, {.3,.7,1,1},
  {1,0,0,1}, {0,1,0,1}, {0,0,1,1}, {1,0,1,1}, {0,1,1,1}, {1,1,0,1},
  {.5,0,0,1}, {0,.5,0,1}, {0,0,.5,1}, {.5,0,.5,1}, {0,.5,.5,1}, {.5,.5,0,1},
  {1,.5,.5,1}, {.5,1,.5,1}, {.5,.5,1,1}, {1,.5,1,1}, {.5,1,1,1}, {1,1,.5,1}
};



////////////////////////////////////////////////////////////////////////
// Data variables
////////////////////////////////////////////////////////////////////////

static Segmentation *segmentation = NULL;
static R3Viewer *viewer = NULL;
static const int max_cuts = 128;
static Cut cuts[max_cuts];
static int active_cut = 0;
static int ncuts = 1;



////////////////////////////////////////////////////////////////////////
// Input/Output stuff
////////////////////////////////////////////////////////////////////////
static R3Viewer *
CreateBirdsEyeViewer(R3Mesh *mesh)
{
  // Setup camera view looking down the Z axis
  R3Box bbox = mesh->BBox();
  assert(!bbox.IsEmpty());
  center = bbox.Centroid();
  RNLength r = bbox.DiagonalRadius();
  assert((r > 0.0) && RNIsFinite(r));
  static R3Vector towards = R3Vector(-0.57735, -0.57735, -0.57735);
  static R3Vector up = R3Vector(-0.57735, 0.57735, 0.5773);
  R3Point eye = center - towards * (2.5 * r);
  R3Camera camera(eye, towards, up, 0.4, 0.4, 0.01 * r, 100.0 * r);
  R2Viewport viewport(0, 0, GLUTwindow_width, GLUTwindow_height);
  return new R3Viewer(camera, viewport);
}



////////////////////////////////////////////////////////////////////////
// Pick stuff
////////////////////////////////////////////////////////////////////////

static R3MeshVertex *
PickClosestVertex(R3Mesh *mesh, int x, int y) 
{
  // Check cursor position
  if ((x < 0) || (GLUTwindow_width <= x) || (y < 0) || (GLUTwindow_height <= y)) {
    printf("Pick point outside viewport\n");
    return NULL;
  }

  // Allocate select buffer
  const int SELECT_BUFFER_SIZE = 1024;
  GLuint select_buffer[SELECT_BUFFER_SIZE];
  GLint select_buffer_hits;

  // Initialize select buffer
  glSelectBuffer(SELECT_BUFFER_SIZE, select_buffer);
  glRenderMode(GL_SELECT);
  glInitNames();
  glPushName(0);

  // Draw faces with pick names into selection buffer
  GLint viewport[4];
  glViewport(0, 0, GLUTwindow_width, GLUTwindow_height);
  glGetIntegerv(GL_VIEWPORT, viewport);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPickMatrix((GLdouble) x, (GLdouble) y, 1, 1, viewport);
  viewer->Camera().Load(TRUE);
  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (int i = 0; i < mesh->NFaces(); i++) {
    R3MeshFace *face = mesh->Face(i);
    glLoadName(i + 1);
    mesh->DrawFace(face);
  }
  glFlush();
  select_buffer_hits = glRenderMode(GL_RENDER);

  // Process select buffer to find front-most hit
  GLuint hit = 0;
  GLuint hit_z = 0xFFFFFFFF;
  GLuint *bufp = select_buffer;
  GLuint numnames, z1, z2;
  for (int i = 0; i < select_buffer_hits; i++) {
    numnames = *bufp++;
    z1 = *bufp++;
    z2 = *bufp++;
    while (numnames--) {
      // This seems wrong?
      if (z1 < hit_z) {
        hit = *bufp;
        hit_z = z1/2 + z2/2;
      }
      bufp++;
    }
  }

  // Check if hit anything
  if (hit <= 0) return NULL;

  // Find hit face
  hit--; // subtract the one added to avoid zero
  if (hit < 0) return NULL;
  if (hit >= (GLuint) mesh->NFaces()) return NULL;
  R3MeshFace *face = mesh->Face(hit);

  // Find hit point
  GLdouble p[3];
  GLdouble modelview_matrix[16];
  GLdouble projection_matrix[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);
  glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);
  GLdouble z = (GLdouble) hit_z / (GLdouble) 0xFFFFFFFF;
  gluUnProject(x, y, z, modelview_matrix, projection_matrix, viewport, &(p[0]), &(p[1]), &(p[2]));
  R3Point cursor_world_position(p[0], p[1], p[2]);
  R2Point cursor_screen_position(x, y);

  // Find closest vertex on hit face
  RNLength closest_distance = FLT_MAX;
  R3MeshVertex *closest_vertex = NULL;
  for (int i = 0; i < 3; i++) {
    R3MeshVertex *vertex = mesh->VertexOnFace(face, i);
    const R3Point& vertex_world_position = mesh->VertexPosition(vertex);
    R2Point vertex_screen_position; RNScalar dummy;
    gluProject(vertex_world_position[0], vertex_world_position[1], vertex_world_position[2], 
      modelview_matrix, projection_matrix, viewport, 
      &(vertex_screen_position[0]), &(vertex_screen_position[1]), &dummy);
    RNLength distance = R2Distance(vertex_screen_position, cursor_screen_position);
    if (distance < closest_distance) {
      closest_vertex = vertex;
      closest_distance = distance;
    }
  }

  // Return closest vertex
  return closest_vertex;
}



////////////////////////////////////////////////////////////////////////
// Shortest path stuff
////////////////////////////////////////////////////////////////////////

struct ShortestPathVertexData {
  R3MeshVertex *vertex;
  ShortestPathVertexData *ancestor;
  ShortestPathVertexData **heappointer;
  RNScalar distance;
};

static Path *
FindShortestPath(R3Mesh *mesh, R3MeshVertex *v1, R3MeshVertex *v2)
{
  // Initialize result
  Path *path = NULL;

  // Check v1 and v2 
  if (!v1 || !v2) return NULL;
  if (mesh->NVertices() == 0) return NULL;
  if (v1 == v2) return NULL;

  // Allocate shortest path vertex data
  ShortestPathVertexData *data = new ShortestPathVertexData [ mesh->NVertices() ];
  if (!data) { fprintf(stderr, "Unable to allocate shortest path vertex data\n"); return NULL; }
  for (int i = 0; i < mesh->NVertices(); i++) {
    R3MeshVertex *vertex = mesh->Vertex(i);
    data[i].vertex = vertex;
    data[i].ancestor = NULL;
    data[i].heappointer = NULL;
    data[i].distance = FLT_MAX;
  }

  // Build heap of shortest path vertex data
  ShortestPathVertexData tmp;
  RNHeap<ShortestPathVertexData *> heap(&tmp, &tmp.distance, &tmp.heappointer);
  int index2 = mesh->VertexID(v2);
  assert(mesh->Vertex(index2) == v2);
  ShortestPathVertexData *d2 = &data[index2];
  d2->distance = 0;
  heap.Push(d2);

  // Iteratively pop closest vertex
  while (!heap.IsEmpty()) {
    ShortestPathVertexData *d = heap.Pop();
    R3MeshVertex *v = d->vertex;

    // Check if found destination
    if (v == v1) {
      // Found connecting path -- traverse list of ancestors and return success
      path = new Path();
      path->length = d->distance;
      while (d) { 
        path->vertices.Insert(d->vertex); 
        d = d->ancestor; 
      }
      break;
    }

    // Push neighbors onto heap
    for (int i = 0; i < mesh->VertexValence(v); i++) {
      R3MeshEdge *ne = mesh->EdgeOnVertex(v, i);
      R3MeshVertex *nv = mesh->VertexAcrossEdge(ne, v);
      int ni = mesh->VertexID(nv);
      ShortestPathVertexData *nd = &data[ni];
      RNScalar new_distance = d->distance + mesh->EdgeLength(ne);
      if (new_distance < nd->distance) {
        if (nd->distance == FLT_MAX) {
          nd->distance = new_distance;
          nd->ancestor = d;
          heap.Push(nd);
        }
        else {
          nd->distance = new_distance;
          nd->ancestor = d;
          heap.Update(nd);
        }
      }
    }
  }

  // Delete shortest path vertex data
  delete [] data;

  // Return path
  return path;
}



////////////////////////////////////////////////////////////////////////
// Cut stuff
////////////////////////////////////////////////////////////////////////

void
ResetCuts(void)
{
  // Empty all cuts
  for (int i = 0; i < ncuts; i++) {
    Cut& cut = cuts[i];
    for (int j = 0; j < cut.paths.NEntries(); j++) {
      delete cut.paths.Kth(j);
    }
    cut.paths.Empty();
    cut.anchors.Empty();
  }

  // Set active cut
  active_cut = 0;
  ncuts = 1;
}



void
InsertAnchor(int x, int y)
{
  // Get cut
  Cut& cut = cuts[active_cut];

  // Pick vertex
  R3Mesh *mesh = segmentation->mesh;
  R3MeshVertex *vertex = PickClosestVertex(mesh, x, y);
  if (!vertex) return;

  // Check if vertex is already an anchor
  for (int i = 0; i < cut.anchors.NEntries(); i++) {
    R3MeshVertex *anchor = cut.anchors.Kth(i);
    if (vertex == anchor) {
      if (print_debug) printf("Vertex is already an anchor\n");
      fflush(stdout);
      return;
    }
  }

  // Check number of anchors
  if (cut.anchors.NEntries() == 0) {
    // Add first anchor 
    cut.anchors.Insert(vertex);
  }
  else if (cut.anchors.NEntries() == 1) {
    // Get first anchor 
    R3MeshVertex *anchor = cut.anchors.Head();

    // Find shortest path
    Path *path1 = FindShortestPath(mesh, vertex, anchor);
    if (!path1) {
      if (print_debug) printf("No path found\n");
      return;
    }

    // Create return path
    Path *path2 = new Path(*path1);
    path2->vertices.Reverse();

    // Add paths to cut
    cut.paths.Insert(path1);
    cut.paths.Insert(path2);

    // Add anchor
    cut.anchors.Insert(vertex);
  }
  else {
    // Make array of paths
    Path **paths = new Path * [ cut.anchors.NEntries() ];

    // Find shortest path to each anchor
    for (int i = 0; i < cut.anchors.NEntries(); i++) {
      R3MeshVertex *anchor = cut.anchors.Kth(i);
      paths[i] = FindShortestPath(mesh, vertex, anchor);
    }

    // Find best path to replace
    int best_index0 = -1;
    int best_index1 = -1;
    RNScalar best_delta = FLT_MAX;
    for (int i = 0; i < cut.anchors.NEntries(); i++) {
      int index0 = i;
      int index1 = (i+1)%cut.anchors.NEntries();
      Path *old_path = cut.paths.Kth(i);
      Path *new_path0 = paths[index0];
      Path *new_path1 = paths[index1];
      if (!new_path0 || !new_path1) continue;
      RNScalar delta = (new_path0->length + new_path1->length) - old_path->length;
      if (delta < best_delta) {
        best_delta = delta;
        best_index0 = index0;
        best_index1 = index1;
      }
    }

    // Check best path
    if ((best_index0 == -1) || (best_index1 == -1)) return;

    // Copy paths
    Path *path0 = new Path(*(paths[best_index0]));
    Path *path1 = new Path(*(paths[best_index1]));
    path0->vertices.Reverse();

    // Replace best path
    cut.paths.RemoveKth(best_index0);
    cut.paths.InsertKth(path1, best_index0);
    cut.paths.InsertKth(path0, best_index0);

    // Delete array of paths
    for (int i = 0; i < cut.anchors.NEntries(); i++) 
      if (paths[i]) delete paths[i];
    delete [] paths;

    // Insert anchor
    cut.anchors.InsertKth(vertex, best_index0 + 1);
  }
}



void
DeleteAnchor(int x, int y)
{
  // Get cut
  Cut& cut = cuts[active_cut];

  // Pick vertex
  R3Mesh *mesh = segmentation->mesh;
  R3MeshVertex *vertex = PickClosestVertex(mesh, x, y);
  if (!vertex) return;

  // Check if vertex is  an anchor
  int anchor_index = -1;
  for (int i = 0; i < cut.anchors.NEntries(); i++) {
    R3MeshVertex *anchor = cut.anchors.Kth(i);
    if (vertex == anchor) {
      anchor_index = i;
      break;
    }
  }

  // Check if vertex is an anchor
  if (anchor_index == -1) {
    if (print_debug) printf("Vertex is not an anchor\n");
    return;
  }

  // Update paths
  if (cut.anchors.NEntries() == 2) {
    assert(cut.paths.NEntries() == 2);
    if (cut.paths.NEntries() == 2) {
      delete cut.paths.Kth(0);
      delete cut.paths.Kth(1);
    }
    cut.paths.Empty();
  }
  else if (cut.anchors.NEntries() > 2) {
    if (anchor_index == 0) {
      Path *path = FindShortestPath(mesh, cut.anchors.Tail(), cut.anchors.Kth(1));
      if (!path) { if (print_debug) printf("Cannot find new path\n"); return; }
      cut.paths.RemoveHead();
      cut.paths.RemoveTail();
      cut.paths.InsertTail(path);
    }
    else if (anchor_index == cut.anchors.NEntries()-1) {
      Path *path = FindShortestPath(mesh, cut.anchors.Kth(cut.anchors.NEntries()-2), cut.anchors.Head());
      if (!path) { if (print_debug) printf("Cannot find new path\n"); return; }
      cut.paths.RemoveKth(anchor_index);
      cut.paths.RemoveKth(anchor_index-1);
      cut.paths.InsertTail(path);
    } 
    else {
      Path *path = FindShortestPath(mesh, cut.anchors.Kth(anchor_index-1), cut.anchors.Kth(anchor_index+1));
      if (!path) { if (print_debug) printf("Cannot find new path\n"); return; }
      delete cut.paths.Kth(anchor_index);
      cut.paths.RemoveKth(anchor_index);
      delete cut.paths.Kth(anchor_index-1);
      cut.paths.RemoveKth(anchor_index-1);
      cut.paths.InsertKth(path, anchor_index-1);
    } 
  }

  // Remove anchor
  cut.anchors.RemoveKth(anchor_index);
}



void
UndoCut(void)
{
  // Get mesh
  R3Mesh *mesh = segmentation->mesh;

  // Check number of segments
  if (segmentation->segments.NEntries() < 2) return;

  // Get segments
  Segment *segment2 = segmentation->segments.Tail();
  if (segment2->parent < 0) return;
  if (segment2->parent >= segmentation->segments.NEntries()) return;
  Segment *segment1 = segmentation->segments.Kth(segment2->parent);

  // Merge faces of segment2 into segment1
  for (int i = 0; i < segment2->faces.NEntries(); i++) {
    R3MeshFace *face = segment2->faces.Kth(i);
    segment1->faces.Insert(face);
    mesh->SetFaceSegment(face, segment1->index);
  }

  // Delete segment2
  segmentation->segments.RemoveTail();
  delete segment2;
}



void
MakeCut(void)
{
  // Get mesh
  R3Mesh *mesh = segmentation->mesh;

  // Allocate votes 
  int *votes = new int [ segmentation->segments.NEntries() ];
  for (int i = 0; i < segmentation->segments.NEntries(); i++) {
    votes[i] = 0;
  }

  // Cast votes 
  for (int c = 0; c < ncuts; c++) {
    Cut& cut = cuts[c];
    for (int i = 0; i < cut.paths.NEntries(); i++) {
      Path *path = cut.paths.Kth(i);
      for (int j = 0; j < path->vertices.NEntries(); j++) {
        R3MeshVertex *vertex = path->vertices.Kth(j);
        for (int k = 0; k < mesh->VertexValence(vertex); k++) {
          R3MeshEdge *edge = mesh->EdgeOnVertex(vertex, k);
          R3MeshFace *face = mesh->FaceOnVertex(vertex, edge, RN_CCW);
          if (!face) continue;
          int segment_id = mesh->FaceSegment(face);
          votes[segment_id]++;
        }
      }
    }
  }

  // Count votes
  int max_votes = 0;
  Segment *segment1 = NULL;
  for (int i = 0; i < segmentation->segments.NEntries(); i++) {
    if (votes[i] > max_votes) {
      max_votes = votes[i];
      segment1 = segmentation->segments.Kth(i);
    }
  }

  // Delete votes
  delete [] votes;

  // Check votes
  if (!segment1) {
    if (print_debug) printf("No segment selected\n");
    return;
  }

  // Print debug message
  if (print_debug) printf("Cutting segment %d\n", segment1->index);

  // Mark cut edges
  R3mesh_mark++;
  for (int c = 0; c < ncuts; c++) {
    Cut& cut = cuts[c];
    for (int i = 0; i < cut.paths.NEntries(); i++) {
      Path *path = cut.paths.Kth(i);
      for (int j = 1; j < path->vertices.NEntries(); j++) {
        R3MeshVertex *v0 = path->vertices.Kth(j-1);
        R3MeshVertex *v1 = path->vertices.Kth(j);
        R3MeshEdge *edge = mesh->EdgeBetweenVertices(v0, v1);
        assert(edge);
        mesh->SetEdgeMark(edge, R3mesh_mark);
      }
    }
  }

  // Find connected components of faces (keep biggest)
  RNScalar biggest_component_area = 0;
  RNArray<R3MeshFace *> biggest_component_faces;
  for (int i = 0; i < mesh->NFaces(); i++) {
    // Find unmarked seed face
    R3MeshFace *seed_face = mesh->Face(i);
    if (mesh->FaceMark(seed_face) == R3mesh_mark) continue;
    if (mesh->FaceSegment(seed_face) != segment1->index) continue;

    // Find connected component
    RNScalar component_area = 0;
    RNArray<R3MeshFace *> component_faces;
    RNArray<R3MeshFace *> stack;
    mesh->SetFaceMark(seed_face, R3mesh_mark);
    stack.Insert(seed_face);
    while (!stack.IsEmpty()) {
      R3MeshFace *face = stack.Tail(); stack.RemoveTail();
      component_faces.Insert(face);
      component_area += mesh->FaceArea(face);
      for (int j = 0; j < 3; j++) {
        R3MeshEdge *edge = mesh->EdgeOnFace(face, j);
        if (mesh->EdgeMark(edge) == R3mesh_mark) continue;
        R3MeshFace *neighbor_face = mesh->FaceAcrossEdge(edge, face);
        if (!neighbor_face) continue;
        if (mesh->FaceMark(neighbor_face) == R3mesh_mark) continue;
        if (mesh->FaceSegment(neighbor_face) != segment1->index) continue;
        mesh->SetFaceMark(neighbor_face, R3mesh_mark);
        stack.Insert(neighbor_face);
      }
    }

    // Check if biggest connected component
    if (component_area > biggest_component_area) {
      biggest_component_faces = component_faces;
      biggest_component_area = component_area;
    }
  }

  // Check biggest component
  if (biggest_component_area == 0) {
    if (print_debug) printf("No connected component found\n");
    return;
  }

  // Check if biggest component splits faces
  if ((biggest_component_faces.NEntries() == 0) ||
      (biggest_component_faces.NEntries() == segment1->faces.NEntries())) {
    if (print_debug) printf("Segment is not split by cut\n");
    return;
  }

  // Print debug message
  if (print_debug) printf("Biggest component has %d out of %d faces\n", 
    biggest_component_faces.NEntries(), segment1->faces.NEntries());

  // Mark faces in biggest component
  R3mesh_mark++;
  for (int i = 0; i < biggest_component_faces.NEntries(); i++) {
    R3MeshFace *face = biggest_component_faces.Kth(i);
    mesh->SetFaceMark(face, R3mesh_mark);
  }

  // Retain marked faces in segment1
  RNArray<R3MeshFace *> faces = segment1->faces;
  segment1->faces.Empty();
  for (int i = 0; i < faces.NEntries(); i++) {
    R3MeshFace *face = faces.Kth(i);
    if (mesh->FaceMark(face) != R3mesh_mark) continue;
    mesh->SetFaceSegment(face, segment1->index);
    segment1->faces.Insert(face);
  }

  // Create segment2 with unmarked faces
  Segment *segment2 = new Segment();
  //segment2->segmentation = segmentation;
  segment2->parent = segment1->index;
  segment2->index = segmentation->segments.NEntries();
  segmentation->segments.Insert(segment2);
  for (int i = 0; i < faces.NEntries(); i++) {
    R3MeshFace *face = faces.Kth(i);
    if (mesh->FaceMark(face) == R3mesh_mark) continue;
    mesh->SetFaceSegment(face, segment2->index);
    segment2->faces.Insert(face);
  }

  // Print debug message
  if (print_debug) printf("Finished cut\n");

  // Reset cut
  ResetCuts();
}



////////////////////////////////////////////////////////////////////////
// GLUT callbacks
////////////////////////////////////////////////////////////////////////

void GLUTDrawText(const R3Point& p, const char *s)
{
  // Draw text string s and position p
  glRasterPos3d(p[0], p[1], p[2]);
  while (*s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *(s++));
}
  


void GLUTStop(void)
{
  // Destroy window 
  glutDestroyWindow(GLUTwindow);

  // Exit
  exit(0);
}



void GLUTRedraw(void)
{
  // Check data 
  if (!segmentation) return;
  if (!viewer) return;
  R3Mesh *mesh = segmentation->mesh;

  // Set viewing transformation
  viewer->Camera().Load();

  // Clear window 
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set backface culling
  if (show_backfacing) glDisable(GL_CULL_FACE);
  else glEnable(GL_CULL_FACE);

  // Set lights
  static GLfloat light1_position[] = { 3.0, 4.0, 5.0, 0.0 };
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  static GLfloat ambient_material[] = { 0.1, 0.1, 0.1, 1.0 };
  static GLfloat diffuse_material[] = { 0.5, 0.5, 1.0, 1.0 };
  static GLfloat specular_material[] = { 0.3, 0.3, 0.3, 1.0};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material); 
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material); 
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material); 
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 1);

  // Draw faces
  if (show_faces) {
    glEnable(GL_LIGHTING);
    static GLfloat material[] = { 0.7, 0.7, 0.7, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material); 
    for (int i = 0; i < segmentation->segments.NEntries(); i++) {
      Segment *segment = segmentation->segments.Kth(i);
      if (show_segments) glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colors[i%ncolors]);
      for (int j = 0; j < segment->faces.NEntries(); j++) {
        R3MeshFace *face = segment->faces.Kth(j);
        mesh->DrawFace(face);
      }
    }
  }

  // Draw edges
  if (show_edges) {
    glDisable(GL_LIGHTING);
    glColor3f(1, 0, 0);
    glPolygonOffset(-2,2);
    glEnable(GL_POLYGON_OFFSET_LINE);
    mesh->DrawEdges();
    glDisable(GL_POLYGON_OFFSET_LINE);
  }

  // Draw vertices
  if (show_vertices) {
    glEnable(GL_LIGHTING);
    static GLfloat material[] = { 0, 0, 1, 1 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material); 
    RNScalar radius = 0.002 * mesh->BBox().DiagonalLength();
    for (int i = 0; i < mesh->NVertices(); i++) {
      R3MeshVertex *vertex = mesh->Vertex(i);
      R3Point position = mesh->VertexPosition(vertex);
      R3Sphere(position, radius).Draw();
    }
  }

  // Draw anchors
  if (show_anchors) {
    glEnable(GL_LIGHTING);
    static GLfloat material[] = { 1, 1, 0, 1 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material); 
    RNScalar radius = 0.004 * mesh->BBox().DiagonalLength();
    for (int i = 0; i < ncuts; i++) {
      Cut& cut = cuts[i];
      for (int j = 0; j < cut.anchors.NEntries(); j++) {
        R3MeshVertex *vertex = cut.anchors.Kth(j);
        R3Point position = mesh->VertexPosition(vertex);
        R3Sphere(position, radius).Draw();
      }
    }
  }

  // Draw paths
  if (show_paths) {
    glEnable(GL_LIGHTING);
    static GLfloat material[] = { 1, 1, 0, 1 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material); 
    for (int i = 0; i < ncuts; i++) {
      Cut& cut = cuts[i];
      RNScalar radius = 0.001 * mesh->BBox().DiagonalLength();
      if (i != active_cut) radius *= 0.25;
      for (int j = 0; j < cut.paths.NEntries(); j++) {
        Path *path = cut.paths.Kth(j);
        for (int k = 1; k < path->vertices.NEntries(); k++) {
          R3MeshVertex *vertex0 = path->vertices.Kth(k-1);
          R3MeshVertex *vertex1 = path->vertices.Kth(k);
          R3Point position0 = mesh->VertexPosition(vertex0);
          R3Point position1 = mesh->VertexPosition(vertex1);
          R3Cylinder(position0, position1, radius).Draw();
        }
      }
    }
  }

  // Capture image and exit
  if (image_name) {
    if (print_verbose) printf("Creating image %s\n", image_name);
    R2Image image(GLUTwindow_width, GLUTwindow_height, 3);
    image.Capture();
    image.Write(image_name);
    image_name = NULL;
  }

  // Swap buffers 
  glutSwapBuffers();
}    



void GLUTResize(int w, int h)
{
  // Resize window
  glViewport(0, 0, w, h);

  // Resize viewer viewport
  viewer->ResizeViewport(0, 0, w, h);

  // Remember window size 
  GLUTwindow_width = w;
  GLUTwindow_height = h;

  // Redraw
  glutPostRedisplay();
}



void GLUTMotion(int x, int y)
{
  // Invert y coordinate
  y = GLUTwindow_height - y;

  // Compute mouse movement
  int dx = x - GLUTmouse[0];
  int dy = y - GLUTmouse[1];
  
  // World in hand navigation 
  if (segmentation && segmentation->mesh) {
    if (GLUTbutton[0]) viewer->RotateWorld(1.0, center, x, y, dx, dy);
    else if (GLUTbutton[1]) viewer->ScaleWorld(1.0, center, x, y, dx, dy);
    else if (GLUTbutton[2]) viewer->TranslateWorld(1.0, center, x, y, dx, dy);
    if (GLUTbutton[0] || GLUTbutton[1] || GLUTbutton[2]) glutPostRedisplay();
  }

  // Remember mouse position 
  GLUTmouse[0] = x;
  GLUTmouse[1] = y;
}



void GLUTMouse(int button, int state, int x, int y)
{
  // Invert y coordinate
  y = GLUTwindow_height - y;
  
  // Process mouse click

  // Remember button state 
  int b = (button == GLUT_LEFT_BUTTON) ? 0 : ((button == GLUT_MIDDLE_BUTTON) ? 1 : 2);
  GLUTbutton[b] = (state == GLUT_DOWN) ? 1 : 0;

  // Remember modifiers 
  GLUTmodifiers = glutGetModifiers();

   // Remember mouse position 
  GLUTmouse[0] = x;
  GLUTmouse[1] = y;

  // Redraw
  glutPostRedisplay();
}



void GLUTSpecial(int key, int x, int y)
{
  // Invert y coordinate
  y = GLUTwindow_height - y;

  // Process keyboard button event 
  switch (key) {
  case GLUT_KEY_INSERT:
    InsertAnchor(x, y);
    break;

  case GLUT_KEY_UP:
    // Create new cut, if necessary
    if (active_cut == ncuts-1) {
      if (ncuts < max_cuts) { 
        for (int j = 0; j < cuts[ncuts].paths.NEntries(); j++) delete cuts[ncuts].paths.Kth(j);
        cuts[ncuts].paths.Empty();
        cuts[ncuts].anchors.Empty();
        active_cut++;
        ncuts++;
      }
      else {
        printf("Exceeded maximum number of cuts: %d\n", max_cuts); 
      }
    }
    else {
      active_cut++;
    }
    break;

  case GLUT_KEY_DOWN:
    if (active_cut > 0) active_cut--;
    break;
  }

  // Remember mouse position 
  GLUTmouse[0] = x;
  GLUTmouse[1] = y;

  // Remember modifiers 
  GLUTmodifiers = glutGetModifiers();

  // Redraw
  glutPostRedisplay();
}



void GLUTKeyboard(unsigned char key, int x, int y)
{
  // Invert y coordinate
  y = GLUTwindow_height - y;

  // Process keyboard button event 
  switch (key) {
  case '~': {
    static char buffer[64];
    static int image_count = 1;
    sprintf(buffer, "i%d.bmp", image_count++);
    image_name = buffer;
    break; }

  case 'A':
    show_anchors = !show_anchors;
    break;

  case 'a':
    InsertAnchor(x, y);
    break;

  case 'B':
    show_backfacing = !show_backfacing;
    break;

  case 13: // ENTER
  case 'c':
    MakeCut();
    break;

  case 8: // BACKSPACE
  case 127: // DELETE
  case 'd':
    DeleteAnchor(x, y);
    break;

  case 'E':
    show_edges = !show_edges;
    break;
 
  case 'F':
    show_faces = !show_faces;
    break;

  case 'P':
    show_paths = !show_paths;
    break;

  case 'r':
    ResetCuts();
    break;

  case 'S':
    show_segments = !show_segments;
    break;

  case 's':
    OutputSegmentation(segmentation->mesh, output_name);
    break;

  case 'u':
    UndoCut();
    break;

  case 'V':
    show_vertices = !show_vertices;
    break;

  case ' ': {
    // Update viewing center
    R3Mesh *mesh = segmentation->mesh;
    R3MeshVertex *vertex = PickClosestVertex(mesh, x, y);
    if (vertex) center = mesh->VertexPosition(vertex);
    break; }

  case 27: // ESCAPE
    OutputSegmentation(segmentation->mesh, output_name);
    GLUTStop();
    break;
  }

  // Remember mouse position 
  GLUTmouse[0] = x;
  GLUTmouse[1] = y;

  // Remember modifiers 
  GLUTmodifiers = glutGetModifiers();

  // Redraw
  glutPostRedisplay();  
}



void GLUTRun(int *argc, char **argv)
{
  // Open window 
  glutInit(argc, argv);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(GLUTwindow_width, GLUTwindow_height);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // | GLUT_STENCIL
  GLUTwindow = glutCreateWindow("OpenGL Viewer");

  // Initialize background color 
  glClearColor(200.0/255.0, 200.0/255.0, 200.0/255.0, 1.0);

  // Initialize lighting
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  static GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING); 

  // Initialize headlight
  static GLfloat light0_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
  static GLfloat light0_position[] = { 0.0, 0.0, 1.0, 0.0 };
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  glEnable(GL_LIGHT0);

  // Initialize graphics modes 
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // Initialize GLUT callback functions 
  glutDisplayFunc(GLUTRedraw);
  glutReshapeFunc(GLUTResize);
  glutKeyboardFunc(GLUTKeyboard);
  glutSpecialFunc(GLUTSpecial);
  glutMouseFunc(GLUTMouse);
  glutMotionFunc(GLUTMotion);

  // Run main loop -- never returns 
  glutMainLoop();
}



////////////////////////////////////////////////////////////////////////
// Argument Parsing Stuff
////////////////////////////////////////////////////////////////////////

static int 
ParseArgs(int argc, char **argv)
{
  // Parse arguments
  argc--; argv++;
  while (argc > 0) {
    if ((*argv)[0] == '-') {
      if (!strcmp(*argv, "-v")) print_verbose = 1;
      else if (!strcmp(*argv, "-debug")) print_debug = 1;
      else if (!strcmp(*argv, "-image")) { argc--; argv++; image_name = *argv; }
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

  // Check filenames
  if (!input_name || !output_name) {
    fprintf(stderr, "Usage: iseg fnMesh fnSeg\n");
    return 0;
  }

  // Return OK status 
  return 1;
}



////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////

int 
main(int argc, char **argv)
{
  // Check number of arguments
  if (!ParseArgs(argc, argv)) exit(1);

  // Create segmentation
  R3Mesh* mesh = ReadMesh(input_name);
  segmentation = CreateSegmentation(mesh);
  if (!segmentation) exit(-1);

  // Create viewer
  viewer = CreateBirdsEyeViewer(segmentation->mesh);
  if (!viewer) exit(-1);

  // Run GLUT interface -- never returns
  GLUTRun(&argc, argv);

  // Return success 
  return 0;
}



