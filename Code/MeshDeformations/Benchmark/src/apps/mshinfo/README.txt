This program outputs information about a mesh model, including:
  number of faces, 
  number of edges, 
  number of vertices, 
  total surface area, 
  average vertex valence, 
  average edge interior angle, 
  bounding box, 
  axial length, etc. 

If segmentation information is given, it will output number of segements as well
as number of segments in each segment.

Usage: mshinfo fnMesh [-seg fnSeg]
======
fnMesh : path to the mesh model
-seg fnSeg: path to the segmentation for fnMesh (optional) 
  
